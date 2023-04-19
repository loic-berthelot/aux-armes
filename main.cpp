#include "jeu.h"

int main() {

    //Jeu jeu;
    //jeu.jouer();
    std::shared_ptr<Unite> uPointeur = std::make_shared<Unite>("Garde Royale", 0, 0);

    Case::creerCaseEtSauvegardeFichier("Montagne", accessibilite::Terre, 200, 150);
    //c.ajoutUniteTeam(0,u1);
    //Armée1 : 
    std::shared_ptr<Armee> armee1 = std::make_shared<Armee>();
    std::shared_ptr<Armee> armee2 = std::make_shared<Armee>();
    armee1->ajoutUnite(std::make_shared<Unite>("Garde Royale", 0, 0));
    armee1->ajoutUnite(std::make_shared<Unite>("Garde Royale", 0, 0));
    armee2->ajoutUnite(std::make_shared<Unite>("Garde Royale", 0, 0));
    std::vector<std::shared_ptr<Armee>> unites;
    unites.push_back(armee1);
    unites.push_back(armee2);

    Jeu jeu(unites, 15);
    jeu.jouer();

    //std::cout << c.ratioAlliesAdversaires(uPointeur, 5, 0);
    

    return 0;

}
