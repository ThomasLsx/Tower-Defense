#pragma once
#include "entity.h"

/**
 * @class Projectile
 * @brief Représente un projectile tiré par une tour dans un jeu de type Tower Defense.
 */
class Projectile : public Entity {
private:
    Vector2 velocity;      ///< Vitesse et direction du projectile.
    int damage;            ///< Dégâts infligés à la cible.
    float lifetime;        ///< Durée de vie restante du projectile (en secondes).
    int sourceTowerId;     ///< ID de la tour source du projectile.
    int targetEntityId;    ///< ID de l'entité cible du projectile.

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
    Projectile(int id, Vector2 pos, Vector2 velocity, int damage, float lifetime, int sourceTowerId, int targetEntityId);

    /**
     * @brief Met à jour la position du projectile et vérifie les collisions ou la fin de vie.
     * @param dt Temps écoulé depuis la dernière frame (en secondes).
     */
    void update(float dt) override;

    /**
     * @brief Appelé quand le projectile touche une cible.
     * @param targetId ID de la cible touchée.
     */
    void onHit(int targetId);

    /**
     * @brief Appelé quand le projectile est détruit.
     */
    void onDestroy() override;

    // Getters
    float getLifetime() const { return lifetime; }          ///< Retourne la durée de vie restante du projectile.
    Vector2 getVelocity() const { return velocity; }        ///< Retourne la vitesse et la direction du projectile.
    int getDamage() const { return damage; }                ///< Retourne les dégâts infligés par le projectile.
    int getSourceTowerId() const { return sourceTowerId; }  ///< Retourne l'ID de la tour source.
    int getTargetEntityId() const { return targetEntityId; }///< Retourne l'ID de l'entité cible.
};
