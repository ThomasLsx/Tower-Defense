#include "minion.h"
#include "path.h"

#include <iostream>

Minion::Minion(int id,/*Path* path, */ unsigned int health, sf::Vector2f velocity, unsigned int reward, sf::Vector2f pos, float rotation, sf::Color color)
    : Entity(id), health(health), velocity(velocity), pathProgress(0.0f),
    /*targetPath(path),*/ rewardOnDeath(reward)
{
	Entity::init();
}

void Minion::move(float dt) {
	sf::Vector2f foo = Entity::getPosition();
    Entity::setPosition( foo += velocity * dt);
}

/* TODO : faire followpath des que class path créer */

void Minion::followPath() {
    // Implémentation temporaire : avance le pathProgress
    // TODO: Remplacer par une vraie logique de suivi de chemin quand Path sera implémenté
    pathProgress += 0.01f;
    if (pathProgress >= 1.0f) {
        pathProgress = 1.0f;
        onDestroy();
    }
}

void Minion::takeDamage(int amount) {
    if (amount < health) {
        health -= amount;
    } else {
        health = 0;
        onDestroy();
	}
}

void Minion::onDestroy() {
    Entity::setIsAlive(false);
    /*
    TODO : appelé fct affichage du BOOM et la thune
    */
}

void Minion::update(float dt) {
    if (Entity::getIsAlive()) {
        followPath();
        move(dt);
    }
}
