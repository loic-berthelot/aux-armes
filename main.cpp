#include "jeu.h"

//Lit le fichier Launcher.txt et le fais joueur
void LancerJeu(){
    std::string nomFichier = "../Launcher.txt";  
    std::ifstream fichier(nomFichier); // Ouverture du fichier en lecture
    if (! fichier.is_open()) throw Exception("Impossible d'ouvrir le fichier : "+nomFichier);
    std::string ligne;
    std::getline(fichier, ligne);//index joueursInfos
    std::getline(fichier, ligne);
    std::string joueurInfo = ligne;

    std::getline(fichier, ligne);//index génération FIchier
    std::getline(fichier, ligne);
    std::string generationInfo = ligne;
    
    std::getline(fichier, ligne);//index armées
    std::getline(fichier, ligne);
    std::string armeeInfo = ligne;
    
    Jeu j(joueurInfo, generationInfo, armeeInfo);
    j.jouer();
    
}

int main() {

    Jeu jeu("iii","gen1","2:Caravane,2:Pégase,Galère,;1:Caravane,;1:Caravane,;");
    jeu.jouer();
    //LancerJeu();


    return 0;

}


