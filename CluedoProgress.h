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

    void addPlayer(int idx, string &name, int cardsInHandNo);
    char addEntry(int player, int cardGroup, int cardIdx, int type);
    char addAnswer(int player, int *cardGroups, int *cardIdxs);
    void resolve();

    int getEntriesNo();
    void printEntry(int idx, string *names, int y, int x);

    void getBestBet(int *c, int *w, int *p);


    double getP(int group, int idx);

    void removeLastEntry();
private:
    vector<CluedoEntry*> entries;
    vector<Player*> players;

    int groupPsStartIdx[3];

    double ps[CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO];

    char checkIfBotHaveIt(int cardGroup, int cardIdx);
};


#endif //CLUEDOBOT_CLUEDOPROGRESS_H
