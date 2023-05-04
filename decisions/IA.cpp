#include "IA.h"

void melanger(std::vector<std::pair<int,int>> & v) {
  std::random_device rd;
  std::default_random_engine rng(rd());
  std::shuffle(v.begin(), v.end(), rng);
}

void IA::calculerCentreArmee(std::shared_ptr<Armee> armee) {
  _centreArmee = std::make_pair(0,0);
  for (unsigned int i = 0; i < armee->size(); i++) {
    _centreArmee.first += armee->getUnite(i)->getX();
    _centreArmee.second += armee->getUnite(i)->getY();
  }
  if (armee->size() > 0) {
    _centreArmee.first /= armee->size();
    _centreArmee.second /= armee->size();
  }
}

bool IA::villeControlee(const std::pair<int,int> & pos, std::shared_ptr<Armee> armee) const {
  for (unsigned int i = 0; i < armee->size(); i++) {
    if (armee->getUnite(i)->getPos() == pos) return true;
  }
  return false;
}

void IA::calculerVillesNecessaires(Carte & carte) { 
  _nombreVillesNecessaires = (unsigned int) std::ceil(static_cast<float>(carte.getArmee()->size())/8); // On considère qu'il faut une ville au moins pour 8 unités
}

// méthode qui calcule l'intérêt que représente la capture de la ville dont la position est passée en paramètre
int IA::scoreVille(Carte & carte, const std::pair<int,int> & pos) const {
  float score = 1/(distance(pos, _centreArmee)+1);
  return static_cast<int>(score);
}

// méthode qui renvoie un score correspondant à la capacité de l'unité à assurer le ravitaillement
int IA::scoreSoutienUnite(std::shared_ptr<Unite> unite) const {
  return unite->getDistanceRavitaillement();
}

// méthode qui renvoie un score correspondant à la capacité de l'unité à assurer l'exploration
int IA::scoreExplorationUnite(std::shared_ptr<Unite> unite) const {
  return unite->getDistanceVue()+unite->getVitesse();
}

std::pair<int,int> IA::plusProcheVoisin(const std::pair<int,int> & pos, const std::vector<std::pair<int,int>> & voisins) const {
  if (voisins.empty()) throw Exception("pas de voisins dans IA::plusProcheVoisin.");
  int distanceMin = distance(pos, voisins.front());
  int dist;
  std::pair<int,int> voisin = voisins.front();
  for (unsigned int i = 1; i < _villesSelectionnees.size(); i++) {
    dist = distance(pos, voisins.at(i));
    if (dist < distanceMin) {
      voisin = voisins.at(i);
      distanceMin = dist;
    }
  } 
  return voisin; 
}

bool IA::uniteDansVille(std::shared_ptr<Unite> unite) const {
  for (unsigned int i = 0; i < _villesControlees.size(); i++) {
    if (unite->getPos() == _villesSelectionnees.at(i)) return true;
  }
  return false;
}

int IA::nombreVoisinsInexplores(const std::pair<int,int> & pos, int rayon, Carte & carte) {
  int compt = 0;
  std::vector<std::pair<int,int>> voisins = carte.getCoordonneesVoisins(pos, rayon);
  for (unsigned int i = 0; i < voisins.size(); i++) {
    if (! carte.caseVisible(voisins[i])) compt++;
  }
  return compt;
}

int IA::nombreVoisinsInexplores(std::shared_ptr<Unite> unite, Carte & carte) {
  return nombreVoisinsInexplores(unite->getPos(), unite->getDistanceVue(), carte);
}

void IA::calculerPositionsEnnemis(const std::map<std::pair<int,int>, std::vector<std::shared_ptr<Unite>>> & ennemis) {
  _positionsEnnemisVisibles.clear(); 
  for (const auto & ennemi : ennemis) _positionsEnnemisVisibles.push_back(ennemi.first);
}

std::map<std::shared_ptr<Unite>, std::pair<int,int>> IA::dispersionAleatoire(const std::map<std::shared_ptr<Unite>, std::vector<std::pair<int,int>>> & casesAccessibles){
  std::map<std::shared_ptr<Unite>, std::pair<int,int>> dispersion;
  for (const auto & paire : casesAccessibles) {
    dispersion[paire.first] = paire.second.at(rand()%paire.second.size());
  }
  return dispersion;
}

