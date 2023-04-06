#pragma once
#include "../monde/carte.h"

class Joueur {
protected:
    unsigned int _idArmee;
public:
    Joueur() { _idArmee = 0; }
    virtual void jouerArmee(Carte & carte)=0;
};