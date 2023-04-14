#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
#include <memory>

class Noeud {
    static int compt;
    std::string _id;
    std::map<const std::shared_ptr<Noeud>, int> _suivants;
    int _posX;
    int _posY;
    float _coutChemin;
    float _heuristique;
    std::shared_ptr<Noeud> _parent;

    void setParent(Noeud* p);    
public:
    Noeud(const std::string & id, int posX, int posY);
    ~Noeud() { std::cout<<"erreur : destruction du noeud "<<this<<std::endl;}
    bool estEgal(const std::shared_ptr<const Noeud> & noeud) const;
    std::shared_ptr<Noeud> getParent()const;
    int getPosX() const;
    int getPosY() const;
    int getCoutParent() const;
    float getCoutChemin() const;
    float getHeuristique() const;
    std::string getId() const;    
    void setCoutChemin(float c);    
    float distanceRepereOrthonorme(float dx, float dy) const;
    float distance(const std::shared_ptr<Noeud> arrivee) const;
    void initialiser(const std::shared_ptr<Noeud> depart, const std::shared_ptr<Noeud> arrivee);
    void ajouterSuivant(const std::shared_ptr<Noeud> noeud, unsigned int valeur);
    void explorerSuivants(std::vector<std::shared_ptr<Noeud>> & noeudsOuverts, std::vector<std::shared_ptr<Noeud>> & noeudsFermes);
    void afficher() const;
};

class Graphe {
    std::map<std::pair<int,int>,std::shared_ptr<Noeud>> _noeuds;
public:
    Graphe(const std::map<std::pair<int,int>,std::shared_ptr<Noeud>> noeuds);
    void retirerNoeud(std::vector<std::shared_ptr<Noeud>> & noeuds, const std::shared_ptr<Noeud> noeud);
    std::shared_ptr<Noeud> plusFaibleScore(const std::vector<std::shared_ptr<Noeud>> & noeuds) const;
    std::vector<std::pair<std::pair<int,int>, int>> aEtoile(std::pair<int,int> depart, std::pair<int,int>arrivee);
    std::vector<std::pair<int,int>> zoneAccessible(int pointsMouvement);
};