#include "joueur.h"

class IA : public Joueur {
public:
    IA() : Joueur() {}
    void jouerArmee(Carte & carte) override;
};