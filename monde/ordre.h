#pragma once
#include <iostream>

enum TypeOrdre { ORDRE_DEPLACER, ORDRE_ATTAQUER };

class Ordre {
    TypeOrdre _type;
    int _posX;
    int _posY;
public :
    Ordre(TypeOrdre type, int posX, int posY);
    TypeOrdre getType() const;
    std::pair<int,int> getPos() const;
};