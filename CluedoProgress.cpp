#include "CluedoProgress.h"

CluedoProgress::CluedoProgress() {
    for(int i=0; i<CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO; i++){
        ps[i]=0;
    }

    groupPsStartIdx[0]=0;
    groupPsStartIdx[1]=CluedoCard::cardsInGroup[0];
    groupPsStartIdx[2]=CluedoCard::cardsInGroup[0]+CluedoCard::cardsInGroup[1];
}


CluedoProgress::~CluedoProgress() {
    for(int i=0; i<entries.size(); i++){
        delete entries[i];
    }
}

void CluedoProgress::addPlayer(int idx, string &name, int cardsInHandNo) {
    players.push_back(new Player(idx, name, cardsInHandNo));
}

char CluedoProgress::addEntry(int player, int cardGroup, int cardIdx, int type) {
    double p=0;

    if(!checkIfBotHaveIt(cardGroup, cardIdx)) {
        if (type == CLUEDOENTRY_TYPE_SURE)
            p = 1;
        else if (type == CLUEDOENTRY_TYPE_ANSWER)
            p = 0.5;

        entries.push_back(new CluedoEntry(player, cardGroup, cardIdx, p));
    } else {
        return 0;
    }

    return 1;
}

/**
 * Najpierw znajdujemy ktore karty ma bot.
 * Pozostale dodajemy jako gracza z pstwem rowny 1/(3 - ilosc kart bota)
 * Chyba że indeks gracza jest -1 to wtedy z pstwem 1
 */

char CluedoProgress::addAnswer(int player, int *cardGroups, int *cardIdxs) {
    char botNo=0;
    int hasBot[3];

    //Find which cards bot has
    for(int i=0; i<3; i++){
        if(checkIfBotHaveIt(cardGroups[i], cardIdxs[i])) {
            hasBot[i]=1;
            botNo++;
        } else {
            hasBot[i]=0;
        }
    }

    for(int i=0; i<3; i++){
        if(!hasBot[i]){
            if(player==-1){
                entries.push_back(new CluedoEntry(player, cardGroups[i], cardIdxs[i], 1.0));
            } else {
                entries.push_back(new CluedoEntry(player, cardGroups[i], cardIdxs[i], (double)1/(3-botNo)));
            }
        }
    }

    return botNo;
}

void CluedoProgress::resolve() {
    double q=0;
    int posNo;
    double posSen[entries.size()];
    int negNo;
    double negSen[entries.size()];

    double pmCandD;
    double pmCminusD;
    double pmDminusC;
    double pmA=0;

    double nmCandD;
    double nmCminusD;
    double nmDminusC;
    double nmA=0;

    double factorK;
    double npmC;


    //Loop over groups
    for(int g=0; g<3; g++){
        //Loop over group indexes
        for(int i=0; i<CluedoCard::cardsInGroup[g]; i++){
            posNo=0;
            negNo=0;

            // Loop over entries to find those refers to current card group [g] and index [i]
            for(int e=0; e<entries.size(); e++){
                if(entries[e]->getCardGroup()==g && entries[e]->getCardIndex()==i) {
                    if (entries[e]->getPlayer() == -1) {
                        negSen[negNo] = entries[e]->getP();
                        negNo++;
                    } else {
                        posSen[posNo] = entries[e]->getP();
                        posNo++;
                    }
                }
            }


            if(posNo>0) {
                // Sum up positive sentences
                pmA = posSen[0];
                for (int s = 1; s < posNo; s++) {
                    pmCandD = pmA * posSen[s];
                    pmCminusD = pmA * (1.0 - posSen[s]);
                    pmDminusC = posSen[s] * (1.0 - pmA);

                    pmA = pmCandD + pmCminusD + pmDminusC;
                }
            }

            if(negNo>0){

                // Sum up negative sentences
                nmA= negSen[0];
                for(int s=1; s<negNo; s++){
                    nmCandD= nmA*negSen[s];
                    nmCminusD= nmA*(1.0-negSen[s]);
                    nmDminusC= negSen[s]*(1.0-nmA);

                    nmA= nmCandD+nmCminusD+nmDminusC;
                }
            }

            if(posNo>0 && negNo>0){
                // Conflict
                factorK= 1.0 / (1.0-(pmA*nmA));

                npmC= factorK*pmA*(1.0-nmA);
                ps[groupPsStartIdx[g]+i]=1-npmC;
            } else if(posNo>0){
                ps[groupPsStartIdx[g]+i]=1-pmA;
            } else if(negNo>0){
                ps[groupPsStartIdx[g]+i]=1-nmA;
            }
        }
    }

    //TODO: Uwzględnić że wiem co mi ktos pokazał
    //TODO: Naprawić że ja coś pokazałem i wtedy nic się nie dzieje

    // Na górze nowa metoda liczenia (zakladamy ze magic box moze miec karty z pstwem < 1
}

int CluedoProgress::getEntriesNo() {
    return (int)entries.size();
}

void CluedoProgress::printEntry(int idx, string *names, int y, int x) {
    entries[idx]->print(names, y, x);
}


void CluedoProgress::getBestBet(int *c, int *w, int *p) {
    *c= 0;
    for(int i=0; i<CLUEDOCARD_C_NO; i++){
        if(ps[groupPsStartIdx[0]+i]>ps[groupPsStartIdx[0]+*c])
            *c=i;
    }

    *w= 0;
    for(int i=0; i<CLUEDOCARD_W_NO; i++){
        if(ps[groupPsStartIdx[1]+i]>ps[groupPsStartIdx[1]+*w])
            *w=i;
    }

    *p= 0;
    for(int i=0; i<CLUEDOCARD_P_NO; i++){
        if(ps[groupPsStartIdx[2]+i]>ps[groupPsStartIdx[2]+*p])
            *p=i;
    }
}

double CluedoProgress::getP(int group, int idx) {
    return ps[groupPsStartIdx[group]+idx];
}

void CluedoProgress::removeLastEntry() {
    entries.pop_back();
}

char CluedoProgress::checkIfBotHaveIt(int cardGroup, int cardIdx) {
    for(int i=0; i<entries.size(); i++){
        if(entries[i]->getPlayer()==0 && entries[i]->getCardGroup()==cardGroup && entries[i]->getCardIndex()==cardIdx){
            return 1;
        }
    }

    return 0;
}
