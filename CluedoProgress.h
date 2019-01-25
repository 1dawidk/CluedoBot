#ifndef CLUEDOBOT_CLUEDOPROGRESS_H
#define CLUEDOBOT_CLUEDOPROGRESS_H


#include "CluedoEntry.h"
#include "Player.h"
#include <vector>
#include <string>

using namespace std;

class CluedoProgress {
public:
    CluedoProgress();
    ~CluedoProgress();

    void addPlayer(int idx, string &name);
    void addEntry(int player, int cardGroup, int cardIdx, int type);
    void resolve();

    int getEntriesNo();
    string getEntryString(int idx, string *names);

    void getBestBet(int *c, int *w, int *p);
private:
    vector<CluedoEntry*> entries;
    vector<Player*> players;
};


#endif //CLUEDOBOT_CLUEDOPROGRESS_H