int IA::calculerScoreDispersion(Carte & carte, const std::map<std::shared_ptr<Unite>, std::pair<int,int>> & dispersion){
  std::vector<std::pair<int,int>> departs = _villesControlees;
  std::vector<std::pair<int,int>> obstacles = _positionsEnnemisVisibles;    
  std::map<std::pair<int,int>,int> relais;
  for (auto & paire : dispersion) {
    relais[paire.second] = paire.first->getDistanceRavitaillement();
  }
  return carte.getGraphe(accessibilite::EauEtTerre)->zoneRavitaillement(departs, obstacles, relais).size();
}  

int IA::scoreAttaque(std::shared_ptr<Unite> unite, const std::vector<std::shared_ptr<Unite>> & ennemis) const {
  int score = 0;
  float agressivite = 0.5;
  std::pair<int,int> resultatCombat;
  for (unsigned int i = 0; i < ennemis.size(); i++) {
    resultatCombat = unite->resultatCombatSimple(ennemis.at(i));
    score += static_cast<int>(agressivite*static_cast<float>(resultatCombat.first)) - resultatCombat.second;
  }
  return score;
}

void IA::calculerPositionsRavitaillees(Carte & carte, const std::vector<std::shared_ptr<Unite>> & unites) {
  _positionsRavitaillees.clear();
  std::vector<std::pair<int,int>> departs = _villesControlees;
  std::vector<std::pair<int,int>> obstacles = _positionsEnnemisVisibles;    
  std::map<std::pair<int,int>,int> relais;
  std::shared_ptr<Unite> unite;
  for (unsigned int i = 0; i < unites.size(); i++) {
    unite = unites.at(i);
    relais[unite->getPos()] = unite->getDistanceRavitaillement();
  }
  std::vector<std::pair<int,int>> zoneRavitaillee = carte.getGraphe(accessibilite::EauEtTerre)->zoneRavitaillement(departs, obstacles, relais);
  for (unsigned int i = 0; i < zoneRavitaillee.size(); i++) _positionsRavitaillees[zoneRavitaillee.at(i)] = true;
}

void IA::initialiser(bool debut) {
  _unitesDispersees.clear();
}

// méthode qui détermine quelle villes sont contrôlées par le joueur courant, puis choisit les villes les plus itéressantes à capturer
void IA::selectionnerVilles(Carte & carte) {
  //initialisation des variables utiles
  _villesControlees.clear();
  _villesSelectionnees.clear();
  int nbVillesControlees = 0;
  std::map<std::pair<int,int>,int> scoreVilles;
  std::vector<std::pair<int,int>> departs = carte.getDepartsRavitaillement();
  
  // on parcourt une première fois les départs de ravitaillement (les villes) pour déterminer lesquels sont contrôlés par le joueur courant
  for (unsigned int i = 0; i < departs.size(); i++) {
    if (villeControlee(departs.at(i), carte.getArmee())){
      _villesControlees.push_back(departs.at(i));
      scoreVilles[departs.at(i)] = -1;
      nbVillesControlees++;
    }
  }
  if (nbVillesControlees >= _nombreVillesNecessaires) return; // Si le nombre de villes déjà contrôlées est suffisant, il est inutile de chercher à en sélectionner d'autres
  
  // on parcourt une deuxième fois les départs de ravitaillement et on calcule le score de tous les départs qui ne sont pas contrôlés par le joueur
  for (unsigned int i = 0; i < departs.size(); i++) {
    std::pair<int,int> depart = departs.at(i);
    if (scoreVilles[depart] == 0) scoreVilles[depart] = scoreVille(carte, depart);
  }

  // on parcourt autant de fois que nécessaire l'ensemble des villes
  for (unsigned int i = 0; i < _nombreVillesNecessaires - nbVillesControlees; i++) {
    std::pair<int,int> villeSelectionnee;
    int scoreMax = -1;
    for (auto ville : scoreVilles) { // on récupère la ville avec le plus grand score à chaque passage
      if (ville.second > scoreMax) {
        villeSelectionnee = ville.first;
        scoreMax = ville.second;
        ville.second = -1;
      }
    }
    // si la ville sélectionnée dans la boucle n'est pas contrôlée (son score est > -1), on l'ajoute aux villes sélectionnées
    if (scoreMax > -1) _villesSelectionnees.push_back(villeSelectionnee); 
  }
}

