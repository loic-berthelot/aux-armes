#include "joueur.h"
#include <algorithm>
#include <random>
#include <math.h>
#include <map>

class IA : public Joueur {
    std::vector<std::pair<int,int>> _villesControlees;
    std::vector<std::pair<int,int>> _villesSelectionnees;
    std::pair<int,int> _centreArmee;
    unsigned int _nombreVillesNecessaires;
    float _agressivite;
    std::vector<std::shared_ptr<Unite>> _unitesDispersees;
    std::vector<std::pair<int,int>> _positionsEnnemisVisibles;
    std::map<std::pair<int,int>, int> _degatsNecessaires;
    std::map<std::pair<int,int>, int> _scoresExploration;
    std::map<std::pair<int,int>, bool> _positionsRavitaillees;
public:
    void calculerCentreArmee(std::shared_ptr<Armee> armee); 
    bool villeControlee(const std::pair<int,int> & pos, std::shared_ptr<Armee> armee) const;
    void calculerVillesNecessaires(Carte & carte);
    int scoreVille(Carte & carte, const std::pair<int,int> & pos) const;
    int nombreVoisinsInexplores(const std::pair<int,int> & pos, int rayon, Carte & carte);
    int nombreVoisinsInexplores(std::shared_ptr<Unite> unite, Carte & carte);      
    int scoreSoutienUnite(std::shared_ptr<Unite> unite) const;
    int scoreExplorationUnite(std::shared_ptr<Unite> unite) const;
    std::pair<int,int> plusProcheVoisin(const std::pair<int,int> & pos, const std::vector<std::pair<int,int>> & voisins) const;
    bool uniteDansVille(std::shared_ptr<Unite> unite) const;
    void calculerPositionsEnnemis(const std::map<std::pair<int,int>, std::vector<std::shared_ptr<Unite>>> & ennemis);
    std::map<std::shared_ptr<Unite>, std::pair<int,int>> dispersionAleatoire(const std::map<std::shared_ptr<Unite>, std::vector<std::pair<int,int>>> & casesAccessibles);
    int calculerScoreDispersion(Carte & carte, const std::map<std::shared_ptr<Unite>, std::pair<int,int>> & dispersion);    
    int scoreAttaque(std::shared_ptr<Unite> unite, const std::vector<std::shared_ptr<Unite>> & ennemis) const;
    void calculerPositionsRavitaillees(Carte & carte, const std::vector<std::shared_ptr<Unite>> & unites);
    std::shared_ptr<Unite> meilleureUniteRavitaillement(Carte & carte, const std::pair<int,int> & pos) const;

    void initialiser(bool debut);
    void selectionnerVilles(Carte & carte);
    void etendreRavitaillement(Carte & carte, std::shared_ptr<Unite> unite); 
    void choisirDispersion(Carte & carte);
    void calculerDegatsNecessaires(const std::map<std::pair<int,int>, std::vector<std::shared_ptr<Unite>>> & ennemis);
    bool attribuerCible(Carte & carte, std::shared_ptr<Unite> unite, const std::map<std::pair<int,int>, std::vector<std::shared_ptr<Unite>>> & ennemis);
    void calculerScoresExploration(Carte & carte);
    void explorer(Carte & carte, std::shared_ptr<Unite> unite);
    void defendre(Carte & carte, std::shared_ptr<Unite> unite);

    void jouerArmee(Carte & carte) override;
};