#include "unite.h"

Unite::Unite(const std::string & categorie, const std::vector<std::string> & types, int posX, int posY, int santeInitiale, int attaque, 
int defense, int distanceVue) : 
_categorie(categorie), _types(types), _posX(posX), _posY(posY), _sante(santeInitiale), _maxSante(santeInitiale), _attaque(attaque), 
_defense(defense), _distanceVue(distanceVue){
    
};

std::string Unite::toString() const {
    std::string resultat = "("+std::to_string(_posX)+","+std::to_string(_posY)+") "+_categorie+" : ";
    for (unsigned int i = 0; i < _types.size(); i++) resultat+=_types[i]+" , ";
    return resultat;
}

void Unite::executerOrdre() {
    if (_ordreRecu) {
        OrdreDeplacer* ordreDeplacer = static_cast<OrdreDeplacer*>(_ordreRecu.get());
        if (ordreDeplacer) {
            _posX += ordreDeplacer->getDx();
            _posY += ordreDeplacer->getDy();
            return;
        }
    }
}


//on considère que le combat est équilibré. Les boots ou autres changements seront fais dans la méthode combat de la classe Map
std::pair<int, int> Unite::resultatCombatSimple(Unite const &ennemy)const{
    std::pair<int, int> resultat;
    //1er int correspond aux dégâts que recoit l'unité this et le deuxième aux dégats de l'unité
    resultat.first = (static_cast<float>(ennemy._attaque)*(1.0 / ( static_cast<float>(_defense) ) ) )*static_cast<float>(ennemy._moral);
    resultat.second = (static_cast<float>(_attaque)*(1.0 / ( static_cast<float>(ennemy._defense) ) ) )*static_cast<float>(_moral);

    return resultat;

}