#include "IA.h"

void IA::jouerArmee(Carte & carte) {
    std::shared_ptr<Armee> armee = carte.getArmee();
    for (unsigned int i = 0; i < armee->taille(); i++) {
      std::pair<int,int> pos = carte.positionAleatoireCarte();
      armee->donnerOrdre(i, std::make_shared<Ordre>(TypeOrdre::DEPLACER, pos.first, pos.second));
    }
}