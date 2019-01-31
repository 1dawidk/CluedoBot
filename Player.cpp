#include "Player.h"


Player::Player(int idx, string &name, int cardsInHandNo) {
    this->name= name;
    this->idx= idx;
    this->cardsInHandNo= cardsInHandNo;

    groupPsStartIdx[0]=0;
    groupPsStartIdx[1]=CluedoCard::cardsInGroup[0];
    groupPsStartIdx[2]=CluedoCard::cardsInGroup[0]+CluedoCard::cardsInGroup[1];
}


void Player::update(vector<CluedoEntry *> *entries) {
    double q=0;
    int posNo;
    double posSen[entries->size()];
    int negNo;
    double negSen[entries->size()];

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

    posSen[0]=0;
    negSen[0]=0;

    //For every available card check probability player has it

    //Loop over groups
    for(int g=0; g<3; g++){
        //Loop over group indexes
        for(int i=0; i<CluedoCard::cardsInGroup[g]; i++){
            posNo=0;
            negNo=0;

            // Loop over entries to find those refers to current card group [g] and index [i]
            for(int e=0; e<entries->size(); e++){
                if((*entries)[e]->getCardGroup()==g && (*entries)[e]->getCardIndex()==i) {
                    if ((*entries)[e]->getPlayer() > 0) {
                        if ((*entries)[e]->getPlayer() == this->idx) {
                            posSen[posNo] = (*entries)[e]->getP();
                            posNo++;
                        } else {
                            negSen[negNo] = (*entries)[e]->getP();
                            negNo++;
                        }
                    } else {
                        negSen[negNo]= 1;
                        negNo++;
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
                ps[groupPsStartIdx[g]+i]=npmC;
            } else if(posNo>0){
                ps[groupPsStartIdx[g]+i]=pmA;
            } else if(negNo>0){
                ps[groupPsStartIdx[g]+i]=0;
            }

        }
    }
}

int *Player::getBestCards() {
    int best[cardsInHandNo];

    for(int i=0; i<CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO; i++){

    }
}

double Player::getP(int g, int i) {
    return ps[groupPsStartIdx[g]+i];
}
