#include "CluedoEntry.h"
#include "CluedoCard.h"

CluedoEntry::CluedoEntry(int player, int cardGroup, int cardIndex, double p){
    this->player= player;
    this->cardGroup= cardGroup;
    this->cardIndex= cardIndex;
    this->p= p;
}

string CluedoEntry::toString(string *playerNames) {
    string ret= playerNames[player]+": ";

    if(cardGroup==0)
        ret+= "Character "+CluedoCard::characters[cardIndex];
    else if(cardGroup==1)
        ret+= "Weapon "+CluedoCard::weapons[cardIndex];
    else
        ret+= "Place "+CluedoCard::places[cardIndex];

    ret += "  with p= "+std::to_string(p);

    return ret;
}
