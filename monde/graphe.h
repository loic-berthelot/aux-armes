#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
#include <memory>
#include "exception.h"

float distanceRepereOrthonorme(float dx, float dy);

float distance(std::pair<int,int> debut, std::pair<int,int> fin);

struct SommetGraphe {
    std::pair<int,int> _pos;
    std::map<const std::shared_ptr<SommetGraphe>, int> _suivants;    
};

struct SommetParcours {
    std::shared_ptr<SommetGraphe> _sommetGraphe;
    float _coutChemin;
    float _heuristique;
    std::shared_ptr<SommetParcours> _parent;  
    bool _passageAutorise;
    bool _visite;
};

class Graphe {
    std::map<std::pair<int,int>, std::shared_ptr<SommetGraphe>> _sommetsGraphe;
    std::map<std::pair<int,int>, std::shared_ptr<SommetParcours>> _sommetsParcours;
    std::shared_ptr<SommetParcours> _sommetDepart;
    std::shared_ptr<SommetParcours> _sommetArrivee;
    std::map<std::pair<int,int>,bool> _obstacles;
    enum typeOperation {A_ETOILE, ZONE_RAVITAILLEMENT};
    typeOperation _typeOperation;
    float _poidsMinimal;
public:    
    Graphe(const std::vector<std::pair<int,int>> & sommets, float poidsMinimal=1);
    void ajouterSuivant(std::pair<int,int> depart, std::pair<int,int> arrivee, int poids);
    std::pair<int,int> position(std::shared_ptr<SommetParcours> sommet) const;
    std::shared_ptr<SommetGraphe> creerSommetGraphe(const std::pair<int,int> & pos);
    std::shared_ptr<SommetParcours> creerSommetAEtoile(std::shared_ptr<SommetGraphe> sommetGraphe, std::shared_ptr<SommetGraphe> depart, std::shared_ptr<SommetGraphe> arrivee) ;
    std::shared_ptr<SommetParcours> creerSommetZoneRavitaillement(std::shared_ptr<SommetGraphe> sommetGraphe);
    std::shared_ptr<SommetParcours> ajouterSommetParcours(std::shared_ptr<SommetGraphe> sommetGraphe);
    std::shared_ptr<SommetParcours> getSommetParcours(std::shared_ptr<SommetGraphe> sommet);
    bool contient(const std::vector<std::shared_ptr<SommetParcours>> & sommets, std::shared_ptr<SommetParcours> sommet) const;
    void explorerSuivants(std::shared_ptr<SommetParcours> sommet, std::vector<std::shared_ptr<SommetParcours>> & sommetsOuverts, std::vector<std::shared_ptr<SommetParcours>> & sommetsFermes);     
    void retirerSommet(std::vector<std::shared_ptr<SommetParcours>> & sommets, const std::shared_ptr<SommetParcours> sommet);
    std::shared_ptr<SommetParcours> plusFaibleScore(const std::vector<std::shared_ptr<SommetParcours>> & sommets) const;
    std::vector<std::pair<std::pair<int,int>, int>> aEtoile(std::pair<int,int> depart, std::pair<int,int>arrivee, std::vector<std::pair<int,int>> obstacles={});
    std::vector<std::pair<int,int>> zoneRavitaillement(std::vector<std::pair<int,int>> departs, std::vector<std::pair<int,int>> obstacles, std::map<std::pair<int,int>,int> relais);
    std::vector<std::pair<int,int>> positionsAccessibles(std::pair<int,int> pos, float distance);
    int longueurChemin(std::pair<int,int> depart, std::pair<int,int> arrivee);
};
