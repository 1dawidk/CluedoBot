#include "DSSentence.h"

DSSentence::DSSentence(int group, int idx, double mass, bool type) {
    this->cardGroup= group;
    this->cardIdx= idx;
    this->m= mass;
    this->pos= type;
}
