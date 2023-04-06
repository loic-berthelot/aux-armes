#include "humain.h"

void Humain::jouerArmee(Carte & carte) {
    Armee * armee = carte.getArmee(_idArmee);
    for (unsigned int i = 0; i < armee->taille(); i++) {
        std::cout<<"Donnez un ordre a l'unite "<<i<<" : ";
        std::string typeOrdre;
        int val1, val2;
        std::cin>>typeOrdre>>val1>>val2;
        if (typeOrdre == "deplacer") armee->donnerOrdre(i, new OrdreDeplacer(val1, val2));
    }      
}