#include <iostream>
#include "towerManager.h"
#include "economySystem.h"
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

void TowerManager::removeTowerAt(unsigned int i, unsigned int j, sf::Vector2u tileSize, float scale, EconomySystem& economySystem)
{
    for (auto it = towers.begin(); it != towers.end(); ++it) {
        sf::Vector2f pos = (*it)->getPosition();
        unsigned int ti = static_cast<unsigned int>(pos.x / (tileSize.x * scale));
        unsigned int tj = static_cast<unsigned int>(pos.y / (tileSize.y * scale));
        if (ti == i && tj == j) {
            // --- Remboursement de 70% de la valeur totale (base + upgrades) ---
            unsigned int baseCopper = (*it)->getCostCopper();
            unsigned int baseSilver = (*it)->getCostSilver();
            unsigned int baseGold   = (*it)->getCostGold();
            unsigned int nbUpgrades = (*it)->getLevel() - 1;
            unsigned int upgradeCopper = static_cast<unsigned int>(baseCopper * 1.5f) * nbUpgrades;
            unsigned int upgradeSilver = static_cast<unsigned int>(baseSilver * 1.5f) * nbUpgrades;
            unsigned int upgradeGold   = static_cast<unsigned int>(baseGold * 1.5f) * nbUpgrades;
            unsigned int totalCopper = baseCopper + upgradeCopper;
            unsigned int totalSilver = baseSilver + upgradeSilver;
            unsigned int totalGold   = baseGold + upgradeGold;
            
            economySystem.addCopper(static_cast<int>(totalCopper * 0.7f));
            economySystem.addSilver(static_cast<int>(totalSilver * 0.7f));
            economySystem.addGold(static_cast<int>(totalGold * 0.7f));

            towers.erase(it);
            return;
        }
    }
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
