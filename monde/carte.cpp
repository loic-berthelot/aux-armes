#include "carte.h"

std::vector<std::string> separerChaine(std::string chaine, char separateur) {
    std::vector<std::string> resultat;
    std::string mot;
    for (unsigned int i = 0; i < chaine.size(); i++) {
        if (chaine[i] == separateur) {
            if (! mot.empty()) resultat.push_back(mot);
            mot = "";
        } else {
            mot+=chaine[i];
        }
    }
    if (! mot.empty()) resultat.push_back(mot);
    return resultat;
}

Carte::Carte(std::string const &nomFichierConfig, std::vector<std::shared_ptr<Armee>> const &armees):_indiceArmee(0), _armees(armees){
    std::string nomFichier = "../monde/génération/"+nomFichierConfig+".txt";  

    std::ifstream fichier(nomFichier); // Ouverture du fichier en lecture
    if (! fichier.is_open()) throw Exception("Impossible d'ouvrir le fichier "+nomFichier);

    std::string ligne;
    std::getline(fichier, ligne);//index taille
    std::getline(fichier, ligne);//taille valeur

    try{
        _rayon = std::stoi(ligne);
    }catch (...){
        throw new Exception(ligne+" n'est pas un int pour le rayon dans le fichier : "+nomFichier);
    }

    int taille = _rayon;
    std::getline(fichier, ligne);//index seed
    std::getline(fichier, ligne);//seed valeur
    int seed;
    try{
        seed = std::stoi(ligne);
    }catch (...){
        throw new Exception(ligne+" n'est pas un int pour seed dans le fichier : "+nomFichier);
    }
    
    srand(seed);
    srand(time(0));
    srand(1683275872);

    std::getline(fichier, ligne);//index nbVilles
    std::getline(fichier, ligne);//val nbVilles
    int nbVilles;
    try{
        nbVilles = std::max(static_cast<int>(armees.size()),getNombreCases()/std::stoi(ligne));
    }catch (...){
        throw new Exception(ligne+" n'est pas un int pour nbVilles dans le fichier : "+nomFichier);
    }

    std::getline(fichier, ligne);//index toursMax
    std::getline(fichier, ligne);//val toursMax 
    
    try{
        std::cout << "ligne -----------: "<<ligne<<std::endl;
        _nbToursMax = std::stoul(ligne);
    }catch (...){
        throw new Exception(ligne+" n'est pas un unsigned int pour nbToursMax dans le fichier : "+nomFichier);
    }

    std::getline(fichier, ligne);//valeurs index
    while (std::getline(fichier, ligne) && ligne != "Dernière:"){
        if (ligne != ""){
            float caseVal = std::stof(ligne);
            std::getline(fichier, ligne);
            _valeursCasesGenerateurs[caseVal] = ligne;
        }
    }
    std::getline(fichier, ligne);
    _mapDernierCase = ligne;


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

    std::vector<std::pair<int, int>> villes = genererVille(nbVilles);

    placerUnites(villes);
    
    affichageSeulementCarte();
    initialiserGraphes();
    calculerDepartsRavitaillement();
}

int Carte::getNombreCases() const {
    return 3*_rayon*_rayon-3*_rayon+1;
}

int Carte::getNombreCases(const std::string & nom) const {
    int compt = 0;   
    int debut = -_rayon+1;
    int fin = 0;
    for (int j = _rayon-1; j > -_rayon; j--) {
        for (int i = debut; i <= fin; i++) { 
            if (getCase(i, j)->getNom() == nom) compt++;
        }
        if (j>0) fin++;
        else debut++;        
    } 
    return compt;
}
Carte::Carte(int rayon)  : _rayon(rayon){
}

void Carte::initialiserGraphes() {
    float coutDeplacement;
    float poidsMinimal = -1;
    std::string ligne;
    for (const auto& entry : std::filesystem::directory_iterator("../monde/Cases")) {
        if (entry.is_regular_file()) {
            std::ifstream fichier("../monde/Cases/"+entry.path().filename().string());
            if (! fichier.is_open()) throw Exception("Impossible d'ouvrir un fichier dans Carte::initialiserGraphes()");
            for (unsigned int i = 0; i < 4; i++) std::getline(fichier, ligne);
            try {
                coutDeplacement = std::stof(ligne);                
            } catch (...) {
                throw Exception("Carte::initialiserGraphes : impossible de calculer le cout de déplacement.");
            }
            if (coutDeplacement < poidsMinimal || poidsMinimal == -1) poidsMinimal = coutDeplacement;
        }
    }
    if (poidsMinimal == -1) poidsMinimal = 1; // si on n'a trouvé aucun fichier case, le poids minimal des arêtes du graphe vaut 1 par défaut
    _grapheAir = creerGraphe(accessibilite::Air, 1, false);
    _grapheTerre = creerGraphe(accessibilite::Terre, poidsMinimal);
    _grapheEauEtTerre = creerGraphe(accessibilite::EauEtTerre, poidsMinimal);
    _grapheEau = creerGraphe(accessibilite::Eau, poidsMinimal);
    _grapheVision = creerGraphe(accessibilite::EauEtTerre, 1, false);
    initialiserVisibilite();
}


