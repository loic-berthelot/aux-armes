#include "graphe.h"

float distanceRepereOrthonorme(float dx, float dy) { return sqrt(dx*dx+dy*dy); }

float distance(std::pair<int,int> debut, std::pair<int,int> fin){
    float di = (float) (fin.first - debut.first); //on calcule di, qui est le nombre de cases séparant la case d'arrivée de la case courante sur l'axe horizontal
    float dj = (float) (fin.second - debut.second); //de la même manière, on calcule dj. Cette distance est mesurée sur l'axe oblique (qui forme un angle de 60° avec l'axe horizontal)
    float dx = di + dj/2; //on calcule la projection sur l'axe x d'un repère orthonormé du vecteur allant de la case courante à la case d'arrivée
    float dy = dj * sqrt(3)/2; //on fait de même pour l'axe y du repère orthonormé
    return distanceRepereOrthonorme(dx, dy); //on retourne la distance euclidienne entre les deux cases grâce à un calcul dans le repère orthonormé
}

std::shared_ptr<SommetGraphe> Graphe::creerSommetGraphe(const std::pair<int,int> & pos) {
    std::shared_ptr<SommetGraphe> sommet = std::make_shared<SommetGraphe>();
    sommet->_pos = pos;
    return sommet;
}

std::shared_ptr<SommetParcours> Graphe::creerSommetAEtoile(std::shared_ptr<SommetGraphe> sommetGraphe, std::shared_ptr<SommetGraphe> depart, std::shared_ptr<SommetGraphe> arrivee) {
    std::shared_ptr<SommetParcours> sommetParcours = std::make_shared<SommetParcours>();
    sommetParcours->_sommetGraphe = sommetGraphe;    
    sommetParcours->_visite = (sommetGraphe == depart);
    sommetParcours->_heuristique = distance(sommetParcours->_sommetGraphe->_pos, arrivee->_pos);
    sommetParcours->_passageAutorise = true;
    sommetParcours->_coutChemin = 0; 
    sommetParcours->_parent = nullptr;
    return sommetParcours;
}

std::shared_ptr<SommetParcours> Graphe::creerSommetZoneRavitaillement(std::shared_ptr<SommetGraphe> sommetGraphe) {
    std::shared_ptr<SommetParcours> sommetParcours = std::make_shared<SommetParcours>();
    sommetParcours->_sommetGraphe = sommetGraphe;
    sommetParcours->_passageAutorise =  ! _obstacles[sommetGraphe->_pos];
    sommetParcours->_coutChemin = 0;
    sommetParcours->_parent = nullptr;
    return sommetParcours;    
}

Graphe::Graphe(const std::vector<std::pair<int,int>> & sommets){
    for (unsigned int i = 0; i < sommets.size(); i++){
        _sommetsGraphe[sommets[i]] = creerSommetGraphe(sommets[i]);
    }
}

void Graphe::ajouterSuivant(std::pair<int,int> depart, std::pair<int,int> arrivee, int poids) {
    _sommetsGraphe[depart]->_suivants[_sommetsGraphe[arrivee]] = poids;
}

std::shared_ptr<SommetParcours> Graphe::ajouterSommetParcours(std::shared_ptr<SommetGraphe> sommetGraphe) {
    if (! _sommetsParcours[sommetGraphe->_pos]){
        if (_typeOperation == A_ETOILE) _sommetsParcours[sommetGraphe->_pos] = creerSommetAEtoile(sommetGraphe,_sommetDepart->_sommetGraphe,_sommetArrivee->_sommetGraphe);
        else if (_typeOperation == ZONE_RAVITAILLEMENT) _sommetsParcours[sommetGraphe->_pos] = creerSommetZoneRavitaillement(sommetGraphe);
    }
    return _sommetsParcours[sommetGraphe->_pos];
}

std::shared_ptr<SommetParcours> Graphe::getSommetParcours(std::shared_ptr<SommetGraphe> sommet) { return _sommetsParcours.at(sommet->_pos); }

bool Graphe::contient(const std::vector<std::shared_ptr<SommetParcours>> & sommets, std::shared_ptr<SommetParcours> sommet) const {
    return (std::find(sommets.begin(), sommets.end(), sommet) != sommets.end());
}

void Graphe::explorerSuivants(std::shared_ptr<SommetParcours> sommet, std::vector<std::shared_ptr<SommetParcours>> & sommetsOuverts, std::vector<std::shared_ptr<SommetParcours>> & sommetsFermes) {   
    for (const auto paire : sommet->_sommetGraphe->_suivants) {
        //pour chacun des sommets-parcours suivants, on calcule le nouveau cout du chemin, qui passe par sommet
        std::shared_ptr<SommetParcours> suivant = getSommetParcours(paire.first);
        float nouveauCout = sommet->_coutChemin+paire.second;        
        if (suivant->_passageAutorise && (! suivant->_visite || nouveauCout < suivant->_coutChemin)) {
            if (! suivant->_visite) {//si on visite le sommet-parcours suivant pour la première fois, on prépare les sommets-parcours adjacents à être visités eux aussi            
                if (! contient(sommetsOuverts, suivant)) sommetsOuverts.push_back(suivant);
                for (const auto paire2 : suivant->_sommetGraphe->_suivants) ajouterSommetParcours(paire2.first); 
                suivant->_visite = true;           
            } 
            suivant->_coutChemin = nouveauCout;  
            suivant->_parent = sommet;
        }
    }
}

void Graphe::retirerSommet(std::vector<std::shared_ptr<SommetParcours>> & sommets, const std::shared_ptr<SommetParcours> sommet) {
    for (auto it = sommets.begin(); it != sommets.end(); it++) {
        if (*it == sommet) {
            sommets.erase(it);
            return;
        }
    }
}

