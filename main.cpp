#include "jeu.h"
#include "monde/case.h"

int main() {

    Carte c;

    c.genererMapVide("Plaine", 30);
    c.affichageSeulementCarte();
    c.getCase(3,0);
    //Jeu jeu;
    //jeu.initialiser();
    //jeu.jouer();
    return 0;

}