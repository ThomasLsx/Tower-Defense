#include "entity.h"

Entity::Entity(float x, float y, const sf::Texture& texture)
{
    position.x = x;
    position.y = y;
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setPosition(position);
}

Entity::Entity() : position(0, 0), sprite(nullptr)
{
    // Le sprite doit être initialisé plus tard si utilisé
}

void Entity::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
    if (sprite) sprite->setPosition(position);
}

sf::Vector2f Entity::getPosition() const
{
    return position;
}

void Entity::draw(sf::RenderWindow& window)
{
    if (sprite) window.draw(*sprite);
}
