#include "carte.h"

std::shared_ptr<Graphe> Carte::creerGraphe() const {
    //création d'un std::map qui recense tous les noeuds correspondant aux cases de la carte
    std::vector<std::pair<int,int>> sommets;
    int debut = -_rayon+1;
    int fin = 0;
    for (int j = _rayon-1; j > -_rayon; j--) {
        for (int i = debut; i <= fin; i++) {
            sommets.push_back(std::make_pair(i,j));
        }
        if (j>0) fin++;
        else debut++;        
    }
    std::shared_ptr<Graphe> graphe = std::make_shared<Graphe>(sommets); 
    //ajout des voisins
    for (auto & paire : sommets) {
        std::vector<std::pair<int, int>> voisins = getCoordonneesVoisins(paire.first, paire.second);
        for (const auto voisin : voisins) {
            graphe->ajouterSuivant(paire, voisin, getCase(voisin.first, voisin.second)->getCoutDeplacement());
        }
    }
    return graphe;
}

Carte::Carte(int rayon) : _rayon(rayon), _indiceArmee(0) {
    genererCarteVide("Plaine", _rayon);
    affichageSeulementCarte();
    //création du graphe qui représente les cases de la carte
    _grapheCases = creerGraphe();
/*
    std::cout<<"A*"<<std::endl;
    std::vector<std::pair<std::pair<int,int>, int>> chemin = _grapheCases->aEtoile(std::make_pair(-1,1), std::make_pair(2,-2));
    for (const auto & paire : chemin)  std::cout<<paire.first.first<<","<<paire.first.second<<std::endl;
*/
/*
    std::cout<<"ZONE RAVITAILLEMENT"<<std::endl;
    std::vector<std::pair<int,int>> departs;
    departs.push_back( std::make_pair(-1,1));
    //departs.push_back( std::make_pair(1,-2));
    std::vector<std::pair<int,int>> obstacles;
    obstacles.push_back(std::make_pair(0,1));
    obstacles.push_back(std::make_pair(0,0));
    obstacles.push_back(std::make_pair(-1,0));
    obstacles.push_back(std::make_pair(0,-1));
    std::map<std::pair<int,int>,int> relais;
    relais[std::make_pair(-1,1)] = 10;
    //relais[std::make_pair(0,2)] = 10;
    //relais[std::make_pair(1,-2)] = 10;
    std::vector<std::pair<int,int>> zone = _grapheCases->zoneRavitaillement(departs, obstacles, relais);
    for (const auto & paire : zone) std::cout<<paire.first<<", "<<paire.second<<std::endl;
*/
}

void Carte::creerArmee() {     
    _armees.emplace_back(std::make_shared<Armee>()); 
}

void Carte::afficherArmees() const{ //n'affiche pour l'instant que les armées, mais il faudra rajouter les cases
    for (unsigned int i = 0; i < _armees.size(); i++) 
        _armees[i]->afficher();
}

void Carte::afficherArmee() const{
    getArmee()->afficher();
}

void Carte::selectionnerArmee(unsigned int indiceArmee) {
    _indiceArmee = indiceArmee;
}

std::vector<std::pair<int,int>> Carte::getDepartsRavitaillement() const {
    std::vector<std::pair<int,int>> departs;
    for (const auto & paire : _cases) {
        if (paire.second->estDepartRavitaillement()) departs.push_back(paire.first);
    }
    return departs;
}

std::vector<std::pair<int,int>> Carte::getPositionsEnnemis() const {
    std::map<std::pair<int,int>, bool> positionsOccupees;
    for (unsigned int i = 0; i < _armees.size(); i++) {
        if (i != _indiceArmee) {
            std::vector<std::shared_ptr<Unite>> unites = _armees[i]->getUnites();
            for (unsigned int j = 0; j < unites.size(); j++) positionsOccupees[unites[j]->getPos()] = true;
        }
    }
    std::vector<std::pair<int,int>> positionsEnnemis;
    for (const auto & pos : positionsOccupees) positionsEnnemis.push_back(pos.first);
    return positionsEnnemis;
}

