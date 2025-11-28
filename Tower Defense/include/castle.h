#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"
#include "economySystem.h"

class TileMap;
class EconomySystem;

class Castle {
private:
	unsigned int health;
	unsigned int maxHealth;
	TileMap* map;
	EconomySystem* economySystem;

	sf::RectangleShape healthBar;
	sf::RectangleShape healthBarBack;
	sf::Vector2u castleTile;

	bool destroyed;

public:
	Castle(TileMap* map = nullptr, EconomySystem* economySystem = nullptr, unsigned int maxHealth = 100);
	~Castle() = default;

	void takeDamage(unsigned int amount);
	void draw(sf::RenderWindow& window);

	inline bool isDefeated() const { return destroyed; }

	inline unsigned int getHealth() const { return health; }
	inline unsigned int getMaxHealth() const { return maxHealth; }

	void destroy();

	/* Add rececourse */
	void addResource(int copper, int silver, int gold);

private:
	void setCastleTile();

};