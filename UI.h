#ifndef CLUEDOBOT_UI_H
#define CLUEDOBOT_UI_H

#include <ncurses.h>
#include "CluedoCard.h"

class UI {
public:
    void init();
    void onLoop();
    void end();

private:
    enum States {EnterPNO, EnterNames, EnterKnownCards, EnterQuestion, EnterResponse, Result};

    void drawStdFrame();

    CluedoCard *cluedoCard;
    int playersNo;
    string *names;
    States state;
};


#endif //CLUEDOBOT_UI_H
