#include <algorithm>
#include "UI.h"

const string UI::logo_l1= "   _____   _                      _                 ____            _   \n";
const string UI::logo_l2= "  / ____| | |                    | |               |  _ \\          | |  \n";
const string UI::logo_l3= " | |      | |  _   _    ___    __| |   ___         | |_) |   ___   | |_ \n";
const string UI::logo_l4= " | |      | | | | | |  / _ \\  / _` |  / _ \\        |  _ <   / _ \\  | __|\n";
const string UI::logo_l5= " | |____  | | | |_| | |  __/ | (_| | | (_) |       | |_) | | (_) | | |_ \n";
const string UI::logo_l6= "  \\_____| |_|  \\__,_|  \\___|  \\__,_|  \\___/        |____/   \\___/   \\__|";

const string UI::text = "     Bot gry Cluedo ktory na podstawie zadawanych pytan i informacji o\n"
                        "     tym kto pokazal karte do danego pytania ustala kto jest w magicznej\n"
                        "     kopercie."
                        "\n"
                        "     Dane zbierane jako wpisy:\n"
                        "          - jakie kart sa znane wszystkim\n"
                        "          - jakie karty posiadam w rece\n"
                        "          - pytan i odpowiedzi:\n"
                        "              + jakich kart dotyczylo pytanie\n"
                        "              + kto pokazal karte\n"
                        "\n"
                        "     Wynik jest reprezentowany jako wartosc prawdopodobienstwa wystąpienia\n"
                        "     danej karty w magicznej kopercie\n"
                        "\n"
                        "     Do wynaczania tego prawdopodobienstwa użyta jest teoria Demstera-Schafera w celu\n"
                        "     okreslenia prawdopodobienstw posiadania danych kart przez kolejnych graczy.\n"
                        "\n"
                        "     Jezeli np pytanie dotyczylo kart 1, 1, 4 i odpowiedzial gracz 2 to twierdzimy ze\n"
                        "     z pstwem 0.333 gracz 2 posiada jedna z tych kart jest prawda natomiast dla pozostalych\n"
                        "     graczy twierdzimy że z pewnym pstwem NIE posiadaja jednej z kart.\n"
                        "\n"
                        "\n"
                        "     Zakladamy ze pierwszy gracz (index 0) to bot";

void UI::init() {
    initscr();

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

    playersNo=0;
    state= EnterPNO;
    cluedoProgress= new CluedoProgress();

    attron(COLOR_PAIR(2));
    mvprintw(1, 30, logo_l1.c_str());
    mvprintw(2, 30, logo_l2.c_str());
    mvprintw(3, 30, logo_l3.c_str());
    mvprintw(4, 30, logo_l4.c_str());
    mvprintw(5, 30, logo_l5.c_str());
    mvprintw(6, 30, logo_l6.c_str());
    attroff(COLOR_PAIR(2));

    mvprintw(10, 0, text.c_str());

    mvprintw(35, 120, "Press [ENTER] to start...");
    getch();
}

