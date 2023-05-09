#pragma once
#include <iostream>

enum class TypeOrdre { DEPLACER, ATTAQUER, IMMOBILISER };

class Ordre {
    TypeOrdre _type;
    int _posX;
    int _posY;
public :
    Ordre(TypeOrdre type, int posX, int posY);
    TypeOrdre getType() const;
    std::pair<int,int> getPos() const;
};