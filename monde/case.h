#pragma once

#include <iostream>
#include <string>
#include <fstream>

enum class accessibilite{Eau, Air, Terre, EauEtTerre};
class Case{
private:
    int _coutDeplacement;//100 valeurs de base pour la plaine
    std::string _nom;
    
    

    accessibilite _typeAccessibilité;
public:
    

    Case(std::string nomFichier);//on part du build

    bool creerCaseEtSauvegardeFichier(std::string const &nom, accessibilite access,int coutDeplacement);


    bool accessibleTerre()const;

    bool accessibleEau()const;

    /*Getters and setters*/

    std::string getNom()const;

    accessibilite stringToAccessibilite(std::string const &s)const;

    std::string accessibiliteToString(accessibilite const e)const;

    int getCoutDeplacement()const;



};