#include "armee.h"
#include "case.h"
#include <sstream>   // Pour les flux de chaînes de caractères


class Carte {
private:
    std::vector<std::vector<Case>> _cases;
    std::vector<std::shared_ptr<Armee>> _armees;
    std::shared_ptr<Graphe> _grapheCases;
    int _rayon;
public:
    Carte(int rayon);

    /*Méthode armée ============================================*/

    //créer une armée vide
    void creerArmee();
    //ne marche pas encore donc à renseigner
    void afficher()const;
    //execute l'ordre du tour
    void executerOrdresTour(unsigned int indiceArmee);

    /*Methode de la carte (MAP ) =============================*/

    //renvoie les coordonnées des voisins
    std::vector<std::pair<int, int>> getVoisinsCoordonnees(int x, int y)const;

    //Attention les X et Y sont les coordonnées en fonction du milieu
    Case getCase(int x, int y)const;

    void affichageSeulementCarte()const;

    void genererMapVide(std::string const &typeCase, unsigned int taille);


    void sauvegarderCarteMap(std::string const &path)const;
    void chargerCarteMap(std::string const &path);

    //getters & setters
    std::vector<std::vector<Case>> getCarte()const;
    std::shared_ptr<Armee> getArmee(unsigned int i) const;
    
    

};