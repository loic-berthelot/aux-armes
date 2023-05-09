#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "exception.h"

enum class accessibilite{Eau, Air, Terre, EauEtTerre};

class Case{
private:
    int _coutDeplacement;//100 valeurs de base pour la plaine
    std::string _nom;
    int _defense;//100 valeur de base
    bool _departRavitaillement; 
    accessibilite _typeAccessibilité;
    int _capaciteAccueil;
    bool _zoneFurtivite = false;
public:
    Case(std::string nomFichier);//on part du build

    static bool creerCaseEtSauvegardeFichier(std::string const &nom, accessibilite access,int coutDeplacement, int defense);

    bool accessibleTerre()const;

    bool accessibleEau()const;

    bool obstacleVision() const;

    /*Getters and setters*/

    std::string getNom()const;

    static accessibilite stringToAccessibilite(std::string const &s);

    static std::string accessibiliteToString(accessibilite const e);

    int getCoutDeplacement()const;

    int getDefense()const;

    bool estDepartRavitaillement() const;

    int getCapaciteAccueil() const;

    bool permetFurtivite() const;
};