#include "entity.h"

Entity::Entity(float x, float y, const sf::Texture& texture)
{
	position.x = x;
	position.y = y;
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

Entity::Entity() : position(0, 0)
{
}

void Entity::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
	sprite.setPosition(position);
}

sf::Vector2f Entity::getPosition() const
{
	return position;
}
