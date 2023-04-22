#include "Type.h"

Type::Type(std::string const &nomFichier):_nom(nomFichier){
    std::ifstream fichier("../monde/TypeUnite/"+nomFichier+".txt");
    
    if (!fichier.is_open()) {
        throw std::invalid_argument("Erreur : Le type n'existe pas." + nomFichier);
    }
    
    std::string ligne;
    bool specificiteSection = false;
    while (std::getline(fichier, ligne)) {
        
        if (ligne != ""){
            if (ligne == "Specificites")
                specificiteSection = true;
            if (specificiteSection){
                _specificites.push_back(Type::stringToSpecificite(ligne));
            }else{
                std::string nomType = ligne;
                std::getline(fichier, ligne);
                float valCoeff = std::stof(ligne);
        
                _coefficients[nomType] = valCoeff;
        
            }
        }

    }
    
    // Fermer le fichier
    fichier.close();
    
}

Specificite Type::stringToSpecificite(std::string const &s){
    if (s == "invisibleForet")
        return Specificite::invisibleForet;
    else{
        std::cout << "Spécificité inconnu : "<<s<<std::endl;
        return Specificite::inconnu;
    }
}


bool Type::possedeSpecificite(Specificite s)const{
    for (unsigned int i = 0; i < _specificites.size();i++)
        if (s == _specificites[i])
            return true;
    return false;
}