void Carte::placerUnites(std::vector<std::pair<int, int>> const &villes){
    std::shared_ptr<Armee> armee;
    for (unsigned int i = 0; i < _armees.size();i++){ 
        armee = _armees.at(i);   
        unsigned int indexEmplacement = 0;
        int rayonEmplacements = 0;
        std::pair<int,int> departEmplacements = villes.at(i);
        std::vector<std::pair<int,int>> emplacements;
        emplacements.push_back(departEmplacements);
        std::pair<int,int> pos;
        bool carteParcourue = false;
        unsigned int j = 0;
        std::vector<std::pair<int,int>> positionsInaccessibles;
        while (j < armee->taille() && ! carteParcourue){ // on tente de placer toutes les unités de l'armée tant que la carte n'est pas entièrement parcourue
            while (indexEmplacement < emplacements.size())  {
                pos = emplacements.at(indexEmplacement);
                if (peutEtreEn(pos, armee->getUnite(j)) && ! caseAvecUnite(pos)) break;
                positionsInaccessibles.push_back(pos);
                indexEmplacement++;
            }
            if (indexEmplacement < emplacements.size()) {
                _armees[i]->getUnite(j)->setPos(pos);
                j++;
                indexEmplacement++;
            } else {
                rayonEmplacements++;
                emplacements = getCoordonneesCouronne(departEmplacements, rayonEmplacements);
                carteParcourue = emplacements.empty();
                indexEmplacement = 0;
            }         
        }
        indexEmplacement = 0;
        while (j < armee->taille()) { // s'il reste des unités à placer, on tente de les placer en changeant l'accessibilité des cases de la carte
            pos = positionsInaccessibles.at(indexEmplacement);
            if (! caseAvecUnite(pos)) {
                if (armee->getUnite(j)->getCategorie() == accessibilite::Eau) _cases[pos] = std::make_shared<Case>("Ocean");
                else if (armee->getUnite(j)->getCategorie() == accessibilite::Terre) _cases[pos] = std::make_shared<Case>("Plaine");
                armee->getUnite(j)->setPos(pos);
                j++;
            }
            indexEmplacement++;
            if (indexEmplacement >= positionsInaccessibles.size()) throw Exception("Trop d'unités pour une carte trop petite dans le constructeur de Carte.");
        }
   }
}

std::vector<std::pair<int, int>> Carte::genererVille(int nbVilles){
    // Paramètres de génération des villes
    int taille = _rayon;
    double distanceMaxDuBord = taille * 1; // Distance maximale du bord pour générer une ville (1/5 de la taille)
    double distanceMinEntreVilles = taille * 0.001; // Distance minimale entre deux villes (1/10 de la taille)
    std::vector<std::pair<int, int>> villes;
    
    for (int n = 0; n < nbVilles;n++) {
        int nbRecherche = 0;
        bool estTropLoinDuBord = false;
        bool estAssezEloigneDesVilles = false;
        bool estAccessibleParTerre = false;
        bool estSurUneVille = true;

        while(nbRecherche < getNombreCases() && (estTropLoinDuBord || !estAssezEloigneDesVilles || !estAccessibleParTerre || estSurUneVille)){
            nbRecherche++;
            std::pair<int,int> pos = positionAleatoireCarte();
            int i = pos.first;
            int j = pos.second;
            double distanceDuBord = std::abs(distance(std::make_pair(0, 0), std::make_pair(i, j))); // Distance du point au bord de la carte
            estTropLoinDuBord = distanceDuBord > distanceMaxDuBord; // Vérification si le point est trop près du bord

            // Vérification si le point est assez éloigné des autres villes
            estAssezEloigneDesVilles = true;
            for (const auto& ville : villes) {
                double distanceEntreVilles = distance(std::make_pair(i, j), ville);
                if (distanceEntreVilles < distanceMinEntreVilles) {
                    estAssezEloigneDesVilles = false;
                    break;
                }
            }
            // Vérification si le point est accessible par la terre
            estAccessibleParTerre = false;
            std::vector<std::pair<int, int>> voisins = getCoordonneesVoisins(std::make_pair(i, j));
            for (const auto& voisin : voisins) {
                if (getCase(voisin) && getCase(voisin)->accessibleTerre()) {
                    estAccessibleParTerre = true;
                    break;
                }
            }
            //sur une ville
            estSurUneVille = false;
            for (const auto& ville : villes) {
                double distanceEntreVilles = distance(std::make_pair(i, j), ville);
                if (ville.first == i && ville.second == ville.second) {
                    estSurUneVille = true;
                    break;
                }
            }
            // Si le point est assez éloigné du bord, des autres villes et accessible par la terre, alors on ajoute une ville
            if (!estTropLoinDuBord && estAssezEloigneDesVilles && estAccessibleParTerre && !estSurUneVille) {
                _cases[std::make_pair(i, j)] = std::make_shared<Case>("Ville");
                villes.push_back(std::make_pair(i, j));
            }
            
        }if (nbRecherche >= getNombreCases()){
            throw std::invalid_argument("Impossible de placer une ville numéro : "+std::to_string(n));
        }
    }
    return villes;
}

