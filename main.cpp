#include "jeu.h"
#include "monde/case.h"

int main() {

    Carte c(3);
    //Jeu jeu;
    //jeu.jouer();

    //c.creerArmee();
    std::shared_ptr<Unite> uPointeur = std::make_shared<Unite>("Garde Royale", 0, 0);

    //c.ajoutUniteTeam(0,u1);

    std::cout << c.ratioAlliesAdversaires(uPointeur, 5, 0);
    

    return 0;

}
