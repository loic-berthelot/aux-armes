#include "carte.h"

Carte::Carte(unsigned int rayon) : _rayon(rayon) {
    genererMapVide("Plaine", _rayon);

    //création d'un std::map qui recense tous les noeuds correspondant aux cases de la carte
    std::map<std::pair<int,int>,std::shared_ptr<Noeud>> positionsNoeuds;
    int debutLigne = -rayon;
    int finLigne = 0;
    for (int j = _rayon; j > -_rayon; j--) {
        for (int i = debutLigne; i < finLigne; i++) {
            positionsNoeuds[std::make_pair(i,j)] = std::make_shared<Noeud>("", i, j);
        }
        if (j<0) finLigne++;
        if (j>=0) debutLigne--;        
    }

    //création d'un vecteur contenant tous les noeuds avec leurs voisins
    std::vector<std::shared_ptr<Noeud>> noeuds;
    for (auto & paire : positionsNoeuds) {
        std::vector<std::pair<int, int>> voisins = getVoisinsCoordonnees(paire.first.first, paire.first.second);
        for (const auto voisin : voisins) {
            paire.second->ajouterSuivant(positionsNoeuds[voisin], getCase(voisin.first, voisin.second).getCoutDeplacement());
        }
        noeuds.push_back(paire.second);
    }

    //création du graphe
    _grapheCases = std::make_shared<Graphe>(noeuds);
}

void Carte::creerArmee() { _armees.emplace_back(std::make_shared<Armee>()); }

void Carte::afficher() const{ //n'affiche pour l'instant que les armées, mais il faudra rajouter les cases
    for (unsigned int i = 0; i < _armees.size(); i++) 
        _armees[i]->afficher();
}

void Carte::executerOrdresTour(unsigned int indiceArmee) {
    _armees[indiceArmee]->executerOrdresTour();
}


std::shared_ptr<Armee> Carte::getArmee(unsigned int i) const{ return _armees[i]; }


/*Méthode carte (map) ===================*/


//renvoie les coordonnées des voisins coordonnées en X, Y à partir du centre
std::vector<std::pair<int, int>> Carte::getVoisinsCoordonnees(int PosX, int PosY)const{
    std::vector<std::pair<int, int>> resultat;
    
    try{
        getCase(PosX, PosY);
    }catch(std::invalid_argument &e){
        throw std::invalid_argument("Dans getVoisins, les coordonnées envoyées ne sont pas bonnes : "+std::to_string(PosX)+ " , "
        +std::to_string(PosY));
    }
    //Création des résultats
    try{
        getCase(PosX-1, PosY);
        resultat.push_back(std::make_pair(PosX-1, PosY));
    }catch(std::invalid_argument &e){
    }
    try{
        getCase(PosX-1, PosY-1);
        resultat.push_back(std::make_pair(PosX-1, PosY-1));
    }catch(std::invalid_argument &e){
    }
    try{
        getCase(PosX+1, PosY-1);
        resultat.push_back(std::make_pair(PosX+1, PosY-1));
    }catch(std::invalid_argument &e){
    }
    try{
        getCase(PosX+1, PosY);
        resultat.push_back(std::make_pair(PosX+1, PosY));
    }catch(std::invalid_argument &e){
    }
    try{
        getCase(PosX+1, PosY+1);
        resultat.push_back(std::make_pair(PosX+1, PosY+1));
    }catch(std::invalid_argument &e){
    }
    try{
        getCase(PosX-1, PosY+1);
        resultat.push_back(std::make_pair(PosX-1, PosY+1));
    }catch(std::invalid_argument &e){
    }

    return resultat;
}


Case Carte::getCase(int x, int y)const{

    x+= _cases.size()/2;
    y+=_cases[x].size()/2;
    //en dehors
    if (x < 0 || x >= _cases.size() || y < 0 || y >= _cases[x].size())
        throw std::invalid_argument("En dehors de la map : ("+std::to_string(x)+ ";"+std::to_string(y)+")");
    return _cases[x][y];

}


//affiche en hexagonale la map (sachant que la map elle meme est construite en hexagonale)
void Carte::affichageSeulementCarte()const{
    std::cout << "Map : -----------------------\n";
    for (unsigned int i = 0; i < _cases.size(); i++) {
        
        if ((_cases.size() /2+1) %2== 0){//si taille initiale paire
            for (unsigned int j = 0; j < (_cases.size()-_cases[i].size())/2+1;j++)
                std::cout << "  ";
            if (i % 2 == 0) // Afficher un décalage vers la droite pour chaque ligne impaire
                std::cout << " ";
            
        }else{//si taille initiale impaire

            for (unsigned int j = 0; j < (_cases.size()-_cases[i].size())/2;j++)
                std::cout << "  ";
            if (i % 2 == 1) // Afficher un décalage vers la droite pour chaque ligne impaire
                std::cout << " ";
            
        }
        //affichage de la ligne
        for (unsigned int j = 0; j < _cases[i].size(); j++) {
            // Afficher le contenu de chaque case
            std::cout << _cases[i][j].getNom()[0] << " ";
        }
        std::cout << std::endl;
    
    }
}

//génère une map hexagonale la taille correspond au nombre de 'tour' d'hexagone,
//aka le nombre d'hexagones sur les côtés
void Carte::genererMapVide(std::string const &typeCase, unsigned int taille){
    Case c(typeCase);
    //taille
    unsigned int hautTampon = taille;
    std::vector<Case> ligne;
    //création des lignes du haut
    //taille*2-1 correspond au nombre de case de la ligne du milieu
    while(hautTampon <= taille*2-1){
        for (unsigned int i = 0; i < hautTampon;i++){
            ligne.push_back(c);
        }
        _cases.push_back(ligne);
        ligne.clear();
        hautTampon++;
    }

    unsigned int basTampon = taille*2-2;

    while(basTampon >= taille){
        for (unsigned int i = 0; i < basTampon;i++){
            ligne.push_back(c);
        }
        _cases.push_back(ligne);
        ligne.clear();
        basTampon--;
    }


}

/*getters and setters ============================*/

std::vector<std::vector<Case>> Carte::getCarte()const{
    return _cases;
}