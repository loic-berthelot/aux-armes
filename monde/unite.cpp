#include "unite.h"

static accessibilite stringToCategorie(std::string const &s){
    if (s == "Air") return accessibilite::Air;
    if (s == "Eau") return accessibilite::Eau;
    if (s == "Terre") return accessibilite::Terre;
    return accessibilite::EauEtTerre;
}
static std::string CategorieToString(accessibilite const c){
    if (c == accessibilite::Air) return "Air";
    if (c == accessibilite::Eau) return "Eau";
    if (c == accessibilite::Terre) return "Terre";
    return "EauEtTerre";
}

//lecture du fichier
Unite::Unite(std::string name, int x,int y, int sante, int moral):_nom(name), _posX(x), _posY(y), _ordreRecu(nullptr){
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
    try{
        _maxSante = std::stoi(ligne);
        _sante = std::stoi(ligne);
    }catch(...){
        throw new Exception("Erreur : La santé n'est pas un int : "+ligne+" dans le fichier unité : "+name);
    }
    if (_maxSante <= 0)
        throw std::invalid_argument("Santé <= 0 pour : "+ligne+ " dans la l'unité : "+name);

    if (sante >= 0) _sante = sante;
    else sante = _maxSante;
    if (moral >= 0) _moral = moral;
    else moral = 50;
    
    std::getline(fichier, ligne);//index attaque
    std::getline(fichier, ligne);//attaque
    try{
        _attaque = std::stoi(ligne);
    }catch(...){
        throw new Exception("Erreur : L'attaque n'est pas un int : "+ligne+" dans le fichier unité : "+name);
    }
    if (_attaque < 0)
        throw std::invalid_argument("Attaque < 0 pour : "+ligne+ " dans la l'unité : "+name);
    std::getline(fichier, ligne);//index défense
    std::getline(fichier, ligne);//défense
    try{
        _defense = std::stoi(ligne);
    }catch(...){
        throw new Exception("Erreur : La défense n'est pas un int : "+ligne+" dans le fichier unité : "+name);
    }
    if (_defense <= 0)
        throw std::invalid_argument("défense <= 0 pour : "+ligne+ " dans la l'unité : "+name);

    std::getline(fichier, ligne);//index distanceVue
    std::getline(fichier, ligne);//distanceVue
    try{
        _distanceVue = std::stoi(ligne);
    }catch(...){
        throw new Exception("Erreur : La distanceVue n'est pas un int : "+ligne+" dans le fichier unité : "+name);
    }
    if (_distanceVue < 0)
        throw std::invalid_argument("distanceVue < 0 pour : "+ligne+ " dans la l'unité : "+name);
    

    std::getline(fichier, ligne);//index distanceRavitaillement
    std::getline(fichier, ligne);//distanceRavitaillement
    try{
        _distanceRavitaillement = std::stoi(ligne);
    }catch(...){
        throw new Exception("Erreur : La distance de ravitaillement n'est pas un int : "+ligne+" dans le fichier unité : "+name);
    }
    if (_distanceRavitaillement < 0)
        throw std::invalid_argument("Distance Ravitaillement <= 0 pour : "+ligne+ " dans la l'unité : "+name);

    std::getline(fichier, ligne);//index portee
    std::getline(fichier, ligne);//portee
    try{
        _portee = std::stoi(ligne);
    }catch(...){
        throw new Exception("Erreur : La portée n'est pas un int : "+ligne+" dans le fichier unité : "+name);
    }
    if (_portee < 0)
        throw std::invalid_argument("Portée < 0 pour : "+ligne+ " dans la l'unité : "+name);

    std::getline(fichier, ligne);//index vitesse déplacement
    std::getline(fichier, ligne);//vitesse déplacement
    try{
        _vitesseDeplacement = std::stof(ligne);
    }catch(...){
        throw new Exception("Erreur : La vitesse de déplacement n'est pas un int : "+ligne+" dans le fichier unité : "+name);
    }
    if (_vitesseDeplacement < 0)
        throw std::invalid_argument("vitesseDéplacement < 0 pour : "+ligne+ " dans la l'unité : "+name);

    std::getline(fichier, ligne);//index espace occupé
    std::getline(fichier, ligne);//espace occupé
    try{
        _espaceOccupe = std::stoi(ligne);
    }catch(...){
        throw new Exception("Erreur : L'espace occupé n'est pas un int : "+ligne+" dans le fichier unité : "+name);
    }
    if (_espaceOccupe < 0)
        throw std::invalid_argument("Espace OCcupé < 0 pour : "+ligne+ " dans la l'unité : "+name);
    
    while (std::getline(fichier, ligne)) {
        if (ligne != "") _types.emplace_back(ligne);
    }
    
    for (unsigned int i = 0; i < _types.size(); i++) {
        if (_types[i].possedeSpecificite(Specificite::degatsDeZone)) _degatsDeZone = true;
        if (_types[i].possedeSpecificite(Specificite::furtif)) _furtif = true;
        if (_types[i].possedeSpecificite(Specificite::inflammable)) _inflammable = true;
        if (_types[i].possedeSpecificite(Specificite::incendiaire)) _incendiaire = true;
    }

    _ordreRecu = std::make_shared<Ordre>(TypeOrdre::IMMOBILISER, 0, 0);

    // Fermer le fichier
    fichier.close();
}

