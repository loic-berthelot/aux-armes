#include "jeu.h"

int main() {
    Jeu jeu;
    jeu.initialiser();
    std::cout<<"Debut de la partie !"<<std::endl;
    jeu.jouer();
    return 0;
}