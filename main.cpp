#include "jeu.h"
#include "monde/case.h"

int main() {

    Carte c(1);

    c.genererMapVide("Plaine", 1);
    c.affichageSeulementCarte();
    c.getCase(3,0);
    c.sauvegarderCarteMap("../map.map");
    c.chargerCarteMap("../map.map");

    

    //Jeu jeu;
    //jeu.initialiser();
    //jeu.jouer();
    return 0;

}
