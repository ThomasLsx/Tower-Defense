#include "tower.h"
#include "projectilesystem.h"

#include <algorithm>
#include <cmath>
#include <iostream>

Tower::Tower(int id, Vector2 pos, float range, float fireRate, int damage, int level)
    : Entity(id, pos), range(range), fireRate(fireRate), damage(damage), level(level), fireAccumulator(0.0f) {
}

void Tower::findTargets(const std::vector<Entity*>& minions) {
    targetIds.clear();
    for (Entity* minion : minions) {
        float dx = minion->getPosition().x - position.x;
        float dy = minion->getPosition().y - position.y;
        float distSq = dx * dx + dy * dy;
        if (distSq <= range * range) {
            targetIds.push_back(minion->getID());
        }
    }

    /*
    TODO : optimiser avec quadtree
    */
}

void Tower::tryFire(float dt, const std::vector<Entity*>& minions, ProjectileSystem& projectileSystem) {
    fireAccumulator += dt;
    float fireInterval = 1.0f / fireRate;

    if (fireAccumulator >= fireInterval && !targetIds.empty()) {
        fireAccumulator = 0.0f;
        // Crée un projectile pour chaque cible
        for (int targetId : targetIds) {
            projectileSystem.createProjectile(id, targetId, damage, 10.0f); // 10.0f = vitesse du projectile
            std::cout << "Tower " << id << " fires at minion " << targetId << std::endl;
        }
    }
}

void Tower::upgrade() {
    level++;
    range *= 1.1f;
    damage *= 1.2f;
    std::cout << "Tower " << id << " upgraded to level " << level << " (Damage: " << damage << ")" << std::endl;
}

void Tower::onDestroy() {
    std::cout << "Tower " << id << " destroyed!" << std::endl;
}

void Tower::update(float dt, const std::vector<Entity*>& minions, ProjectileSystem& projectileSystem) {
    findTargets(minions);
    tryFire(dt, minions, projectileSystem);
}