std::map<std::pair<int,int>, int> Carte::getRelaisRavitaillement() const {
    std::map<std::pair<int,int>, int> relais;
    std::vector<std::shared_ptr<Unite>> unites = getArmee()->getUnites();
    for (unsigned int i = 0; i < unites.size(); i++) {
        if (relais[unites[i]->getPos()] < unites[i]->getRayonRavitaillement()) {
            relais[unites[i]->getPos()] = unites[i]->getRayonRavitaillement();
        }
    }
    for (const auto & paire : relais) {
        if (paire.second == 0) relais.erase(paire.first);
    }
    return relais;
}

void Carte::ravitaillerArmee() {
    std::cout<<"RAVITAILLEMENT"<<std::endl;
    std::vector<std::pair<int,int>> departs = getDepartsRavitaillement();
    std::vector<std::pair<int,int>> obstacles = getPositionsEnnemis();
    std::map<std::pair<int,int>,int> relais = getRelaisRavitaillement();

    std::vector<std::pair<int,int>> zoneRavitaillement = _grapheCases->zoneRavitaillement(departs, obstacles, relais);
    for (const auto & paire : zoneRavitaillement) std::cout<<paire.first<<", "<<paire.second<<std::endl;

    std::vector<std::shared_ptr<Unite>> unites = getArmee()->getUnites();
    for (unsigned int i = 0; i < unites.size(); i++) {
        unites[i]->ravitailler();
    }
}

void Carte::appliquerAttritionArmee(){

}

void Carte::executerOrdresArmee() {
    std::vector<std::shared_ptr<Unite>> unites = getArmee()->getUnites();
    for (unsigned int i = 0; i < unites.size(); i++) {
        std::vector<std::pair<std::pair<int,int>, int>> chemin;
        if (unites[i]->getOrdre()->getType() == TypeOrdre::DEPLACER || unites[i]->getOrdre()->getType() == TypeOrdre::ATTAQUER) {
            std::pair<int,int> debut = unites[i]->getPos();
            std::pair<int,int> fin = unites[i]->getOrdre()->getPos();
            chemin = _grapheCases->aEtoile(debut, fin);
        }
        unites[i]->initialiserMouvement(chemin);
    }
    for (unsigned int pm = 0; pm < 100; pm++) { //on distribue un par un 100 points de mouvement aux unités
        for (unsigned int i = 0; i < unites.size(); i++) {
            unites[i]->avancer();
        }        
    }
}

//renvoie un vecteur contenant les coordonnées des 6 voisins (au plus) de la case choisie
std::vector<std::pair<int, int>> Carte::getCoordonneesVoisins(int posX, int posY)const{
    std::vector<std::pair<int, int>> resultat;
    if (getCase(posX-1, posY+1)) resultat.push_back(std::make_pair(posX-1, posY+1));
    if (getCase(posX, posY+1)) resultat.push_back(std::make_pair(posX, posY+1));
    if (getCase(posX-1, posY)) resultat.push_back(std::make_pair(posX-1, posY));
    if (getCase(posX+1, posY)) resultat.push_back(std::make_pair(posX+1, posY));
    if (getCase(posX, posY-1)) resultat.push_back(std::make_pair(posX, posY-1));
    if (getCase(posX+1, posY-1)) resultat.push_back(std::make_pair(posX+1, posY-1));
    return resultat;
}

std::shared_ptr<Case> Carte::getCase(int x, int y)const{
    if (-_rayon >= y || y >= _rayon) return nullptr;
    if (y>=0) {
        if (-_rayon >= x || x >= _rayon-y) return nullptr;
    } else {
        if (-_rayon - y >= x || x >= _rayon) return nullptr;
    }
    return _cases.at(std::make_pair(x,y));    
}

