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


int main () {
	Game game;
	game.run();

	return 0;
}