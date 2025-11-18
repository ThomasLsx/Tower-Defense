#include "tower.h"
#include "projectilesystem.h"
#include "minion.h"
#include <iostream>
#include <memory>
#include <cmath>

Tower::Tower(unsigned int id, float range, float fireRate, unsigned int level, unsigned int damage, sf::Vector2f pos, sf::Color color)
    : Entity(id),
    range(range),
    fireRate(fireRate),
    fireCooldown(0.0f),
    level(level),
    damage(damage)
{
    Entity::init(20, 20,color);
    Entity::setPosition(pos);
}

void Tower::tryFire(ProjectileSystem& projectileSystem) {
    for (auto it = targets.begin(); it != targets.end();)
    {
        if (auto targetPtr = it->lock())
        {
            // SUCCÈS: Le minion (targetPtr) existe toujours
            projectileSystem.createProjectile(*this, targetPtr, damage, 300.0f); // 300.0f = vitesse projectile
            std::cout << "Tower " << Entity::getId() << " fires at minion " << targetPtr->getId() << std::endl;
            fireCooldown = fireRate; // Réinitialise le timer

            break;

            // ++it; Si on voulait tirer sur toutes les cibles, on ferait ++it
        }
        else
        {
            // ÉCHEC: Le minion n'existe plus (mort, etc.)
            it = targets.erase(it);
        }
    }
}

void Tower::upgrade() {
    level++;
    range *= 1.1f;
    damage = static_cast<unsigned int>(damage * 1.2f);
    std::cout << "Tower " << Entity::getId() << " upgraded to level " << level << " (Damage: " << damage << ")" << std::endl;
}

void Tower::onDestroy() {
    std::cout << "Tower " << Entity::getId() << " destroyed!" << std::endl;
    Entity::setIsAlive(false);
}


void Tower::update(float dt, const std::vector<std::shared_ptr<Minion>>& minions, ProjectileSystem& projectileSystem)
{
    // Met à jour le cooldown
    if (fireCooldown > 0) {
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

    // TODO: Trier les cibles (ex: la plus proche, la plus faible, etc.)
}

void Tower::update(float dt)
{
    if (fireCooldown > 0) {
        fireCooldown -= dt;
    }
}

// ==========================================================
// 1. BASIC TOWER (Tour de base)
// ==========================================================

BasicTower::BasicTower(unsigned int id, sf::Vector2f pos)
// Initialisation : Portée moyenne, cadence moyenne, dégâts moyens, couleur neutre
    : Tower(id, 250.0f, 1.0f, 1, 15, pos, sf::Color(100, 100, 100))
{
}

void BasicTower::upgrade()
{
    // Amélioration standard : équilibrée
    level++;
    range *= 1.10f; // +10% de portée
    damage += 10;   // +10 dégâts
    fireRate *= 1.10f; // +10% de cadence
    std::cout << "BasicTower upgraded to level " << level << std::endl;
}


// ==========================================================
// 2. SPEED TOWER (Tour rapide)
// ==========================================================

SpeedTower::SpeedTower(unsigned int id, sf::Vector2f pos)
// Initialisation : Portée courte, cadence ÉLEVÉE, dégâts FAIBLES, couleur verte
    : Tower(id, 200.0f, 0.1f, 1, 5, pos, sf::Color(0, 200, 0))
{
    // Logique de construction spécifique
}

void SpeedTower::upgrade()
{
    // Amélioration centrée sur la cadence de tir
    level++;
    range *= 1.05f; // Petite augmentation de portée
    damage += 3;    // Très petite augmentation de dégâts
    fireRate *= 1.20f; // Grande augmentation de cadence (+20%)
    std::cout << "SpeedTower upgraded to level " << level << std::endl;
}


// ==========================================================
// 3. SNIPER TOWER (Tour de précision)
// ==========================================================

SniperTower::SniperTower(unsigned int id, sf::Vector2f pos)
// Initialisation : Portée TRÈS longue, cadence BASSE, dégâts TRÈS élevés, couleur bleue
    : Tower(id, 450.0f, 0.4f, 1, 50, pos, sf::Color(0, 0, 255))
{
}

void SniperTower::upgrade()
{
    // Amélioration centrée sur la portée et les dégâts
    level++;
    range += 75.0f; // Grosse augmentation de portée
    damage += 35;   // Grosse augmentation de dégâts
    fireRate *= 1.05f; // Très petite augmentation de cadence
    std::cout << "SniperTower upgraded to level " << level << std::endl;
}


// ==========================================================
// 4. SLOW TOWER (Tour de ralentissement)
// ==========================================================

SlowTower::SlowTower(unsigned int id, sf::Vector2f pos)
// Initialisation : Portée moyenne, cadence moyenne/faible, dégâts TRÈS faibles, couleur cyan
    : Tower(id, 220.0f, 0.7f, 1, 2, pos, sf::Color(0, 255, 255))
{
    // Logique de construction spécifique
}

void SlowTower::upgrade()
{
    // Amélioration centrée sur la fréquence de ralentissement (cadence de tir)
    // et la zone d'effet (portée).
    level++;
    range *= 1.15f; // Augmentation significative de portée
    damage += 1;    // Le dégât reste négligeable
    fireRate *= 1.15f; // Augmentation significative de cadence

    // Note: L'effet de ralentissement lui-même devrait être amélioré ici,
    // mais cela dépend de votre implémentation de l'effet dans ProjectileSystem/Minion.

    std::cout << "SlowTower upgraded to level " << level << std::endl;
}