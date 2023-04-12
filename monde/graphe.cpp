#include "graphe.h"

Noeud::Noeud(const std::string & id, int posX, int posY) : _id(id), _posX(posX), _posY(posY){}

std::shared_ptr<Noeud> Noeud::getParent() const { return _parent; }

int Noeud::getPosX() const { return _posX; }

int Noeud::getPosY() const { return _posY; }

float Noeud::getCoutChemin() const { return _coutChemin; }

float Noeud::getHeuristique() const { return _heuristique; }

std::string Noeud::getId() const { return _id; }

void Noeud::setCoutChemin(float c) { _coutChemin = c; }

void Noeud::setParent(std::shared_ptr<Noeud> p) { _parent = p; }

float Noeud::distanceRepereOrthonorme(float dx, float dy) { 
    return sqrt(dx*dx+dy*dy);
}

float Noeud::distance(std::shared_ptr<Noeud> arrivee) {
    float di = (float) (arrivee->getPosX() - _posX); //on calcule di, qui est le nombre de cases séparant la case d'arrivée de la case courante sur l'axe horizontal
    float dj = (float) (arrivee->getPosY() - _posY); //de la même manière, on calcule dj. Cette distance est mesurée sur l'axe oblique (qui forme un angle de 60° avec l'axe horizontal)
    float dx = di + dj/2; //on calcule la projection sur l'axe x d'un repère orthonormé du vecteur allant de la case courante à la case d'arrivée
    float dy = dj * sqrt(3)/2; //on fait de même pour l'axe y du repère orthonormé
    return distanceRepereOrthonorme(dx, dy); //on retourne la distance euclidienne entre les deux cases grâce à un calcul dans le repère orthonormé
}

void Noeud::initialiser(std::shared_ptr<Noeud> depart, std::shared_ptr<Noeud> arrivee) { 
    if (std::shared_ptr<Noeud>(this) == depart) _coutChemin = 0; 
    else _coutChemin = -1; 
    _heuristique = distance(arrivee);
    _parent = nullptr;
}
void Noeud::ajouterSuivant(std::shared_ptr<Noeud> noeud, unsigned int valeur) { 
    _suivants[noeud]=valeur; 
    //noeud->_suivants[std::shared_ptr<Noeud>(this)]=valeur;
}
void Noeud::explorerSuivants(std::vector<std::shared_ptr<Noeud>> & noeudsOuverts, std::vector<std::shared_ptr<Noeud>> & noeudsFermes) {
    for (const auto & paire : _suivants) {
        float nouveauCout = _coutChemin+paire.second;
        if (paire.first->getCoutChemin() == -1 || nouveauCout < paire.first->getCoutChemin()) {
            if (paire.first->getCoutChemin()==-1) noeudsOuverts.push_back(paire.first);
            paire.first->setCoutChemin(nouveauCout);  
            paire.first->setParent(std::shared_ptr<Noeud>(this));
        }
    }
}
void Noeud::afficher() {
    std::string resultat = _id+"("+std::to_string(_posX)+","+std::to_string(_posY)+") : ";
    for (const auto & paire : _suivants) {
        resultat += paire.first->getId()+","+std::to_string(paire.second)+"; ";
    }
    std::cout<<resultat<<std::endl;
}







Graphe::Graphe(const std::map<std::pair<int,int>,std::shared_ptr<Noeud>> noeuds) : _noeuds(noeuds) {}

void Graphe::retirerNoeud(std::vector<std::shared_ptr<Noeud>> & noeuds, std::shared_ptr<Noeud> noeud) {
    for (auto it = noeuds.begin(); it != noeuds.end(); it++) {
        if (*it == noeud) noeuds.erase(it);
    }
}

std::shared_ptr<Noeud> Graphe::plusFaibleScore(const std::vector<std::shared_ptr<Noeud>> & noeuds) {
    return *std::min_element(noeuds.begin(), noeuds.end(), [](std::shared_ptr<Noeud> n1, std::shared_ptr<Noeud> n2) { 
        return n1->getCoutChemin() + n1->getHeuristique() < n2->getCoutChemin() + n2->getHeuristique(); 
    });
}

std::vector<std::shared_ptr<Noeud>> Graphe::aEtoile(std::pair<int,int> depart, std::pair<int,int> arrivee) {
    std::shared_ptr<Noeud> noeudDepart = _noeuds[depart];
    std::shared_ptr<Noeud> noeudArrivee = _noeuds[arrivee];
    std::vector<std::shared_ptr<Noeud>> noeudsOuverts = {noeudDepart};
    std::vector<std::shared_ptr<Noeud>> noeudsFermes;
    std::vector<std::shared_ptr<Noeud>> plusCourtChemin;
    
    std::shared_ptr<Noeud> noeudCourant = noeudDepart;
    for (const auto & noeud : _noeuds) {
        std::cout<<noeud.first.first<<", "<<noeud.first.second<<", "<<noeud.second<<std::endl;
        noeud.second->initialiser(noeudDepart, noeudArrivee);
    }
    while (noeudCourant != noeudArrivee) {
        noeudCourant->explorerSuivants(noeudsOuverts, noeudsFermes);
        noeudsFermes.push_back(noeudCourant);
        retirerNoeud(noeudsOuverts,noeudCourant);
        noeudCourant = plusFaibleScore(noeudsOuverts);
    }    
    while (noeudCourant->getParent() != nullptr) {
        plusCourtChemin.push_back(noeudCourant);
        noeudCourant = noeudCourant->getParent();
    }
    plusCourtChemin.push_back(noeudCourant);
    std::reverse(plusCourtChemin.begin(), plusCourtChemin.end());
    return plusCourtChemin;
}