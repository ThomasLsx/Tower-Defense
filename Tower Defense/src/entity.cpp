/* MIT License

Copyright (c) 2022 Pierre Lefebvre

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

/// entity.cpp
#include "entity.h"
#include <cmath>

Entity::Entity(unsigned int id)
    : _shape(), _position(0.f, 0.f), _velocity(1.f, 1.f), _acceleration(1.f, 1.f),
    _area(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)), _squaredRadius(0), _isAlive(true), _id(id) {
}

void Entity::init(int radius, const sf::Color& color, const sf::Color& outline, int thickness) {
    _shape.setRadius(radius);
    _shape.setFillColor(color);
    _shape.setOutlineColor(outline);
    _shape.setOutlineThickness(thickness);
    _shape.setOrigin(sf::Vector2f(radius, radius));
    _squaredRadius = radius * radius;
}

void Entity::update(float dt) {
    // Mise à jour de la position en fonction de la vélocité
    _position += _velocity * dt;
    _shape.setPosition(_position);

    // Rebond sur les bords de l'aire de jeu
    if (_position.x < _area.position.x) {
        _position.x = _area.position.x;
        _velocity.x = -_velocity.x * 0.9f; // Rebond avec un peu de frottement
    }
    else if (_position.x > _area.position.x + _area.size.x) {
        _position.x = _area.position.x + _area.size.x;
        _velocity.x = -_velocity.x * 0.9f;
    }
    if (_position.y < _area.position.y) {
        _position.y = _area.position.y;
        _velocity.y = -_velocity.y * 0.9f;
    }
    else if (_position.y > _area.position.y + _area.size.y) {
        _position.y = _area.position.y + _area.size.y;
        _velocity.y = -_velocity.y * 0.9f;
    }
}

void Entity::draw(sf::RenderWindow& window)
{
    if (getIsAlive())
    window.draw(_shape);
}

bool Entity::isColliding(const Entity& e) const {
    float dx = _position.x - e._position.x;
    float dy = _position.y - e._position.y;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = _shape.getRadius() + e._shape.getRadius();
    return distanceSquared < radiusSum * radiusSum;
}

void Entity::bounce(Entity& e) {
    _velocity = sf::Vector2f(0, 0);
    e._velocity = sf::Vector2f(0, 0);
}
