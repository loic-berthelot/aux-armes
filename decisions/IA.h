#include "joueur.h"
#include <algorithm>
#include <random>

class IA : public Joueur {
public:
    void jouerUnite(Carte & carte, std::shared_ptr<Unite> unite);
    void jouerArmee(Carte & carte) override;
    void poursuivreEnnemi(std::shared_ptr<Unite> unite, std::pair<int,int> pos);
    void suivreAllie(std::shared_ptr<Unite> unite, std::shared_ptr<Unite> ennemi);
    void attaquerEnnemi(std::shared_ptr<Unite> unite, std::pair<int,int> pos);
    int nombreVoisinsInexplores(std::pair<int,int> pos, int rayon, Carte & carte);
    int nombreVoisinsInexplores(std::shared_ptr<Unite> unite, Carte & carte);
    void explorer(std::shared_ptr<Unite> unite, Carte & carte);
    void fuir(std::shared_ptr<Unite> unite, Carte & carte);
    void garderPosition(std::shared_ptr<Unite> unite, std::pair<int,int> pos, Carte & carte);
    void ravitailler(std::shared_ptr<Unite> unite, std::pair<int,int> arrivee, Carte & carte);
};