#include "entity.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

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