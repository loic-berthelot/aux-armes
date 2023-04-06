#include "joueur.h"

class Humain : public Joueur {
public:
    Humain() : Joueur() {}
    void jouerArmee(Carte & carte) override;
};