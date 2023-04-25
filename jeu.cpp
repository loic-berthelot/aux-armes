#include "jeu.h"


Jeu::Jeu(unsigned int tailleMap, std::string const &armeeDesc){
    unsigned int team = 0;
    std::vector<std::shared_ptr<Armee>> buffer;
    std::shared_ptr<Armee> armeeCourante = std::make_shared<Armee>();
    unsigned int nbUnites = 0;
    unsigned int indexPrecedent = 0;
    for (unsigned int i = 0; i < armeeDesc.size();i++){
        
        if (armeeDesc[i] == ';'){
            buffer.push_back(armeeCourante);
            armeeCourante = std::make_shared<Armee>();
            team++;
            indexPrecedent = i+1;
        }else if (armeeDesc[i] == ':'){
            nbUnites = std::stoul(armeeDesc.substr(indexPrecedent, i-indexPrecedent));
            indexPrecedent = i+1;
        }else if (armeeDesc[i] == ','){
            for (unsigned int j = 0; j < nbUnites;j++)
                armeeCourante->ajoutUnite(std::make_shared<Unite>(armeeDesc.substr(indexPrecedent ,i-indexPrecedent), 0, 0));
            indexPrecedent = i+1;
        }
    }
    
    _carte = std::make_unique<Carte>(tailleMap, buffer);
    for (unsigned int i = 0; i < team;i++){
        ajouterJoueur();
    }
    
}


void Jeu::ajouterJoueur(bool estHumain) {
    if (estHumain) _joueurs.emplace_back(new Humain());
    else _joueurs.emplace_back(new IA());
}

bool Jeu::partieFinie() {
    //return _toursPasses > 2;
    return (_carte->nombreArmeesVivantes() <= 1);
}

void Jeu::jouer() {
    std::cout<<"Debut de la partie !"<<std::endl;
    while (!partieFinie()) {
        std::cout<<"---Tour n°"<<_toursPasses<<"---"<<std::endl;
        for (unsigned int i = 0; i < _joueurs.size(); i++) {
            std::cout<<"Tour du joueur "<<i<<" : "<<std::endl;
            _carte->selectionnerArmee(i);
            _joueurs[i]->jouerArmee(*_carte);

            //Il faut calculer le brouillard de guerre par ici
            _carte->evolutionMoralArmee();
            _carte->ravitaillerArmee();
            _carte->appliquerAttritionArmee();
            _carte->executerOrdresArmee();
            
            _carte->retirerCadavres();
            _carte->afficherArmee();            
            std::cout<<std::endl;
        }  
        _toursPasses++;
    }
    std::cout<<"Fin de la partie !"<<std::endl;
}


Jeu::Jeu(std::string const &configurationMap, std::string const &armeeDesc){
    unsigned int team = 0;
    std::vector<std::shared_ptr<Armee>> buffer;
    std::shared_ptr<Armee> armeeCourante = std::make_shared<Armee>();
    unsigned int nbUnites = 0;
    unsigned int indexPrecedent = 0;
    for (unsigned int i = 0; i < armeeDesc.size();i++){
        
        if (armeeDesc[i] == ';'){
            buffer.push_back(armeeCourante);
            armeeCourante = std::make_shared<Armee>();
            team++;
            indexPrecedent = i+1;
        }else if (armeeDesc[i] == ':'){
            nbUnites = std::stoul(armeeDesc.substr(indexPrecedent, i-indexPrecedent));
            indexPrecedent = i+1;
        }else if (armeeDesc[i] == ','){
            for (unsigned int j = 0; j < nbUnites;j++)
                armeeCourante->ajoutUnite(std::make_shared<Unite>(armeeDesc.substr(indexPrecedent ,i-indexPrecedent), 0, 0));
            indexPrecedent = i+1;
        }
    }
    
    _carte = std::make_unique<Carte>(configurationMap, buffer);
    for (unsigned int i = 0; i < team;i++){
        ajouterJoueur();
    }
}