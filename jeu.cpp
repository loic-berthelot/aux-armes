#include "jeu.h"

void Jeu::ajouterJoueur(bool estHumain) {
    if (estHumain) _joueurs.emplace_back(new Humain());
    else _joueurs.emplace_back(new IA());
}

bool Jeu::partieFinie() {
    return _toursPasses > 10;
}

void Jeu::initialiser() {
    //Joueur 0 : IA
    _carte.creerArmee();
    ajouterJoueur();

    //Joueur 1 : Humain
    _carte.creerArmee();
    ajouterJoueur(true);
}

void Jeu::jouer() {
    std::cout<<"Debut de la partie !"<<std::endl;

    


    while (! partieFinie()) {
        std::cout<<"Tour n°"<<_toursPasses<<std::endl;
        for (unsigned int i = 0; i < _joueurs.size(); i++) {
            std::cout<<"Tour du joueur "<<i<<std::endl;
            _joueurs[i]->jouerArmee(_carte);
            _carte.executerOrdresArmee(i);
            _carte.afficher();
        }   

        std::cout<<std::endl<<"_____________"<<std::endl;
        _toursPasses++;
    }
    std::cout<<"Fin de la partie !"<<std::endl;
}