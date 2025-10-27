
#include "tower.h"
#include "projectilesystem.h"
#include "minion.h"
#include <iostream>

Tower::Tower(unsigned int id, float range, float fireRate, unsigned int level, unsigned int damage, sf::Vector2f pos, float rotation, sf::Color color)
    : Entity(id),
      range(range),
      fireRate(fireRate),
      level(level),
      damage(damage) 
{
    Entity::init();
}

void Tower::tryFire(const std::vector<Entity*>& minions, ProjectileSystem& projectileSystem) {
    for (int targetId : targetIds) {
        projectileSystem.createProjectile(Entity::getId(), targetId, damage, 10.0f);
        std::cout << "Tower " << Entity::getId() << " fires at minion " << targetId << std::endl;
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

// La méthode update de la base Entity ne correspond pas à celle de Tower
// On ajoute une surcharge non override pour la version Tower
void Tower::update(float dt, const std::vector<Entity*>& minions, ProjectileSystem& projectileSystem)
{
    tryFire(minions, projectileSystem);
}

void Tower::SearchTargets(std::vector<Minion> mimi)
{
    for (auto it = mimi.begin(); it != mimi.end(); ++it)
    {
        sf::Vector2f direction = it->getPosition() - _position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance <= range)
            targetIds.push_back(it->getId());
    }
}

// Ajout d'une version override de update pour la base
void Tower::update(float dt)
{
    // Optionnel : logique de base si besoin
}

// Ajout d'une version override de update pour la base
void Tower::update(float dt)
{
    // Optionnel : logique de base si besoin
}
