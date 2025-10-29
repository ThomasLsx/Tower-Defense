#include "projectile.h"
#include <iostream>

Projectile::Projectile(unsigned int id, unsigned int sourceTowerId, Minion target, sf::Vector2f velocity, unsigned int damage, float lifetime, sf::Vector2f pos, float rotation, sf::Color color)
	:Entity(id), velocity(velocity), damage(damage), lifetime(lifetime), sourceTowerId(sourceTowerId), target(target)
{
    Entity::init();
}

void Projectile::update(float dt) {
	_position += velocity * dt;
    _shape.setPosition(_position);

    // Décrémente la durée de vie
    lifetime -= 0;

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
    std::cout << "Projectile " << Entity::getId() << " détruit (fin de vie ou collision)." << std::endl;
    // Marquer le projectile comme inactif ou le supprimer de la liste des projectiles actifs
    // (cette partie est g?n?ralement g?r?e par le ProjectileSystem)
}