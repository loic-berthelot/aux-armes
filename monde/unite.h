#pragma once
#include <iostream>
#include <vector>
#include "../ordres/ordreDeplacer.h"

class Unite{
    std::string _categorie;
    std::vector<std::string> _types;
    //Les coordonnées de la case centrale du plateau sont (0,0). Les axes sont dirigés vers le haut et la droite.
    int _posX;
    int _posY;
    Ordre * _ordreRecu;
public:
    Unite(const std::string & categorie, const std::vector<std::string> & types, int posX, int posY);
    std::string toString() const;
    void donnerOrdre(Ordre * ordre) { _ordreRecu = ordre; }
    void executerOrdre();
};