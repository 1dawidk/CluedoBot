#ifndef CLUEDOBOT_PLAYER_H
#define CLUEDOBOT_PLAYER_H

#include <string>
#include <vector>
#include "CluedoEntry.h"
#include "CluedoCard.h"

using namespace std;

class Player {
public:
    Player(int idx, string &name, int cardsInHandNo);

    void update(vector<CluedoEntry*> *entries);
    int* getBestCards();
    double getP(int g, int i);
private:
    string name;
    int idx;
    int cardsInHandNo;

    double ps[CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO];
    int groupPsStartIdx[3];
};


#endif //CLUEDOBOT_PLAYER_H
