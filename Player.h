#ifndef CLUEDOBOT_PLAYER_H
#define CLUEDOBOT_PLAYER_H

#include <string>
#include <vector>
#include "CluedoEntry.h"

using namespace std;

class Player {
public:
    Player(int idx, string &name);

    void update(vector<CluedoEntry*> *entries);
private:
    string name;
    int idx;
};


#endif //CLUEDOBOT_PLAYER_H
