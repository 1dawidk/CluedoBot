#include "CluedoProgress.h"

CluedoProgress::CluedoProgress() {
}


void CluedoProgress::addEntry(int player, int cardGroup, int cardIdx, int type) {

    //TODO: define probability
    entries.push_back(new CluedoEntry(player, cardGroup, cardIdx, 1.0));
}


CluedoProgress::~CluedoProgress() {
    for(int i=0; i<entries.size(); i++){
        delete entries[i];
    }
}
