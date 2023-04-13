#include "carte.h"

Carte::Carte(int rayon) : _rayon(rayon) {
    genererCarteVide("Plaine", _rayon);
    affichageSeulementCarte();

    //création d'un std::map qui recense tous les noeuds correspondant aux cases de la carte
    std::map<std::pair<int,int>,std::shared_ptr<Noeud>> noeuds;
    int debut = -_rayon+1;
    int fin = 0;
    for (int j = _rayon-1; j > -_rayon; j--) {
        for (int i = debut; i <= fin; i++) {
            noeuds[std::make_pair(i,j)] = std::make_shared<Noeud>("case", i, j);
        }
        if (j>0) fin++;
        else debut++;        
    }
    //ajout des voisins
    for (auto & paire : noeuds) {
        std::vector<std::pair<int, int>> voisins = getCoordonneesVoisins(paire.first.first, paire.first.second);
        for (const auto voisin : voisins) {
            paire.second->ajouterSuivant(noeuds[voisin], getCase(voisin.first, voisin.second)->getCoutDeplacement());
        }
    }
    //création du graphe qui représente les cases de la carte
    _grapheCases = std::make_shared<Graphe>(noeuds); 
}

void Carte::creerArmee() {     
    _armees.emplace_back(std::make_shared<Armee>()); 
}

void Carte::afficher() const{ //n'affiche pour l'instant que les armées, mais il faudra rajouter les cases
    for (unsigned int i = 0; i < _armees.size(); i++) 
        _armees[i]->afficher();
}

void Carte::executerOrdresArmee(unsigned int indiceArmee) {
    std::vector<std::shared_ptr<Unite>> unites = _armees[indiceArmee]->getUnites();
    for (unsigned int i = 0; i < unites.size(); i++) {
        std::vector<std::pair<int,int>> chemin;
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
void Carte::combat(Unite &u1, Unite &u2){
    std::pair<int, int> degats = u1.resultatCombatSimple(u2);


    std::cout << degats.first<< " : "<<degats.second<<std::endl;
}



void Carte::brouillardDeGuerreUnite(Unite const &unite, std::vector<std::pair<int, int>> &vecteur)const{
    std::vector<std::pair<int, int>> vue;
    std::vector<std::pair<int, int>> vueTampon;

    vue.push_back(std::make_pair(unite.getX(), unite.getY()));
    vecteur.push_back(std::make_pair(unite.getX(), unite.getY()));

    for (unsigned int i = 0; i < unite.getDistanceVue(); i++){
        for (unsigned int j = 0; j < vue.size();j++){
            std::vector<std::pair<int, int>> voisins = getCoordonneesVoisins(vue[j].first, vue[j].second);

            for (unsigned int k = 0; k < voisins.size();k++){
                //si pas déjà dans la fonction et si c'est accessible
                if (!(std::find(vue.begin(), vue.end(), voisins[k]) != vue.end())
                    && (_cases.at(voisins[j])->accessibleTerre()
                    || _cases.at(voisins[j])->accessibleEau())
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


void Carte::ajoutUniteTeam(unsigned int IDarmee, Unite const &u){
    _armees[IDarmee]->ajoutUnite(u);
}

//a corriger il se compte lui meme
float Carte::ratioAlliesAdversaires(Unite &unite, unsigned int zoneAutour, unsigned int idEquipeJoueur)const{
    std::vector<std::pair<int, int>> vision;
    unsigned int ancienneVision = unite.getDistanceVue();
    unite.setDistanceVue(zoneAutour);
    brouillardDeGuerreUnite(unite, vision);

    unsigned int nbAllies = 0;
    unsigned int nbEnnemis = 0;

    for (unsigned int i = 0; i < vision.size();i++){
        for (unsigned int j = 0; j < _armees.size();j++){
            for (unsigned int k = 0; k < _armees[j]->size();k++){
                if (_armees[i]->getUnite(k).getX() == vision[i].first && _armees[i]->getUnite(k).getY() == vision[i].second){
                    if (j == idEquipeJoueur)
                        nbAllies++;
                    else 
                        nbEnnemis++;
                }


            }
        } 
    }

    unite.setDistanceVue(ancienneVision);


    return static_cast<float>(nbAllies)/static_cast<float>(nbEnnemis);
}

/*getters and setters ============================*/
std::shared_ptr<Armee> Carte::getArmee(unsigned int i) const {
    return _armees.at(i);
}
