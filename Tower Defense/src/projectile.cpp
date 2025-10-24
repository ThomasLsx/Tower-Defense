#include "projectile.h"
#include <iostream>

Projectile::Projectile(unsigned int id, unsigned int sourceTowerId, unsigned int targetEntityId, sf::Vector2f velocity, unsigned int damage, float lifetime, sf::Vector2f pos, float rotation, sf::Color color)
    :Entity(id), velocity(velocity), damage(damage), lifetime(lifetime), sourceTowerId(sourceTowerId), targetEntityId(targetEntityId)
{
    Entity::init();
}

void Projectile::update(float dt) {
    // Met ? jour la position du projectile en fonction de sa vitesse
    sf::Vector2f position;
    Entity::setPosition(position += velocity * dt);

    // D?cr?mente la dur?e de vie
    lifetime -= dt;

    if (lifetime <= 0.0f) {
        onDestroy();
    }
}

void Projectile::onHit()
{
    onDestroy();
}

void Projectile::onDestroy() {
    // Logique de destruction du projectile
    std::cout << "Projectile " << Entity::getId() << " d?truit (fin de vie ou collision)." << std::endl;
    // Marquer le projectile comme inactif ou le supprimer de la liste des projectiles actifs
    // (cette partie est g?n?ralement g?r?e par le ProjectileSystem)
}