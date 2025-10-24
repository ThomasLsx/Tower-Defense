///projectile.h

#pragma once
#include "entity.h"

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
    unsigned int targetEntityId;    ///< ID de l'entité cible du projectile.

public:
    /**
     * @brief Constructeur de Projectile.
     * @param id Identifiant unique du projectile.
     * @param pos Position initiale du projectile.
     * @param velocity Vitesse et direction du projectile.
     * @param damage Dégâts infligés à la cible.
     * @param lifetime Durée de vie initiale du projectile (en secondes).
     * @param sourceTowerId ID de la tour source.
     * @param targetEntityId ID de l'entité cible.
     */
    Projectile(unsigned int id, unsigned int sourceTowerId, unsigned int targetEntityId, sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f), unsigned int damage = 10, float lifetime = 20, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), float rotation = 0.0f, sf::Color color = sf::Color::White);

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
    float getLifetime() const { return lifetime; }          ///< Retourne la durée de vie restante du projectile.
    sf::Vector2f getVelocity() const { return velocity; }        ///< Retourne la vitesse et la direction du projectile.
    int getDamage() const { return damage; }                ///< Retourne les dégâts infligés par le projectile.
    int getSourceTowerId() const { return sourceTowerId; }  ///< Retourne l'ID de la tour source.
    int getTargetEntityId() const { return targetEntityId; }///< Retourne l'ID de l'entité cible.
    int getId() const { return Entity::getId(); } // Ajout pour compatibilité avec ProjectileSystem
    bool hasReachedTarget() const { return false; } // Stub à implémenter
    bool isExpired() const { return lifetime <= 0; } // Simple expiration
};