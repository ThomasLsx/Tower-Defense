#include "minion.h"
#include "path.h"

#include <iostream>

Minion::Minion(int id, Path* path, unsigned int health, sf::Vector2f velocity, unsigned int reward, sf::Vector2f pos, float rotation, sf::Color color, sf::IntRect textureRect, const sf::Texture& texture)
    : Entity(id, pos, rotation, color, textureRect, texture), health(health), velocity(velocity), pathProgress(0.0f),
    targetPath(path), rewardOnDeath(reward)
{
}

void Minion::move(float dt) {
	sf::Vector2f foo = Entity::getPosition();
    Entity::setPosition( foo += velocity * dt);
}

/* TODO : faire followpath des que class path créer */
void Minion::followPath() {
    if (targetPath && pathProgress < 1.0f) {
        sf::Vector2f targetPos = targetPath->getPointAt(pathProgress);
        sf::Vector2f direction = targetPos - getPosition();
        if (direction.lengthSquared() < 0.1f) {
            pathProgress += 0.01f;
            if (pathProgress >= 1.0f) {
                pathProgress = 1.0f;
                onDestroy();
                return;
            }
            targetPos = targetPath->getPointAt(pathProgress);
            direction = targetPos - getPosition();
        }
        /*TODO : créer fonction normalize*/
        direction.normalize();
        velocity = direction * 2.0f;
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
    Entity::setisAlive(false);
    /*
    TODO : appelé fct affichage du BOOM et la thune
    */
}

void Minion::update(float dt) {
    if (Entity::getisAlive()) {
        followPath();
        move(dt);
    }
}
