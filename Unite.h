#include <iostream>
#include <vector>

class Unite{
    std::string _categorie;
    std::vector<std::string> _types;
    //Les coordonnées de la case centrale du plateau sont (0,0). Les axes sont dirigés vers le haut et la droite.
    int _posX;
    int _posY;
public:
    Unite(const std::string & categorie, const std::vector<std::string> & types, int posX, int posY);
    std::string getCategorie() { return _categorie; }
    std::vector<std::string> getTypes() { return _types; }
};