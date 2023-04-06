#include "joueur.h"

void Joueur::jouerArmee(Carte & carte) {
    Armee * armee = carte.getArmee(_idArmee);
    //for (unsigned int i = 0; i < armee->taille(); i++) 
      //  armee->donnerOrdre(i, new OrdreDeplacer(1, 1));
}