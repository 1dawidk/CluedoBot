#include "CluedoProgress.h"

CluedoProgress::CluedoProgress() {
    for(int i=0; i<CLUEDOCARD_C_NO; i++){
        charactersPs[i]=0;
    }

    for(int i=0; i<CLUEDOCARD_W_NO; i++){
        weaponsPs[i]=0;
    }

    for(int i=0; i<CLUEDOCARD_P_NO; i++){
        placesPs[i]=0;
    }
}


CluedoProgress::~CluedoProgress() {
    for(int i=0; i<entries.size(); i++){
        delete entries[i];
    }
}

void CluedoProgress::addPlayer(int idx, string &name) {
    players.push_back(new Player(idx, name));
}

void CluedoProgress::addEntry(int player, int cardGroup, int cardIdx, int type) {
    double p=0;

    if(type==CLUEDOENTRY_TYPE_SURE)
        p=1;
    else if(type==CLUEDOENTRY_TYPE_ANSWER)
        p=0.5;

    entries.push_back(new CluedoEntry(player, cardGroup, cardIdx, p));
}

void CluedoProgress::resolve() {
    /**
     * Find probability for each card players has it
     */
    for(int i=0; i<players.size(); i++){
        players[i]->update(&entries);
    }

    /**
     * Now assume each player says it has specific card with probability [p] computed
     * in previous step. So it say magic box does not have it with probability [p']
     * equal to 1-p
     */
     double q=0;

     for(int cg=0; cg<3; cg++){
     }
    //TODO: Gether players data
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
        if(charactersPs[i]>charactersPs[*c])
            *c=i;
    }

    *w= 0;
    for(int i=0; i<CLUEDOCARD_C_NO; i++){
        if(weaponsPs[i]>weaponsPs[*w])
            *w=i;
    }

    *p= 0;
    for(int i=0; i<CLUEDOCARD_C_NO; i++){
        if(placesPs[i]>placesPs[*p])
            *p=i;
    }
}

double CluedoProgress::getCharacterP(int idx) {
    return charactersPs[idx];
}

double CluedoProgress::getWeaponP(int idx) {
    return weaponsPs[idx];
}

double CluedoProgress::getPlaceP(int idx) {
    return placesPs[idx];
}
