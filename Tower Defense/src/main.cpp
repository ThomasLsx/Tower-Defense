#include "game.h"
#include <iostream>
#include <cmath>
#include "Window.h"
#include <SFML/Graphics.hpp>

#include "entity.h"
#include "minion.h"
#include "path.h"
#include "projectile.h"
#include "tower.h"

//int main() {
//
//	Minion mimi(0);
//	mimi.setPosition(sf::Vector2f(500, 500));
//	auto direction = mimi.getPosition() - sf::Vector2f(100, 100);
//	auto distance = sqrt(direction.x * direction.x + direction.y * direction.y);
//	sf::Vector2f velocity(direction.x / distance, direction.y / distance);
//
//	Projectile proj(0, 0, 0, velocity, 10, 100, sf::Vector2f(100, 100));
//	
//	Window window;
//	UI* ui; 
//
//	window.create();
//	window.getRenderWindow().setFramerateLimit(60);
//	ui = new UI(window.getRenderWindow());
//
//	while (true) {
//		proj.update(2);
//
//		window.clear(sf::Color(50, 50, 50));
//		mimi.draw(window.getRenderWindow());
//		proj.draw(window.getRenderWindow());
//		ui->draw();
//		window.display();
//	}
//	return 0;
//}
//
//void processEvents(Window window, UI* ui)
//{
//    while (const std::optional<sf::Event> event = window.pollEvent())
//    {
//        if (event->is<sf::Event::Closed>())
//            window.close();
//        ui->handleEvent(*event);
//    }
//}

//#include <fstream>
//#include <vector>
//#include <sstream>
//#include <optional>
//#include "minion.h"
//#include "path.h"
//#include <iostream>


int main () {
	//Entity* minion = new Minion(1);
	//minion->init(30, sf::Color::Green, sf::Color::Black, 2);

 //   std::ifstream fichier("assets/map1.txt");
 //   std::vector<std::vector<int>> map;
 //   std::string ligne;


 //   if (!fichier.is_open()) {
 //       std::cerr << "Erreur : impossible d'ouvrir le fichier " << "assets/map1.txt" << std::endl;
 //   }

 //   while (std::getline(fichier, ligne)) {
 //       std::istringstream iss(ligne);
 //       std::vector<int> ligneMap;
 //       int nombre;
 //       while (iss >> nombre) {
 //           ligneMap.push_back(nombre);
 //       }
 //       map.push_back(ligneMap);
 //   }

 //   fichier.close();
 //   

 //   // 1. Initialiser le système de pathfinding avec la grille
 //   Pathfinding pf(map);

 //   // 2. Définir le départ et l'arrivée
 //   Position start = { 4, 2 };
 //   Position goal = { 4, 18 };

 //   std::cout << "Recherche de chemin de (" << start.x << ", " << start.y
 //       << ") à (" << goal.x << ", " << goal.y << ")" << std::endl;

 //   // 3. Trouver le chemin
 //   std::optional<std::vector<Position>> pathOpt = pf.findPath(start, goal);

 //   static_cast<Minion*>(minion)->setPath(*pathOpt, 100);

	Game game;
	game.run();

	return 0;
}