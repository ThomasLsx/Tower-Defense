#include "castle.h"

Castle::Castle(int id, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), float rotation = 0.0f, sf::Color color = sf::Color::White, sf::IntRect textureRect = sf::IntRect(), const sf::Texture& texture = sf::Texture(), unsigned int maxHealth = 100)
	:Entity(id, pos, rotation, color, textureRect, texture), health(maxHealth), maxHealth(maxHealth)
{
}

void Castle::takeDamage(unsigned int amount)
{
	if (amount < health) health -= amount;
	else
	{
		onDestroy();
	}
}

void Castle::repair(unsigned int amount)
{
	if (isAlive())
	{
		health += amount;
		if (health > maxHealth) health = maxHealth;
	}
}

void Castle::onDestroy()
{
	setisAlive(false);
	std::cout << "GAME OVER" << std::endl;
	std::cout << "Castle " << getID() << " destroyed!" << std::endl;
}
