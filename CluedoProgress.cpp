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
    /**
     * Find probability for each card players has it
     */
    for(int i=1; i<players.size(); i++){
        players[i]->update(&entries);
    }

    /**
     * Now assume each player says it has specific card with probability [p] computed
     * in previous step. So it say magic box does not have it with probability [p']
     * equal to 1-p
     */

    int boxCardsNo=0;
    int boxCards[10];

    double mBandC;
    double mBminusC;
    double mCminusB;
    double mA=0;

    for(int i=0; i<entries.size(); i++){
        if(entries[i]->getPlayer()==-1){
            boxCards[boxCardsNo]= groupPsStartIdx[entries[i]->getCardGroup()]+entries[i]->getCardIndex();
            boxCardsNo++;
        }
    }

    for(int g=0; g<3; g++){
        for(int i=0; i<CluedoCard::cardsInGroup[g]; i++){
            if(checkIfBotHaveIt(g, i)){
                ps[groupPsStartIdx[g]+i]=0;
            } else {
                int isForSure=0;

                //Check if we already now that it is in box
                for(int j=0; j<boxCardsNo; j++){
                    if(boxCards[j]==groupPsStartIdx[g]+i){
                        isForSure=1;
                        break;
                    }
                }

                if(isForSure){
                    ps[groupPsStartIdx[g]+i]=1;
                } else {
                    mA= players[1]->getP(g, i);
                    for(int p=2; p<players.size(); p++){
                        mBandC = mA * players[p]->getP(g, i);
                        mBminusC = mA * (1.0 - players[p]->getP(g, i));
                        mCminusB = players[p]->getP(g, i) * (1.0 - mA);

                        mA = mBandC + mBminusC + mCminusB;
                    }

                    ps[groupPsStartIdx[g]+i]= 1.0-mA;
                }
            }
        }
    }
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
