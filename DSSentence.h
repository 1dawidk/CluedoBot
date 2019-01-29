#ifndef CLUEDOBOT_DSSENTENCE_H
#define CLUEDOBOT_DSSENTENCE_H


class DSSentence {
public:
    DSSentence(int group, int idx, double mass, bool type);

private:
    bool pos;
    int cardIdx;
    int cardGroup;
    double m;
};


#endif //CLUEDOBOT_DSSENTENCE_H