void Carte::affichageSeulementCarte()const{
    std::cout << "Map : -----------------------\n";
    int debut = -_rayon+1;
    int fin = 0;
    for (int j = _rayon-1; j > -_rayon; j--) {
        std::string decalage = "";
        for (unsigned int k = 0; k < abs(j); k++) decalage += " ";
        std::cout<<decalage;
        for (int i = debut; i <= fin; i++) {
            std::string nom = _cases.at(std::make_pair(i,j))->getNom();
            std::cout<<nom[0]<<" ";          
        }
        std::cout<<std::endl;
        if (j>0) fin++;
        else debut++;        
    }
}

//création d'une carte remplie de cases du même type
void Carte::genererCarteVide(std::string const &typeCase, unsigned int taille){
    int debut = -_rayon+1;
    int fin = 0;
    for (int j = _rayon-1; j > -_rayon; j--) {
        for (int i = debut; i <= fin; i++) {
            _cases[std::make_pair(i,j)] = std::make_shared<Case>(typeCase);
        }
        if (j>0) fin++;
        else debut++;        
    }
    std::cout << "_CASES : "<<_cases.size()<<std::endl;
}

void Carte::sauvegarderCarteMap(std::string const &path)const{
    /*
    std::ifstream fichierLecture(path);
    if (fichierLecture.is_open()){//le fichier existe
        std::cout << "Ecrasement de l'ancienne sauvegarde : " << path << "\n";
    }

    std::ofstream fichier(path); // Création du fichier

    if (fichier.is_open()) { // Vérification si le fichier est ouvert
        for (unsigned int i = 0; i < _cases.size();i++){
            for (unsigned int j = 0; j < _cases[i].size();j++){
                fichier << _cases[i][j].getNom()+",";
            }
            fichier << "\n";
        }
        
        
        fichier.close(); // Fermeture du fichier
    } else {
        std::cerr << "Erreur lors de la création du fichier : " << path << std::endl;
    }
    
*/

}


void Carte::chargerCarteMap(std::string const &path) {/*
    std::ifstream fichier(path);

    if (fichier) {
        // L'ouverture s'est bien passée, on peut donc lire

        std::string ligne; // Une variable pour stocker les lignes lues
        std::vector<Case> tampon;
        while (getline(fichier, ligne)) {
            std::stringstream ss(ligne);
            std::string element;
            while (getline(ss, element, ',')) {
                tampon.push_back(Case(element));
            }
            _cases.push_back(tampon);
            tampon.clear();
        }
        affichageSeulementCarte();
    } else {
        std::cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
    }*/
}



/*
Pour le combat on cherche les alliés/ennemis que voit l'unité

*/
//U1 attaque, U2 se défend
void Carte::combat(std::shared_ptr<Unite> u1,unsigned int idTeam1, std::shared_ptr<Unite> u2, unsigned int idTeam2){
    //on applique mtn les calculs du nombre (par exemple si il y a plus d'ennemis, on va un peu perdre du morale)
    u1->setMoral(u1->getMoral()+(ratioAlliesAdversaires(u1, 5, idTeam1)*6-15));//moral baisse ou augmente de 15
    u2->setMoral(u2->getMoral()+(ratioAlliesAdversaires(u2, 5, idTeam2)*6-15));//moral baisse ou augmente de 15
 
    //calcul de base
    std::pair<int, int> degats = u1->resultatCombatSimple(u2);
    int defense = 100;
    //on applique les calculs du terrain du défenseur
    for (const auto& pair : _cases) {
        if (pair.first.first == u2->getX() && pair.first.second == u2->getY())
            defense = pair.second->getDefense();
    }
    degats.first = static_cast<float>(degats.first)*(static_cast<float>(defense)/100);
    degats.second = static_cast<float>(degats.second)*(static_cast<float>(100+(100-defense))/100);

    u1->infligerDegats(degats.first);
    u2->infligerDegats(degats.second);

    std::cout << degats.first<< " : "<<degats.second<<std::endl;
}



