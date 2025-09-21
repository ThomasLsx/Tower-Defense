#pragma once

#include "entity.h"

class Projectile : public Entity {
private:
	float damage;

	//path finding ? type Path ou direction + vit ?
	sf::Vector2f direction;
	float speed;

public:
	Projectile(int x, int y, float damage, float speed, sf::Vector2f direction, const sf::Texture& texture);

	Projectile(void) = default;
	~Projectile(void) override = default;

	float getDamage(void);
	void setDamage(float damage);

	sf::Vector2f getDirection(void);
	void setDirection(sf::Vector2f direction);

	float getSpeed(void);
	void setSpeed(float speed);
};