// minion.cpp
#include "minion.h"
#include "path.h"
#include <iostream>
#include <cmath> 

Minion::Minion(int id, TileMap* map, Castle* castle, unsigned int health, float speed, unsigned int reward, sf::Vector2f pos, float rotation, sf::Color color)
    : Entity(id), map(map), castle(castle), health(health), rewardOnDeath(reward), speed(speed), currentTargetIndex(0)
{
}

void Minion::move()
{
    // 1. Initialiser le système de pathfinding avec la grille
    Pathfinding pf(map->getLevel2D());

    // 2. Définir le départ et l'arrivée
    sf::Vector2u pos = map->getCurentTile(this->getPosition());
    Position start = { pos.y, pos.x };

    // Trouver la position de fin 
    sf::Vector2u endTile = map->findEdgeTile(7);
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