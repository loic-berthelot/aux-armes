#include "armee.h"

Armee::Armee() {
    std::vector<std::string> temp1 = {"T34"};
    _unites.emplace_back("terrestre", temp1, 1, 1);
    std::vector<std::string> temp2 = {"CRS"};
    _unites.emplace_back("terrestre", temp2, 2, 3);
}


void Armee::afficher() {
    for (unsigned int i = 0; i < _unites.size(); i++) std::cout<<_unites[i].toString();
}