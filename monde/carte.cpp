#include "carte.h"

Carte::Carte(int taille, std::vector<std::shared_ptr<Armee>> const &armees) : _rayon(taille), _indiceArmee(0), _armees(armees) {
    
    	
    srand(time(NULL));
    // Code pour générer la carte en utilisant la fonction perlin2D
    int debut = -_rayon+1;
    int fin = 0;
    for (int j = _rayon-1; j > -_rayon; j--) {
        for (int i = debut; i <= fin; i++) {
                double x = static_cast<double>(i) / taille;
                double y = static_cast<double>(j) / taille;
                double value = perlin2D(x, y); // Appel de la fonction perlin2D pour obtenir la valeur de bruit de Perlin
                // Utilisation de la valeur de bruit de Perlin pour créer la case correspondante dans la carte
                _cases[std::make_pair(i, j)] = std::make_shared<Case>(valueToCaseNom(value));
        }
        if (j>0) fin++;
        else debut++;        
    }
               
        

    // Paramètres de génération des villes
    //int nbVilles = armees.size(); // Nombre de villes à générer
    int nbVilles = 10;
    double distanceMinDuBord = taille * 0.1; // Distance minimale du bord pour générer une ville (1/10 de la taille)
    double distanceMaxDuBord = taille * 0.2; // Distance maximale du bord pour générer une ville (1/5 de la taille)
    double distanceMinEntreVilles = taille * 0.1; // Distance minimale entre deux villes (1/10 de la taille)
    std::vector<std::pair<int, int>> villes;
    
    for (int n = 0; n < nbVilles;n++) {
        int i = rand() % (taille * 2) - taille + 1; // Coordonnée i aléatoire
        int j = rand() % (taille * 2) - taille + 1; // Coordonnée j aléatoire
        double distanceDuBord = std::abs(distanceEntrePointsHexagonaux(0, 0, i, j)); // Distance du point au bord de la carte
        bool estTropPresDuBord = distanceDuBord < distanceMinDuBord; // Vérification si le point est trop près du bord

        // Vérification si le point est assez éloigné des autres villes
        bool estAssezEloigneDesVilles = true;
        for (const auto& ville : villes) {
            double distanceEntreVilles = distanceEntrePointsHexagonaux(i, j, ville.first, ville.second);
            if (distanceEntreVilles < distanceMinEntreVilles) {
                estAssezEloigneDesVilles = false;
                break;
            }
        }

        // Vérification si le point est accessible par la terre
        bool estAccessibleParTerre = false;
        std::vector<std::pair<int, int>> voisins = getCoordonneesVoisins(i, j);
        for (const auto& voisin : voisins) {
            if (getCase(voisin.first, voisin.second) && getCase(voisin.first, voisin.second)->accessibleTerre()) {
                estAccessibleParTerre = true;
                break;
            }
        }

        // Si le point est assez éloigné du bord, des autres villes et accessible par la terre, alors on ajoute une ville
        if (!estTropPresDuBord && estAssezEloigneDesVilles && estAccessibleParTerre) {
            _cases[std::make_pair(i, j)] = std::make_shared<Case>("Ville");
            villes.push_back(std::make_pair(i, j));
            n++;
        }
    }

    affichageSeulementCarte();


    /*Placement des unités*/
   for (unsigned int i = 0; i < _armees.size();i++){
        std::vector<std::pair<int, int>> emplacements;
        emplacements.push_back(std::make_pair(villes[i].first, villes[i].second));
        unsigned int indexEmplacements = 0;
        for (unsigned int j = 0; j < _armees[i]->getUnites().size();j++){
            
            while(indexEmplacements >= emplacements.size() || !peutEtreEn(emplacements[indexEmplacements].first, emplacements[indexEmplacements].second, _armees[i]->getUnites()[j])){
                indexEmplacements++;
                std::cout << "j : "<<j<<std::endl;
            
                if (indexEmplacements < emplacements.size()){
                    _armees[i]->getUnites()[j]->setX(emplacements[j].first);
                    _armees[i]->getUnites()[j]->setY(emplacements[j].second);
                }else{//pas d'emplacements donc on élargit la zone
                    //on ajoute les voisins qui n'ont pas d'unités et qui ne sont pas des villes et qui sont accessible pour l'unité
                    std::vector<std::pair<int, int>> emplacementsBuffer;
                    for (unsigned int k = 0; k < emplacements.size();k++){
                        std::vector<std::pair<int, int>> voisins = getCoordonneesVoisins(emplacements[k].first, emplacements[k].second);
                        for (unsigned int l = 0; l < voisins.size();l++){
                            std::cout << "j : "<<j << voisins[l].first<<std::endl;
                            //Mtn on vérifie si l'emplacement n'est pas déjà dans l'emplacement
                            bool appartientEmplacement = false;
                            for (unsigned int m = 0; m < emplacements.size();m++)
                                if (emplacements[m].first == voisins[l].first && emplacements[m].second == voisins[l].second)
                                    appartientEmplacement = true;
                            //donc mtn on l'ajoute
                            if (!appartientEmplacement && _cases[voisins[l]]->getNom() != "Ville" && 
                            !caseAvecUnite(voisins[l].first, voisins[l].second))
                                emplacementsBuffer.push_back(voisins[l]);
                        }
                    }
                    //mtn on ajoute les emplacements
                    for (unsigned int k = 0; k < emplacementsBuffer.size();k++){
                        emplacements.push_back(emplacementsBuffer[k]);
                    }
                }
            }
            std::cout <<"taille : "<<emplacements[indexEmplacements].first<<std::endl;
            _armees[i]->getUnites()[j]->setX(emplacements[indexEmplacements].first);
            _armees[i]->getUnites()[j]->setY(emplacements[indexEmplacements].second);
            indexEmplacements++;
        }
   }
   


}

