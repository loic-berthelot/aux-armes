#include "carte.h"

void Carte::afficher() { //n'affiche pour l'instant que les armées, mais il faudra rajouter les cases
    for (unsigned int i = 0; i < _armees.size(); i++) 
        _armees[i]->afficher();
}

void Carte::executerOrdresTour(unsigned int indiceArmee) {
    _armees[indiceArmee]->executerOrdresTour();
}