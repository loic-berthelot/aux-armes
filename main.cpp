#include "jeu.h"
#include "monde/case.h"

int main() {

    Carte c;

    c.genererMapVide("Plaine", 20);
    c.affichageSeulementCarte();
    c.getCase(3,0);
    c.sauvegarderCarteMap("../map.map");
    c.chargerCarteMap("../map.map");

    

    //Jeu jeu;
    //jeu.initialiser();
    //jeu.jouer();
    return 0;

}
