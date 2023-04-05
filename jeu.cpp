#include "jeu.h"

void Jeu::ajouterJoueur(bool estHumain) {
    _joueurs.push_back(new Joueur());
}

void Jeu::jouer() {
    if (_joueurs.size() == 0) return; //L'état du jeu n'évolue pas après cette ligne s'il n'y a aucun joueur dans la partie
    for (int i = 0; i < 10; i++) {
        std::cout<<"Tour du joueur "<<i%_joueurs.size()<<std::endl;
        _joueurs[i%_joueurs.size()]->jouer();
    }
}