#include "carte.h"

void Carte::creerArmee() { _armees.push_back(new Armee()); }


void Carte::afficher() const{ //n'affiche pour l'instant que les armées, mais il faudra rajouter les cases
    for (unsigned int i = 0; i < _armees.size(); i++) 
        _armees[i]->afficher();
}

void Carte::executerOrdresTour(unsigned int indiceArmee) {
    _armees[indiceArmee]->executerOrdresTour();
}


Armee* Carte::getArmee(unsigned int i) const{ return _armees[i]; }


/*getters and setters*/

std::vector<std::vector<Case>> Carte::getCarte()const{
    return _carte;
}