std::shared_ptr<Graphe> Carte::creerGraphe(accessibilite acces) const {
    //création d'un std::map qui recense tous les noeuds correspondant aux cases de la carte
    std::vector<std::pair<int,int>> sommets;
    int debut = -_rayon+1;
    int fin = 0;
    for (int j = _rayon-1; j > -_rayon; j--) {
        for (int i = debut; i <= fin; i++) {
            if (acces==accessibilite::Air) sommets.push_back(std::make_pair(i,j));
            else if (getCase(i,j)->accessibleEau() && (acces==accessibilite::EauEtTerre || acces==accessibilite::Eau)) sommets.push_back(std::make_pair(i,j));
            else if (getCase(i,j)->accessibleTerre() && (acces==accessibilite::EauEtTerre || acces==accessibilite::Terre)) sommets.push_back(std::make_pair(i,j));        
        }
        if (j>0) fin++;
        else debut++;        
    }
    std::shared_ptr<Graphe> graphe = std::make_shared<Graphe>(sommets); 
    //ajout des voisins
    for (auto & paire : sommets) {
        std::vector<std::pair<int, int>> voisins = getCoordonneesVoisins(paire.first, paire.second);
        for (const auto voisin : voisins) {
            if (std::find(sommets.begin(), sommets.end(), voisin) != sommets.end()) {
                graphe->ajouterSuivant(paire, voisin, getCase(voisin.first, voisin.second)->getCoutDeplacement());
            }
        }
    }
    return graphe;
}

std::shared_ptr<Graphe> Carte::getGraphe(accessibilite acces) {
    switch (acces) {
        case accessibilite::Terre : return _grapheTerre; break;
        case accessibilite::EauEtTerre : return _grapheEauEtTerre; break;
        case accessibilite::Eau : return _grapheEau; break;
        case accessibilite::Air : 
        default : return _grapheAir; break;
    }
}

Carte::Carte(int rayon) : _rayon(rayon), _indiceArmee(0) {
    genererCarteVide("Plaine", _rayon);
    affichageSeulementCarte();
    //création du graphe qui représente les cases de la carte
    _grapheAir = creerGraphe(accessibilite::Air);
    _grapheTerre = creerGraphe(accessibilite::Terre);
    _grapheEauEtTerre = creerGraphe(accessibilite::EauEtTerre);
    _grapheEau = creerGraphe(accessibilite::Eau);
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

    std::vector<std::pair<int,int>> zoneRavitaillement = _grapheAir->zoneRavitaillement(departs, obstacles, relais);
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
            chemin = getGraphe(unites[i]-> getCategorie())->aEtoile(debut, fin);
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


double Carte::distanceEntrePointsHexagonaux(int i, int j, int k, int l) {
    double x1 = i * 1.5;
    double y1 = (j - i / 2.0) * std::sqrt(3);
    double x2 = k * 1.5;
    double y2 = (l - k / 2.0) * std::sqrt(3);
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
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


/*Bruit de Perlin ======================*/
double Carte::calculIntermediaire(double pointA, double pointB, double parametreMelange) const{
    return pointA + parametreMelange * (pointB - pointA);
}


// Fonction pour calculer le produit scalaire entre un gradient et un vecteur
//gradient
double Carte::vecteurPente(int hash, double x, double y) const{
    switch (hash & 0x3) {
        case 0x0: return x + y;
        case 0x1: return -x + y;
        case 0x2: return x - y;
        case 0x3: return -x - y;
        default: return 0;
    }
}


// Fonction pour calculer la valeur de bruit de Perlin en 2D
double Carte::perlin2D(double x, double y) const{
    int permutation[256];

    // Remplir le tableau de permutation avec des valeurs aléatoires entre 0 et 255
    for (int i = 0; i < 256; ++i) {
        permutation[i] = i;
    }

    for (int i = 255; i > 0; --i) {
        permutation[i] = rand()%255;
    }
    int xi = static_cast<int>(x) & 255;
    int yi = static_cast<int>(y) & 255;
    double xf = x - static_cast<int>(x);
    double yf = y - static_cast<int>(y);
    double u = fade(xf);
    double v = fade(yf);

    int aa = permutation[permutation[xi] + yi];
    int ab = permutation[permutation[xi] + yi + 1];
    int ba = permutation[permutation[xi + 1] + yi];
    int bb = permutation[permutation[xi + 1] + yi + 1];

    double x1 = calculIntermediaire(vecteurPente(aa, xf, yf), vecteurPente(ba, xf - 1, yf), u);
    double x2 = calculIntermediaire(vecteurPente(ab, xf, yf - 1), vecteurPente(bb, xf - 1, yf - 1), u);

    return calculIntermediaire(x1, x2, v);
    
}

// Fonction pour interpoler
double Carte::fade(double t) const{
    return t * t * t * (t * (t * 6 - 15) + 10);
}


std::string Carte::valueToCaseNom(float Value){
    if (Value < -0.5)
        return "Ocean";
    else if (Value < 0)
        return "Plaine";
    else if (Value < 45)
        return "Plaine";
    else if (Value < 50)
        return "Foret";
    else if (Value < 0.91)
        return "Marecage";
    else
        return "Montagne";
}


bool Carte::caseAvecUnite(int x, int y)const{
    for (unsigned int i = 0; i < _armees.size();i++)
        for (unsigned int j = 0; j < _armees[i]->getUnites().size();j++)
            if (_armees[i]->getUnites()[j]->getX() == x && _armees[i]->getUnites()[j]->getY() == y)
                return true;
    
    return false;
}