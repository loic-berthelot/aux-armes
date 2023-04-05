#include <iostream>

class Unite{
    std::string _type;
    //Les coordonnées de la case centrale du plateau sont (0,0). Les axes sont dirigés vers le haut et la droite.
    int _posX;
    int _posY;
public:
    Unite(const std::string & type, int posX, int posY);
    std::string getType() { return _type; }
};