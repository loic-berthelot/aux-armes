#include "unite.h"

Unite::Unite(std::string name, accessibilite categorie, const std::vector<Type> & types, int posX, int posY, int santeInitiale, int attaque, 
    int defense, int distanceVue): 
_categorie(categorie), _types(types), _posX(posX), _posY(posY), _sante(santeInitiale), _maxSante(santeInitiale), _attaque(attaque), 
_defense(defense), _distanceVue(distanceVue), _nom(name){
    
};

//lecture du fichier
Unite::Unite(std::string name, int x,int y):_nom(name), _posX(x), _posY(y), _ordreRecu(nullptr){
std::ifstream fichier("../monde/Unites/"+name+".txt");
    
    if (!fichier.is_open()) {
        throw std::invalid_argument("Erreur : L'unité n'existe pas." + name);
    }
    
    std::string ligne;
    std::getline(fichier, ligne);//index catégorie
    std::getline(fichier, ligne);//valeurs catégorie
    _categorie = stringToCategorie(ligne);
    std::getline(fichier, ligne);//index santé
    std::getline(fichier, ligne);//santé 
    _sante = std::stoi(ligne);
    _maxSante = std::stoi(ligne);
    std::getline(fichier, ligne);//index attaque
    std::getline(fichier, ligne);//attaque
    _attaque = std::stoi(ligne);
    std::getline(fichier, ligne);//index défense
    std::getline(fichier, ligne);//défense
    _defense = std::stoi(ligne);

    std::getline(fichier, ligne);//index distanceVue
    std::getline(fichier, ligne);//distanceVue
    _distanceVue = std::stoi(ligne);
    
    std::getline(fichier, ligne);//index pm
    std::getline(fichier, ligne);//pm
    _pointsMouvement = std::stoi(ligne);
    

    std::getline(fichier, ligne);//index typeUnité

    while (std::getline(fichier, ligne)) {
        _types.push_back(Type(ligne));
    }
    
    // Fermer le fichier
    fichier.close();
}

std::string Unite::toString() const {
    return "X : "+ std::to_string(_posX)+"Y : "+std::to_string(_posY);
}

//on considère que le combat est équilibré. Les boots ou autres changements seront fais dans la méthode combat de la classe Map
std::pair<int, int> Unite::resultatCombatSimple(std::shared_ptr<Unite> ennemy)const{

    //calcul du coefficient


    float moy = 1;
    float nb = 0;
    float nbBuffer = 0;
    float moyBuffer = 1;
    for (unsigned int i = 0; i < _types.size();i++){
        
        for (unsigned int j = 0; j <ennemy->getTypes().size();j++){
            moyBuffer+=_types[i].getCoefficients(ennemy->getTypes()[j].getNom());
            nbBuffer++;

        }
        if (nbBuffer != 0){
            moy+=(moyBuffer/nbBuffer);
            nb++;
            nbBuffer = 0;
            moyBuffer = 1;
        }
    }
    int coef;
    if (nb != 0){
        coef = 1;
    }else{
        coef = moy/nb;
    }


    std::pair<int, int> resultat;
    //1er int correspond aux dégâts que recoit l'unité this et le deuxième aux dégats de l'unité
    resultat.first = (static_cast<float>(ennemy->_attaque)*(1.0 / ( static_cast<float>(_defense) ) ) )*static_cast<float>(ennemy->_moral)*coef;
    resultat.second = (static_cast<float>(_attaque)*(1.0 / ( static_cast<float>(ennemy->_defense) ) ) )*static_cast<float>(_moral)*coef;

    return resultat;

}



void Unite::CreationNouvelleUnite(std::string const &nom, std::vector<Type> const &types, int sante, accessibilite categorie,
int attaque, int defense, int distanceVue, int pointsMouvement){
    std::ofstream fichier("../monde/Unites/"+nom+".txt");

    if (fichier.is_open()) {
        // Écriture dans le fichier ligne par ligne
        fichier << "Catégorie:" << std::endl;
        fichier << CategorieToString(categorie) << std::endl;
        fichier << "Santé:" << std::endl;
        fichier << std::to_string(sante)<<std::endl;
        fichier << "Attaque:" << std::endl;
        fichier << std::to_string(attaque)<<std::endl;
        fichier << "Défense:" << std::endl;
        fichier << std::to_string(defense)<<std::endl;
        fichier << "Distance de vue:" << std::endl;
        fichier << std::to_string(distanceVue)<<std::endl;
        fichier << "Point de mouvement:" << std::endl;
        fichier << std::to_string(pointsMouvement)<<std::endl;

        for (unsigned int i = 0; i < types.size();i++){
            fichier << types[i].getNom()<<std::endl;
        }

        // Fermeture du fichier
        fichier.close();
        std::cout << "Écriture dans le fichier terminée." << std::endl;
    } else {
        std::cout << "Erreur : Impossible d'ouvrir le fichier." << std::endl;
    }
}

/*Getters / SETTERS*/


void Unite::donnerOrdre(std::shared_ptr<Ordre> ordre){
    _ordreRecu = ordre;
}

int Unite::getX()const{
    return _posX;
}

int Unite::getY()const{
    return _posY;
}

int Unite::getDistanceVue()const{
    return _distanceVue;
}

int Unite::getRayonRavitaillement() const {
    if (_nom == "Caravane") return 10;
    return 0;
}

accessibilite Unite::getCategorie() const {
    return _categorie;
}

void Unite::avancer() {
    if (_chemin.empty()) {
        _pointsMouvement = 0;
    } else {
        _pointsMouvement += _vitesseDeplacement;
        if (_pointsMouvement > _chemin[0].second) {
            _pointsMouvement = 0;
            _posX = _chemin[0].first.first;
            _posY = _chemin[0].first.second;
            _chemin.erase(_chemin.begin());
        }
    }
}

void Unite::initialiserMouvement(std::vector<std::pair<std::pair<int,int>, int>> chemin) {
    _pointsMouvement = 0;
    _chemin = chemin;
}

std::shared_ptr<Ordre> Unite::getOrdre() const{
    return _ordreRecu;
}

std::pair<int,int> Unite::getPos() const {
    return std::make_pair(_posX, _posY);
}

void Unite::ravitailler() {
    std::cout<<"unite ravitaille !"<<std::endl;
}