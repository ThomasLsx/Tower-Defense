#include "projectilesystem.h"
#include "projectile.h"
#include "entity.h"
#include "tower.h"

#include <memory>
#include <algorithm>
#include <iostream>

ProjectileSystem::ProjectileSystem() = default;
ProjectileSystem::~ProjectileSystem() = default;

void ProjectileSystem::createProjectile(Tower source, Minion target, int dmg, float speed) {
    projectiles.emplace_back(std::make_shared<Projectile>(projectiles.size(), source.getId(), target, sf::Vector2f(speed, 0.0f), dmg, 20, source.getPosition()));
	projectiles.back()->setPosition(source.getPosition());
}

void ProjectileSystem::update(float dt) {
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        Projectile& projectile = **it;
        projectile.update(dt);

        if (projectile.isColliding(projectile.getTarget()) || projectile.isExpired()) {
            projectile.onHit();
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
            [id](const std::shared_ptr<Projectile>& p) { return p->getId() == id; }),
        projectiles.end());
}

int ProjectileSystem::getActiveCount() const {
    return static_cast<int>(projectiles.size());
}