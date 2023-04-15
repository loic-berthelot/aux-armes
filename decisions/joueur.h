#pragma once
#include "../monde/carte.h"

class Joueur {
public:
    virtual void jouerArmee(Carte & carte)=0;
};