void UI::onLoop() {
    char inbuff[512];

    clear();
    drawStdFrame();

    move(33, 1);
    if(state==EnterPNO){
        printw("Enter number of players");
    } else if(state==EnterNames){
        printw("Enter player names");
        mvprintw(34, 1, "( comma separated )");
    } else if(state==EnterKnownCards){
        printw("Enter known cards");
        mvprintw(34, 1, "( type [0-character, 1-weapon, 2-place], index OR \"q\" )");
    } else if(state==EnterQuestion){
        printw("What was the question?");
        mvprintw(34, 1, "( character, weapon, place OR \"quit\" OR \"remove_last\" )");
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
        mvprintw(34, 1, "( index, name OR \"noone\" )");
    } else if(state==Result){
        int c, w, p;
        cluedoProgress->getBestBet(&c, &w, &p);
        printw("I bet %s killed with %s in %s [Press enter to continue...]", CluedoCard::characters[c].c_str(), CluedoCard::weapons[w].c_str(), CluedoCard::places[p].c_str());
    }

    if(!lastError.empty()){
        attron(COLOR_PAIR(3));
        mvprintw(0,0,lastError.c_str());
        attroff(COLOR_PAIR(3));
        lastError="";
    }

    mvprintw(35, 5, "Input: ");
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
                cluedoProgress->addPlayer(i, names[i], ((CLUEDOCARD_C_NO+CLUEDOCARD_P_NO+CLUEDOCARD_W_NO)/playersNo));

                lastPos = nextPos + 1;
            }
            capitalizeNames(names, playersNo);

            state = EnterKnownCards;
        } else if (state == EnterKnownCards) {
            //Check command on input
            if (input == "q") {
                state = EnterQuestion;
            } else {
                //Get card type and idx
                string cardType;
                string cardIdx;

                size_t comapos = input.find(',');

                cardType = input.substr(0, comapos);
                cardIdx = input.substr(comapos + 1);

                char r= cluedoProgress->addEntry(0, stoi(cardType), stoi(cardIdx), CLUEDOENTRY_TYPE_SURE);
                if(!r){
                    lastError="Bot already has this card";
                }
            }
        } else if (state == EnterQuestion) {
            //Check command on input
            if(input == "resolve"){
                state= Result;
            } else if(input == "quit") {
                state= Quit;
            } else if (input=="remove_last") {
                cluedoProgress->removeLastEntry();
                cluedoProgress->resolve();
            }else {
                //Get cards idxs user asked for
                size_t firstComa= input.find(',');
                size_t secondComa= input.find(',', firstComa+1);

                if(firstComa!=string::npos && secondComa!=string::npos) {
                    qc = stoi(input.substr(0, firstComa));
                    qw = stoi(input.substr(firstComa + 1, firstComa - secondComa - 1));
                    qp = stoi(input.substr(secondComa + 1));
                    state = EnterResponse;
                } else {
                    lastError= "To few arguments :(";
                }
            }
        } else if (state == EnterResponse) {
            int myQuestion=0;
            int g=0;
            //Get question response
            if(input=="noone"){
                aplayer=-1;
                state= EnterQuestion;
            } else {
                if(input!="0") {
                    if (input.length() > 1) {
                        aplayer= stoi(input.substr(0, 1));
                        myQuestion= 1;
                        g= stoi(input.substr(2, 1));
                    } else {
                        aplayer = stoi(input);
                        if (aplayer < playersNo) {
                            state = EnterQuestion;
                        } else {
                            lastError = "Player index must be >= 0";
                            state = EnterResponse;
                        }
                    }
                }
            }

            //If all went ok
            if(state==EnterQuestion) {
                if(myQuestion){

                } else {
                    char r;
                    int cardGroups[] = {0, 1, 2};
                    int cardIndxs[] = {qc, qw, qp};

                    r = cluedoProgress->addAnswer(aplayer, cardGroups, cardIndxs);

                    if (r) {
                        lastError = "Some of those card were not added because Bot has them";
                    }
                }

                cluedoProgress->resolve();
            }

        } else if (state == Result){
            state = EnterQuestion;
        }
    } catch (const std::exception& e){
        lastError= "Something went wrong :(";
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
        mvprintw(i+3, FRAME_END_X-7, "%.3f", cluedoProgress->getP(0, i));
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
        mvprintw(CLUEDOCARD_C_NO+5+i, FRAME_END_X-7, "%.3f", cluedoProgress->getP(1, i));
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
        mvprintw(CLUEDOCARD_W_NO+CLUEDOCARD_C_NO+7+i, FRAME_END_X-7, "%.3f", cluedoProgress->getP(2, i));
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

int UI::findPlayerIdx(const string &name) {
    for(int i=0; i<playersNo; i++){
        if(name==names[i])
            return i;
    }

    return -1;
}

void UI::capitalizeNames(string *names, int playersNo) {
    for(int i=0; i<playersNo; i++){
        (*(names+i))[0]= toupper((*(names+i))[0]);
    }
}
