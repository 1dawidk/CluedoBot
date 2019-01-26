#ifndef CLUEDOBOT_CLUEDOPROGRESS_H
#define CLUEDOBOT_CLUEDOPROGRESS_H


#include "CluedoEntry.h"
#include "CluedoCard.h"
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
    void printEntry(int idx, string *names, int y, int x);

    void getBestBet(int *c, int *w, int *p);

    double getCharacterP(int idx);
    double getWeaponP(int idx);
    double getPlaceP(int idx);
private:
    vector<CluedoEntry*> entries;
    vector<Player*> players;

    double charactersPs[CLUEDOCARD_C_NO];
    double weaponsPs[CLUEDOCARD_W_NO];
    double placesPs[CLUEDOCARD_P_NO];
};


#endif //CLUEDOBOT_CLUEDOPROGRESS_H
