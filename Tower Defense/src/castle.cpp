#include "castle.h"
#include <iostream>

/**
 * @brief Constructeur de Castle
 *
 * Initialise le château avec une position et des points de vie.
 */
Castle::Castle(unsigned int id, sf::Vector2f pos, sf::Color color, unsigned int maxHealth)
	:Entity(id, pos), health(maxHealth), maxHealth(maxHealth)
{
	Entity::init(5,5);
}

/**
 * @brief Inflige des dégâts au château.
 */
void Castle::takeDamage(unsigned int amount)
{
	if (amount < health) health -= amount;
	else
	{
		onDestroy();
	}
}

/**
 * @brief Répare le château si il est encore en vie.
 */
void Castle::repair(unsigned int amount)
{
	if (Entity::getIsAlive())
	{
		health += amount;
		if (health > maxHealth) health = maxHealth;
	}
}

/**
 * @brief Appelé lorsque le château est détruit.
 */
void Castle::onDestroy()
{
	Entity::setIsAlive(false);
	std::cout << "GAME OVER" << std::endl;
	std::cout << "Castle " << Entity::getId() << " destroyed!" << std::endl;
}

/**
 * @brief Mise à jour du château (placeholder pour logique future).
 */
void Castle::update(float dt)
{
	// Intentionally empty: extend with castle-specific logic if needed
}
