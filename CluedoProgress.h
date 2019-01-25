#ifndef CLUEDOBOT_CLUEDOPROGRESS_H
#define CLUEDOBOT_CLUEDOPROGRESS_H


#include "CluedoEntry.h"
#include <vector>

using namespace std;

class CluedoProgress {
public:
    CluedoProgress();
    ~CluedoProgress();

    void addEntry(int player, int cardGroup, int cardIdx, int type);
private:
    vector<CluedoEntry*> entries;
};


#endif //CLUEDOBOT_CLUEDOPROGRESS_H
