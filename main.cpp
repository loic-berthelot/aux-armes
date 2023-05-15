#include "jeu.h"

//Lit le fichier Launcher.txt et le fais joueur
void LancerJeu(){
    std::string nomFichier = "../options.txt";  
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
    
    
}

int main() {
    LancerJeu();
    
    
    
    //Jeu jeu("hi","gen1","1:Garde Royale,;1:Garde Royale,;");
    //jeu.jouer();
    
    
    //LancerJeu();


    return 0;

}
