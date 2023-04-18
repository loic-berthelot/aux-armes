#include "jeu.h"

Jeu::Jeu() : _carte(3) {
    //Joueur 0 : Humain
    _carte.creerArmee();
    ajouterJoueur(true);

    //Joueur 1 : IA
    _carte.creerArmee();
    ajouterJoueur();
}

void Jeu::ajouterJoueur(bool estHumain) {
    if (estHumain) _joueurs.emplace_back(new Humain());
    else _joueurs.emplace_back(new IA());
}

bool Jeu::partieFinie() {
    return _toursPasses > 10;
}

void Jeu::jouer() {
    std::cout<<"Debut de la partie !"<<std::endl;
    while (! partieFinie()) {
        std::cout<<"---Tour n°"<<_toursPasses<<"---"<<std::endl;
        for (unsigned int i = 0; i < _joueurs.size(); i++) {
            std::cout<<"Tour du joueur "<<i<<" : "<<std::endl;
            _carte.selectionnerArmee(i);
            _joueurs[i]->jouerArmee(_carte);

            //Il faut calculer le brouillard de guerre par ici
            _carte.ravitaillerArmee();
            _carte.appliquerAttritionArmee();
            _carte.executerOrdresArmee();
            _carte.afficherArmee();
            std::cout<<std::endl;
        }  
        _toursPasses++;
    }
    std::cout<<"Fin de la partie !"<<std::endl;
}