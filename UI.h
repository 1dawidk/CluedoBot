#ifndef CLUEDOBOT_UI_H
#define CLUEDOBOT_UI_H

#include <ncurses.h>
#include "CluedoCard.h"
#include "CluedoProgress.h"

#define FRAME_START_X  113
#define FRAME_END_X    146

class UI {
public:
    enum States {EnterPNO, EnterNames, EnterKnownCards, EnterQuestion, EnterResponse, Result, Quit};


    void init();
    void onLoop();
    void end();

    States getState();
private:
    void removeInputBlanks(string *input);

    int findPlayerIdx(const string &name);

    void drawStdFrame();
    void drawFrameHLine(int y);
    void capitalizeNames(string *names, int playersNo);

    CluedoProgress *cluedoProgress;
    int playersNo;
    string *names;
    States state;

    int aplayer;
    int qc;
    int qw;
    int qp;
    int qplayer;

    const static string logo_l1;
    const static string logo_l2;
    const static string logo_l3;
    const static string logo_l4;
    const static string logo_l5;
    const static string logo_l6;

    const static string text;
    string lastError;
};


#endif //CLUEDOBOT_UI_H
