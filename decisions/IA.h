#include "joueur.h"

class IA : public Joueur {
public:
    void jouerArmee(Carte & carte) override;
};