std::string Unite::toString() const {
    return _nom+" en ("+ std::to_string(_posX)+","+std::to_string(_posY)+") avec "+std::to_string(_sante)+"/"+std::to_string(_maxSante)+" sante et "+std::to_string(_moral)+"/"+std::to_string(_maxMoral)+" moral.";
}

//on considère que le combat est équilibré. Les boots ou autres changements seront fais dans la méthode combat de la classe Map
std::pair<int, int> Unite::resultatCombatSimple(std::shared_ptr<Unite> const ennemi)const{
    //calcul du coefficient
    float moy = 1;
    float nb = 0;
    float nbBuffer = 0;
    float moyBuffer = 1;
    for (unsigned int i = 0; i < _types.size();i++){        
        for (unsigned int j = 0; j <ennemi->getTypes().size();j++){
            moyBuffer+=_types[i].getCoefficients(ennemi->getType(j).getNom());
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
    if (nb == 0) coef = 1;
    else coef = moy/nb;
    std::pair<int, int> resultat;
    //1er int correspond aux dégâts infligées par l'unité this et le deuxième aux dégats de l'unité
    resultat.first = static_cast<float>(_attaque) / (static_cast<float>(ennemi->_defense)) * static_cast<float>(_moral)*coef*(static_cast<float>(_sante)/static_cast<float>(_maxSante));
    resultat.second = static_cast<float>(ennemi->_attaque) / (static_cast<float>(_defense)) * static_cast<float>(ennemi->_moral)*coef*(static_cast<float>(ennemi->_sante)/static_cast<float>(ennemi->_maxSante));    
    return resultat;
}

void Unite::CreationNouvelleUnite(std::string const &nom, std::vector<Type> const &types, int sante, accessibilite categorie,
int attaque, int defense, int distanceVue, int distanceRavitaillement, float vitesseDeplacement, int espaceOccupe){
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
        fichier << "Distance de ravitaillement:" << std::endl;
        fichier << std::to_string(distanceRavitaillement)<<std::endl;
        fichier << "Vitesse déplacement:" << std::endl;
        fichier << std::to_string(vitesseDeplacement)<<std::endl;
        fichier << "Espace occupé:" << std::endl;
        fichier << std::to_string(espaceOccupe)<<std::endl;

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


bool Unite::possedeSpecificite(Specificite e)const{
    for (unsigned int i = 0; i < _types.size();i++)
        if (_types[i].possedeSpecificite(e))
            return true;
    return false;
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

int Unite::getDistanceRavitaillement() const {
    return _distanceRavitaillement;
}

int Unite::getSante() const {
    return _sante;
}

int Unite::getAttaque() const {
    return _attaque;
}

int Unite::getDefense() const {
    return _defense;
}

accessibilite Unite::getCategorie() const {
    return _categorie;
}

bool Unite::avancer() {
    if (_chemin.empty()) {
        _pointsMouvement = 0;
        return false;
    }
    
    _pointsMouvement += _vitesseDeplacement;
    const auto& noeudSuivant = _chemin[0];

    if (_pointsMouvement > noeudSuivant.second) {
        _pointsMouvement -= noeudSuivant.second;
        _positionPrecedente = std::make_pair(_posX, _posY);
        _posX = noeudSuivant.first.first;
        _posY = noeudSuivant.first.second;
        _chemin.erase(_chemin.begin());
        return true;
    }
    return false;
}

void Unite::reculer() {
    _posX = _positionPrecedente.first;
    _posY = _positionPrecedente.second;
}

void Unite::initialiserMouvement(std::vector<std::pair<std::pair<int,int>, int>>const &chemin) {
    _pointsMouvement = 0;
    _chemin = chemin;
}

std::shared_ptr<Ordre> Unite::getOrdre() const{
    return _ordreRecu;
}

std::pair<int,int> Unite::getPos() const {
    return std::make_pair(_posX, _posY);
}

void Unite::regenererMoral(int pointsMoral) {
    _moral += pointsMoral;
    if (_moral > _maxMoral) _moral = _maxMoral;
    if (_moral < 0) _moral = 0;
}

void Unite::infligerDegats(unsigned int degats) {
    if (_sante - degats <= 0){
        _sante = 0;
        _enVie = false;
    }else
        _sante-=static_cast<int>(degats);

}


bool Unite::getIntrepide()const{
    return _intrepide;
}

bool Unite::getAutonome()const{
    return _autonome;
}

void Unite::subirAttrition(float attrition) {
    unsigned int degats = static_cast<unsigned int>(attrition);
    infligerDegats(degats); // on inflige d'abord des dégâts équivalents à l'attrition dûe à la surpopulation
    if (degats > 0) {
        std::cout<<"Un "<<_nom<<" en ("<<_posX<<","<<_posY<<") subit "<<degats<<" degats à cause de l'attrition."<<std::endl;
    }

    if (!_estRavitaille && !_autonome) {
        infligerDegats(static_cast<unsigned int>(0.1*_maxSante)); // puis on inflige des dégâts supplémentaires si l'unité n'est pas ravitaillée
        std::cout<<"Un "<<_nom<<" en ("<<_posX<<","<<_posY<<") subit "<<static_cast<unsigned int>(0.1*_maxSante)<<" degats car il n'est pas ravitaillé."<<std::endl;
    }
    _estRavitaille = false;
}

void Unite::ravitailler() {
    std::cout<<_nom<<" ravitaille en ("<<_posX<<","<<_posY<<")"<<std::endl;
    _estRavitaille = true;
    regenererMoral(10);  
}

void Unite::setX(int x){
    _posX = x;
}

void Unite::setY(int y){
    _posY = y;
}

void Unite::setPos(const std::pair<int,int> & pos) {
    _posX = pos.first;
    _posY = pos.second;   
}

unsigned int Unite::getPortee()const{
    return _portee;
}

bool Unite::estVivant() const {
    return _enVie;
}

int Unite::getEspaceOccupe() const {
    return _espaceOccupe;
}

int Unite::getMoral()const {
    return _moral;
}

void Unite::setDistanceVue(int distanceVue) {
    _distanceVue = distanceVue;
}

std::vector<Type> Unite::getTypes()const {
    return _types;
}

Type Unite::getType(int i) const {
    return _types.at(i);
}

bool Unite::peutAttaquer(int x, int y) {
    return (distance(std::make_pair(x,y), std::make_pair(_posX, _posY)) < static_cast<float>(_portee));
}

bool Unite::possedeDegatsDeZone() const {
    return _degatsDeZone;
}

bool Unite::estFurtif() const {
    return _furtif;
}

bool Unite::estIncendiaire() const {
    return _incendiaire;
}

void Unite::recevoirBrulure() {
    if (_inflammable) _effetBrulure = 3;
}

void Unite::evolutionBrulure() {
    if (_effetBrulure > 0) {
        std::cout<<"Un "<<_nom<<" en ("<<_posX<<","<<_posY<<") recoit 50 degats de brulure."<<std::endl;
        _effetBrulure--;
        infligerDegats(50);
    }
}

accessibilite Unite::stringToCategorie(std::string const &s){
    switch (s[0]) {
        case 'A':
            return accessibilite::Air;
        case 'E':
            if (s == "Eau")
                return accessibilite::Eau;
            else
             return accessibilite::EauEtTerre;
        default:
            return accessibilite::Terre;
    }
}

std::string Unite::CategorieToString(accessibilite const c){
    if (c == accessibilite::Air)
        return "Air";
    else if (c == accessibilite::Eau)
        return "Eau";
    else if (c == accessibilite::Terre)
        return "Terre";
    else return "EauEtTerre";
}

std::string Unite::getNom() const {
    return _nom;
}

float Unite::getVitesse() const {
    return _vitesseDeplacement;
}