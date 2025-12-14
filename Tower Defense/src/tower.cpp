#include "tower.h"

#include <cmath>
#include <iostream>
#include <memory>

#include "Game.h"
#include "minion.h"
#include "projectilesystem.h"

extern Game* g_game_instance;

Tower::Tower(unsigned int id, float range, float fireRate, unsigned int level, unsigned int damage,
    unsigned int costCopper, unsigned int costSilver, unsigned int costGold,
    sf::Vector2f pos, sf::Color color)
    : Entity(id),
    range(range),
    fireRate(fireRate),
    fireCooldown(0.0f),
    level(level),
    damage(damage),
    costCopper(costCopper),
    costSilver(costSilver),
    costGold(costGold)
{
    this->init(20, 20, color);
    this->setPosition(pos);
}

void Tower::update(float dt)
{
}

void Tower::tryFire(ProjectileSystem& projectileSystem)
{
    for (auto it = targets.begin(); it != targets.end();)
    {
        if (auto targetPtr = it->lock())
        {
            // Le minion (targetPtr) existe toujours
            projectileSystem.createProjectile(*this, targetPtr, damage, 300.0f);

            fireCooldown = fireRate;

            attackSpecialEffect(*it);

            break;
        }
        else
        {
            // Le minion n'existe plus (mort, etc.)
            it = targets.erase(it);
        }
    }
}

void Tower::upgrade()
{
    if (!g_game_instance)
    {
        return;
    }

    EconomySystem* eco = g_game_instance->getEconomySystem();

    // Prix d'upgrade = 1.5x le prix de base
    unsigned int upgradeCopper = static_cast<unsigned int>(costCopper * 1.5f);
    unsigned int upgradeSilver = static_cast<unsigned int>(costSilver * 1.5f);
    unsigned int upgradeGold = static_cast<unsigned int>(costGold * 1.5f);

    if (eco->getCopper() < (int)upgradeCopper || eco->getSilver() < (int)upgradeSilver || eco->getGold() < (int)upgradeGold)
    {
        std::cout << "Pas assez de ressources pour améliorer cette tour !\n";
        return;
    }

    eco->spendCopper(upgradeCopper);
    eco->spendSilver(upgradeSilver);
    eco->spendGold(upgradeGold);

    level++;
    range *= 1.1f;
    damage = static_cast<unsigned int>(damage * 1.2f);

    std::cout << "Tower " << Entity::getId() << " upgraded to level " << level << " (Damage: " << damage << ")" << std::endl;
}

void Tower::onDestroy()
{
    std::cout << "Tower " << Entity::getId() << " destroyed!" << std::endl;
    Entity::setIsAlive(false);
}

void Tower::update(float dt, const std::vector<std::shared_ptr<Minion>>& minions, ProjectileSystem& projectileSystem)
{
    if (fireCooldown > 0)
    {
        fireCooldown -= dt;
    }

    // 1. Chercher des cibles
    SearchTargets(minions);

    // 2. Si on peut tirer et qu'on a des cibles, tirer.
    if (fireCooldown <= 0 && !targets.empty())
    {
        tryFire(projectileSystem);
    }
}

void Tower::SearchTargets(const std::vector<std::shared_ptr<Minion>>& allMinions)
{
    targets.clear();

    for (const auto& minionPtr : allMinions)
    {
        sf::Vector2f direction = minionPtr->getPosition() - _position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance <= range)
        {
            targets.push_back(minionPtr);
        }
    }
}

// ==========================================================
// 1. BASIC TOWER (Tour de base)
// ==========================================================

BasicTower::BasicTower(unsigned int id, sf::Vector2f pos)
    : Tower(id, 250.0f, 1.0f, 1, 15, 30, 0, 0, pos, sf::Color(100, 100, 100))
{
}

void BasicTower::upgrade()
{
    if (!g_game_instance)
    {
        return;
    }

    EconomySystem* eco = g_game_instance->getEconomySystem();

    unsigned int upgradeCopper = static_cast<unsigned int>(costCopper * 1.5f);
    unsigned int upgradeSilver = static_cast<unsigned int>(costSilver * 1.5f);
    unsigned int upgradeGold = static_cast<unsigned int>(costGold * 1.5f);

    if (eco->getCopper() < (int)upgradeCopper || eco->getSilver() < (int)upgradeSilver || eco->getGold() < (int)upgradeGold)
    {
        std::cout << "Pas assez de ressources pour améliorer cette tour !\n";
        return;
    }

    eco->spendCopper(upgradeCopper);
    eco->spendSilver(upgradeSilver);
    eco->spendGold(upgradeGold);

    level++;
    range *= 1.10f;
    damage += 10;
    fireRate *= 1.10f;

    std::cout << "BasicTower upgraded to level " << level << std::endl;
}

