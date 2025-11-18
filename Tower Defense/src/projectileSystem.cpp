// projectileSystem.cpp
#include "projectilesystem.h"
#include "projectile.h"
#include "entity.h"
#include "tower.h"
#include "minion.h" 

#include <memory>
#include <algorithm>
#include <iostream>

ProjectileSystem::ProjectileSystem() = default;
ProjectileSystem::~ProjectileSystem() = default;

void ProjectileSystem::createProjectile(const Tower& source, std::shared_ptr<Minion> target, int dmg, float speed) {
    auto proj = std::make_shared<Projectile>(
        static_cast<unsigned int>(projectiles.size()), // ID (simple)
        source.getId(),
        target,
        sf::Vector2f(speed, 0.0f),
        dmg,
        2.0f,
        source.getPosition()
    );

    projectiles.emplace_back(proj);
}

void ProjectileSystem::update(float dt) {
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        Projectile& projectile = **it;

        projectile.update(dt);

        if (!projectile.getIsAlive()) {
            it = projectiles.erase(it);
            continue;
        }

        if (auto targetPtr = projectile.getTarget().lock())
        {
            if (projectile.isColliding(*targetPtr))
            {
                projectile.onHit();
                it = projectiles.erase(it);
            }
            else {
                ++it;
            }
        }
        else
        {
            ++it;
        }
    }
}

void ProjectileSystem::draw(sf::RenderWindow& window)
{
    for (const auto& projectile : projectiles)
    {
        projectile->draw(window);
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