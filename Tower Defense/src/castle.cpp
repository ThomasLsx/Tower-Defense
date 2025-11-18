#include "castle.h"
#include <iostream>

/**
 * @brief Constructeur de Castle
 *
 * Initialise le château avec une position et des points de vie.
 */
Castle::Castle(TileMap* map, unsigned int maxHealth)
	:map(map), health(maxHealth), maxHealth(maxHealth), destroyed(false)
{
	setCastleTile();
}

/**
 * @brief Inflige des dégâts au château.
 */
void Castle::takeDamage(unsigned int amount)
{
	if (amount <= health) health -= amount;
	if (health == 0) destroy(); 
	std::cout << "Castle health: " << health << std::endl;

}

/**
 * @brief Dessine la barre de vie du château.
 */
void Castle::draw(sf::RenderWindow& window)
{
	// Dessiner l'arrière-plan de la barre de vie
	healthBarBack = sf::RectangleShape(sf::Vector2f(70.f, 10.f));
	healthBarBack.setFillColor(sf::Color::Transparent);
	healthBarBack.setOutlineColor(sf::Color::Black);
	healthBarBack.setOutlineThickness(2.f);
	healthBarBack.setOrigin(sf::Vector2f(35.f, 15.f + map->getHeight()));
	healthBarBack.setPosition(map->Tile2Position(castleTile));

	window.draw(healthBarBack);

	// Dessiner la barre de vie
	healthBar = sf::RectangleShape(sf::Vector2f(70.f, 10.f));
	healthBar.setFillColor(sf::Color::Red);
	healthBar.setOrigin(sf::Vector2f(35.f, 15.f + map->getHeight()));
	healthBar.setPosition(map->Tile2Position(castleTile));
	float healthPercent = static_cast<float>(health) / static_cast<float>(maxHealth);
	healthBar.setSize(sf::Vector2f(70.f * healthPercent, 10.f));

	window.draw(healthBar);
}

/**
 * @brief Appelé lorsque le château est détruit.
 */
void Castle::destroy()
{
	destroyed = true;
	std::cout << "GAME OVER" << std::endl;
}

void Castle::setCastleTile()
{
	castleTile = map->getCastleTile();
}


