#include "unite.h"

Unite::Unite(const std::string & categorie, const std::vector<std::string> & types, int posX, int posY) : _categorie(categorie), _types(types), _posX(posX), _posY(posY) {
    
};

std::string Unite::toString() const {
    std::string resultat = "("+std::to_string(_posX)+","+std::to_string(_posY)+") "+_categorie+" : ";
    for (unsigned int i = 0; i < _types.size(); i++) resultat+=_types[i]+" , ";
    return resultat;
}

void Unite::executerOrdre() {
    if (_ordreRecu) {
        OrdreDeplacer* ordreDeplacer = static_cast<OrdreDeplacer*>(_ordreRecu);
        if (ordreDeplacer) {
            _posX += ordreDeplacer->getDx();
            _posY += ordreDeplacer->getDy();
            return;
        }
    }
}