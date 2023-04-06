#include <vector>
#include "decisions/humain.h"
#include "decisions/IA.h"

class Jeu {
    Carte _carte;
    std::vector<Joueur*> _joueurs;
    unsigned int _toursPasses = 0;
public:
    void ajouterJoueur(bool estHumain = false);
    bool partieFinie();
    void initialiser();
    void jouer();v
};