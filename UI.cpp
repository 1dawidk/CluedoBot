#include "UI.h"


void UI::init() {
    initscr();
    playersNo=0;
    state= EnterPNO;
}

void UI::onLoop() {
    char inbuff[512];

    clear();
    drawStdFrame();

    move(34, 1);
    if(state==EnterPNO){
        printw("Enter number of players");
    } else if(state==EnterNames){
        printw("Enter player names (comma separated)");
    } else if(state==EnterKnownCards){
        printw("Enter known cards (type [0-character, 1-weapon, 2-place], index) (q to finish)");
    } else if(state==EnterQuestion){
        printw("What was the question? (Character, Weapon, Place, Player)");
    } else if(state==EnterResponse){
        printw("Who showed something?");
    }

    move(35, 5);
    printw("Input: ");
    refresh();
    getstr(inbuff);
    string input(inbuff);

    if(state==EnterPNO){
        playersNo= stoi(inbuff);
        names= new string[playersNo];
        state= EnterNames;
    } else if(state==EnterNames){
        //TODO: Split and get names
        size_t lastPos=0;
        size_t nextPos=0;

        for(int i=0; i<playersNo; i++){
            nextPos= input.find(',', lastPos);
            names[i]= input.substr(lastPos, nextPos-lastPos);

            lastPos= nextPos+2;
        }

        state= EnterKnownCards;
    } else if(state==EnterKnownCards){
        if(input=="q"){
            state= EnterQuestion;
        } else {
            //TODO: add known card
        }
    } else if(state==EnterQuestion){
        //TODO: get command or question
        state= EnterResponse;
    } else if(state==EnterResponse){
        //TODO: get response
        state= EnterQuestion;
    }
}

void UI::end() {
    endwin();
}

void UI::drawStdFrame() {

    mvprintw(0, 120, "Characters:");
    for(int i=0; i<CLUEDOCARD_CHARACTERS_NO; i++){
        mvprintw(i+1, 120, "%d - %s", i, CluedoCard::characters[i].c_str());
    }

    mvprintw(CLUEDOCARD_CHARACTERS_NO+2, 120, "Weapons:");
    for(int i=0; i<CLUEDOCARD_WEAPONS_NO; i++){
        mvprintw(CLUEDOCARD_CHARACTERS_NO+3+i, 120,
                "%d - %s", i, CluedoCard::weapons[i].c_str());
    }

    mvprintw(CLUEDOCARD_CHARACTERS_NO+4+CLUEDOCARD_CHARACTERS_NO, 120, "Places:");
    for(int i=0; i<CLUEDOCARD_PLACES_NO; i++){
        mvprintw(CLUEDOCARD_WEAPONS_NO+CLUEDOCARD_CHARACTERS_NO+5+i, 120,
                "%d - %s\n\r", i, CluedoCard::places[i].c_str());
    }

    if(playersNo>0){
        mvprintw(CLUEDOCARD_CHARACTERS_NO+CLUEDOCARD_WEAPONS_NO+CLUEDOCARD_PLACES_NO+6, 120, "Players:");
        for(int i=0; i<playersNo; i++){
            mvprintw(CLUEDOCARD_CHARACTERS_NO+CLUEDOCARD_WEAPONS_NO+CLUEDOCARD_PLACES_NO+7+i, 120,
                    "%d - %s", i, names[i].c_str());
        }
    }

    refresh();
}

