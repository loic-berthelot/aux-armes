#include "jeu.h"


Jeu::Jeu(std::vector<std::shared_ptr<Armee>> const &toutesArmees, unsigned int tailleMap):_carte(tailleMap, toutesArmees){
    for (unsigned int i = 0; i < toutesArmees.size();i++){
        ajouterJoueur();
    }
    
}


void Jeu::ajouterJoueur(bool estHumain) {
    if (estHumain) _joueurs.emplace_back(new Humain());
    else _joueurs.emplace_back(new IA());
}

bool Jeu::partieFinie() {
    //return _toursPasses > 2;
    return (_carte.nombreArmeesVivantes() <= 1);
}

void Jeu::jouer() {
    std::cout<<"Debut de la partie !"<<std::endl;
    while (!partieFinie()) {
        std::cout<<"---Tour n°"<<_toursPasses<<"---"<<std::endl;
        for (unsigned int i = 0; i < _joueurs.size(); i++) {
            std::cout<<"Tour du joueur "<<i<<" : "<<std::endl;
            _carte.selectionnerArmee(i);
            _joueurs[i]->jouerArmee(_carte);

            //Il faut calculer le brouillard de guerre par ici
            _carte.ravitaillerArmee();
            _carte.appliquerAttritionArmee();
            _carte.executerOrdresArmee();
            _carte.retirerCadavres();
            _carte.afficherArmee();            
            std::cout<<std::endl;
        }  
        _toursPasses++;
    }
    std::cout<<"Fin de la partie !"<<std::endl;
}