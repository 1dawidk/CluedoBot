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

    void drawStdFrame();
    void drawFrameHLine(int y);

    CluedoProgress *cluedoProgress;
    int playersNo;
    string *names;
    States state;

    int qc;
    int qw;
    int qp;
};


#endif //CLUEDOBOT_UI_H
