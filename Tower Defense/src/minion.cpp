#include "minion.h"
#include <iostream>

Minion::Minion(int id, Vector2 pos, Path* path, int health, Vector2 velocity, int reward)
    : Entity(id, pos), health(health), velocity(velocity), pathProgress(0.0f),
    targetPath(path), rewardOnDeath(reward), alive(true) {
}

void Minion::move(float dt) {
    position += velocity * dt;
}

void Minion::followPath() {
    if (targetPath && pathProgress < 1.0f) {
        Vector2 targetPos = targetPath->getPointAt(pathProgress);
        Vector2 direction = targetPos - position;
        if (direction.lengthSquared() < 0.1f) {
            pathProgress += 0.01f;
            if (pathProgress >= 1.0f) {
                pathProgress = 1.0f;
                onDestroy();
                return;
            }
            targetPos = targetPath->getPointAt(pathProgress);
            direction = targetPos - position;
        }
        /*TODO : créer fonction normalize*/
        direction.normalize();
        velocity = direction * 2.0f;
    }
}

void Minion::takeDamage(int amount, int sourceId) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        alive = false;
        onDestroy();
    }
}

bool Minion::isAlive() const {
    return alive;
}

void Minion::onDestroy() {
    alive = false;
}

void Minion::update(float dt) {
    if (alive) {
        followPath();
        move(dt);
    }
}
