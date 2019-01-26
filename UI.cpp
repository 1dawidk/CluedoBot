#include <algorithm>
#include "UI.h"


void UI::init() {
    initscr();

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

    playersNo=0;
    state= EnterPNO;
    cluedoProgress= new CluedoProgress();
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
        printw("What was the question? (Character, Weapon, Place)");
    } else if(state==EnterResponse){
        printw("Who showed something? ");

        printw("( ");
        attron(COLOR_PAIR(2));
        printw(CluedoCard::characters[qc].c_str());
        attroff(COLOR_PAIR(2));

        printw(" with ");
        attron(COLOR_PAIR(2));
        printw(CluedoCard::weapons[qw].c_str());
        attroff(COLOR_PAIR(2));

        printw(" in ");
        attron(COLOR_PAIR(2));
        printw(CluedoCard::places[qp].c_str());
        attroff(COLOR_PAIR(2));

        printw(" )");

    } else if(state==Result){
        int c, w, p;
        cluedoProgress->getBestBet(&c, &w, &p);
        printw("I bet %s killed with %s in %s [Press enter to continue...]", CluedoCard::characters[c].c_str(), CluedoCard::weapons[w].c_str(), CluedoCard::places[p].c_str());
    }

    move(35, 5);
    printw("Input: ");
    refresh();
    getstr(inbuff);
    string input(inbuff);
    removeInputBlanks(&input);

    try {
        if (state == EnterPNO) {
            playersNo = stoi(inbuff);
            names = new string[playersNo];
            state = EnterNames;
        } else if (state == EnterNames) {
            size_t lastPos = 0;
            size_t nextPos = 0;

            for (int i = 0; i < playersNo; i++) {
                nextPos = input.find(',', lastPos);
                names[i] = input.substr(lastPos, nextPos - lastPos);
                cluedoProgress->addPlayer(i, names[i]);

                lastPos = nextPos + 1;
            }

            state = EnterKnownCards;
        } else if (state == EnterKnownCards) {
            if (input == "q") {
                state = EnterQuestion;
            } else {
                string cardType;
                string cardIdx;

                size_t comapos = input.find(',');

                cardType = input.substr(0, comapos);
                cardIdx = input.substr(comapos + 1);

                cluedoProgress->addEntry(0, stoi(cardType), stoi(cardIdx), CLUEDOENTRY_TYPE_SURE);
            }
        } else if (state == EnterQuestion) {
            if(input == "resolve"){
                state= Result;
            } else if(input == "quit") {
                state= Quit;
            } else {
                size_t firstComa= input.find(',');
                size_t secondComa= input.find(',', firstComa+1);

                qc= stoi(input.substr(0, firstComa));
                qw= stoi(input.substr(firstComa+1, firstComa-secondComa-1));
                qp= stoi(input.substr(secondComa+1));
                state = EnterResponse;
            }
        } else if (state == EnterResponse) {
            //TODO: get response
            state = EnterQuestion;
        } else if (state == Result){
            state = EnterQuestion;
        }
    } catch (const std::exception& e){

    }
}

void UI::end() {
    endwin();
}

