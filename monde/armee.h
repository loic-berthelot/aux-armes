#include <vector>
#include "unite.h"

class Armee {
    std::vector<std::shared_ptr<Unite>> _unites;
public :
    Armee();
    void afficher() const;
    unsigned int taille() const;
    void donnerOrdre(unsigned int indiceUnite, std::shared_ptr<Ordre> ordre);
    
    void ajoutUnite(std::shared_ptr<Unite> u);

    /*GETTERS*/   
    std::vector<std::shared_ptr<Unite>> getUnites();    
    std::shared_ptr<Unite> getUnite(unsigned int i) const;
    unsigned int size()const;

};