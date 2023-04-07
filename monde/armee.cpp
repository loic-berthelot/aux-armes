#include "armee.h"

Armee::Armee() {
    std::vector<std::string> temp1 = {"T34"};
    _unites.emplace_back("terrestre", temp1, 1, 1);
    std::vector<std::string> temp2 = {"CRS"};
    _unites.emplace_back("terrestre", temp2, 2, 3);
}


void Armee::afficher() const {
    for (unsigned int i = 0; i < _unites.size(); i++) std::cout<<_unites[i].toString();
}

unsigned int Armee::taille() const { 
    return _unites.size(); 
}

void Armee::donnerOrdre(unsigned int indiceUnite, std::shared_ptr<Ordre> ordre) { 
    _unites[indiceUnite].donnerOrdre(ordre); 
}

void Armee::executerOrdresTour()
{
    for (unsigned int i = 0; i < _unites.size(); i++)
        _unites[i].executerOrdre();
};