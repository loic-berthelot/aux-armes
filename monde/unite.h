#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Type.h"
#include "ordre.h"
#include "case.h"
#include "exception.h"

static accessibilite stringToCategorie(std::string const &s);

static std::string CategorieToString(accessibilite const c);

class Unite{
    std::string _nom;
    accessibilite _categorie;
    std::vector<Type> _types;
    //Les coordonnées de la case centrale du plateau sont (0,0). Les axes sont dirigés vers le haut et la droite.
    int _posX;
    int _posY;
    std::shared_ptr<Ordre> _ordreRecu;    
    int _maxMoral = 100;
    int _moral = _maxMoral;
    int _sante;
    int _maxSante;
    int _attaque;//100 de base
    int _defense;//100 de base
    int _distanceVue;
    unsigned int _portee;
    int _distanceRavitaillement;
    float _vitesseDeplacement = 0.2;
    float _pointsMouvement;
    int _espaceOccupe = 1;
    bool _enVie = true;;
    bool _estRavitaille = false;
    std::vector<std::pair<std::pair<int,int>, int>> _chemin;
    std::pair<int,int> _positionPrecedente = std::make_pair(_posX, _posY);

public:
    ~Unite() {std::cout<<"destruction de l'Unite"<<std::endl; }

    Unite(std::string name, int x, int y);

    std::string toString() const;

    void donnerOrdre(std::shared_ptr<Ordre> ordre);

    std::pair<int, int> resultatCombatSimple(std::shared_ptr<Unite> ennemi)const;

    bool avancer();

    void initialiserMouvement(std::vector<std::pair<std::pair<int,int>, int>> chemin);

    static void CreationNouvelleUnite(std::string const &nom, std::vector<Type> const &types, int sante, accessibilite categorie, int attaque, int defense, int distanceVue, int pointsMouvement, int distanceRavitaillement);

    /*GETTERS AND SETTERS ================================*/

    int getMoral()const;

    int getX()const;

    int getY()const;

    int getDistanceVue()const;

    int getDistanceRavitaillement() const;

    accessibilite getCategorie() const;

    void setDistanceVue(int distanceVue);

    std::shared_ptr<Ordre> getOrdre() const;
    
    std::pair<int,int> getPos() const;

    std::vector<Type> getTypes()const;

    void regenererSante(int pointsSante);

    void regenererMoral(int pointsMoral);

    void infligerDegats(unsigned int degats);

    void ravitailler();

    void subirAttrition(float attrition);

    void setX(int x);

    void setY(int y);

    unsigned int getPortee()const;

    bool estVivant() const;

    int getEspaceOccupe() const;

    void reculer();
};