#include "unite.h"

Unite::Unite(const std::string & categorie, const std::vector<std::string> & types, int posX, int posY, int santeInitiale, int attaque, 
int defense, int distanceVue) : 
_categorie(categorie), _types(types), _posX(posX), _posY(posY), _sante(santeInitiale), _maxSante(santeInitiale), _attaque(attaque), 
_defense(defense), _distanceVue(distanceVue){
    _ordreRecu = std::make_shared<Ordre>(TypeOrdre::IMMOBILISER, 0, 0);
};

void Unite::donnerOrdre(std::shared_ptr<Ordre> ordre) { 
    _ordreRecu = ordre; 
}

std::string Unite::toString() const {
    std::string resultat = "("+std::to_string(_posX)+","+std::to_string(_posY)+") "+_categorie+" : ";
    for (unsigned int i = 0; i < _types.size(); i++) resultat+=_types[i]+" , ";
    return resultat;
}

//on considère que le combat est équilibré. Les boots ou autres changements seront fais dans la méthode combat de la classe Map
std::pair<int, int> Unite::resultatCombatSimple(Unite const &ennemy)const{
    std::pair<int, int> resultat;
    //1er int correspond aux dégâts que recoit l'unité this et le deuxième aux dégats de l'unité
    resultat.first = (static_cast<float>(ennemy._attaque)*(1.0 / ( static_cast<float>(_defense) ) ) )*static_cast<float>(ennemy._moral);
    resultat.second = (static_cast<float>(_attaque)*(1.0 / ( static_cast<float>(ennemy._defense) ) ) )*static_cast<float>(_moral);

    return resultat;

}


/*Getters / SETTERS*/


int Unite::getX()const{
    return _posX;
}

int Unite::getY()const{
    return _posY;
}

int Unite::getDistanceVue()const{
    return _distanceVue;
}

void Unite::avancer() {
    if (_chemin.empty()) {
        _pointsMouvement = 0;
    } else {
        _pointsMouvement++;
        if (_pointsMouvement > 10) {
            std::cout<<_chemin[0].first<<", "<<_chemin[0].second<<std::endl;
            _pointsMouvement = 0;
            _posX = _chemin[0].first;
            _posY = _chemin[0].second;
            _chemin.erase(_chemin.begin());
        }
    }
}

void Unite::initialiserMouvement(std::vector<std::pair<int,int>> chemin) {
    _pointsMouvement = 0;
    _chemin = chemin;
}

std::shared_ptr<Ordre> Unite::getOrdre() const{
    return _ordreRecu;
}

std::pair<int,int> Unite::getPos() const {
    return std::make_pair(_posX, _posY);
}