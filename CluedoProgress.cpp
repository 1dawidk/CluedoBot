#include "CluedoProgress.h"

CluedoProgress::CluedoProgress() {
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
    //TODO: define probability and type
    double p=0;

    if(type==CLUEDOENTRY_TYPE_SURE)
        p=1;

    entries.push_back(new CluedoEntry(player, cardGroup, cardIdx, p));
}

void CluedoProgress::resolve() {
    for(int i=0; i<players.size(); i++){
        players[i]->update(&entries);
    }

    //TODO: Gether players data
}

int CluedoProgress::getEntriesNo() {
    return (int)entries.size();
}

string CluedoProgress::getEntryString(int idx, string *names) {
    return entries[idx]->toString(names);
}


void CluedoProgress::getBestBet(int *c, int *w, int *p) {
    *c= 0;
    *w= 0;
    *p= 0;
}
