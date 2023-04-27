#include "Type.h"

Type::Type(std::string const &nomFichier):_nom(nomFichier){
    std::ifstream fichier("../monde/TypeUnite/"+nomFichier+".txt");
    
    if (!fichier.is_open()) throw std::invalid_argument("Erreur : Le type n'existe pas." + nomFichier);
    
    std::string ligne;
    bool specificiteSection = false;
    while (std::getline(fichier, ligne)) {        
        if (ligne != ""){
            if (ligne == "Specificites")
                specificiteSection = true;
            if (specificiteSection){
                _specificites.push_back(stringToSpecificite(ligne));
            }else{
                std::string nomType = ligne;
                std::getline(fichier, ligne);
                float valCoeff = 0;
                try{
                    valCoeff = std::stof(ligne);
                }catch(...){
                    throw new Exception("Erreur : La valeur n'est pas un float : "+ligne+" dans le fichier Type : "+nomFichier);
                }

                if (_coutDeplacement < 0)
                    throw std::invalid_argument("Coeff < 0 pour : "+ligne+ " dans le type : "+nomFichier);
                
                _coefficients[nomType] = valCoeff;        
            }
        }
    }    
    fichier.close();  // Fermer le fichier  
}

Specificite Type::stringToSpecificite(std::string const &s){
    if (s == "furtif") return Specificite::furtif;
    if (s == "degats de zone") return Specificite::degatsDeZone;
    if (s == "incendiaire") return Specificite::incendiaire;
    if (s == "inflammable") return Specificite::inflammable;
    throw Exception ("Specificite inconnue : "+s);
}

float Type::getCoefficients(std::string const &key) const {
    if (_coefficients.find(key) != _coefficients.end())
        return _coefficients.at(key); // Utiliser la fonction at() pour accéder à la valeur associée à la clé
    else
        return 1.0;
}



bool Type::possedeSpecificite(Specificite s)const{
    for (unsigned int i = 0; i < _specificites.size();i++)
        if (s == _specificites[i])
            return true;
    return false;
}