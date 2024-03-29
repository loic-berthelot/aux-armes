#include "case.h"

Case::Case(std::string nomFichier) : _nom(nomFichier), _coutDeplacement(100), _capaciteAccueil(100){
    nomFichier = "../monde/Cases/"+nomFichier+".case";  
    std::ifstream fichier(nomFichier); // Ouverture du fichier en lecture
    if (! fichier.is_open()) throw Exception("Impossible d'ouvrir le fichier "+nomFichier);
    std::string ligne;
    std::getline(fichier, ligne);//index typeAccessibilité
    std::getline(fichier, ligne);
    _typeAccessibilité = stringToAccessibilite(ligne);

    std::getline(fichier, ligne);//index coutDeplacement
    std::getline(fichier, ligne);//valeur coutDeplacement
    try {
        _coutDeplacement = std::stoi(ligne);
    } catch (...) {
        throw std::invalid_argument("Cout déplacement pas en int dans la case  dans : " + nomFichier+" pour la valeur : "+ligne);
    }
    if (_coutDeplacement <= 0)
        throw std::invalid_argument("Cout Déplacement <= 0 pour : "+ligne+ " dans la case : "+nomFichier);
    

    std::getline(fichier, ligne);//index defense
    std::getline(fichier, ligne);//valeur Defense
    try {
        _defense = std::stoi(ligne);
    } catch (...) {
        throw std::invalid_argument("Défense pas en int dans la case : " + nomFichier+" pour la valeur : "+ligne);
    }
    if (_defense <= 0)
        throw std::invalid_argument("Cout defense <= 0 pour : "+ligne+ " dans la case : "+nomFichier);
    
    std::getline(fichier, ligne);//index capacité d'accueil
    std::getline(fichier, ligne);//valeur capacité d'accueil

    try {
        _capaciteAccueil = std::stoi(ligne);
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Capacité d'accueil pas en int dans la case : " + nomFichier+ " Pour :"+ligne);
    }
    if (_capaciteAccueil <= 0)
        throw std::invalid_argument("Cout capacité d'accueil <= 0 pour : "+ligne+ " dans la case : "+nomFichier);

    while (std::getline(fichier, ligne)) {
        if (ligne == "Depart Ravitaillement") _departRavitaillement = true;
        if (ligne == "Permet furtivite") _zoneFurtivite = true;
    }
    fichier.close(); // Fermeture du fichier
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
            fichier << "capacitéAccueil:"<<std::endl;
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

bool Case::obstacleVision() const {
    return _typeAccessibilité != accessibilite::Terre && _typeAccessibilité != accessibilite::Eau && _typeAccessibilité != accessibilite::EauEtTerre;
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
    if (e == accessibilite::Eau) return "Eau";
    if (e == accessibilite::Air) return "Air";
    if (e == accessibilite::Terre)return "Terre";
    return "EauEtTerre";
}


accessibilite Case::stringToAccessibilite(std::string const &s) {
    if (s == "Eau")
        return accessibilite::Eau;
    else if (s == "Terre")
        return accessibilite::Terre;
    else if (s == "Air")
        return accessibilite::Air;
    else if (s == "EauEtTerre")
        return accessibilite::EauEtTerre;
    else 
        throw new Exception("Accessibilité non définie : " + s);
}

bool Case::estDepartRavitaillement() const {
    return _departRavitaillement;
}

int Case::getCapaciteAccueil() const {
    return _capaciteAccueil;
}

bool Case::permetFurtivite() const {
    return _zoneFurtivite;
}