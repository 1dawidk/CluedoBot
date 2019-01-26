#ifndef CLUEDOBOT_CLUEDOCARD_H
#define CLUEDOBOT_CLUEDOCARD_H

#include <string>

#define CLUEDOCARD_C_NO    6
#define CLUEDOCARD_W_NO    6
#define CLUEDOCARD_P_NO    9

using namespace std;

class CluedoCard {
public:
    static const string characters[];
    static const string weapons[];
    static const string places[];
    static const int cardsInGroup[];
};


#endif //CLUEDOBOT_CLUEDOCARD_H
