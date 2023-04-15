#include "IA.h"

void IA::jouerArmee(Carte & carte) {
    std::shared_ptr<Armee> armee = carte.getArmee();
    for (unsigned int i = 0; i < armee->taille(); i++) 
      armee->donnerOrdre(i, std::make_shared<Ordre>(TypeOrdre::DEPLACER, 0, 0));
}