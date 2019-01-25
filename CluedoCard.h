#ifndef CLUEDOBOT_CLUEDOCARD_H
#define CLUEDOBOT_CLUEDOCARD_H

#include <string>

#define CLUEDOCARD_CHARACTERS_NO    6
#define CLUEDOCARD_WEAPONS_NO       6
#define CLUEDOCARD_PLACES_NO        9

#define CLUEDOCARD_CHECK_CHARACTER  0
#define CLUEDOCARD_CHECK_WEAPON     1
#define CLUEDOCARD_CHECK_PLACE      2

#define CLUEDOCARD_CHECKTYPE_NONE       0
#define CLUEDOCARD_CHECKTYPE_ANSWER     1
#define CLUEDOCARD_CHECKTYPE_FORSURE    2

using namespace std;

class CluedoCard {
public:
    static const string characters[];
    static const string weapons[];
    static const string places[];

    CluedoCard(int playersNo);
    ~CluedoCard();

    /**
     *
     * @param field - in which filed to check: characters, weapons, places
     * @param idx - index of weapon, character or place
     * @param p - player idx
     * @param type - check type: none, answer, forsure
     */
    void check(char field, char idx, char p, char type);

    /**
     * @param c - character who killed idx (returned)
     * @param w - weapon used idx (returned)
     * @param p - place where it happened idx (returned)
     * @return - probability of above parameters are true
     */
    double resolve(int *c, int *w, int *p);
private:
    int playersNo;

    //Columns - players
    char *checkedCharacters;
    char *checkedPlaces;
    char *checkedWeapons;

};


#endif //CLUEDOBOT_CLUEDOCARD_H
