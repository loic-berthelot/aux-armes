#include "armee.h"
#include "case.h"

class Carte {
private:
    std::vector<std::vector<Case>> _carte;
    std::vector<std::shared_ptr<Armee>> _armees;


public:

    /*Méthode armée ============================================*/

    //créer une armée vide
    void creerArmee();
    //ne marche pas encore donc à renseigner
    void afficher()const;
    //execute l'ordre du tour
    void executerOrdresTour(unsigned int indiceArmee);

    /*Methode de la carte (MAP ) =============================*/

    //renvoie les coordonnées des voisins
    std::vector<std::pair<int, int>> getVoisinsCoordonnes(int x, int y)const;

    //Attention les X et Y sont les coordonnées en fonction du milieu
    Case getCase(int x, int y)const;

    void affichageSeulementCarte()const;

    void genererMapVide(std::string const &typeCase, unsigned int taille);

    //getters & setters
    std::vector<std::vector<Case>> getCarte()const;
    std::shared_ptr<Armee> getArmee(unsigned int i) const;
    
    

};