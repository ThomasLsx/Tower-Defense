#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

class Tower;
class Minion;
class ProjectileSystem; 
class EconomySystem;

class TowerManager
{
public:
	enum class TowerType
	{
		Basic,
		Sniper,
		Speed,
		Slow
	};

	TowerManager() = default;
	~TowerManager() = default;
	void addTower(const sf::Vector2f& position, int type);
	void updateTowers(const std::vector<std::shared_ptr<Minion>>& minions, float deltaTime, ProjectileSystem& projectileSystem);
	void drawTowers(sf::RenderWindow& window) const;
	void removeTowerAt(unsigned int i, unsigned int j, sf::Vector2u tileSize, float scale, EconomySystem& economySystem);
	bool isTowerAt(unsigned int i, unsigned int j, sf::Vector2u tileSize, float scale) const;
	Tower* getTowerAtPosition(const sf::Vector2f& position, sf::Vector2u tileSize, float scale) const;

	const std::vector<std::unique_ptr<Tower>>& getTowers() const { return towers; }

private:
	std::vector<std::unique_ptr<Tower>> towers;

	unsigned int towerIDCounter = 0;
};
