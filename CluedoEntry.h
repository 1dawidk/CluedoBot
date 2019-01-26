#ifndef CLUEDOBOT_CLUEDOENTRY_H
#define CLUEDOBOT_CLUEDOENTRY_H

#define CLUEDOENTRY_TYPE_SURE       0
#define CLUEDOENTRY_TYPE_ANSWER     1

#include <string>

using namespace std;

class CluedoEntry {
public:
    CluedoEntry(int player, int cardGroup, int cardIndex, double p);

    void print(string *playerNames, int y, int x);

    int getPlayer() const;

    int getCardGroup() const;

    int getCardIndex() const;

    double getP() const;

private:
    int player;
    int cardGroup;
    int cardIndex;

    double p;

};


#endif //CLUEDOBOT_CLUEDOENTRY_H
