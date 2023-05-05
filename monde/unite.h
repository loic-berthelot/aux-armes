#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Type.h"
#include "ordre.h"
#include <algorithm>
#include "case.h"
#include "graphe.h"


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
    bool _enVie = true;
    bool _estRavitaille = false;
    std::vector<std::pair<std::pair<int,int>, int>> _chemin;
    std::pair<int,int> _positionPrecedente = std::make_pair(_posX, _posY);
    bool _degatsDeZone  = false;
    bool _incendiaire = false;
    bool _inflammable = false;
    bool _furtif = false;
    int _effetBrulure = 0;

public:
    
    Unite(std::string name, int x, int y);

    std::string toString() const;

    void donnerOrdre(std::shared_ptr<Ordre> ordre);

    std::pair<int, int> resultatCombatSimple(std::shared_ptr<Unite> const ennemi)const;

    bool avancer();

    void initialiserMouvement(std::vector<std::pair<std::pair<int,int>, int>> const &chemin);

    static accessibilite stringToCategorie(std::string const &s);

    static std::string CategorieToString(accessibilite const c);

    bool possedeSpecificite(Specificite e)const;

    static void CreationNouvelleUnite(std::string const &nom, std::vector<Type> const &types, int sante, accessibilite categorie, int attaque, int defense, int distanceVue, int pointsMouvement, float vitesseDeplacement, int espaceOccupe);
    /*GETTERS AND SETTERS ================================*/

    int getMoral()const;

    int getX()const;
    
    int getY()const;  

    int getDistanceVue()const;

    int getDistanceRavitaillement() const;

    int getSante() const;

    int getAttaque() const;

    int getDefense() const;

    accessibilite getCategorie() const;

    void setDistanceVue(int distanceVue);

    std::shared_ptr<Ordre> getOrdre() const;
    
    std::pair<int,int> getPos() const;

    std::vector<Type> getTypes()const;

    Type getType(int i)const;

    bool peutAttaquer(int x, int y);

    void regenererMoral(int pointsMoral);

    void infligerDegats(unsigned int degats);

    void ravitailler();

    void subirAttrition(float attrition);

    void setX(int x);

    void setY(int y);

    void setPos(const std::pair<int,int> & pos);

    unsigned int getPortee()const;

    bool estVivant() const;

    int getEspaceOccupe() const;

    void reculer();

    bool possedeDegatsDeZone() const;

    bool estFurtif() const;

    bool estIncendiaire() const;

    void recevoirBrulure();

    void evolutionBrulure();

    std::string getNom() const;

    float getVitesse() const;
};