// méthode qui ordonne à l'unité de se placer à l'endroit optimal pour assurer un bon ravitaillement
void IA::etendreRavitaillement(Carte & carte, std::shared_ptr<Unite> unite) {
  std::pair<int,int> pos = unite->getPos();
  std::pair<int,int> villeVoisine = plusProcheVoisin(pos, _villesSelectionnees);
  int porteeVille = carte.porteeRavitaillementAllie(villeVoisine);
  if (uniteDansVille(unite)) { // Si l'unité se trouve dans une ville avec un allié qui ravitaille plus loin, l'unité part
    if (unite->getDistanceRavitaillement() < porteeVille) _unitesDispersees.push_back(unite);
  } else if (! _villesSelectionnees.empty() || unite->getDistanceRavitaillement() > porteeVille) { 
    // Si la ville la plus proche est sélectionnée, ou est occupée mais peut augmenter sa portée de ravitaillement, l'unité se dirige vers elle
    unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::DEPLACER, villeVoisine.first, villeVoisine.second)); 
  } else { // sinon l'unité est dispersée
    _unitesDispersees.push_back(unite);
  }
}

void IA::choisirDispersion(Carte & carte) {
  std::map<std::shared_ptr<Unite>, std::vector<std::pair<int,int>>> casesAccessibles;
  std::shared_ptr<Unite> unite;
  for (unsigned int i = 0; i < _unitesDispersees.size(); i++) {
    unite = _unitesDispersees.at(i);
    casesAccessibles[unite] = carte.positionsAccessibles(unite);
  }
  int scoreSelection;
  int score;
  std::map<std::shared_ptr<Unite>, std::pair<int,int>> dispersionSelectionnee;
  std::map<std::shared_ptr<Unite>, std::pair<int,int>> dispersion;
  for (unsigned int i = 0; i < 10; i++){
    dispersion = dispersionAleatoire(casesAccessibles);
    score = calculerScoreDispersion(carte, dispersion);
    if (score > scoreSelection) {
      dispersionSelectionnee = dispersion;
      scoreSelection = score;
    }
  }
}

void IA::calculerDegatsNecessaires(const std::map<std::pair<int,int>, std::vector<std::shared_ptr<Unite>>> & ennemis) {
  _degatsNecessaires.clear();
  for (const auto & position : ennemis){
    for (unsigned int i = 0; i < position.second.size(); i++) {
      _degatsNecessaires[position.first] += position.second.at(i)->getSante();
    }
  }
}

bool IA::attribuerCible(Carte & carte, std::shared_ptr<Unite> unite, const std::map<std::pair<int,int>, std::vector<std::shared_ptr<Unite>>> & ennemis) {
  std::vector<std::pair<int,int>> zoneAccessible = carte.positionsAccessibles(unite); //On étudie les cases pouvant être atteintes en un tour
  if (zoneAccessible.empty()) return false;
  int scoreMax = -1;
  int score;
  std::pair<int,int> cibleChoisie;
  std::pair<int,int> pos;  
  for (unsigned int i = 0; i < zoneAccessible.size(); i++) { //On parcourt toutes les cases accessibles pour trouver celle qui est la plus intéressante
    pos = zoneAccessible.at(i);
    if (_degatsNecessaires[pos] > 0) { // on n'étudie que les cases où il est nécessaires d'infliger des dégâts
      score = scoreAttaque(unite, ennemis.at(pos)); // on calcule ensuite un score qui dépend des dégâts infligés et reçus
      if (score > scoreMax) { // si le score précédent était moins grand, on le remplace par le nouveau score
        cibleChoisie = pos;
        scoreMax = score;
      }
    }
  }
  if (scoreMax > 0) { // si un score suffisamment intéressant a été trouvé, on ordonne à l'unité de marcher sur cette case pour attaquer les ennemis qui s'y trouvent
    unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::DEPLACER, cibleChoisie.first, cibleChoisie.second));
    _degatsNecessaires[cibleChoisie] -= unite->resultatCombatSimple(ennemis.at(cibleChoisie).front()).first;//Cette ligne est provisoire : il faudrait retirer les dégats de l'ensemble des ennemis frappés
  }
  return (scoreMax > 0);
}

