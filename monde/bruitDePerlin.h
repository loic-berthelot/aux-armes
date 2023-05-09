#pragma once


#include <iostream>
#include <random>

//calcul une valeur entre a et b en fonction de parametreMelange
//qui est compris entre 0 et 1, plus c'est proche de 0 plus la valeur sera proche de A
double calculIntermediaire(double pointA, double pointB, double parametreMelange);

// Fonction pour calculer le produit scalaire entre un gradient et un vecteur
double vecteurPente(int hash, double x, double y);

double fade(double t);// Fonction pour interpoler

// Fonction pour calculer la valeur de bruit de Perlin en 2D
double perlin2D(double x, double y);