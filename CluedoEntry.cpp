#include "CluedoEntry.h"
#include "CluedoCard.h"

#include "ncurses.h"

CluedoEntry::CluedoEntry(int player, int cardGroup, int cardIndex, double p){
    this->player= player;
    this->cardGroup= cardGroup;
    this->cardIndex= cardIndex;
    this->p= p;
}

void CluedoEntry::print(string *playerNames, int y, int x) {
    string ret= playerNames[player]+": ";

    move(y, x);
    attron(COLOR_PAIR(1));
    printw(playerNames[player].c_str());
    attroff(COLOR_PAIR(1));
    printw(" has ");


    if(cardGroup==0) {
        printw("character ");
        attron(COLOR_PAIR(1));
        printw("%s ", CluedoCard::characters[cardIndex].c_str());
        attroff(COLOR_PAIR(1));
    } else if(cardGroup==1) {
        printw("weapon ");
        attron(COLOR_PAIR(1));
        printw("%s ", CluedoCard::weapons[cardIndex].c_str());
        attroff(COLOR_PAIR(1));
    } else {
        printw("place ");
        attron(COLOR_PAIR(1));
        printw("%s ", CluedoCard::places[cardIndex].c_str());
        attroff(COLOR_PAIR(1));
    }

    printw("with p= %.3f", p);
}

int CluedoEntry::getPlayer() const {
    return player;
}

int CluedoEntry::getCardGroup() const {
    return cardGroup;
}

int CluedoEntry::getCardIndex() const {
    return cardIndex;
}

double CluedoEntry::getP() const {
    return p;
}
