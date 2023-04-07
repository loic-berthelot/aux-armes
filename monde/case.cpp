#include "case.h"



Case::Case(std::string nomFichier):
    _nom(nomFichier), _coutDeplacement(100){
    nomFichier = "../monde/Cases/"+nomFichier+".case";
    

    std::ifstream fichier(nomFichier); // Ouverture du fichier en lecture
    if (fichier.is_open()) { // Vérification si le fichier est ouvert
        std::string ligne;
        std::getline(fichier, ligne);//index coutDeplacement
        std::getline(fichier, ligne);//valeur coutDeplacement

        _coutDeplacement = std::stoi(ligne);

        fichier.close(); // Fermeture du fichier
    } else {
        
        std::cout << "Impossible d'ouvrir le fichier : " <<nomFichier<< std::endl;
    }
    
}


bool Case::creerCaseEtSauvegardeFichier(std::string const &nom, int coutDeplacement){
    std::string chemin = "../monde/Cases/"+nom+".case";
    std::ifstream fichier(chemin);
    if (fichier.is_open()){//le fichier existe
        std::cout << "Création impossible cette case existe déjà : " << nom << "\n";
        return false;
    }else{
        std::ofstream fichier(chemin); // Création du fichier

        if (fichier.is_open()) { // Vérification si le fichier est ouvert
            fichier << "coutDeplacement:" << std::endl; 
            fichier << std::to_string(coutDeplacement) << std::endl; 
            fichier.close(); // Fermeture du fichier
        } else {
            std::cerr << "Erreur lors de la création du fichier : " << nom << std::endl;
        }
        return true;
    }


}

/*GETTERS AND SETTERS  ======*/
std::string Case::getNom()const{
    return _nom;
}


int Case::getCoutDeplacement()const{
    return _coutDeplacement;
}