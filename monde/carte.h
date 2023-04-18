#include "armee.h"
#include "case.h"
#include "graphe.h"
#include <sstream>   // Pour les flux de chaînes de caractères
#include <map>

class Carte {
private:
    std::map<std::pair<int,int>,std::shared_ptr<Case>> _cases;
    std::vector<std::shared_ptr<Armee>> _armees;
    std::shared_ptr<Graphe> _grapheCases;
    int _rayon;
    unsigned int _indiceArmee;

public:
    std::shared_ptr<Graphe> creerGraphe() const;

    Carte(int rayon);

    /*Méthode armée ============================================*/

    //créer une armée vide
    void creerArmee();
    //ne marche pas encore donc à renseigner
    void afficherArmee() const;

    void afficherArmees() const;

    void selectionnerArmee(unsigned int indiceArmee);
    //execute l'ordre du tour
    void executerOrdresArmee();

    //fais combattre 2 unités

    void combat(std::shared_ptr<Unite> u1,unsigned int idTeam1, 
    std::shared_ptr<Unite> u2, unsigned int idTeam2);

    /*Methode de la carte (MAP ) =============================*/


    void brouillardDeGuerreUnite(std::shared_ptr<Unite> unite, std::vector<std::pair<int,int>> &vecteur)const;

    std::vector<std::pair<int, int>> brouillardDeGuerreEquipe(unsigned int i)const;

    //renvoie les coordonnées des voisins
    std::vector<std::pair<int, int>> getCoordonneesVoisins(int posX, int posY)const;

    //Attention les X et Y sont les coordonnées en fonction du milieu
    std::shared_ptr<Case> getCase(int x, int y)const;

    void affichageSeulementCarte()const;

    void genererCarteVide(std::string const &typeCase, unsigned int taille);

    void sauvegarderCarteMap(std::string const &path)const;

    void chargerCarteMap(std::string const &path);

    void ajoutUniteTeam(unsigned int IDarmee, std::shared_ptr<Unite> unite);
    
    float ratioAlliesAdversaires(std::shared_ptr<Unite> unite, unsigned int zoneAutour, unsigned int idEquipeJoueur)const;

    //getters & setters

    std::shared_ptr<Armee> getArmee() const;
};