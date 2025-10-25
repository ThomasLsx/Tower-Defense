#include "game.h"
#include <iostream>
#include <cmath>
#include "Window.h"
#include <SFML/Graphics.hpp>

#include "entity.h"
#include "minion.h"
#include "path.h"

Game::Game()
    : window(800, 600, "SFML + TGUI Example"),
    detectionZone(sf::Vector2f(200, 150), sf::Vector2f(400, 200)),
    isRunning(true)
{
	this->entity = new Minion(1);
	entity->init(30, sf::Color::Green, sf::Color::Black, 2);
    this->entity2 = new Minion(2);
	entity2->init(30, sf::Color::Blue, sf::Color::Black, 2);

    // Matrice d'adjacence (0 = accessible, 1 = mur)
    std::vector<std::vector<int>> adjacencyMatrix = {
        {0, 0, 1, 0},
        {1, 0, 1, 0},
        {1, 0, 1, 0},
        {1, 0, 0, 0}
    };

    // 1. Initialiser le système de pathfinding avec la grille
    Pathfinding pf(adjacencyMatrix);

    // 2. Définir le départ et l'arrivée
    Position start = { 0, 0 };
    Position goal = { 0, 3 };

    std::cout << "Recherche de chemin de (" << start.x << ", " << start.y
        << ") à (" << goal.x << ", " << goal.y << ")" << std::endl;

    // 3. Trouver le chemin
    std::optional<std::vector<Position>> pathOpt = pf.findPath(start, goal);

    entity->setPath(*pathOpt, 100);

    window.create();
    window.getRenderWindow().setFramerateLimit(60);
    ui = new UI(window.getRenderWindow());
}

void Game::run()
{
    while (window.isOpen() && isRunning)
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional<sf::Event> event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
        ui->handleEvent(*event);
        if (event->is<sf::Event::MouseButtonPressed>())
        {
            auto mouse = event->getIf<sf::Event::MouseButtonPressed>();
            sf::Vector2f pos(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));
            if (detectionZone.contains(pos))
            {
                if (mouse->button == sf::Mouse::Button::Left)
                    std::cout << "Left click at (" << mouse->position.x << ", " << mouse->position.y << ")\n";
                if (mouse->button == sf::Mouse::Button::Right)
                    std::cout << "Right click at (" << mouse->position.x << ", " << mouse->position.y << ")\n";
            }
        }
        if (event->is<sf::Event::KeyPressed>())
        {
            auto key = event->getIf<sf::Event::KeyPressed>();
            if (key->code == sf::Keyboard::Key::Up) keyState.up = true;
            if (key->code == sf::Keyboard::Key::Down) keyState.down = true;
            if (key->code == sf::Keyboard::Key::Left) keyState.left = true;
            if (key->code == sf::Keyboard::Key::Right) keyState.right = true;
            if (key->code == sf::Keyboard::Key::A) keyState.a = true;
            if (key->code == sf::Keyboard::Key::E) keyState.e = true;
        }
        if (event->is<sf::Event::KeyReleased>())
        {
            auto key = event->getIf<sf::Event::KeyReleased>();
            if (key->code == sf::Keyboard::Key::Up) keyState.up = false;
            if (key->code == sf::Keyboard::Key::Down) keyState.down = false;
            if (key->code == sf::Keyboard::Key::Left) keyState.left = false;
            if (key->code == sf::Keyboard::Key::Right) keyState.right = false;
            if (key->code == sf::Keyboard::Key::A) keyState.a = false;
            if (key->code == sf::Keyboard::Key::E) keyState.e = false;
        }
    }

    float moveAmount = 1.5f;
    float dx = 0.f, dy = 0.f;
    if (keyState.up)
        dy -= 1.f;
    if (keyState.down)
        dy += 1.f;
    if (keyState.left)
        dx -= 1.f;
    if (keyState.right)
        dx += 1.f;
    currentSpeed = 0.f;
    if (dx != 0.f || dy != 0.f) {
        float length = std::sqrt(dx * dx + dy * dy);
        dx /= length;
        dy /= length;
        sf::Vector2f newPos = entity->getPosition() + sf::Vector2f(dx * moveAmount, dy * moveAmount);
        entity->setPosition(newPos);
        currentSpeed = moveAmount;
    }
    /*
    if (keyState.a)
        entity->setRotation(entity->getRotation() - sf::degrees(2.f));
    if (keyState.e)
        entity->setRotation(entity->getRotation() + sf::degrees(2.f));
     */

    // Gestion des limites de l'écran
    sf::Vector2f pos = entity->getPosition();
    sf::FloatRect bounds = entity->getGlobalBounds();
    if (pos.x < 0) pos.x = 0;
    if (pos.y < 0) pos.y = 0;
    if (pos.x > 800 - bounds.size.x) pos.x = 800 - bounds.size.x;
    if (pos.y > 600 - bounds.size.y) pos.y = 600 - bounds.size.y;
    entity->setPosition(pos);
}

void Game::update()
{
    ui->setSpeed(currentSpeed);
    entity->update(1);
    std::cout << "Position : " << entity->getPosition().x << " " << entity->getPosition().y << std::endl;
}

void Game::render()
{
    window.clear(sf::Color(50, 50, 50));
    entity->draw(window.getRenderWindow());
    ui->draw();
    window.display();
}

