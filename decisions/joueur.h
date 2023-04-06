#pragma once
#include "../monde/carte.h"

class Joueur {
    unsigned int _idArmee = 0;
public:
    void jouerArmee(Carte & carte);
};