std::shared_ptr<SommetParcours> Graphe::plusFaibleScore(const std::vector<std::shared_ptr<SommetParcours>> & sommets) const {
    return *std::min_element(sommets.begin(), sommets.end(), [](std::shared_ptr<SommetParcours> s1, std::shared_ptr<SommetParcours> s2) { 
        return s1->_coutChemin + s1->_heuristique < s2->_coutChemin + s2->_heuristique; 
    });
}

std::vector<std::pair<std::pair<int,int>, int>> Graphe::aEtoile(std::pair<int,int> depart, std::pair<int,int> arrivee) {
    _typeOperation = A_ETOILE;
    
    //on initialise les sommets de départ et d'arrivée, ainsi que l'ensemble des sommets-parcours
    _sommetDepart = creerSommetAEtoile(_sommetsGraphe.at(depart), _sommetsGraphe.at(depart), _sommetsGraphe.at(arrivee));
    _sommetArrivee = creerSommetAEtoile(_sommetsGraphe.at(arrivee), _sommetsGraphe.at(depart), _sommetsGraphe.at(arrivee));
    _sommetsParcours.clear();
    _sommetsParcours[depart] = _sommetDepart;
    _sommetsParcours[arrivee] = _sommetArrivee;
    for (const auto paire : _sommetDepart->_sommetGraphe->_suivants) ajouterSommetParcours(paire.first);
    
    //on initialise les vecteurs de sommets-parcours sommetsOuverts et sommetsFermes, ainsi que le plus court chemin qui sera renvoyé
    std::vector<std::shared_ptr<SommetParcours>> sommetsOuverts = {_sommetDepart};
    std::vector<std::shared_ptr<SommetParcours>> sommetsFermes;
    std::vector<std::pair<std::pair<int,int>, int>> plusCourtChemin;
    
    //on calcule le parent de chacun des sommets-parcours visités, jursqu'à atteindre l'arrivée
    std::shared_ptr<SommetParcours> sommetCourant = _sommetDepart;
    while (sommetCourant != _sommetArrivee) {
        if (sommetsOuverts.empty()) return plusCourtChemin; //si aucun chemin n'existe, on renvoie un vecteur vide
        explorerSuivants(sommetCourant, sommetsOuverts, sommetsFermes);
        sommetsFermes.push_back(sommetCourant);
        retirerSommet(sommetsOuverts, sommetCourant);
        sommetCourant = plusFaibleScore(sommetsOuverts);
    }   
    
    //on calcule le plus court chemin        
    while (sommetCourant->_parent != nullptr) {
        int coutParent = sommetCourant->_sommetGraphe->_suivants.at(sommetCourant->_parent->_sommetGraphe);
        plusCourtChemin.push_back(std::make_pair(sommetCourant->_sommetGraphe->_pos, coutParent));//on récupère toutes les étapes du chemin, en partant de la fin
        sommetCourant = sommetCourant->_parent;
    }
    //on replace les étapes du chemin dans le bon ordre, avant de le renvoyer
    std::reverse(plusCourtChemin.begin(), plusCourtChemin.end());
    return plusCourtChemin;
}

std::vector<std::pair<int,int>> Graphe::zoneRavitaillement(std::vector<std::pair<int,int>> departs, std::vector<std::pair<int,int>> obstacles, std::map<std::pair<int,int>,int> relais) {
    _typeOperation = ZONE_RAVITAILLEMENT;
    
    //on initialise les vecteurs de sommets-parcours sommetsOuverts et sommetsFermes
    std::vector<std::shared_ptr<SommetParcours>> sommetsOuverts;
    std::vector<std::shared_ptr<SommetParcours>> sommetsFermes;

    //on initialise la variable _obstacle
    for (unsigned int i = 0; i < obstacles.size(); i++) _obstacles[obstacles.at(i)] = true;

    _sommetsParcours.clear();
    for (unsigned int i = 0; i < departs.size(); i++) {            
        _sommetsParcours[departs[i]] = creerSommetZoneRavitaillement(_sommetsGraphe.at(departs[i])); 
        _sommetDepart = _sommetsParcours[departs[i]];  
        if (relais[_sommetDepart->_sommetGraphe->_pos]) _sommetDepart->_coutChemin = -relais.at(_sommetDepart->_sommetGraphe->_pos); 
        for (const auto paire : _sommetDepart->_sommetGraphe->_suivants) ajouterSommetParcours(paire.first);
        sommetsOuverts.push_back(_sommetDepart);
    }    
    
   
    //on cherche toutes les cases accessibles
    std::shared_ptr<SommetParcours> sommetCourant = sommetsOuverts[0];
    while (! sommetsOuverts.empty()) {
        if (-relais[sommetCourant->_sommetGraphe->_pos] < sommetCourant->_coutChemin) {
            std::cout<<-relais[sommetCourant->_sommetGraphe->_pos] <<std::endl;
            sommetCourant->_coutChemin = -relais[sommetCourant->_sommetGraphe->_pos];
        }
        explorerSuivants(sommetCourant, sommetsOuverts, sommetsFermes);
        if (sommetCourant->_coutChemin <= 0 && ! contient(sommetsFermes, sommetCourant)) sommetsFermes.push_back(sommetCourant);
        retirerSommet(sommetsOuverts, sommetCourant);
        sommetCourant = sommetsOuverts[0];
    } 
    
    //on retourne la position des sommets accessibles
    std::vector<std::pair<int,int>> zone;
    for (unsigned int i = 0; i < sommetsFermes.size(); i++) zone.push_back(sommetsFermes[i]->_sommetGraphe->_pos);
    return zone;
}
