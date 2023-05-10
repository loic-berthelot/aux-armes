#include "ordre.h"

Ordre::Ordre(TypeOrdre type, int posX, int posY) : _type(type), _posX(posX), _posY(posY) {};

TypeOrdre Ordre::getType() const {
    return _type;
}

std::pair<int,int> Ordre::getPos() const {
    return std::make_pair(_posX, _posY);
}