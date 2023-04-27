#include "jeu.h"


Jeu::Jeu(std::string const & joueurs,std::string const &configurationMap, std::string const &armeeDesc){
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
    for (unsigned int i = 0; i < joueurs.size(); i++){
        switch(joueurs[i]) {
            case 'i' : ajouterJoueur(); break;
            default:
            case 'h' : ajouterJoueur(true); break;
        }
    }
    
}


void Jeu::ajouterJoueur(bool estHumain) {
    if (estHumain) _joueurs.emplace_back(new Humain());
    else _joueurs.emplace_back(new IA());
}

bool Jeu::partieFinie() {
    //return _toursPasses > 2;
    return (_carte->nombreArmeesVivantes() <= 1 || _toursPasses > _carte->getMaxTours());
}

void Jeu::jouer() {
    std::cout<<"Debut de la partie !"<<std::endl;
    while (!partieFinie()) {
        std::cout<<"---Tour n°"<<_toursPasses<<"---"<<std::endl;
        for (unsigned int i = 0; i < _joueurs.size(); i++) {
            std::cout<<"Tour du joueur "<<i<<" : "<<std::endl;
            _carte->selectionnerArmee(i);            

            //Il faut calculer le brouillard de guerre par ici
            _carte->evolutionMoralArmee();            
            _carte->ravitaillerArmee();
            _carte->appliquerAttritionArmee();
            _carte->brouillardDeGuerreEquipe();
            _joueurs[i]->jouerArmee(*_carte);
            _carte->executerOrdresArmee();
            
            _carte->retirerCadavres();
            _carte->afficherArmee();            
            std::cout<<std::endl;
        }  
        _toursPasses++;
    }
    std::cout<<"Fin de la partie ! Affichage des résultats. ----------------"<<std::endl<<"Survivant(s) : "<<std::endl;
    for (unsigned int i = 0; i < _carte->getNbArmee();i++)
        if (_carte->getArmee(i)->taille() > 0)
            std::cout << "Equipe : "<<i<<std::endl;
        
    //affichage classement
    std::cout <<"Classement\n";
    std::vector<std::pair<unsigned int, int>> scoreEquipe = _carte->getScoreEquipe();
    int places = 1;
    std::sort(scoreEquipe.begin(), scoreEquipe.end(), compareScoreDecroissant);
    bool estMort = false;
    for (unsigned int i = 0; i < scoreEquipe.size();i++)
        if (i > 1 && scoreEquipe[i].second == scoreEquipe[i-1].second)
            std::cout << " Equipe : "<<scoreEquipe[i].first<< ";";
        else{
            if (scoreEquipe[i].second < 0 && !estMort){
                std::cout << "\n\nEquipes éliminées (classées en fonction des dégâts infligés) -------------\n";
                places = 1;
            }if (scoreEquipe[i].second < 0){
                estMort = true;
                std::cout << "\nPlace : "<<places<<" - Equipe : "<<scoreEquipe[i].first;
            }
            else
                std::cout << "\nPlace : "<<places<<"- Score : "<<scoreEquipe[i].second<<" Equipe : "<<scoreEquipe[i].first;
            places++;
        }
    
    std::cout << "\n";

}


bool Jeu::compareScoreDecroissant(const std::pair<unsigned int, int>& a, const std::pair<unsigned int, int>& b) {
    return a.second > b.second;
}
