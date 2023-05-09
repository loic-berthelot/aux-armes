#include "case.h"
#include <sstream>   // Pour les flux de chaînes de caractères
#include <map>
#include "armee.h"
#include "bruitDePerlin.h"

class Carte {
private:
    std::map<std::pair<int,int>,std::shared_ptr<Case>> _cases;
    std::map<std::pair<int,int>,bool> _casesVisibles;
    std::vector<std::shared_ptr<Armee>> _armees;
    std::shared_ptr<Graphe> _grapheAir;
    std::shared_ptr<Graphe> _grapheTerre;
    std::shared_ptr<Graphe> _grapheEauEtTerre;
    std::shared_ptr<Graphe> _grapheEau;
    std::shared_ptr<Graphe> _grapheVision;
    int _rayon;
    unsigned int _indiceArmee;
    unsigned int _nbToursMax;
    std::map<float, std::string> _valeursCasesGenerateurs;//utilisé dans le bruit de Perlin
    std::string _mapDernierCase;
    std::vector<std::pair<int,int>> _departsRavitaillement;
public:
    std::vector<std::pair<int, int>> genererVille(int nbVilles);

    void placerUnites(std::vector<std::pair<int, int>> const &villes);

    std::shared_ptr<Graphe> creerGraphe(accessibilite acces, bool coutDeplacement = true) const; 

    std::shared_ptr<Graphe> getGraphe(accessibilite acces) const;

    Carte(std::string const &nomFichierConfig, std::vector<std::shared_ptr<Armee>> const &armees);

<<<<<<< HEAD
    void sauvegarder(const std::string & nom);

    std::vector<std::string> separerChaine(std::string chaine, char separateur) const;

    void chargerSauvegarde(const std::string & nomFichier);

=======
>>>>>>> a771ba045e851b1d58ecdae66ddbbe660bc869b6
    void initialiserVisibilite();

    std::pair<int,int> positionAleatoireCarte();
    
    std::vector<std::pair<int,int>> positionsAccessibles(std::shared_ptr<Unite> unite, int nbTours=1) const;

    /*Méthode armée ============================================*/    
    /*Méthode armée ============================================*/

    std::shared_ptr<Armee> getArmee(unsigned int i)const{
        return _armees[i];
    }

    unsigned int getNbArmee()const{
        return _armees.size();
    }

    int getRayon() const;

    void creerArmee();//créer une armée vide
    
    void afficherArmee() const;//ne marche pas encore donc à renseigner

    void afficherArmees() const;

    unsigned int nombreArmeesVivantes() const;

    unsigned int getMaxTours()const;

    void selectionnerArmee(unsigned int indiceArmee);

    void calculerDepartsRavitaillement();

    std::vector<std::pair<int,int>> getDepartsRavitaillement() const;

    std::vector<std::pair<int,int>> getPositionsEnnemis() const;

    std::map<std::pair<int,int>, int> getRelaisRavitaillement(std::shared_ptr<Unite> unite = nullptr) const;

    std::map<std::pair<int,int>, std::vector<std::shared_ptr<Unite>>> getUnitesVisibles(bool allies = true);

    void ravitaillerArmee();

    void appliquerAttritionArmee();

    std::vector<std::shared_ptr<Unite>> unitesSurCase(std::pair<int,int> pos);

    void executerOrdresArmee();

    void infligerDegatsDeZone(std::pair<int,int> pos, int degats);

    void retirerCadavres();
    
    void evolutionMoralArmee();

    void combat(std::shared_ptr<Unite> u1, unsigned int idTeam, std::pair<int,int> positionCombat);//fait combattre 2 unités

    /*Methode de la carte (MAP ) =============================*/
    void brouillardDeGuerreUnite(std::shared_ptr<Unite> unite);

    void brouillardDeGuerreEquipe();
    
    std::vector<std::pair<int, int>> getCoordonneesVoisins(std::pair<int,int> pos)const;//renvoie les coordonnées des voisins
    
    std::vector<std::pair<int, int>> getCoordonneesVoisins(std::pair<int,int> pos, int rayon)const;

    std::vector<std::pair<int, int>> getCoordonneesCouronne(std::pair<int,int> pos, int rayon)const;

    std::shared_ptr<Case> getCase(int x, int y)const;//Attention les X et Y sont les coordonnées en fonction du milieu

    std::shared_ptr<Case> getCase(std::pair<int,int> pos) const;

    void affichageSeulementCarte()const;

    void ajoutUniteTeam(unsigned int IDarmee, std::shared_ptr<Unite> unite);
    
    float ratioAlliesAdversaires(std::shared_ptr<Unite> unite, unsigned int zoneAutour, unsigned int idEquipeJoueur)const;

    std::shared_ptr<Armee> getArmee() const;

    std::vector<std::pair<unsigned int, int>> getScoreEquipe()const;
    
    std::string valueToCaseNom(float Value);

    bool peutEtreEn(std::pair<int,int> pos, std::shared_ptr<Unite> u1);

    bool caseAvecUnite(std::pair<int,int> pos)const;

    bool ennemiSurCase(int x, int y)const;

    bool ennemiSurCase(std::pair<int,int> pos) const;
    
    bool caseVisible(std::pair<int,int> pos) const;

    std::vector<std::shared_ptr<Unite>> unitesAllieesSurCase(std::pair<int,int> pos);

    int porteeRavitaillementAllie(std::pair<int,int> pos) const; //renvoie la portée de ravitaillement maximale des unités alliées de cette case
};
