#include "decisions/humain.h"
#include "decisions/IA.h"
#include <memory>

class Jeu {
    std::unique_ptr<Carte> _carte;
    std::vector<std::shared_ptr<Joueur>> _joueurs;
    unsigned int _toursPasses = 0;
public:

    Jeu(std::string const & joueurs,std::string const &configurationMap, std::string const &armeeDesc);
    void ajouterJoueur(bool estHumain = false);
    void sauvegarder(const std::string & nom);
    void chargerSauvegarde(const std::string & nomFichier);
    bool partieFinie()const;
    void jouer();


    static bool compareScoreDecroissant(const std::pair<unsigned int, int>& a, const std::pair<unsigned int, int>& b);

};