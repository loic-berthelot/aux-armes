#include "IA.h"

void melanger(std::vector<std::pair<int,int>> & v) {
  std::random_device rd;
  std::default_random_engine rng(rd());
  std::shuffle(v.begin(), v.end(), rng);
}

void IA::jouerUnite(Carte & carte, std::shared_ptr<Unite> unite) { 
  std::vector<std::pair<int,int>> casesVoisines = carte.getCoordonneesVoisins(unite->getPos(), unite->getPortee());
  melanger(casesVoisines);
  if (unite->getPortee() > 0) { //Si l'unité peut tirer à distance, on essaie de la faire attaquer le premier ennemi à portée
    for (unsigned int i = 0; i < casesVoisines.size(); i++) {      
      if (carte.ennemiSurCase(casesVoisines[i])) {
        attaquerEnnemi(unite, casesVoisines[i]);
        return;
      } 
    }
  }
  std::vector<std::pair<int,int>> casesAccessibles = carte.positionsAccessibles(unite);
  melanger(casesAccessibles);
  for (unsigned int i = 0; i < casesAccessibles.size(); i++) {// On essaie de faire marcher l'unité sur une position tenue par un ennemi, et accessible en un tour
    if (carte.ennemiSurCase(casesAccessibles[i])) {
      poursuivreEnnemi(unite, casesAccessibles[i]);
      return;
    } 
  }
  std::map<std::pair<int,int>, std::shared_ptr<Unite>> ennemisVisibles = carte.getUnitesVisibles(false);//on récupère les unités visibles qui n'appartiennent pas à l'armée courante   
  for (const auto & ennemi : ennemisVisibles) {// On essaie de faire marcher l'unité sur une position tenue par un ennemi, non accessible ce tour-ci
    poursuivreEnnemi(unite, ennemi.first);std::cout<<ennemi.first.first<<","<<ennemi.first.second<<std::endl;
    return;
  }
  explorer(unite, carte); //on explore les cases autour de l'unité
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

int IA::nombreVoisinsInexplores(std::pair<int,int> pos, int rayon, Carte & carte) {
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

//L'unité passée en paramètre va recevoir l'ordre de se diriger vers la case la moins bien explorée et atteignable en un tour
void IA::explorer(std::shared_ptr<Unite> unite, Carte & carte) {
  std::vector<std::pair<int,int>> positionsAccessibles = carte.positionsAccessibles(unite);
  melanger(positionsAccessibles);
  int score = nombreVoisinsInexplores(unite, carte);
  std::pair<int,int> position = unite->getPos();
  for (unsigned int i = 0; i < positionsAccessibles.size(); i++) {
    int voisinsInexplores = nombreVoisinsInexplores(positionsAccessibles[i], unite->getDistanceVue(), carte);
    if (voisinsInexplores > score) {
      position = positionsAccessibles[i];
      score = voisinsInexplores;      
    }
  }
  unite->donnerOrdre(std::make_shared<Ordre>(TypeOrdre::DEPLACER, position.first, position.second));
}

void IA::fuir(std::shared_ptr<Unite> unite, Carte & carte) {
  std::vector<std::pair<int,int>> positionsAccessibles = carte.positionsAccessibles(unite);
  melanger(positionsAccessibles);
  std::pair<int,int> position = unite->getPos();
  for (unsigned int i = 0; i < positionsAccessibles.size(); i++) {
    if (! carte.ennemiSurCase(positionsAccessibles[i])) {
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