void IA::calculerScoresExploration(Carte & carte) {
  int rayon = carte.getRayon();
  std::pair<int,int> pos;
  int dist;
  int debut = -rayon+1;
  int fin = 0;
  for (int j = rayon-1; j > -rayon; j--) {
      for (int i = debut; i <= fin; i++) {        
        pos = std::make_pair(i,j);
        if (carte.caseVisible(pos)) {
          _scoresExploration[pos] = 0;
        } else {
          dist = distance(pos, _centreArmee);
          _scoresExploration[pos] += (int) 100/(dist*dist+10);
        }
      }
      if (j>0) fin++;
      else debut++;        
  }
}

void IA::explorer(Carte & carte, std::shared_ptr<Unite> unite) {
  std::vector<std::pair<int,int>> zoneAccessible = carte.positionsAccessibles(unite);
  std::vector<std::pair<int,int>> zoneRavitaillee;
  std::pair<int,int> pos;
  for (unsigned int i = 0; i < zoneAccessible.size(); i++) {
    pos = zoneAccessible.at(i);
    if (_positionsRavitaillees[pos]) zoneRavitaillee.push_back(pos);
  } 
  int scoreMax = -1;
  int score;
  std::pair<int,int> cible;
  for (unsigned int i = 0; i < zoneRavitaillee.size(); i++) {
    pos = zoneRavitaillee.at(i);
    score = _scoresExploration.at(pos);
    if (score > scoreMax){
      cible = pos;
      scoreMax = score;
    }
  } 
  if (scoreMax > 0) {     
    unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::DEPLACER, cible.first, cible.second));
    _scoresExploration[cible] = 0;
    std::vector<std::pair<int,int>> voisins = carte.getCoordonneesVoisins(cible);
    for (unsigned int i = 0; i < voisins.size(); i++) _scoresExploration[voisins.at(i)] = 0;
  }
}

void IA::defendre(Carte & carte, std::shared_ptr<Unite> unite) {
  int dist;
  int distmin = carte.getRayon()*2;
  std::pair<int,int> posUnite = unite->getPos();
  std::pair<int,int> posVille;
  std::pair<int,int> cible;
  for (unsigned int i = 0; i < _villesControlees.size(); i++) { //On récupère la position de la ville la plus proche de l'unité
    posVille = _villesControlees.at(i);
    dist = distance(posUnite, posVille);
    if (dist < distmin) {
      cible = posVille;
      distmin = dist;
    }
  }
  if (distmin < carte.getRayon()*2) { // On donne l'ordre à l'unité de se placer sur l'une des cases voisines de la case cible
    std::vector<std::pair<int,int>> voisins = carte.getCoordonneesVoisins(cible);
    std::pair<int,int> voisin = voisins.at(rand()%voisins.size());
    unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::DEPLACER, voisin.first, voisin.second));
  }
}

void IA::jouerArmee(Carte & carte) {
  
  initialiser(true);
  std::map<std::pair<int,int>, std::vector<std::shared_ptr<Unite>>> ennemis = carte.getUnitesVisibles(false);
  calculerPositionsEnnemis(ennemis);
  std::shared_ptr<Armee> armee = carte.getArmee();
  calculerCentreArmee(armee);
  calculerVillesNecessaires(carte);
  selectionnerVilles(carte);
  std::vector<std::shared_ptr<Unite>> unites1 = armee->getUnites();
  std::vector<std::shared_ptr<Unite>> unites2;
  std::vector<std::shared_ptr<Unite>> unites3;
  std::vector<std::shared_ptr<Unite>> unites4;
  for (unsigned int i = 0; i < unites1.size(); i++) {
    if (scoreSoutienUnite(unites1.at(i)) > 0) {
      etendreRavitaillement(carte, unites1.at(i));
      unites2.push_back(unites1.at(i));
    }
    
    else unites3.push_back(unites1.at(i));
  }  
  calculerDegatsNecessaires(ennemis);  
  for (unsigned int i = 0; i < unites3.size(); i++) {
    if(! attribuerCible(carte, unites3.at(i), ennemis)) {
      unites4.push_back(unites3.at(i));
    }
  }  
  calculerScoresExploration(carte);
  std::shared_ptr<Unite> unite;
  for (unsigned int i = 0; i < unites4.size(); i++) {
    unite = unites4.at(i);
    if (scoreExplorationUnite(unite) > 2) explorer(carte, unite);
    else defendre(carte, unite); 
  }

  initialiser(false); // //!\\ Il faut IMPERATIVEMENT faire une nouvelle initialisation à la fin de l'utilisation de l'IA, afin de supprimer les shared_ptr sur les unités
}