void UI::drawStdFrame() {
    int c, w, p;
    cluedoProgress->getBestBet(&c, &w, &p);

    drawFrameHLine(0);
    mvprintw(1, FRAME_START_X, "#");
    mvprintw(1, FRAME_END_X, "#");


    //Print characters
    mvprintw(2, FRAME_START_X, "#   ");
    attron(COLOR_PAIR(2));
    printw("Characters:");
    attroff(COLOR_PAIR(2));
    mvprintw(2, FRAME_END_X, "#");
    for(int i=0; i<CLUEDOCARD_C_NO; i++){
        mvprintw(i+3, FRAME_START_X, "#   %d - %s", i, CluedoCard::characters[i].c_str());
        if(i==c)
            attron(COLOR_PAIR(3));
        mvprintw(i+3, FRAME_END_X-7, "%.3f", cluedoProgress->getCharacterP(i));
        if(i==c)
            attroff(COLOR_PAIR(3));
        mvprintw(i+3, FRAME_END_X, "#");
    }


    //Print weapons
    mvprintw(CLUEDOCARD_C_NO+3, FRAME_START_X, "#");
    mvprintw(CLUEDOCARD_C_NO+3, FRAME_END_X, "#");
    mvprintw(CLUEDOCARD_C_NO+4, FRAME_START_X, "#   ");
    attron(COLOR_PAIR(2));
    printw("Weapons:");
    attroff(COLOR_PAIR(2));
    mvprintw(CLUEDOCARD_C_NO+4, FRAME_END_X, "#");
    for(int i=0; i<CLUEDOCARD_W_NO; i++){
        mvprintw(CLUEDOCARD_C_NO+5+i, FRAME_START_X,
                 "#   %d - %s", i, CluedoCard::weapons[i].c_str());
        if(i==w)
            attron(COLOR_PAIR(3));
        mvprintw(CLUEDOCARD_C_NO+5+i, FRAME_END_X-7, "%.3f", cluedoProgress->getWeaponP(i));
        if(i==w)
            attroff(COLOR_PAIR(3));

        mvprintw(CLUEDOCARD_C_NO+5+i, FRAME_END_X, "#");
    }


    //Print places
    mvprintw(CLUEDOCARD_C_NO+5+CLUEDOCARD_C_NO, FRAME_START_X, "#");
    mvprintw(CLUEDOCARD_C_NO+5+CLUEDOCARD_C_NO, FRAME_END_X, "#");
    mvprintw(CLUEDOCARD_C_NO+6+CLUEDOCARD_C_NO, FRAME_START_X, "#   ");
    attron(COLOR_PAIR(2));
    printw("Places:");
    attroff(COLOR_PAIR(2));
    mvprintw(CLUEDOCARD_C_NO+6+CLUEDOCARD_C_NO, FRAME_END_X, "#");
    for(int i=0; i<CLUEDOCARD_P_NO; i++){
        mvprintw(CLUEDOCARD_W_NO+CLUEDOCARD_C_NO+7+i, FRAME_START_X,
                 "#   %d - %s\n\r", i, CluedoCard::places[i].c_str());
        if(i==p)
            attron(COLOR_PAIR(3));
        mvprintw(CLUEDOCARD_W_NO+CLUEDOCARD_C_NO+7+i, FRAME_END_X-7, "%.3f", cluedoProgress->getPlaceP(i));
        if(i==p)
            attroff(COLOR_PAIR(3));
        mvprintw(CLUEDOCARD_W_NO+CLUEDOCARD_C_NO+7+i, FRAME_END_X, "#");
    }


    mvprintw(CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO+7, FRAME_START_X, "#");
    mvprintw(CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO+7, FRAME_END_X, "#");

    //Print players
    if(playersNo>0){
        mvprintw(CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO+8, FRAME_START_X, "#   ");
        attron(COLOR_PAIR(2));
        printw("Players:");
        attroff(COLOR_PAIR(2));
        mvprintw(CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO+8, FRAME_END_X, "#");
        for(int i=0; i<playersNo; i++){
            mvprintw(CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO+9+i, FRAME_START_X,
                     "#   %d - %s", i, names[i].c_str());
            mvprintw(CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO+9+i, FRAME_END_X, "#");
        }
        mvprintw(CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO+9+playersNo, FRAME_START_X, "#");
        mvprintw(CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO+9+playersNo, FRAME_END_X, "#");
        drawFrameHLine(CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO+10+playersNo);
    } else {
        drawFrameHLine(CLUEDOCARD_C_NO+CLUEDOCARD_W_NO+CLUEDOCARD_P_NO+8);
    }



    //Print last 30 entries
    if(playersNo>0){
        attron(COLOR_PAIR(2));
        mvprintw(1, 1, "Entries:");
        attroff(COLOR_PAIR(2));
        int showEntriesNo=cluedoProgress->getEntriesNo();
        int showFrom= 0;

        if(showEntriesNo>30){
            showFrom= showEntriesNo-30;
            showEntriesNo=30;
        }


        for(int i=0; i<showEntriesNo; i++){
            cluedoProgress->printEntry(showFrom+i, names, i+2, 3);
        }
    }
}

void UI::drawFrameHLine(int y) {
    for(int i=0; i<FRAME_END_X-FRAME_START_X+1; i++){
        mvprintw(y, FRAME_START_X+i, "#");
    }
}

UI::States UI::getState() {
    return state;
}

void UI::removeInputBlanks(string *input) {
    input->erase(remove(input->begin(), input->end(), ' '), input->end());
}
