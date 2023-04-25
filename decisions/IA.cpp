#include "IA.h"

void IA::jouerUnite(Carte & carte, std::shared_ptr<Unite> unite) { 
  std::map<std::pair<int,int>, std::shared_ptr<Unite>> unitesVisibles = carte.getUnitesVisibles(false);//on récupère les unités visibles qui n'appartiennent pas à l'armée courante   
  std::vector<std::pair<int,int>> casesVoisines = carte.getCoordonneesRayon(unite->getPos(), unite->getPortee());
  if (unite->getPortee() > 0) { //Si l'unité peut tirer à distance, on essaie de la faire attaquer le premier ennemi à portée
    for (unsigned int i = 0; i < casesVoisines.size(); i++) {
      if (carte.caseAvecEnnemi(casesVoisines[i].first, casesVoisines[i].second)) {
        attaquerEnnemi(unite, casesVoisines[i]);
        return;
      } 
    }
  }
  std::vector<std::pair<int,int>> casesAccessibles = carte.positionsAccessibles(unite);
  for (unsigned int i = 0; i < casesVoisines.size(); i++) {// On essaie de faire marcher l'unité sur une position tenue par un ennemi accessible en un tour
    if (carte.caseAvecEnnemi(casesVoisines[i].first, casesVoisines[i].second)) {
      poursuivreEnnemi(unite, casesVoisines[i]);
      return;
    } 
  }
  explorer(unite, carte);
}

void IA::jouerArmee(Carte & carte) {
    std::shared_ptr<Armee> armee = carte.getArmee();
    for (unsigned int i = 0; i < armee->taille(); i++) {
      jouerUnite(carte, armee->getUnite(i));//on choisit le comportement de toutes les unités de l'armée courante
    }     
}

void IA::poursuivreEnnemi(std::shared_ptr<Unite> unite, std::pair<int,int> pos) {
  unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::DEPLACER, pos.first, pos.second));
}

void IA::suivreAllie(std::shared_ptr<Unite> unite, std::shared_ptr<Unite> ennemi) {
  std::pair<int,int> pos = ennemi->getPos();
  unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::DEPLACER, pos.first, pos.second));
}

void IA::attaquerEnnemi(std::shared_ptr<Unite> unite, std::pair<int,int> pos) {
  unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::ATTAQUER, pos.first, pos.second));
}

int IA::nombreVoisinsInexplores(std::pair<int,int> pos, Carte & carte) {
  int compt = 0;
  std::vector<std::pair<int,int>> voisins = carte.getCoordonneesVoisins(pos.first, pos.second);
  for (unsigned int i = 0; i < voisins.size(); i++) {
    if (carte.caseVisible(voisins[i])) compt++;
  }
  return compt;
}

void IA::explorer(std::shared_ptr<Unite> unite, Carte & carte) {
  std::vector<std::pair<int,int>> positionsAccessibles = carte.positionsAccessibles(unite);
  int score = nombreVoisinsInexplores( unite->getPos(), carte);
  std::pair<int,int> position = unite->getPos();
  for (unsigned int i = 0; i < positionsAccessibles.size(); i++) {
    int voisinsInexplores = nombreVoisinsInexplores(positionsAccessibles[i], carte);
    if (voisinsInexplores > score) {
      position = positionsAccessibles[i];
      score = voisinsInexplores;
    }
  }
  unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::DEPLACER, position.first, position.second));
}

void IA::fuir(std::shared_ptr<Unite> unite, Carte & carte) {
  std::vector<std::pair<int,int>> positionsAccessibles = carte.positionsAccessibles(unite);
  std::pair<int,int> position = unite->getPos();
  for (unsigned int i = 0; i < positionsAccessibles.size(); i++) {
    if (! carte.caseAvecEnnemi(positionsAccessibles[i].first, positionsAccessibles[i].second)) {
      position = positionsAccessibles[i];
      break;
    }
  }
  unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::DEPLACER, position.first, position.second));
}

void IA::garderPosition(std::shared_ptr<Unite> unite, std::pair<int,int> pos, Carte & carte) {
  std::vector<std::pair<int,int>> positionsAccessibles = carte.positionsAccessibles(unite);
  std::pair<int,int> position = positionsAccessibles[rand()%positionsAccessibles.size()];
  unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::DEPLACER, position.first, position.second));
}

void IA::ravitailler(std::shared_ptr<Unite> unite, std::pair<int,int> arrivee, Carte & carte){
  std::vector<std::pair<int,int>> departs = carte.getDepartsRavitaillement();
  std::vector<std::pair<int,int>> obstacles =  carte.getPositionsEnnemis();    
  std::map<std::pair<int,int>,int> relais =  carte.getRelaisRavitaillement(unite);
  std::vector<std::pair<int,int>> zoneRavitaillement = carte.getGraphe(accessibilite::EauEtTerre)->zoneRavitaillement(departs, obstacles, relais);
  std::pair<int,int> position = unite->getPos();
  float distanceMinimale = distance(position, arrivee);
  for (unsigned int i = 0; i < zoneRavitaillement.size(); i++) {
    if(distance(zoneRavitaillement[i], arrivee) < distanceMinimale) {
      position = zoneRavitaillement[i];
      distanceMinimale = distance(position, arrivee);
    }
  }
  unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::DEPLACER, position.first, position.second));
}