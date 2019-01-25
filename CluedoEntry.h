#ifndef CLUEDOBOT_CLUEDOENTRY_H
#define CLUEDOBOT_CLUEDOENTRY_H


class CluedoEntry {
public:
    CluedoEntry(int player, int cardGroup, int cardIndex, double p);

private:
    int player;
    int cardGroup;
    int cardIndex;

    double p;

};


#endif //CLUEDOBOT_CLUEDOENTRY_H
