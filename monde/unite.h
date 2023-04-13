#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "../ordres/ordreDeplacer.h"

class Unite{
    std::string _categorie;
    std::vector<std::string> _types;
    //Les coordonnées de la case centrale du plateau sont (0,0). Les axes sont dirigés vers le haut et la droite.
    int _posX;
    int _posY;
    std::shared_ptr<Ordre> _ordreRecu;
    int _moral = 100;//100 la valeur de base, remonte au fil du temps
    int _sante;
    int _maxSante;
    int _attaque;//100 de base
    int _defense;//100 de base
    int _distanceVue;

public:
    Unite(const std::string & categorie, const std::vector<std::string> & types, int posX, int posY, int santeInitiale, int attaque, 
    int defense, int distanceVue);
    std::string toString() const;
    void donnerOrdre(std::shared_ptr<Ordre> ordre) { _ordreRecu = ordre; }
    void executerOrdre();
    std::pair<int, int> resultatCombatSimple(Unite const &ennemy)const;


    /*GETTERS AND SETTERS ================================*/

    void setMorale(int moral){
        _moral = moral;
    }


    int getX()const;

    int getY()const;

    int getDistanceVue()const;

    void setDistanceVue(int distanceVue){
        _distanceVue = distanceVue;
    }
};