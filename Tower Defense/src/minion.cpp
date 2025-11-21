// minion.cpp
#include "minion.h"
#include "path.h"
#include <iostream>
#include <cmath> 

Minion::Minion(int id, TileMap* map, Castle* castle, unsigned int health, float speed, unsigned int reward, sf::Vector2f pos, float rotation, sf::Color color)
	: Entity(id), map(map), castle(castle), health(health), rewardOnDeath(reward), speed(speed), currentTargetIndex(0), maxHealth(health)
{
}

void Minion::move()
{
    // 1. Initialiser le système de pathfinding avec la grille
    Pathfinding pf(map->getTowerLevel2D());

    // 2. Définir le départ et l'arrivée
    sf::Vector2u pos = map->getCurentTile(this->getPosition());
    Position start = { pos.y, pos.x };

    // Trouver la position de fin 
    sf::Vector2u endTile = map->findEdgeTile(3);
    Position goal = { endTile.y, endTile.x };

    // 3. Trouver le chemin
    std::optional<std::vector<Position>> pathOpt = pf.findPath(start, goal);

    if (pathOpt.has_value() && !pathOpt->empty()) {
        static_cast<Minion*>(this)->setPath(*pathOpt, map->getTileSize().x * map->getScale());
    }
    else {
        std::cout << "Aucun chemin valide trouve !" << std::endl;
    }
}

void Minion::setPath(const std::vector<Position>& gridPath, float tileSize) {
    targetPath.clear();
    currentTargetIndex = 0;

    for (const Position& p : gridPath) {
        // ATTENTION: p.x est la LIGNE (Y), p.y est la COLONNE (X)
        float worldX = (p.y * tileSize) + (tileSize / 2.0f);
        float worldY = (p.x * tileSize) + (tileSize / 2.0f);
        targetPath.push_back(sf::Vector2f(worldX, worldY));
    }

    if (!targetPath.empty()) {
        _position = targetPath[0]; 
        if (_shape) _shape->setPosition(_position);
    }
}

void Minion::update(float dt) {
    if (health > 0) {
        followPath(dt);
    }
}

void Minion::draw(sf::RenderWindow& window)
{
    if (getIsAlive())
    { 
    // Dessiner l'arrière-plan de la barre de vie
    healthBarBack = sf::RectangleShape(sf::Vector2f(35.f, 10.f));
    healthBarBack.setFillColor(sf::Color::Transparent);
    healthBarBack.setOutlineColor(sf::Color::Black);
    healthBarBack.setOutlineThickness(2.f);
    healthBarBack.setOrigin(sf::Vector2f(17.5f, 15.f + map->getHeight()));
    healthBarBack.setPosition(this->getPosition());
    window.draw(healthBarBack);

    // Dessiner la barre de vie
    healthBar = sf::RectangleShape(sf::Vector2f(35.f, 10.f));
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setOrigin(sf::Vector2f(17.5f, 15.f + map->getHeight()));
    healthBar.setPosition(this->getPosition());
    float healthPercent = static_cast<float>(health) / static_cast<float>(maxHealth);
    healthBar.setSize(sf::Vector2f(35.f * healthPercent, 10.f));
    window.draw(healthBar);
    
    window.draw(*_shape);
    }
}

void Minion::followPath(float dt) {
    if (targetPath.empty() || currentTargetIndex >= targetPath.size()) {
        return;
    }

    sf::Vector2f target = targetPath[currentTargetIndex];
    sf::Vector2f direction = target - _position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float moveAmount = speed * dt;

    if (distance <= moveAmount) {
        _position = target;
        if (_shape) _shape->setPosition(_position);
        currentTargetIndex++;

        if (currentTargetIndex >= targetPath.size()) {
            std::cout << "Minion " << _id << " a atteint la base!" << std::endl;
			castle->takeDamage(10); 
            this->onDestroy();
        }
    }
    else {
        direction /= distance;
        _position += direction * moveAmount;
        if (_shape) _shape->setPosition(_position);
    }
}

void Minion::makeDamage(int amount)
{
    // TODO
}

void Minion::onDestroy() {
    if (_isAlive) {
        Entity::setIsAlive(false);
    }
}

void Minion::takeDamage(int amount) {
    if (amount <= 0 || health == 0) return;

    if ((unsigned int)amount >= health) {
        health = 0;
        std::cout << "Minion " << _id << " est mort." << std::endl;
        this->onDestroy();
    }
    else {
        health -= amount;
    }
}