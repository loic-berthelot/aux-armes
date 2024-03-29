#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include "exception.h"

enum class Specificite{
    furtif,
    degatsDeZone,
    incendiaire,
    inflammable,
    autonome,
    intrepide
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

    float getCoefficients(std::string const &key) const;

    bool possedeSpecificite(Specificite const s)const;
    

};