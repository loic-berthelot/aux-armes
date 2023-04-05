#include "joueur.h"

Joueur::Joueur() {
    _unites.emplace_back("T34", 1, 1);
    _unites.emplace_back("CRS", 2, 3);
}

void Joueur::jouer() {
    for (unsigned int i = 0; i < _unites.size(); i++) {
        std::cout<<_unites[i].getType()<<std::endl;
    }
}