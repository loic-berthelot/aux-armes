#include "case.h"



Case::Case(std::string nomFichier):
    _nom(nomFichier), _coutDeplacement(100){
    nomFichier = "../monde/Cases/"+nomFichier+".case";
    

    std::ifstream fichier(nomFichier); // Ouverture du fichier en lecture
    if (fichier.is_open()) { // Vérification si le fichier est ouvert
        std::string ligne;
        std::getline(fichier, ligne);//index typeAccessibilité
        std::getline(fichier, ligne);
        _typeAccessibilité = stringToAccessibilite(ligne);
        std::getline(fichier, ligne);//index coutDeplacement
        std::getline(fichier, ligne);//valeur coutDeplacement

        _coutDeplacement = std::stoi(ligne);

        std::getline(fichier, ligne);//index defense
        std::getline(fichier, ligne);//valeur Defense

        _defense = std::stoi(ligne);

        
        fichier.close(); // Fermeture du fichier
    } else {
        
        std::cout << "Impossible d'ouvrir le fichier : " <<nomFichier<< std::endl;
    }
    
}


bool Case::creerCaseEtSauvegardeFichier(std::string const &nom, accessibilite access,int coutDeplacement, int defense){
    std::string chemin = "../monde/Cases/"+nom+".case";
    std::ifstream fichier(chemin);
    if (fichier.is_open()){//le fichier existe
        std::cout << "Création impossible cette case existe déjà : " << nom << "\n";
        return false;
    }else{
        std::ofstream fichier(chemin); // Création du fichier

        if (fichier.is_open()) { // Vérification si le fichier est ouvert
            fichier << "Accessibilité"<< std::endl;
            fichier << accessibiliteToString(access)<<std::endl;
            fichier << "coutDeplacement:" << std::endl; 
            fichier << std::to_string(coutDeplacement) << std::endl; 
            fichier << "Défense:"<<std::endl;
            fichier << std::to_string(defense)<<std::endl;
            fichier.close(); // Fermeture du fichier
        } else {
            std::cerr << "Erreur lors de la création du fichier : " << nom << std::endl;
        }
        return true;
    }


}

bool Case::accessibleEau()const{
    return (_typeAccessibilité == accessibilite::Eau || _typeAccessibilité == accessibilite::EauEtTerre);
}

bool Case::accessibleTerre()const{
    return (_typeAccessibilité == accessibilite::Terre || _typeAccessibilité == accessibilite::EauEtTerre);
}

/*GETTERS AND SETTERS  ======*/


int Case::getDefense()const{
    return _defense;
}

std::string Case::getNom()const{
    return _nom;
}


int Case::getCoutDeplacement()const{
    return _coutDeplacement;
}

std::string Case::accessibiliteToString(accessibilite const e){
    if (e == accessibilite::Eau)
        return "Eau";
    else if (e == accessibilite::Air)
        return "Air";
    else if (e == accessibilite::Terre)
        return "Terre";
    else
        return "EauEtTerre";
}


accessibilite Case::stringToAccessibilite(std::string const &s) {
    if (s == "Eau")
        return accessibilite::Eau;
    else if (s == "Terre")
        return accessibilite::Terre;
    else if (s == "Air")
        return accessibilite::Air;
    else 
        return accessibilite::EauEtTerre;
}

bool Case::estDepartRavitaillement() const {
    return (_nom == "Ville");
    //return _departRavitaillement;
}