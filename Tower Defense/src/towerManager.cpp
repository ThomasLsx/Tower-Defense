#include <iostream>
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

void TowerManager::removeTowerAt(unsigned int i, unsigned int j, sf::Vector2u tileSize, float scale)
{
    towers.erase(
        std::remove_if(towers.begin(), towers.end(),
            [i, j, tileSize, scale](const std::unique_ptr<Tower>& tower) {
                sf::Vector2f pos = tower->getPosition();
                unsigned int ti = static_cast<unsigned int>(pos.x / (tileSize.x * scale));
                unsigned int tj = static_cast<unsigned int>(pos.y / (tileSize.y * scale));
                return ti == i && tj == j;
            }),
        towers.end());
}

bool TowerManager::isTowerAt(unsigned int i, unsigned int j, sf::Vector2u tileSize, float scale) const {
    for (const auto& tower : towers) {
        sf::Vector2f pos = tower->getPosition();
        unsigned int ti = static_cast<unsigned int>(pos.x / (tileSize.x * scale));
        unsigned int tj = static_cast<unsigned int>(pos.y / (tileSize.y * scale));
        if (ti == i && tj == j)
            return true;
    }
    return false;
}

Tower* TowerManager::getTowerAtPosition(const sf::Vector2f& position, sf::Vector2u tileSize, float scale) const
{
    unsigned int i = static_cast<unsigned int>(position.x / (tileSize.x * scale));
    unsigned int j = static_cast<unsigned int>(position.y / (tileSize.y * scale));
    for (const auto& tower : towers) {
        sf::Vector2f pos = tower->getPosition();
        unsigned int ti = static_cast<unsigned int>(pos.x / (tileSize.x * scale));
        unsigned int tj = static_cast<unsigned int>(pos.y / (tileSize.y * scale));
        if (ti == i && tj == j) {
            return tower.get();
        }
    }
    return nullptr;
}
