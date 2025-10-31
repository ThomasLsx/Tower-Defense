///projectile.h

#pragma once
#include "entity.h"
#include "minion.h" 
#include <memory>

/**
 * @class Projectile
 * @brief Représente un projectile tiré par une tour dans un jeu de type Tower Defense.
 */
class Projectile : public Entity {
private:
    sf::Vector2f velocity;      ///< Vitesse et direction du projectile.
    unsigned int damage;            ///< Dégâts infligés à la cible.
    float lifetime;        ///< Durée de vie restante du projectile (en secondes).
    unsigned int sourceTowerId;     ///< ID de la tour source du projectile.

    std::weak_ptr<Minion> target;

    sf::Vector2f direction;

public:
    /**
     * @brief Constructeur de Projectile.
     * @param id Identifiant unique du projectile.
     * @param sourceTowerId ID de la tour source.
     * @param target Cible (Minion) (sera stocké comme weak_ptr).
     * @param velocity Vitesse (utilisé comme scalaire pour le homing).
     * @param damage Dégâts infligés à la cible.
     * @param lifetime Durée de vie initiale du projectile (en secondes).
     * @param pos Position initiale du projectile.
     */
     Projectile(unsigned int id, unsigned int sourceTowerId, std::shared_ptr<Minion> target, sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f), unsigned int damage = 10, float lifetime = 20, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), float rotation = 0.0f, sf::Color color = sf::Color::White);

    /**
     * @brief Met à jour la position du projectile et vérifie les collisions ou la fin de vie.
     * @param dt Temps écoulé depuis la dernière frame (en secondes).
     */
    void update(float dt) override;

    /**
     * @brief Appelé quand le projectile touche une cible.
     */
    void onHit();

    /**
     * @brief Appelé quand le projectile est détruit.
     */
    void onDestroy() override;

    // Getters
    float getLifetime() const { return lifetime; }
    sf::Vector2f getVelocity() const { return velocity; }
    int getDamage() const { return damage; }
    int getSourceTowerId() const { return sourceTowerId; }

    std::weak_ptr<Minion> getTarget() const { return target; }

    int getId() const { return Entity::getId(); }

    bool isExpired() const { return lifetime <= 0; }
};