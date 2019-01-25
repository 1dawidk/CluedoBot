#include "CluedoCard.h"

const string CluedoCard::characters[]={"Zielnicki", "Gorczynski", "Kostkiewicz", "Purpulewski", "Gwiazdowicz", "Siwecka"};
const string CluedoCard::weapons[]={"Klucz francuski", "Swiecznik", "Sztylet", "Rewolwer", "Olowiana rura", "Lina"};
const string CluedoCard::places[]={"Lazienka", "Gabinet", "Jadalnia", "Pokoj bilardowy", "Garaz", "Sypialnia", "Salon", "Kuchnia", "Hol"};

CluedoCard::CluedoCard(int playersNo) {
    this->playersNo= playersNo;

    checkedCharacters= new char[playersNo*CLUEDOCARD_CHARACTERS_NO];
    checkedWeapons= new char[playersNo*CLUEDOCARD_WEAPONS_NO];
    checkedPlaces= new char[playersNo*CLUEDOCARD_PLACES_NO];
}

CluedoCard::~CluedoCard() {
    delete[] checkedPlaces;
    delete[] checkedWeapons;
    delete[] checkedCharacters;
}

void CluedoCard::check(char field, char idx, char p, char type) {
    if(field==CLUEDOCARD_CHECK_CHARACTER)
        *(checkedCharacters+(idx*playersNo)+p)= type;
    else if(field==CLUEDOCARD_CHECK_WEAPON)
        *(checkedWeapons+(idx*playersNo)+p)= type;
    else if(field==CLUEDOCARD_CHECK_PLACE)
        *(checkedPlaces+(idx*playersNo)+p)= type;
}

double CluedoCard::resolve(int *c, int *w, int *p) {
    return 0;
}
