#include "towerManager.h"

#include "tower.h"
#include "projectileSystem.h"

void TowerManager::addTower(const sf::Vector2f& position, int type)
{
	switch (static_cast<TowerType>(type))
	{
	case TowerType::Basic:
		towers.push_back(std::make_unique<BasicTower>(towerIDCounter, position));
		break;
	case TowerType::Sniper:
		towers.push_back(std::make_unique<SniperTower>(towerIDCounter, position));
		break;
	case TowerType::Speed:
		towers.push_back(std::make_unique<SpeedTower>(towerIDCounter, position));
		break;
	case TowerType::Slow:
		towers.push_back(std::make_unique<SlowTower>(towerIDCounter, position));
		break;
	default:
		break;
	}
}

void TowerManager::updateTowers(const std::vector<std::shared_ptr<Minion>>& minions, float deltaTime, ProjectileSystem& projectileSystem)
{
	for (auto& tower : towers)
	{
		tower->update(deltaTime, minions, projectileSystem);
	}
}

void TowerManager::drawTowers(sf::RenderWindow& window) const
{
	for (const auto& tower : towers)
	{
		tower->draw(window);
	}
}
