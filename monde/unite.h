#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Type.h"
#include "ordre.h"
#include "case.h"
#include "exception.h"

class Unite{
    std::string _nom;
    accessibilite _categorie;
    std::vector<Type> _types;
    //Les coordonnées de la case centrale du plateau sont (0,0). Les axes sont dirigés vers le haut et la droite.
    int _posX;
    int _posY;
    std::shared_ptr<Ordre> _ordreRecu;
    int _moral;
    int _maxMoral;
    int _sante;
    int _maxSante;
    int _attaque;//100 de base
    int _defense;//100 de base
    int _distanceVue;
    unsigned int _portee = 3;
    int _rayonRavitaillement;
    float _vitesseDeplacement;
    float _pointsMouvement;
    int _espaceOccupe;
    bool _enVie;
    bool _estRavitaille;
    std::vector<std::pair<std::pair<int,int>, int>> _chemin;
    std::pair<int,int> _positionPrecedente;
public:
    Unite(std::string nom,accessibilite categorie, const std::vector<Type> & types, int posX, int posY, int santeInitiale, int attaque, 
    int defense, int distanceVue);


    Unite(std::string name, int x, int y);


    std::string toString() const;
    void donnerOrdre(std::shared_ptr<Ordre> ordre);
    std::pair<int, int> resultatCombatSimple(std::shared_ptr<Unite> ennemy)const;
    bool avancer();
    void initialiserMouvement(std::vector<std::pair<std::pair<int,int>, int>> chemin);


    static accessibilite stringToCategorie(std::string const &s){
        if (s == "Air")
            return accessibilite::Air;
        else if (s == "Eau")
            return accessibilite::Eau;
        else if (s == "Terre")
            return accessibilite::Terre;
        else return accessibilite::EauEtTerre;
    }
    static std::string CategorieToString(accessibilite const c){
        if (c == accessibilite::Air)
            return "Air";
        else if (c == accessibilite::Eau)
            return "Eau";
        else if (c == accessibilite::Terre)
            return "Terre";
        else return "EauEtTerre";
    }


    static void CreationNouvelleUnite(std::string const &nom, std::vector<Type> const &types, int sante, accessibilite categorie,
    int attaque, int defense, int distanceVue, int pointsMouvement);

    /*GETTERS AND SETTERS ================================*/


    int getMoral()const{
        return _moral;
    }

    int getX()const;

    int getY()const;

    int getDistanceVue()const;

    int getRayonRavitaillement() const;

    accessibilite getCategorie() const;

    void setDistanceVue(int distanceVue){
        _distanceVue = distanceVue;
    }
    std::shared_ptr<Ordre> getOrdre() const;
    std::pair<int,int> getPos() const;

    std::vector<Type> getTypes()const{
        return _types;
    }

    void regenererSante(int pointsSante);

    void regenererMoral(int pointsMoral);

    void infligerDegats(unsigned int degats);

    void ravitailler();

    void subirAttrition(float attrition);

    void setX(int x){
        _posX = x;
    }

    void setY(int y){
        _posY = y;
    }

    unsigned int getPortee()const;
    bool estVivant() const;

    int getEspaceOccupe() const;

    void reculer();
};