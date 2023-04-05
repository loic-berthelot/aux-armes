#include <vector>
#include "joueur.h"
#include "carte.h"

class Jeu {
    Carte _carte;
    std::vector<Joueur> _joueurs;
public:
    void jouer();
};