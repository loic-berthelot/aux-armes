#include "jeu.h"
#include "monde/case.h"

int main() {

<<<<<<< HEAD
    Carte c(6);
    std::vector<Type> types;
    types.push_back(Type("Infanterie"));
    Unite::CreationNouvelleUnite("Garde Royale", types, 1000, Categorie::Terre, 150, 300, 4, 100);
=======
    Jeu jeu;
    jeu.jouer();
    std::vector<std::string> types;
/*
>>>>>>> 2f9aa9287ac78531a7f0d32c87a29b7a20a0a55a
    c.creerArmee();
    Unite u("Garde Royale", 0, 0);
    //Unite(const std::string & categorie, const std::vector<std::string> & types, int posX, int posY, int santeInitiale, int attaque, 
    //int defense, int distanceVue);
    //Unite u1("Archer", types, 0, 0, 100, 100, 100, 2);
    //c.ajoutUniteTeam(0,u1);


<<<<<<< HEAD
    
    Type t("Infanterie");

    //std::cout << c.ratioAlliesAdversaires(u1, 5, 0);
    /*u1.setMoral(200);
=======
    for (int i = 0; i < brouillard.size(); i ++){
        std::cout << "X : "<<brouillard[i].first << " y : "<<brouillard[i].second<< std::endl;
    }
    std::cout << c.ratioAlliesAdversaires(u1, 5, 0);
    u1.setMoral(200);
>>>>>>> 2f9aa9287ac78531a7f0d32c87a29b7a20a0a55a
    std::pair<int, int> result = u1.resultatCombatSimple(u2);

    std::cout << result.first << " : "<<result.second<<std::endl;

    //const std::string & categorie, const std::vector<std::string> & types, int posX, int posY, int santeInitiale, int attaque, int defense
*/
    

    //c.affichageSeulementCarte();
    //c.getCase(3,0);
    //Jeu jeu;
    //jeu.initialiser();
    //jeu.jouer();
    return 0;

}
