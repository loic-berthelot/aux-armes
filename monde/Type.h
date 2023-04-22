#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <fstream>


enum class Specificite{
    invisibleForet,
    inconnu
};

class Type{
    std::map<std::string, float> _coefficients;
    std::string _nom;
    std::vector<Specificite> _specificites;

    public:

    //lit un fichier
    Type(std::string const &path);

    std::string getNom()const{
        return _nom;
    }

    static Specificite stringToSpecificite(std::string const &s);

    float getCoefficients(std::string const &key) const {
        if (_coefficients.find(key) != _coefficients.end())
            return _coefficients.at(key); // Utiliser la fonction at() pour accéder à la valeur associée à la clé
        else
            return 1.0;
    }

    bool possedeSpecificite(Specificite s)const;
    

};