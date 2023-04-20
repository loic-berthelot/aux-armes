#include "decisions/humain.h"
#include "decisions/IA.h"
#include <memory>

class Jeu {
    Carte _carte;
    std::vector<std::shared_ptr<Joueur>> _joueurs;
    unsigned int _toursPasses = 0;
public:
    Jeu();
    Jeu(unsigned int tailleMap, std::string const &armeeDesc);
    void ajouterJoueur(bool estHumain = false);
    bool partieFinie();
    void jouer();
};