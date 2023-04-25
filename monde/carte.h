#include "armee.h"
#include "case.h"
#include "graphe.h"
#include <sstream>   // Pour les flux de chaînes de caractères
#include <map>

class Carte {
private:
    std::map<std::pair<int,int>,std::shared_ptr<Case>> _cases;
    std::map<std::pair<int,int>,bool> _casesVisibles;
    std::vector<std::shared_ptr<Armee>> _armees;
    std::shared_ptr<Graphe> _grapheAir;
    std::shared_ptr<Graphe> _grapheTerre;
    std::shared_ptr<Graphe> _grapheEauEtTerre;
    std::shared_ptr<Graphe> _grapheEau;
    int _rayon;
    unsigned int _indiceArmee;

    unsigned int _nbToursMax;

    std::map<float, std::string> _valeursCasesGenerateurs;//utilisé dans le bruit de Perlin
    std::string _mapDernierCase;

public:
    
    std::shared_ptr<Graphe> creerGraphe(accessibilite acces) const;

    std::shared_ptr<Graphe> getGraphe(accessibilite acces) const;

    Carte(std::string const &nomFichierConfig, std::vector<std::shared_ptr<Armee>> const &armees);

    void initialiserVisibilite();

    std::pair<int,int> positionAleatoireCarte();
    
    std::vector<std::pair<int,int>> positionsAccessibles(std::shared_ptr<Unite> unite) const;

    /*Méthode armée ============================================*/    
    /*Méthode armée ============================================*/

    std::shared_ptr<Armee> getArmee(unsigned int i)const{
        return _armees[i];
    }

    unsigned int getNbArmee()const{
        return _armees.size();
    }

    void creerArmee();//créer une armée vide
    
    void afficherArmee() const;//ne marche pas encore donc à renseigner

    void afficherArmees() const;

    unsigned int nombreArmeesVivantes() const;

    unsigned int getMaxTours()const;

    void selectionnerArmee(unsigned int indiceArmee);

    std::vector<std::pair<int,int>> getDepartsRavitaillement() const;

    std::vector<std::pair<int,int>> getPositionsEnnemis() const;

    std::map<std::pair<int,int>, int> getRelaisRavitaillement(std::shared_ptr<Unite> unite = nullptr) const;

    std::map<std::pair<int,int>, std::shared_ptr<Unite>> getUnitesVisibles(bool allies = true);

    void ravitaillerArmee();

    void appliquerAttritionArmee();

    std::vector<std::shared_ptr<Unite>> unitesSurCase(std::pair<int,int> pos);

    void executerOrdresArmee();

    void infligerDegatsDeZone(std::pair<int,int> pos, int degats);

    bool ennemiSurCase(std::pair<int,int> pos) const;

    void retirerCadavres();
    
    void evolutionMoralArmee();

    void combat(std::shared_ptr<Unite> u1, unsigned int idTeam, std::pair<int,int> positionCombat);//fait combattre 2 unités

    /*Methode de la carte (MAP ) =============================*/
    void brouillardDeGuerreUnite(std::shared_ptr<Unite> unite, std::vector<std::pair<int,int>> &vecteur)const;

    void brouillardDeGuerreEquipe();
    
    std::vector<std::pair<int, int>> getCoordonneesVoisins(int posX, int posY)const;//renvoie les coordonnées des voisins
    
    std::vector<std::pair<int, int>> getCoordonneesRayon(std::pair<int,int> pos, int rayon)const;

    std::shared_ptr<Case> getCase(int x, int y)const;//Attention les X et Y sont les coordonnées en fonction du milieu

    std::shared_ptr<Case> getCase(std::pair<int,int> pos) const;

    void affichageSeulementCarte()const;

    void ajoutUniteTeam(unsigned int IDarmee, std::shared_ptr<Unite> unite);
    
    float ratioAlliesAdversaires(std::shared_ptr<Unite> unite, unsigned int zoneAutour, unsigned int idEquipeJoueur)const;

    std::shared_ptr<Armee> getArmee() const;

    std::vector<std::pair<unsigned int, int>> getScoreEquipe()const;
    /*
    Editeur de map
    */
    //calcul une valeur entre a et b en fonction de parametreMelange
    //qui est compris entre 0 et 1, plus c'est proche de 0 plus la valeur sera proche de A
    double calculIntermediaire(double pointA, double pointB, double parametreMelange)const;

    // Fonction pour calculer le produit scalaire entre un gradient et un vecteur
    double vecteurPente(int hash, double x, double y) const;
    
    double fade(double t)const ;// Fonction pour interpoler

    // Fonction pour calculer la valeur de bruit de Perlin en 2D
    double perlin2D(double x, double y) const;

    std::string valueToCaseNom(float Value);

    bool peutEtreEn(int x, int y, std::shared_ptr<Unite> u1);

    bool caseAvecUnite(int x, int y)const;

    bool caseAvecEnnemi(int x, int y)const;
    
    bool caseVisible(std::pair<int,int> pos) const;
};