// ==========================================================
// 2. SPEED TOWER (Tour rapide)
// ==========================================================

SpeedTower::SpeedTower(unsigned int id, sf::Vector2f pos)
    : Tower(id, 200.0f, 0.1f, 1, 5, 20, 5, 0, pos, sf::Color(0, 200, 0))
{
}

void SpeedTower::upgrade()
{
    if (!g_game_instance)
    {
        return;
    }

    EconomySystem* eco = g_game_instance->getEconomySystem();

    unsigned int upgradeCopper = static_cast<unsigned int>(costCopper * 1.5f);
    unsigned int upgradeSilver = static_cast<unsigned int>(costSilver * 1.5f);
    unsigned int upgradeGold = static_cast<unsigned int>(costGold * 1.5f);

    if (eco->getCopper() < (int)upgradeCopper || eco->getSilver() < (int)upgradeSilver || eco->getGold() < (int)upgradeGold)
    {
        std::cout << "Pas assez de ressources pour améliorer cette tour !\n";
        return;
    }

    eco->spendCopper(upgradeCopper);
    eco->spendSilver(upgradeSilver);
    eco->spendGold(upgradeGold);

    level++;
    range *= 1.05f;
    damage += 3;
    fireRate *= 1.20f;

    std::cout << "SpeedTower upgraded to level " << level << std::endl;
}

// ==========================================================
// 3. SNIPER TOWER (Tour de précision)
// ==========================================================

SniperTower::SniperTower(unsigned int id, sf::Vector2f pos)
    : Tower(id, 450.0f, 0.4f, 1, 50, 10, 10, 0, pos, sf::Color(0, 0, 255))
{
}

void SniperTower::upgrade()
{
    if (!g_game_instance)
    {
        return;
    }

    EconomySystem* eco = g_game_instance->getEconomySystem();

    unsigned int upgradeCopper = static_cast<unsigned int>(costCopper * 1.5f);
    unsigned int upgradeSilver = static_cast<unsigned int>(costSilver * 1.5f);
    unsigned int upgradeGold = static_cast<unsigned int>(costGold * 1.5f);

    if (eco->getCopper() < (int)upgradeCopper || eco->getSilver() < (int)upgradeSilver || eco->getGold() < (int)upgradeGold)
    {
        std::cout << "Pas assez de ressources pour améliorer cette tour !\n";
        return;
    }

    eco->spendCopper(upgradeCopper);
    eco->spendSilver(upgradeSilver);
    eco->spendGold(upgradeGold);

    level++;
    range += 75.0f;
    damage += 35;
    fireRate *= 1.05f;

    std::cout << "SniperTower upgraded to level " << level << std::endl;
}

// ==========================================================
// 4. SLOW TOWER (Tour de ralentissement)
// ==========================================================

SlowTower::SlowTower(unsigned int id, sf::Vector2f pos)
    : Tower(id, 220.0f, 0.7f, 1, 2, 10, 0, 2, pos, sf::Color(0, 255, 255)),
    slowEffect(0.3f)
{
}

void SlowTower::upgrade()
{
    if (!g_game_instance)
    {
        return;
    }

    EconomySystem* eco = g_game_instance->getEconomySystem();

    unsigned int upgradeCopper = static_cast<unsigned int>(costCopper * 1.5f);
    unsigned int upgradeSilver = static_cast<unsigned int>(costSilver * 1.5f);
    unsigned int upgradeGold = static_cast<unsigned int>(costGold * 1.5f);

    if (eco->getCopper() < (int)upgradeCopper || eco->getSilver() < (int)upgradeSilver || eco->getGold() < (int)upgradeGold)
    {
        std::cout << "Pas assez de ressources pour améliorer cette tour !\n";
        return;
    }

    eco->spendCopper(upgradeCopper);
    eco->spendSilver(upgradeSilver);
    eco->spendGold(upgradeGold);

    level++;
    range *= 1.15f;
    damage += 1;
    fireRate *= 1.15f;

    std::cout << "SlowTower upgraded to level " << level << std::endl;
}

void SlowTower::attackSpecialEffect(std::weak_ptr<Minion> target)
{
    if (auto targetPtr = target.lock())
    {
        if (targetPtr->getSpecialStateTimer() <= 0.0f)
        {
            // Appliquer l'effet de ralentissement au minion
            float originalSpeed = targetPtr->getSpeed();
            float newSpeed = originalSpeed * (1.0f - slowEffect);

            targetPtr->setSpeed(newSpeed);
            targetPtr->setSpecialStateTimer(5.0f); // Ralentissement pendant 5 secondes
        }
        else
        {
            // Si le minion est déjà ralenti, on réinitialise juste le timer
            targetPtr->setSpecialStateTimer(5.0f);
        }
    }
}