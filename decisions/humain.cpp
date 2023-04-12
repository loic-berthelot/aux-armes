#include "humain.h"

void Humain::jouerArmee(Carte & carte) {
    std::shared_ptr<Armee> armee = carte.getArmee(_idArmee);
    for (unsigned int i = 0; i < armee->taille(); i++) {
        std::cout<<"Donnez un ordre a l'unite "<<i<<" : ";
        std::string typeOrdre;
        int val1, val2;
        std::cin>>typeOrdre>>val1>>val2;
        if (typeOrdre == "deplacer") armee->donnerOrdre(i, std::make_shared<Ordre>(ORDRE_DEPLACER, val1, val2));
    }      
}