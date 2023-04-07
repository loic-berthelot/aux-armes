#pragma once

#include <iostream>
#include <string>
#include <fstream>

class Case{
private:
    int _coutDeplacement;//100 valeurs de base pour la plaine
    std::string _nom;


public:

    

    Case(std::string nomFichier);//on part du build

    static bool creerCaseEtSauvegardeFichier(std::string const &nom, int coutDeplacement);

    /*Getters and setters*/

    std::string getNom()const;

    int getCoutDeplacement()const;

};