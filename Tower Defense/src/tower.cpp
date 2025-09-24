#include "tower.h"
#include "projectilesystem.h"

Tower::Tower(unsigned int id, float range, float fireRate, unsigned int level, unsigned int damage, sf::Vector2f pos, float rotation, sf::Color color, sf::IntRect textureRect, const sf::Texture& texture)
    : Entity(id, pos, rotation, color, textureRect, texture),
      range(range),
      fireRate(fireRate),
      level(level),
      damage(damage) 
{
}

void Tower::tryFire(const std::vector<Entity*>& minions, ProjectileSystem& projectileSystem) {
    for (int targetId : targetIds) {
        projectileSystem.createProjectile(id, targetId, damage, 10.0f);
        std::cout << "Tower " << id << " fires at minion " << targetId << std::endl;
    }
}

void Tower::upgrade() {
    level++;
    range *= 1.1f;
    damage *= 1.2f;
    std::cout << "Tower " << Entity::getID() << " upgraded to level " << level << " (Damage: " << damage << ")" << std::endl;
}

void Tower::onDestroy() {
    std::cout << "Tower " << Entity::getID() << " destroyed!" << std::endl;
	Entity::setisAlive(false);
}

void Tower::update(float dt, const std::vector<Entity*>& minions, ProjectileSystem& projectileSystem) 
{
	tryFire(minions, projectileSystem); 
}

std::vector<int> Tower::SearchTargets(unsigned int maxTargets)
{
	//use projsys pour get targets
    return std::vector<int>();
}
