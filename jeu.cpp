#include "jeu.h"


Jeu::Jeu(std::string const & joueurs,std::string const &configurationMap, std::string const &armeeDesc){
    if(true) {
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
}


Jeu::Jeu(const std::string & nomFichier) {
    chargerSauvegarde(nomFichier);
}

void Jeu::ajouterJoueur(bool estHumain) {
    if (estHumain) _joueurs.emplace_back(new Humain());
    else _joueurs.emplace_back(new IA());
}

bool Jeu::partieFinie() const{
    return (_carte->nombreArmeesVivantes() <= 1 || _toursPasses > _carte->getMaxTours());
}

void Jeu::sauvegarder(const std::string & nom) {
    std::ofstream fichier("../sauvegardes/"+nom+".txt");//on doit revenir au répertoire parent pour atteindre la racine du projet depuis le répertoire build
    if (! fichier.is_open()) throw Exception("Erreur lors de l'ouverture du fichier dans Carte::sauvegarder");

    int rayon = _carte->getRayon();
    fichier<<"RAYON "+std::to_string(rayon)+"\n";
    fichier<<"TOUR "+std::to_string(_toursPasses)+"\n";
    fichier<<"TOURS_MAX "+std::to_string(_carte->getMaxTours())+"\n";
    int debut = -rayon+1;
    int fin = 0;
    for (int j = rayon-1; j > -rayon; j--) {
        for (int i = debut; i <= fin; i++) {
            fichier<<_carte->getCase(i,j)->getNom()+" "+std::to_string(i)+" "+std::to_string(j)+"\n";
        }
        if (j>0) fin++;
        else debut++;        
    }
    std::shared_ptr<Armee> armee;
    std::shared_ptr<Unite> unite;
    for (unsigned int i = 0; i < _joueurs.size(); i++){
        armee = _carte->getArmee(i);
        std::string type = (dynamic_cast<Humain*>(_joueurs[i].get())) ? "Humain" : "IA";
        fichier << "JOUEUR "+type<< std::endl;
        for (unsigned int j = 0; j < armee->size(); j++) {
            unite = armee->getUnite(j);
            fichier << unite->getNom() + " " + std::to_string(unite->getX()) + " " + std::to_string(unite->getY()) + " " + std::to_string(unite->getSante()) + " " + std::to_string(unite->getMoral()) + "\n";
        }
    }       
    fichier.close(); 
}

void Jeu::chargerSauvegarde(const std::string & nomFichier) {
    std::ifstream fichier("../sauvegardes/"+nomFichier+".txt"); // Ouverture du fichier en lecture
    if (! fichier.is_open()) throw Exception("Erreur lors de l'ouverture du fichier dans Carte::chargerSauvegarde");
    
    std::string ligne;
    std::getline(fichier, ligne);
    int rayon = std::stoi(separerChaine(ligne, ' ').at(1));
    _carte = std::make_unique<Carte>(rayon);
    std::getline(fichier, ligne);
    _toursPasses = std::stoi(separerChaine(ligne, ' ').at(1));
    std::getline(fichier, ligne);
    _carte->setMaxTours(std::stoi(separerChaine(ligne, ' ').at(1)));

    bool modeCases = true;
    std::vector<std::string> mots;
    int nombreArmees = 0;
    while (std::getline(fichier, ligne)) {
        mots = separerChaine(ligne, ' ');
        if (mots[0] == "JOUEUR") {
            modeCases = false;
            ajouterJoueur(mots[1] == "Humain");
            _carte->creerArmee();
            nombreArmees++;
        }
        else if (! ligne.empty()) { 
            if (modeCases) _carte->ajouterCase(std::make_pair(std::stoi(mots[1]),std::stoi(mots[2])),mots[0]); 
            else _carte->ajoutUniteTeam(nombreArmees-1, std::make_shared<Unite>(mots[0], std::stoi(mots[1]), std::stoi(mots[2]), std::stoi(mots[3]), std::stoi(mots[4])));
        }
    }
    _carte->initialiserGraphes();
    _carte->calculerDepartsRavitaillement();   
    _carte->affichageSeulementCarte();
}

void Jeu::jouer() {
    std::cout<<"Debut de la partie !"<<std::endl;
    sauvegarder("sauvegarde1");
    while (!partieFinie()) {
        std::cout<<"---Tour n°"<<_toursPasses<<"---"<<std::endl;
        for (unsigned int i = 0; i < _joueurs.size(); i++) {
            std::cout<<"Tour du joueur "<<i<<" : "<<std::endl;
            _carte->selectionnerArmee(i);            

            _carte->evolutionMoralArmee(); 
            //on ne se préoccupe pas du ravitaillement au premier tour car le joueur n'a pas eu le temps de positionner ses troupes :
            if (_toursPasses > 0) {
                _carte->ravitaillerArmee();
                _carte->appliquerAttritionArmee();
            }
            _carte->retirerCadavres();

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
