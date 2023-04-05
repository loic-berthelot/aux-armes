#include <iostream>
#include "jeu.h"

int main() {
    Jeu jeu;
    jeu.ajouterJoueur();
    std::cout<<"Debut de la partie !"<<std::endl;
    jeu.jouer();
    return 0;
}