#include "minion.h"
#include "path.h"

#include <iostream>

Minion::Minion(int id,/*Path* path, */ unsigned int health, unsigned int reward, sf::Vector2f pos, float rotation, sf::Color color)
    : Entity(id), health(health), rewardOnDeath(reward)
{
	Entity::init();
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

    // Place le minion au début du chemin
    if (!targetPath.empty()) {
       _position = targetPath[0];
    }
	// TODO : gérer le cas où le chemin est vide
}

void Minion::update(float dt) {
    // Entity::update(dt); // Appelle la mise à jour de base (si elle existe)

    // Le minion ne bouge que s'il est en vie
    if (health > 0) {
        followPath(dt);
    }
}

void Minion::followPath(float dt) {
    // S'il n'y a pas de chemin ou si on a atteint la fin, on ne fait rien.
    if (targetPath.empty() || currentTargetIndex >= targetPath.size()) {
        return;
    }

    // 1. Définir la cible actuelle
    sf::Vector2f target = targetPath[currentTargetIndex];

    // 2. Calculer le vecteur direction et la distance
    sf::Vector2f foo;
    if (currentTargetIndex == 0) {
        foo = targetPath[currentTargetIndex];
    }
    else {
        foo = targetPath[currentTargetIndex-1];
    }
    sf::Vector2f direction = target - _position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // 3. Calculer le déplacement pour cette frame
    float moveAmount = std::sqrt(_velocity.x* _velocity.x + _velocity.y * _velocity.y) * dt;

    // 4. Vérifier si on atteint (ou dépasse) la cible
    if (distance <= moveAmount) {
        // On a atteint la cible
        _position = target; // On "snap" à la position cible pour éviter les dépassements
        _shape.setPosition(_position);
        currentTargetIndex++; // On vise le point suivant

        // Vérifier si c'était le dernier point
        if (currentTargetIndex >= targetPath.size()) {
            std::cout << "Minion " << _id << " a atteint la fin du chemin !" << std::endl;
            // Ici, déclenchez la logique de fin de chemin (ex: infliger dégâts à la base)
            this->onDestroy();
        }
    }
    else {
        // 5. On n'a pas atteint la cible, on s'en approche
        // Normaliser le vecteur direction (longueur de 1) et le multiplier par le déplacement
        direction /= distance; // (direction.x / distance, direction.y / distance)
        _position += direction * moveAmount;
        _shape.setPosition(_position);

        // Optionnel : faire tourner le minion pour qu'il regarde sa cible
        // this->rotation = std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f;
    }
}

void Minion::makeDamage(int amount)
{
}

void Minion::onDestroy() {
    Entity::setIsAlive(false);
    /*
    TODO : appelé fct affichage du BOOM et la thune
    */
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