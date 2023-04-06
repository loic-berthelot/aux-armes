#include "armee.h"

class Carte {
    std::vector<Armee*> _armees;
public:
    void creerArmee() { _armees.push_back(new Armee); }
    void afficher();
    Armee* getArmee(unsigned int i) { return _armees[i]; }
};