#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Type.h"
#include "ordre.h"

enum class Categorie{
    Air,
    Eau,
    Terre,
    EauEtTerre
};

class Unite{
    std::string _nom;
    Categorie _categorie;
    std::vector<Type> _types;
    //Les coordonnées de la case centrale du plateau sont (0,0). Les axes sont dirigés vers le haut et la droite.
    int _posX;
    int _posY;
    std::shared_ptr<Ordre> _ordreRecu;
    int _moral = 100;//100 la valeur de base, remonte au fil du temps
    int _sante;
    int _maxSante;
    int _attaque;//100 de base
    int _defense;//100 de base
    int _distanceVue;
    float _vitesseDeplacement = 0.2;
    float _pointsMouvement;
    std::vector<std::pair<std::pair<int,int>, int>> _chemin;
public:
    Unite(std::string nom,Categorie categorie, const std::vector<Type> & types, int posX, int posY, int santeInitiale, int attaque, 
    int defense, int distanceVue);


    Unite(std::string name, int x, int y);


    std::string toString() const;
    void donnerOrdre(std::shared_ptr<Ordre> ordre);
    std::pair<int, int> resultatCombatSimple(std::shared_ptr<Unite> ennemy)const;
    void avancer();
    void initialiserMouvement(std::vector<std::pair<std::pair<int,int>, int>> chemin);


    static Categorie stringToCategorie(std::string const &s){
        if (s == "Air")
            return Categorie::Air;
        else if (s == "Eau")
            return Categorie::Eau;
        else if (s == "Terre")
            return Categorie::Terre;
        else return Categorie::EauEtTerre;
    }
    static std::string CategorieToString(Categorie const c){
        if (c == Categorie::Air)
            return "Air";
        else if (c == Categorie::Eau)
            return "Eau";
        else if (c == Categorie::Terre)
            return "Terre";
        else return "EauEtTerre";
    }


    static void CreationNouvelleUnite(std::string const &nom, std::vector<Type> const &types, int sante, Categorie categorie,
    int attaque, int defense, int distanceVue, int pointsMouvement);

    /*GETTERS AND SETTERS ================================*/

    void setMoral(int moral){
        _moral = moral;
    }

    int getMoral()const{
        return _moral;
    }

    int getX()const;

    int getY()const;

    int getDistanceVue()const;

    void setDistanceVue(int distanceVue){
        _distanceVue = distanceVue;
    }
    std::shared_ptr<Ordre> getOrdre() const;
    std::pair<int,int> getPos() const;

    std::vector<Type> getTypes()const{
        return _types;
    }



    void infligerDegats(unsigned int damage){
        _sante-=damage;
    }
};