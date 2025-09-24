#pragma once
#include <utility>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.inl>

#include <iostream>

class Entity {
private:
    int id;
	sf::Sprite sprite;
	bool isAlive;

public:
    Entity(int id, sf::Vector2f pos = sf::Vector2f(0.0f,0.0f), float rotation = 0.0f, sf::Color color = sf::Color::White, sf::IntRect textureRect = sf::IntRect(), const sf::Texture& texture = sf::Texture());
    virtual ~Entity() = default;

	sf::Vector2f getPosition() const { return sprite.getPosition(); }
	void setPosition(const sf::Vector2f& p) { sprite.setPosition(p); }
	sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }

	sf::Angle getRotation() const { return sprite.getRotation(); }
	void setRotation(const sf::Angle& a) { sprite.setRotation(a); }
	void setRotation(const float angle) { sprite.setRotation(sf::Angle(sf::degrees(angle))); }

    int getID() const { return id; }

	bool getisAlive(void) const { return isAlive; }
	void setisAlive(bool val) { isAlive = val; }

    virtual void onDestroy() = 0;
	virtual void update(float dt) = 0;
};