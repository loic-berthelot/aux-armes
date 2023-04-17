#include "armee.h"

Armee::Armee() {
    //std::vector<std::string> temp1 = {"T34"};
    //_unites.emplace_back(std::make_shared<Unite>("terrestre", temp1, 1, 1, 10, 2, 2, 2));
}

void Armee::afficher() const {
    for (unsigned int i = 0; i < _unites.size(); i++) std::cout<<_unites[i]->toString()<<std::endl;
}

unsigned int Armee::taille() const { 
    return _unites.size(); 
}

void Armee::donnerOrdre(unsigned int indiceUnite, std::shared_ptr<Ordre> ordre) { 
    _unites[indiceUnite]->donnerOrdre(ordre); 
}

void Armee::ajoutUnite(std::shared_ptr<Unite> u){
    _unites.push_back(u);
}


    /*GETTERS = ===================*/
std::vector<std::shared_ptr<Unite>> Armee::getUnites()  {
    return _unites;
}

std::shared_ptr<Unite> Armee::getUnite(unsigned int i) const {
    return _unites[i];
}

unsigned int Armee::size()const{
    return _unites.size();

}
