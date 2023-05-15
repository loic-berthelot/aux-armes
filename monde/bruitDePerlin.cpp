#include "bruitDePerlin.h"

//calcul une valeur entre a et b en fonction de parametreMelange
//qui est compris entre 0 et 1, plus c'est proche de 0 plus la valeur sera proche de A
double calculIntermediaire(double pointA, double pointB, double parametreMelange){
    return pointA + parametreMelange * (pointB - pointA);
}

// Fonction pour calculer le produit scalaire entre un gradient et un vecteur
double vecteurPente(int hash, double x, double y){
    switch (hash & 0x3) {
        case 0x0: return x + y;
        case 0x1: return -x + y;
        case 0x2: return x - y;
        case 0x3: return -x - y;
        default: return 0;
    }
}


double fade(double t){
    return t * t * t * (t * (t * 6 - 15) + 10);   
}    
// Fonction pour interpoler

// Fonction pour calculer la valeur de bruit de Perlin en 2D
double perlin2D(double x, double y){
    int permutation[256];


    // Remplir le tableau de permutation avec des valeurs aléatoires entre 0 et 255
    for (int i = 0; i < 256; ++i){
        permutation[i] = i+rand()%5-2;
        if (permutation[i] <= 0)
        permutation[i] = 0;
        if (permutation[i] >= 255)
        permutation[i] = 255;
    }
    
    int xi = static_cast<int>(x) & 255;
    int yi = static_cast<int>(y) & 255;
    double xf = x - static_cast<int>(x);
    double yf = y - static_cast<int>(y);
    double u = fade(xf);
    double v = fade(yf);

    int aa = permutation[permutation[xi] + yi];
    int ab = permutation[permutation[xi] + yi + 1];
    int ba = permutation[permutation[xi + 1] + yi];
    int bb = permutation[permutation[xi + 1] + yi + 1];


    double x1 = calculIntermediaire(vecteurPente(aa, xf, yf), vecteurPente(ba, xf - 1, yf), u);
    double x2 = calculIntermediaire(vecteurPente(ab, xf, yf - 1), vecteurPente(bb, xf - 1, yf - 1), u);

    return calculIntermediaire(x1, x2, v);
}