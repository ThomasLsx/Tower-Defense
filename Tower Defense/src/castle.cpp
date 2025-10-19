#include "castle.h"
#include <iostream>

Castle::Castle(unsigned int id, sf::Vector2f pos, float rotation, sf::Color color, unsigned int maxHealth)
	:Entity(id), health(maxHealth), maxHealth(maxHealth)
{
	Entity::init();
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
	if (Entity::getIsAlive())
	{
		health += amount;
		if (health > maxHealth) health = maxHealth;
	}
}

void Castle::onDestroy()
{
	Entity::setIsAlive(false);
	std::cout << "GAME OVER" << std::endl;
	std::cout << "Castle " << Entity::getId() << " destroyed!" << std::endl;
}

void Castle::update(float dt)
{
	// Ajoute ici la logique d'update du château si besoin
}
