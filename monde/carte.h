#include "armee.h"
#include "case.h"
#include "graphe.h"
#include <sstream>   // Pour les flux de chaînes de caractères
#include <map>

class Carte {
private:
    std::map<std::pair<int,int>,std::shared_ptr<Case>> _cases;
    std::vector<std::shared_ptr<Armee>> _armees;
    std::shared_ptr<Graphe> _grapheAir;
    std::shared_ptr<Graphe> _grapheTerre;
    std::shared_ptr<Graphe> _grapheEauEtTerre;
    std::shared_ptr<Graphe> _grapheEau;
    int _rayon;
    unsigned int _indiceArmee;

public:
    std::shared_ptr<Graphe> creerGraphe(accessibilite acces) const;

    std::shared_ptr<Graphe> getGraphe(accessibilite acces);

    Carte(int taille, std::vector<std::shared_ptr<Armee>> const &armees);

    Carte(){
        
    }
    Carte(int rayon);

    std::pair<int,int> positionAleatoireCarte();
    /*Méthode armée ============================================*/

    //créer une armée vide
    void creerArmee();
    //ne marche pas encore donc à renseigner
    void afficherArmee() const;

    void afficherArmees() const;

    unsigned int nombreArmeesVivantes() const;

    void selectionnerArmee(unsigned int indiceArmee);

    std::vector<std::pair<int,int>> getDepartsRavitaillement() const;

    std::vector<std::pair<int,int>> getPositionsEnnemis() const;

    std::map<std::pair<int,int>, int> getRelaisRavitaillement() const;

    void ravitaillerArmee();

    void appliquerAttritionArmee();

    //execute l'ordre du tour
    void executerOrdresArmee();

    bool ennemiSurCase(std::pair<int,int> pos) const;

    void retirerCadavres();
    //fait combattre 2 unités

    void combat(std::shared_ptr<Unite> u1, unsigned int idTeam, int x, int y);

    /*Methode de la carte (MAP ) =============================*/

    

    void brouillardDeGuerreUnite(std::shared_ptr<Unite> unite, std::vector<std::pair<int,int>> &vecteur)const;

    std::vector<std::pair<int, int>> brouillardDeGuerreEquipe(unsigned int i)const;

    //renvoie les coordonnées des voisins
    std::vector<std::pair<int, int>> getCoordonneesVoisins(int posX, int posY)const;

    //Attention les X et Y sont les coordonnées en fonction du milieu
    std::shared_ptr<Case> getCase(int x, int y)const;

    void affichageSeulementCarte()const;

    void genererCarteVide(std::string const &typeCase, unsigned int taille);


    void ajoutUniteTeam(unsigned int IDarmee, std::shared_ptr<Unite> unite);
    
    float ratioAlliesAdversaires(std::shared_ptr<Unite> unite, unsigned int zoneAutour, unsigned int idEquipeJoueur)const;

    //getters & setters

    std::shared_ptr<Armee> getArmee() const;
    

    // Fonction pour calculer la distance entre deux points hexagonaux (i, j) et (k, l) sur la carte
    static double distanceEntrePointsHexagonaux(int i, int j, int k, int l);

    /*
    Editeur de map
    */
    //calcul une valeur entre a et b en fonction de parametreMelange
    //qui est compris entre 0 et 1, plus c'est proche de 0 plus la valeur sera proche de A
    double calculIntermediaire(double pointA, double pointB, double parametreMelange)const;


    // Fonction pour calculer le produit scalaire entre un gradient et un vecteur
    double vecteurPente(int hash, double x, double y) const;

    // Fonction pour interpoler
    double fade(double t)const ;


    // Fonction pour calculer la valeur de bruit de Perlin en 2D
    double perlin2D(double x, double y) const;

    static std::string valueToCaseNom(float Value);


    bool peutEtreEn(int x, int y, std::shared_ptr<Unite> u1){
        return (u1->getCategorie() == accessibilite::Air)||((u1->getCategorie() == accessibilite::Eau || u1->getCategorie() == accessibilite::EauEtTerre) && _cases[std::make_pair(x, y)]->accessibleEau())
        || ((u1->getCategorie() == accessibilite::Terre || u1->getCategorie() == accessibilite::EauEtTerre) && _cases[std::make_pair(x, y)]->accessibleTerre());
    }

    bool caseAvecUnite(int x, int y)const;
    
    
};


