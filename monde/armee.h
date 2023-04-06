#include <vector>
#include "unite.h"

class Armee {
    std::vector<Unite> _unites;
public :
    Armee();
    void afficher();
    unsigned int taille() { return _unites.size(); }
    void donnerOrdre(unsigned int indiceUnite, Ordre* ordre) { _unites[indiceUnite].donnerOrdre(ordre); }
    void executerOrdresTour();
};