#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

class Tower;
class Minion;
class ProjectileSystem; // forward declaration added

class TowerManager
{
public:
	enum class TowerType
	{
		Basic,
		Sniper,
		Speed
	};

	TowerManager() = default;
	~TowerManager() = default;
	void addTower(const sf::Vector2f& position, int type);
	void updateTowers(const std::vector<std::shared_ptr<Minion>>& minions, float deltaTime, ProjectileSystem& projectileSystem);
	void drawTowers(sf::RenderWindow& window) const;

	const std::vector<std::unique_ptr<Tower>>& getTowers() const { return towers; }

private:
	std::vector<std::unique_ptr<Tower>> towers;

	unsigned int towerIDCounter = 0;
};
