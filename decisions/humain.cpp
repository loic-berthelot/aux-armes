#include "humain.h"

void Humain::jouerArmee(Carte & carte) {
    std::shared_ptr<Armee> armee = carte.getArmee();
    for (unsigned int i = 0; i < armee->taille(); i++) {
        std::cout<<"Donnez un ordre à l'unite "<<i<<" ("<<armee->getUnite(i)->getNom()<<") en ("<<armee->getUnite(i)->getX()<<","<<armee->getUnite(i)->getY()<<") : ";
        std::string typeOrdre;
        int val1, val2;
        std::cin>>typeOrdre>>val1>>val2;
        if (typeOrdre == "deplacer") armee->donnerOrdre(i, std::make_shared<Ordre>(TypeOrdre::DEPLACER, val1, val2));
        else if (typeOrdre == "attaquer") armee->donnerOrdre(i, std::make_shared<Ordre>(TypeOrdre::ATTAQUER, val1, val2));
        else if (typeOrdre == "immobiliser") armee->donnerOrdre(i, std::make_shared<Ordre>(TypeOrdre::IMMOBILISER, val1, val2));
    }      
}