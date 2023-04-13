#include "jeu.h"
#include "monde/case.h"

int main() {

    Carte c(6);
    std::vector<std::string> types;
    c.creerArmee();
    //Unite(const std::string & categorie, const std::vector<std::string> & types, int posX, int posY, int santeInitiale, int attaque, 
    //int defense, int distanceVue);
    Unite u1("Archer", types, 0, 0, 100, 100, 100, 2);
    c.ajoutUniteTeam(0,u1);

    std::vector<std::pair<int, int>> brouillard = c.brouillardDeGuerreEquipe(0);

    for (int i = 0; i < brouillard.size(); i ++){
        std::cout << "X : "<<brouillard[i].first << " y : "<<brouillard[i].second<< std::endl;
    }
    std::cout << c.ratioAlliesAdversaires(u1, 5, 0);
    /*u1.setMoral(200);
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