void Carte::brouillardDeGuerreUnite(std::shared_ptr<Unite> unite, std::vector<std::pair<int, int>> &vecteur)const{
    std::vector<std::pair<int, int>> vue;
    std::vector<std::pair<int, int>> vueTampon;

    vue.push_back(std::make_pair(unite->getX(), unite->getY()));
    vecteur.push_back(std::make_pair(unite->getX(), unite->getY()));

    for (unsigned int i = 0; i < unite->getDistanceVue(); i++){


        for (unsigned int j = 0; j < vue.size();j++){
            std::vector<std::pair<int, int>> voisins = getCoordonneesVoisins(vue[j].first, vue[j].second);
            for (unsigned int k = 0; k < voisins.size();k++){
                //si pas déjà dans la fonction et si c'est accessible
                if (!(std::find(vue.begin(), vue.end(), voisins[k]) != vue.end())
                    && (_cases.at(voisins[k])->accessibleTerre()
                    || _cases.at(voisins[k])->accessibleEau())
                    ){
                        
                    vueTampon.push_back(voisins[k]);
                    vecteur.push_back(voisins[k]);
                    
                }
            }

            
        }

        //On remet dans la vue ce qu'il y a dans le bufferVue dans la vue
        for (unsigned int k = 0; k < vueTampon.size();k++){
            vue.push_back(vueTampon[k]);
        }
        vueTampon.clear();
    }
    //rajoute les montagnes si elles sont à une distance suffisante (on rajoute que la case MONTAGNE)
    for (unsigned int i = 0; i < vue.size();i++){
        std::vector<std::pair<int, int>> voisins = getCoordonneesVoisins(vue[i].first, vue[i].second);
        for (unsigned int j = 0; j < voisins.size();j++){
            if (!(std::find(vue.begin(), vue.end(), voisins[j]) != vue.end())
                && !_cases.at(voisins[j])->accessibleTerre() 
                && !_cases.at(voisins[j])->accessibleEau()
                //&& distance(voisins[j], std::make_pair(unite.getX(), unite.getY())) <= unite.getDistanceVue()
                )
                    vecteur.push_back(voisins[j]);
        }   
    }
    
}

std::vector<std::pair<int, int>> Carte::brouillardDeGuerreEquipe(unsigned int i)const{
    std::vector<std::pair<int, int>> resultat;
    for (unsigned int j = 0; j < _armees[i]->size();j++)
        brouillardDeGuerreUnite(_armees[i]->getUnite(j), resultat);
    return resultat;
}


void Carte::ajoutUniteTeam(unsigned int IDarmee, std::shared_ptr<Unite> unite){
    _armees[IDarmee]->ajoutUnite(unite);
}

float Carte::ratioAlliesAdversaires(std::shared_ptr<Unite> unite, unsigned int zoneAutour, unsigned int idEquipeJoueur)const{
    std::vector<std::pair<int, int>> vision;
    unsigned int ancienneVision = unite->getDistanceVue();
    unite->setDistanceVue(zoneAutour);
    brouillardDeGuerreUnite(unite, vision);
    
    unsigned int nbAllies = 0;
    unsigned int nbEnnemis = 0;

    for (unsigned int i = 0; i < vision.size();i++)
        for (unsigned int j = 0; j < _armees.size();j++)
            for (unsigned int k = 0; k < _armees[j]->size();k++)
                if (_armees[i]->getUnite(k)->getX() == vision[i].first && _armees[i]->getUnite(k)->getY() == vision[i].second){
                    if (j == idEquipeJoueur)
                        nbAllies++;
                    else 
                        nbEnnemis++;
                }

    unite->setDistanceVue(ancienneVision);

    if (nbEnnemis == 0)return 1;
    return static_cast<float>(nbAllies)/static_cast<float>(nbEnnemis);
}

/*getters and setters ============================*/
std::shared_ptr<Armee> Carte::getArmee() const {
    return _armees.at(_indiceArmee);
}