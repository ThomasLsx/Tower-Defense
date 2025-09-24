#include "entity.h"


Entity::Entity(int id, sf::Vector2f pos, float rotation, sf::Color color, sf::IntRect textureRect, const sf::Texture& texture)
    : id(id), sprite(texture), isAlive(true)
{
    sprite.setPosition(pos);
    sprite.setRotation(sf::Angle(sf::degrees(rotation)));
    sprite.setColor(color);
    if (textureRect != sf::IntRect())
        sprite.setTextureRect(textureRect);
 }

void Entity::draw(sf::RenderWindow& window)
{
    if (sprite) window.draw(*sprite);
}
