#include "projectile.h"
#include <iostream>

Projectile::Projectile(unsigned int id, unsigned int sourceTowerId, unsigned int targetEntityId, sf::Vector2f velocity, unsigned int damage, float lifetime, sf::Vector2f pos, float rotation, sf::Color color, sf::IntRect textureRect, const sf::Texture& texture)
	:Entity(id, pos, rotation, color, textureRect, texture), velocity(velocity), damage(damage), lifetime(lifetime), sourceTowerId(sourceTowerId), targetEntityId(targetEntityId)
{
}

void Projectile::update(float dt) {
    // Met à jour la position du projectile en fonction de sa vitesse
    sf::Vector2f position;
	Entity::setPosition(position += velocity * dt);

    // Décrémente la durée de vie
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
    std::cout << "Projectile " << Entity::getID() << " détruit (fin de vie ou collision)." << std::endl;
    // Marquer le projectile comme inactif ou le supprimer de la liste des projectiles actifs
    // (cette partie est généralement gérée par le ProjectileSystem)
}
