#include "projectilesystem.h"
#include "projectile.h"
#include "entity.h"
#include <algorithm>
#include <iostream>

ProjectileSystem::ProjectileSystem() = default;
ProjectileSystem::~ProjectileSystem() = default;

void ProjectileSystem::createProjectile(int sourceId, int targetId, int dmg, float speed) {
    projectiles.emplace_back(std::make_unique<Projectile>(projectiles.size(), sourceId, targetId, sf::Vector2f(speed, 0.0f), dmg, 20.0f));
}

void ProjectileSystem::update(float dt, const std::vector<Entity*>& entities) {
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        Projectile& projectile = **it;
        projectile.update(dt);

        if (projectile.hasReachedTarget() || projectile.isExpired()) {
            it = projectiles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ProjectileSystem::removeProjectile(int id) {
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [id](const std::unique_ptr<Projectile>& p) { return p->getId() == id; }),
        projectiles.end());
}

int ProjectileSystem::getActiveCount() const {
    return static_cast<int>(projectiles.size());
}