void Carte::ajouterCase(const std::pair<int,int> & position, const std::string & nom) {
    _cases[position] = std::make_shared<Case>(nom);
}

std::vector<std::pair<unsigned int, int>> Carte::getScoreEquipe()const{
    std::vector<std::pair<unsigned int, int>> scoreEquipe;

    for (unsigned int i = 0; i < _armees.size();i++){
        if (_armees[i]->taille() > 0)
            scoreEquipe.push_back(std::make_pair<unsigned int, int>(std::move(i),_armees[i]->getScore()));
        else 
            scoreEquipe.push_back(std::make_pair<unsigned int, int>(std::move(i),_armees[i]->getScore()-1000000000));
    }
    return scoreEquipe;

}



std::shared_ptr<Graphe> Carte::creerGraphe(accessibilite const acces, float poidsMinimal, bool coutDeplacement) const {
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
    std::shared_ptr<Graphe> graphe = std::make_shared<Graphe>(sommets, poidsMinimal); 
    //ajout des voisins
    for (auto & paire : sommets) {
        std::vector<std::pair<int, int>> voisins = getCoordonneesVoisins(paire);
        for (const auto voisin : voisins) {
            if (std::find(sommets.begin(), sommets.end(), voisin) != sommets.end()) {
                if (coutDeplacement) graphe->ajouterSuivant(paire, voisin, getCase(voisin)->getCoutDeplacement());
                else graphe->ajouterSuivant(paire, voisin, 1);
            }
        }
    }
    return graphe;
}

std::shared_ptr<Graphe> Carte::getGraphe(accessibilite const acces) const {
    switch (acces) {
        case accessibilite::Terre : return _grapheEauEtTerre; break;
        case accessibilite::EauEtTerre : return _grapheEauEtTerre; break;
        case accessibilite::Eau : return _grapheEau; break;
        case accessibilite::Air : 
        default : return _grapheAir; break;
    }
}

void Carte::initialiserVisibilite() {
    for (const auto & paire : _cases) _casesVisibles[paire.first] = false;
}

std::pair<int,int> Carte::positionAleatoireCarte() { //méthode de complexité O(n) avec n le rayon, à utiliser avec modération
    long int indiceLineaireCase = rand()%(3*_rayon*_rayon-3*_rayon+1); // dans la carte, il y a 3r²-3r+1 cases, avec r le rayon
    int debut = -_rayon+1;
    int fin = 0;
    for (int j = _rayon-1; j > -_rayon; j--) { //on parcourt toutes les lignes jusqu'à tomber sur
        if (debut+indiceLineaireCase <= fin) { // si on se trouve sur la bonne ligne, on renvoie la position calculée
            return std::make_pair(debut+indiceLineaireCase,j);
        } else { //sinon on retire à l'indice linéaire de la case le nombre de cases sur la ligne courante
            indiceLineaireCase -= (fin-debut)+1;
        }
        if (j>0) fin++;
        else debut++;        
    }
    throw Exception("Erreur dans Carte::positionAleatoireCarte() : depassement de "+std::to_string(indiceLineaireCase)+" cases.");
}

std::vector<std::pair<int,int>> Carte::positionsAccessibles(std::shared_ptr<Unite> const unite, int nbTours) const {
    return getGraphe(unite->getCategorie())->positionsAccessibles(unite->getPos(), 100*nbTours*unite->getVitesse());
}

