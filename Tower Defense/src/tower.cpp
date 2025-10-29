#include "tower.h"
#include "projectilesystem.h"
#include "minion.h"
#include <iostream>
#include <memory>

Tower::Tower(unsigned int id, float range, float fireRate, unsigned int level, unsigned int damage, sf::Vector2f pos, float rotation, sf::Color color)
    : Entity(id),
      range(range),
      fireRate(fireRate),
      level(level),
      damage(damage) 
{
    Entity::init();
}

void Tower::tryFire(ProjectileSystem& projectileSystem) {
    for (auto& target : targets) {
        projectileSystem.createProjectile(*this, target, damage, 10.0f);
        std::cout << "Tower " << Entity::getId() << " fires at minion " << target.getId() << std::endl;
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
	SearchTargets(std::vector<Minion>());
    tryFire(projectileSystem);
}

void Tower::SearchTargets(std::vector<Minion> mimi)
{
    for (auto it = mimi.begin(); it != mimi.end(); ++it)
    {
        sf::Vector2f direction = it->getPosition() - _position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance <= range)
            targets.push_back(*it);

    }
}

void Tower::update(float dt)
{
}
