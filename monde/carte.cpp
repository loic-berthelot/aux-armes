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
            //std::cout<<(getCase(i,j)==nullptr)<<std::endl;
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
        //paire.second->afficher();
    }
    //std::cout<<"sortie"<<std::endl;
    //création du graphe qui représente les cases de la carte
    _grapheCases = std::make_shared<Graphe>(noeuds);

    //std::vector<std::shared_ptr<Noeud>> chemin = _grapheCases->aEtoile(std::make_pair<int,int>(-1,1),std::make_pair<int,int>(4,3));
    //for (const auto & noeud : chemin) noeud->afficher();
    
}

void Carte::creerArmee() { _armees.emplace_back(std::make_shared<Armee>()); }

void Carte::afficher() const{ //n'affiche pour l'instant que les armées, mais il faudra rajouter les cases
    for (unsigned int i = 0; i < _armees.size(); i++) 
        _armees[i]->afficher();
}

void Carte::executerOrdresTour(unsigned int indiceArmee) {
    _armees[indiceArmee]->executerOrdresTour();
}

/*Méthode carte (map) ===================*/


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
    if (y <= -_rayon || y >= _rayon) return nullptr;
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

/*getters and setters ============================*/
std::shared_ptr<Armee> Carte::getArmee(unsigned int i) const {
    return _armees.at(i);
}