#include "joueur.h"

class Humain : public Joueur {
public:
    void jouerArmee(Carte & carte) override;
};