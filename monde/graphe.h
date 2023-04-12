#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
#include <memory>

class Noeud {
    std::string _id;
    std::map<std::shared_ptr<Noeud>, unsigned int> _suivants;
    int _posX;
    int _posY;
    float _coutChemin;
    float _heuristique;
    std::shared_ptr<Noeud> _parent;
public:
    Noeud(const std::string & id, int posX, int posY);
    std::shared_ptr<Noeud> getParent()const;
    int getPosX() const;
    int getPosY() const;
    float getCoutChemin() const;
    float getHeuristique() const;
    std::string getId() const;    
    void setCoutChemin(float c);
    void setParent(std::shared_ptr<Noeud> p);    
    float distanceRepereOrthonorme(float dx, float dy);
    float distance(std::shared_ptr<Noeud> arrivee);
    void initialiser(std::shared_ptr<Noeud> depart, std::shared_ptr<Noeud> arrivee);
    void ajouterSuivant(std::shared_ptr<Noeud> noeud, unsigned int valeur);
    void explorerSuivants(std::vector<std::shared_ptr<Noeud>> & noeudsOuverts, std::vector<std::shared_ptr<Noeud>> & noeudsFermes);
    void afficher() const;
};

class Graphe {
    std::map<std::pair<int,int>,std::shared_ptr<Noeud>> _noeuds;
public:
    Graphe(const std::map<std::pair<int,int>,std::shared_ptr<Noeud>> noeuds);
    void retirerNoeud(std::vector<std::shared_ptr<Noeud>> & noeuds, std::shared_ptr<Noeud> noeud);
    std::shared_ptr<Noeud> plusFaibleScore(const std::vector<std::shared_ptr<Noeud>> & noeuds);
    std::vector<std::shared_ptr<Noeud>> aEtoile(std::pair<int,int> depart, std::pair<int,int>arrivee);
};