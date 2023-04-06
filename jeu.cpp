#include "jeu.h"

void Jeu::ajouterJoueur(bool estHumain) {
    if (estHumain) _joueurs.emplace_back(new Humain());
    else _joueurs.emplace_back(new IA());
}

bool Jeu::partieFinie() {
    return _toursPasses > 10;
}

void Jeu::initialiser() {
    _carte.creerArmee();
    ajouterJoueur();
}

void Jeu::jouer() {
    while (! partieFinie()) {
        std::cout<<"Tour n°"<<_toursPasses<<std::endl;
        for (unsigned int i = 0; i < _joueurs.size(); i++) {
            std::cout<<"Tour du joueur "<<i<<std::endl;
            _joueurs[i]->jouerArmee(_carte);
        }        

        _toursPasses++;
    }
}