int Carte::getRayon() const {
    return _rayon;
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

unsigned int Carte::nombreArmeesVivantes() const {
    unsigned int compt = 0;
    for (unsigned int i = 0; i < _armees.size(); i++) {
        
        if (! _armees[i]->estEliminee()) compt++;
    }
    return compt;
}

void Carte::selectionnerArmee(unsigned int indiceArmee) {
    _indiceArmee = indiceArmee;
}

void Carte::calculerDepartsRavitaillement() {
    _departsRavitaillement.clear();
    for (const auto & paire : _cases) {
        if (paire.second->estDepartRavitaillement()) _departsRavitaillement.push_back(paire.first);
    }
}

std::vector<std::pair<int,int>> Carte::getDepartsRavitaillement() const {
    return _departsRavitaillement;
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

std::map<std::pair<int,int>, int> Carte::getRelaisRavitaillement(std::shared_ptr<Unite> const unite) const {
    std::map<std::pair<int,int>, int> relais;
    std::vector<std::shared_ptr<Unite>> unites = getArmee()->getUnites();
    std::pair<int,int> pos;
    int rayon;
    for (unsigned int i = 0; i < unites.size(); i++) {
        if(unites[i] != unite) {
            pos = unites[i]->getPos();
            rayon = unites[i]->getDistanceRavitaillement();
            if (relais.count(pos) == 0) {
                if ( rayon > 0) relais[pos] = rayon;
            } else if (relais.at(pos) < rayon) {
                relais[pos] = rayon;
            }
        }        
    }    
    return relais;
}

std::map<std::pair<int,int>, std::vector<std::shared_ptr<Unite>>> Carte::getUnitesVisibles(bool allies) {
    std::map<std::pair<int,int>, std::vector<std::shared_ptr<Unite>>> unites;
    for (unsigned int i = 0; i < _armees.size(); i++) {
        std::vector<std::shared_ptr<Unite>> unitesArmee = _armees[i]->getUnites();
        for (unsigned int j = 0; j < unitesArmee.size(); j++) {
            if (i == _indiceArmee) {
                if (allies) unites[unitesArmee[j]->getPos()].push_back(unitesArmee.at(j));
            } else if (caseVisible(unitesArmee[j]->getPos())) {
                if (! unitesArmee[j]->estFurtif() || ! getCase(unitesArmee[j]->getPos())->permetFurtivite()) unites[unitesArmee[j]->getPos()].push_back(unitesArmee.at(j));
            } 
        }     
    }
    return unites;
}

void Carte::ravitaillerArmee() {
    std::vector<std::pair<int,int>> departs = getDepartsRavitaillement();
    std::vector<std::pair<int,int>> obstacles = getPositionsEnnemis();    
    std::map<std::pair<int,int>,int> relais = getRelaisRavitaillement();
    std::vector<std::pair<int,int>> zoneRavitaillement = _grapheEauEtTerre->zoneRavitaillement(departs, obstacles, relais);
    std::vector<std::shared_ptr<Unite>> unites = getArmee()->getUnites();
    for (unsigned int i = 0; i < unites.size(); i++) {
        if (std::find(zoneRavitaillement.begin(), zoneRavitaillement.end(), unites[i]->getPos()) != zoneRavitaillement.end()) {
            unites[i]->ravitailler();            
        } 
    }
}

void Carte::appliquerAttritionArmee(){
    std::vector<std::shared_ptr<Unite>> unites = getArmee()->getUnites();
    // on calcule l'occupation totale de chaque case occupée par une unité de l'armée
    std::map<std::pair<int,int>, int> casesOccupees;
    for (unsigned int i = 0; i < unites.size(); i++) {
        casesOccupees[unites[i]->getPos()] += unites[i]->getEspaceOccupe();
    }
    // pour chacune des unites de l'armée, on calcule et applique l'attrition
    int capaciteAcceuil, occupation;
    std::pair<int,int> pos;
    for (unsigned int i = 0; i < unites.size(); i++) {
        pos = unites[i]->getPos();
        occupation = casesOccupees.at(pos);
        capaciteAcceuil = getCase(pos)->getCapaciteAccueil();
        if (occupation > capaciteAcceuil) unites[i]->subirAttrition(50*(occupation/capaciteAcceuil-1)); //taux d'attrition à ajuster  
        else unites[i]->subirAttrition(0);//il faut quand même appeler subirAttrition() pour chaque unité à chaque tour
    }
}

void Carte::retirerCadavres(){
    for (unsigned int i = 0; i < _armees.size(); i++) _armees[i]->retirerUnitesMortes();
}

std::vector<std::pair<int,int>> Carte::positionsEnnemisVolants() const {
    std::map<std::pair<int,int>, bool> positions; // on enregistre les positions dns un std::map dans un premier temps, afin d'éviter les positions en double
    std::shared_ptr<Armee> armee;
    std::shared_ptr<Unite> unite;
    for (unsigned int i = 0; i < _armees.size(); i++) {
        if (i != _indiceArmee) {
            armee = _armees.at(i);
            for (unsigned int j = 0; j < armee->size(); j++) {
                unite = armee->getUnite(j);
                if (unite->getCategorie() == accessibilite::Air) positions[unite->getPos()] = true;
            }
        }
    }
    std::vector<std::pair<int,int>> resultat;
    for (const auto & pos : positions) resultat.push_back(pos.first); // on enregistre ensuite les positions des unités ennemies volantes dans un vecteur, que l'on retourne
    return resultat;
}

void Carte::executerOrdresArmee() {
    std::vector<std::shared_ptr<Unite>> unites = getArmee()->getUnites();
    for (unsigned int i = 0; i < unites.size(); i++) unites[i]->evolutionBrulure();//on brûle les unites qui sont en feu au début de chaque tour
    getArmee()->retirerUnitesMortes(); // on retire ensuite les unités mortes de l'armée courante
    for (unsigned int i = 0; i < unites.size(); i++) {
        std::vector<std::pair<std::pair<int,int>, int>> chemin;
        if (unites[i]->getOrdre()->getType() == TypeOrdre::DEPLACER) { // on prépare les unités à se déplacer
            std::pair<int,int> debut = unites[i]->getPos(); 
            std::pair<int,int> fin = unites[i]->getOrdre()->getPos();
            accessibilite categorieUnite = unites[i]-> getCategorie();
            if (categorieUnite == accessibilite::Air) chemin = getGraphe(categorieUnite)->aEtoile(debut, fin); // on calcule le chemin que devra prendre l'unité
            else chemin = getGraphe(categorieUnite)->aEtoile(debut, fin, positionsEnnemisVolants()); // si l'unité ne sait pas voler, elle devra contourner les ennemis volants
        }else if (unites[i]->getOrdre()->getType() == TypeOrdre::ATTAQUER){ // On fait attaquer les unités à distance
            combat(unites[i], _indiceArmee, unites[i]->getOrdre()->getPos());
        }
        unites[i]->initialiserMouvement(chemin);            
    }
    
    for (unsigned int pm = 0; pm < 100; pm++) { //on distribue un par un 100 points de mouvement aux unités
        for (unsigned int i = 0; i < unites.size(); i++) {
            if (unites[i]->getOrdre()->getType() == TypeOrdre::DEPLACER && unites[i]->estVivant()) { // on ne fait bouger que les unités vivantes
                bool seDeplace = unites[i]->avancer(); //l'unité avance (elle accumule des points de mouvement et avance parfois d'une case)
                if (seDeplace) { //si en avançant l'unité arrive sur une nouvelle case, on vérifie si cette case est tenue par un ennemi
                    if (ennemiSurCase(unites[i]->getPos())) {
                        unites[i]->initialiserMouvement({}); // Le déplacement de l'unité prend fin si elle rencontre des ennmis sur la même case
                        combat(unites[i], _indiceArmee, unites[i]->getPos()); //on lance alors un combat
                        if (ennemiSurCase(unites[i]->getPos())) unites[i]->reculer(); //si l'une des unités ennemies survit, elle repousse l'attaque(l'unité courante doit reculer)
                    }
                }
            }            
        }        
    }
}

//renvoie un vecteur contenant les coordonnées des 6 voisins (au plus) de la case choisie
std::vector<std::pair<int, int>> Carte::getCoordonneesVoisins(std::pair<int,int> const &pos)const{
    std::vector<std::pair<int, int>> resultat;
    if (getCase(pos.first-1, pos.second+1)) resultat.push_back(std::make_pair(pos.first-1, pos.second+1));
    if (getCase(pos.first, pos.second+1)) resultat.push_back(std::make_pair(pos.first, pos.second+1));
    if (getCase(pos.first-1, pos.second)) resultat.push_back(std::make_pair(pos.first-1, pos.second));
    if (getCase(pos.first+1, pos.second)) resultat.push_back(std::make_pair(pos.first+1, pos.second));
    if (getCase(pos.first, pos.second-1)) resultat.push_back(std::make_pair(pos.first, pos.second-1));
    if (getCase(pos.first+1, pos.second-1)) resultat.push_back(std::make_pair(pos.first+1, pos.second-1));
    return resultat;
}

std::vector<std::pair<int, int>> Carte::getCoordonneesVoisins(std::pair<int,int> const &pos, int rayon)const{
    std::vector<std::pair<int, int>> resultat;
    int debut = pos.first-rayon+1;
    int fin = pos.first;
    for (int j = pos.second+rayon-1; j > pos.second-rayon; j--) {
        for (int i = debut; i <= fin; i++) {
            if (getCase(i, j)) resultat.push_back(std::make_pair(i,j));
        }
        if (j>pos.second) fin++;
        else debut++;        
    }    
    return resultat;
}

std::vector<std::pair<int, int>> Carte::getCoordonneesCouronne(std::pair<int,int> const &pos, int rayon)const {
    std::vector<std::pair<int, int>> resultat;
    if (rayon == 0) {
        if (getCase(pos.first, pos.second)) resultat.emplace_back(pos.first, pos.second);
    } else {
        for (unsigned int i = 0; i < rayon; i++) {
            if (getCase(pos.first-i, pos.second+rayon)) resultat.emplace_back(pos.first-i, pos.second+rayon);
            if (getCase(pos.first+rayon-i, pos.second+i)) resultat.emplace_back(pos.first+rayon-i, pos.second+i);
            if (getCase(pos.first+rayon, pos.second-i)) resultat.emplace_back(pos.first+rayon, pos.second-i);
            if (getCase(pos.first+i, pos.second-rayon)) resultat.emplace_back(pos.first+i, pos.second-rayon);
            if (getCase(pos.first-i, pos.second+i-rayon)) resultat.emplace_back(pos.first-i, pos.second+i-rayon);
            if (getCase(pos.first-rayon, pos.second+i)) resultat.emplace_back(pos.first-rayon, pos.second+i);
        }    
    }
    return resultat;
}

std::shared_ptr<Case> Carte::getCase(std::pair<int,int> const &pos) const {
    if (-_rayon >= pos.second || pos.second >= _rayon) return nullptr;
    if (pos.second>=0) {
        if (-_rayon >= pos.first || pos.first >= _rayon-pos.second) return nullptr;
    } else {
        if (-_rayon - pos.second >= pos.first || pos.first >= _rayon) return nullptr;
    }
    return _cases.at(pos);    
}

void Carte::affichageSeulementCarte()const{
    std::cout << "Carte : -----------------------"<<_rayon<<std::endl;
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

void Carte::evolutionMoralArmee() {
    std::vector<std::shared_ptr<Unite>> unites = getArmee()->getUnites();
    for (unsigned int i = 0; i < unites.size(); i++) {
        unites[i]->regenererMoral(ratioAlliesAdversaires(unites[i], 5, _indiceArmee)*6-15);//moral baisse ou augmente de 15 maximum
    }    
}        


//Pour le combat on cherche les alliés/ennemis que voit l'unité
void Carte::combat(std::shared_ptr<Unite> const u, unsigned int idTeam, std::pair<int,int> const &positionCombat){
    if (distance(positionCombat, u->getPos()) > u->getPortee()) throw Exception("Portée pas assez grande pour attaquer.");

    std::vector<std::shared_ptr<Unite>> unites = unitesSurCase(positionCombat);
    std::random_device rd;
    std::mt19937 rng(rd());

    // Mélange du vecteur
    std::shuffle(unites.begin(), unites.end(), rng);

    for (unsigned int k = 0; k < unites.size(); k++){
        if (unites[k] == u) continue; //L'unité u ne doit pas s'attaquer elle-même          
        std::pair<int, int> degats = u->resultatCombatSimple(unites[k]);//calcul de base

        //on applique les calculs du terrain du défenseur
        int defense = getCase(unites[k]->getPos())->getDefense();
        degats.first = static_cast<float>(degats.first)*(static_cast<float>(defense)/100);
        degats.second = static_cast<float>(degats.second)*(static_cast<float>(200-defense)/100);

        bool caseMaritime = getCase(u->getPos())->accessibleEau();//Si la case où a lieu le combat est une case d'eau, on conserve cette information

        //L'unité u inflige d'abord des dégâts à l'unité ennemie
        if (u->getCategorie() != accessibilite::Eau || !caseMaritime) { //Les unités terrestres embarquées n'infligent pas de dégâts
            unites[k]->infligerDegats(degats.first);
            if (u->estIncendiaire()) unites[k]->recevoirBrulure();
            _armees[idTeam]->ajoutScore(degats.first);
        } else {
            degats.first = 0;
        }

        bool ennemiPeutRepliquer = (distance(u->getPos(), positionCombat) <= unites[k]->getPortee());      
        //C'est ensuite à l'ennemi d'infliger des dégâts (s'il peut évidemment)
        if (ennemiPeutRepliquer) {
            if (u->getCategorie() != accessibilite::Eau || !caseMaritime) { //Les unités terrestres embarquées n'infligent pas de dégâts
                u->infligerDegats(degats.second);
                if (unites[k]->estIncendiaire()) u->recevoirBrulure();                
            } else {
                degats.second = 0;
            }

            if (degats.first < degats.second){
                if (!u->getIntrepide())
                    u->regenererMoral(-15);
                unites[k]->regenererMoral(15);
            }else if (degats.first > degats.second){
                u->regenererMoral(15);
                if (!unites[k]->getIntrepide())
                    unites[k]->regenererMoral(-15);
            }
        }else 
            u->regenererMoral(15);
        
        
        std::string message =  "Un " +u->getNom()+" inflige "+std::to_string(degats.first)+" degats";
        if  (ennemiPeutRepliquer) message +=" et en recoit "+std::to_string(degats.second)+" en retour.";
        else message+=".";
        std::cout <<message<<std::endl;
    }   
    if (u->possedeDegatsDeZone()) infligerDegatsDeZone(positionCombat, 0.5*u->getAttaque()); // u inflige des dégâts de zone
}

std::vector<std::shared_ptr<Unite>> Carte::unitesSurCase(std::pair<int,int> const &pos) {
    std::vector<std::shared_ptr<Unite>> unites;
    for (unsigned int i = 0; i < _armees.size(); i++) {
        std::vector<std::shared_ptr<Unite>> unitesArmee = _armees[i]->getUnites();
        for (unsigned int j = 0; j < unitesArmee.size(); j++) {
            if (unitesArmee[j]->getPos() == pos) unites.push_back(unitesArmee[j]);
        }
    }
    return unites;
}  

void Carte::infligerDegatsDeZone(std::pair<int,int> const &pos, int degats) {
    std::vector<std::pair<int,int>> cases = getCoordonneesVoisins(pos);
    cases.push_back(pos);
    for (unsigned int i = 0; i < cases.size(); i++) {
        std::vector<std::shared_ptr<Unite>> unites = unitesSurCase(cases[i]);
        for (unsigned int j = 0; j < unites.size(); j++) {
            unites[j]->infligerDegats(degats/getCase(cases[i])->getDefense());
            std::cout<<"Un "<< unites[j]->getNom()<<"("<<cases[i].first<<","<<cases[i].second<<") recoit "<<degats/getCase(cases[i])->getDefense()<<" degats de zone."<<std::endl;
        }
    }
}

void Carte::brouillardDeGuerreUnite(std::shared_ptr<Unite> const unite){
    std::vector<std::pair<int,int>> zoneVision = getCoordonneesVoisins(unite->getPos(), 1+unite->getDistanceVue());
    if (unite->getCategorie() == accessibilite::Air) { // Si l'unité est volante, elle peut voir toutes les cases dans la limite de son champ de vision
        for (unsigned int i = 0; i < zoneVision.size(); i++) _casesVisibles[zoneVision.at(i)] = true;
    } else {// Si l'unité n'est pas volante, elle peut voir toutes les cases qui ne sont pas cachées derrière un obstacle
        for (unsigned int i = 0; i < zoneVision.size(); i++) { 
            int distanceAuSol = _grapheVision->longueurChemin(unite->getPos(), zoneVision[i]);
            int distanceVolOiseau = _grapheAir->longueurChemin(unite->getPos(), zoneVision[i]);
            if (distanceAuSol == distanceVolOiseau) { // une case est cachée derrière un obstacle si la distance en tenant compte des obstacles est égale à la distance sans tenir compte des obstacles
                _casesVisibles[zoneVision.at(i)] = true;
                if (distanceAuSol < unite->getDistanceVue()) { //Si la case étudiée n'est pas à l'extrémité du champ de vision on essaie aussi d'ajouter les cases voisines
                    std::vector<std::pair<int,int>> voisins = getCoordonneesVoisins(zoneVision[i], 1);
                    for (unsigned int j = 0; j < voisins.size(); j++) {
                        if (_grapheAir->longueurChemin(unite->getPos(), voisins.at(j)) > distanceAuSol) _casesVisibles[voisins.at(j)] = true; // on n'ajoute la case voisine que si elle n'est pas cachée derrière un obstacle
                    }
                }
            }
        }
    }
}

void Carte::brouillardDeGuerreEquipe(){
    initialiserVisibilite();
    for (unsigned int j = 0; j < getArmee()->size();j++) brouillardDeGuerreUnite(getArmee()->getUnite(j));
}

void Carte::ajoutUniteTeam(unsigned int IDarmee, std::shared_ptr<Unite> const unite){
    _armees[IDarmee]->ajoutUnite(unite);
}

float Carte::ratioAlliesAdversaires(std::shared_ptr<Unite> const unite, unsigned int zoneAutour, unsigned int idEquipeJoueur)const{
    std::vector<std::pair<int, int>> vision;
    unsigned int ancienneVision = unite->getDistanceVue();
    unite->setDistanceVue(zoneAutour);
    //brouillardDeGuerreUnite(unite, vision);
    
    unsigned int nbAllies = 0;
    unsigned int nbEnnemis = 0;

    for (unsigned int i = 0; i < vision.size();i++)
        for (unsigned int j = 0; j < _armees.size();j++){
            for (unsigned int k = 0; k < _armees[j]->size();k++)
                if (_armees[j]->getUnite(k)->getPos() == vision[i]){
                    if (j == idEquipeJoueur)
                        nbAllies++;
                    else 
                        nbEnnemis++;
                }
        }
    unite->setDistanceVue(ancienneVision);

    if (nbEnnemis == 0)return 1;
    return static_cast<float>(nbAllies)/static_cast<float>(nbEnnemis);
}

/*getters and setters ============================*/

std::string Carte::valueToCaseNom(float Value){
    for (auto it = _valeursCasesGenerateurs.begin(); it != _valeursCasesGenerateurs.end(); ++it) {
        if (Value <= it->first)
            return it->second;
    }
    
    return _mapDernierCase;
}

bool Carte::caseAvecUnite(std::pair<int,int> const &pos)const{
    for (unsigned int i = 0; i < _armees.size();i++)
        for (unsigned int j = 0; j < _armees[i]->getUnites().size();j++)
            if (_armees[i]->getUnite(j)->getPos() == pos && _armees[i]->getUnite(j)->estVivant()) return true;    
    return false;
}

bool Carte::ennemiSurCase(std::pair<int,int> const &pos) const {
    for (unsigned int i = 0; i < _armees.size(); i++) {
        if (i != _indiceArmee) {
            std::vector<std::shared_ptr<Unite>> unites = _armees[i]->getUnites();
            for (unsigned int j = 0; j < unites.size(); j++) {
                if (unites[j]->getPos() == pos && unites[j]->estVivant()) return true;
            }
        }
    }
    return false;
}

bool Carte::ennemiSurCase(int x, int y)const{    
    return ennemiSurCase(std::make_pair(x,y));
}

bool Carte::peutEtreEn(std::pair<int,int> const &pos, std::shared_ptr<Unite> const u1){
    return (u1->getCategorie() == accessibilite::Air)||((u1->getCategorie() == accessibilite::Eau || u1->getCategorie() == accessibilite::EauEtTerre) && _cases.at(pos)->accessibleEau())
    || ((u1->getCategorie() == accessibilite::Terre || u1->getCategorie() == accessibilite::EauEtTerre) && _cases.at(pos)->accessibleTerre());
}

bool Carte::caseVisible(std::pair<int,int> const &pos) const {
    return _casesVisibles.at(pos);
}

/*Getters*/

unsigned int Carte::getMaxTours()const{
    std::cout<<"tm : "<<_nbToursMax<<std::endl;
    return _nbToursMax;    
}

void Carte::setMaxTours(unsigned int nbTours) {
    _nbToursMax = nbTours;
}

std::shared_ptr<Armee> Carte::getArmee() const {
    return _armees.at(_indiceArmee);
}

std::shared_ptr<Case> Carte::getCase(int x, int y)const{
    return getCase(std::make_pair(x,y));    
}

std::vector<std::shared_ptr<Unite>> Carte::unitesAllieesSurCase(std::pair<int,int> const &pos) {
    std::vector<std::shared_ptr<Unite>> unites = getArmee()->getUnites();
    std::vector<std::shared_ptr<Unite>> resultat;
    for (unsigned int i = 0; i < unites.size(); i++) {
        if (unites.at(i)->getPos() == pos) resultat.push_back(unites.at(i));
    }
    return resultat;
}

int Carte::porteeRavitaillementAllie(std::pair<int,int> const &pos) const {
    std::vector<std::shared_ptr<Unite>> unites = getArmee()->getUnites();
    int portee = 0;
    int porteeUnite;
    for (unsigned int i = 0; i < unites.size(); i++) {
        if (unites.at(i)->getPos() == pos) {
            porteeUnite = unites.at(i)->getDistanceRavitaillement();
            if (porteeUnite > portee) portee = porteeUnite;
        }
    }
    return portee;
}