// projectile.cpp
#include "projectile.h"
#include <iostream>
#include <cmath> 

Projectile::Projectile(unsigned int id, unsigned int sourceTowerId, std::shared_ptr<Minion> target, sf::Vector2f velocity, unsigned int damage, float lifetime, sf::Vector2f pos, float rotation, sf::Color color)
    :Entity(id), velocity(velocity), damage(damage), lifetime(lifetime), sourceTowerId(sourceTowerId),
    target(target)
{
    Entity::init(5, sf::Color::Yellow);
    Entity::setPosition(pos);
}

void Projectile::update(float dt) {
    if (!_isAlive) return;

    lifetime -= dt;
    if (lifetime <= 0.0f) {
        onDestroy();
        return;
    }

    if (auto targetPtr = target.lock())
    {
        sf::Vector2f targetPos = targetPtr->getPosition();
        sf::Vector2f direction = targetPos - _position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        float moveSpeed = velocity.x;
        float moveAmount = moveSpeed * dt;

        if (distance < moveAmount)
        {
            _position = targetPos;
        }
        else
        {
            direction /= distance;
            _position += direction * moveAmount;
            if (_shape) _shape->setPosition(_position);
        }
    }
    else
    {
        onDestroy();
    }
}

void Projectile::onHit()
{
    if (_isAlive)
    {
        if (auto targetPtr = target.lock())
        {
            targetPtr->takeDamage(damage);
        }
        onDestroy();
    }
}

void Projectile::onDestroy() {
    if (_isAlive)
    {
        Entity::setIsAlive(false);
    }
}