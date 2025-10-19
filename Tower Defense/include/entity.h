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
///entity.h
#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "utils.h"

class Entity {
public:
    /**
     * Constructor
     */
    Entity(unsigned int id = 0);

    virtual ~Entity() = default;
    virtual void onDestroy() {}

    /**
     * Set visual properties
     * @param radius in pixel
     * @param fill color
     * @param outline color
     * @param outline thickness
     */
    void init(int radius = 50, const sf::Color& color = sf::Color::Red , const sf::Color& outline = sf::Color::Black, int thickness = 1);

    /**
     * Upadte object at each frame
     * @param time since last update
     */
    virtual void update(float dt);

    /**
     * Render object
     * @param the window to draw to
	 */
    void draw(sf::RenderWindow& window);

    /**
     * Getter for position
     * @return object position
     */
    inline const sf::Vector2f& getPosition() const {
        return _position;
    }

    /**
     * Setter for position
     * @param new position
     */
    inline void setPosition(const sf::Vector2f& position) {
        _position = position;
    }

    /**
     * Getter for ID
     * @return object ID
     */
    inline unsigned int getId() const {
        return _id;
	}

    /**
     * Collision detection
     * @param the other entity
     * @return true if it's colliding with the other entity
     */
    bool isColliding(const Entity& e) const;

    /**
     * Revert velocity according to angle of collision
     * @param the other entity
     */
    void bounce(Entity& e);

    /**
     * Getter for shape
     * @return object acceleration
     */
    inline const sf::Drawable& getShape() const {
        return _shape;
    }

    /**
     * Move particle
     * @param velocity
     */
    inline void move(const sf::Vector2f& v) {
        _velocity = v;
    }

    /**
     * Define external bounds for the object
     * @param bounds coords
     */
    inline void setPlayableArea(const sf::FloatRect& area) {
        _area = area;
    }

    inline void setIsAlive(bool alive) {
        _isAlive = alive;
    }

    inline bool getIsAlive() {
        return _isAlive;
    }

    inline sf::FloatRect getGlobalBounds() const {
        return _shape.getGlobalBounds();
	}

    
protected:
    sf::CircleShape _shape;
    sf::Vector2f _position;
    sf::Vector2f _velocity;
    sf::Vector2f _acceleration;
    sf::FloatRect _area;
    unsigned int _squaredRadius;

    bool _isAlive;

	unsigned int _id;
};

#endif
