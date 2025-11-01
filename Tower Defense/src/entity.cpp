/// entity.cpp
#include "entity.h"
#include <cmath>
#include "utils.h"

/**
 * @brief Constructeur d'Entity
 */
Entity::Entity(unsigned int id, sf::Vector2f position)
    :_position(position), _isAlive(true), _id(id)
{
}

/**
 * @brief Initialise une forme circulaire.
 */
void Entity::init(int radius, const sf::Color& color, const sf::Color& outline, int thickness)
{
    _shape = new sf::CircleShape(radius);
    _shape->setPosition(_position);
    _shape->setFillColor(color);
    _shape->setOutlineColor(outline);
    _shape->setOutlineThickness(thickness);
    _shape->setOrigin(sf::Vector2f(radius, radius));
    _squaredRadius = radius * radius;
}

/**
 * @brief Initialise une forme rectangulaire.
 */
void Entity::init(int l, int L, const sf::Color& color, const sf::Color& outline, int thickness)
{
    _shape = new sf::RectangleShape(sf::Vector2f(l,L));
    _shape->setFillColor(color);
    _shape->setOutlineColor(outline);
    _shape->setOutlineThickness(thickness);
    _shape->setOrigin(sf::Vector2f(l / 2, L / 2));
    _squaredRadius = (l / 2) * (l / 2) + (L / 2) * (L / 2);
}

/**
 * @brief Dessine l'entité si elle est vivante.
 */
void Entity::draw(sf::RenderWindow& window)
{
    if (getIsAlive())
        window.draw(*_shape);
}

/**
 * @brief Collision AABB entre deux entités.
 */
bool Entity::isColliding(const Entity& e) const {
    const sf::FloatRect& boundsA = _shape->getGlobalBounds();
    const sf::FloatRect& boundsB = e._shape->getGlobalBounds();
    return intersects(boundsA, boundsB);
}
