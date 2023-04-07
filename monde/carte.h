#include "armee.h"
#include "case.h"

class Carte {
private:
    std::vector<std::vector<Case>> _carte;
    std::vector<std::shared_ptr<Armee>> _armees;


public:

    //créer une armée vide
    void creerArmee();
    //ne marche pas encore donc à renseigner
    void afficher()const;
    //execute l'ordre du tour
    void executerOrdresTour(unsigned int indiceArmee);
    
    


    //getters & setters
    std::vector<std::vector<Case>> getCarte()const;
    std::shared_ptr<Armee> getArmee(unsigned int i) const;

};