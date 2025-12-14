#pragma once

#include <memory>

#include "entity.h"
#include "minion.h"

/**
 * @class Projectile
 * @brief Représente un projectile tiré par une tour dans un jeu de type Tower Defense.
 */
class Projectile : public Entity
{
private:
    /** @brief Vitesse et direction du projectile. */
    sf::Vector2f velocity;

    /** @brief Dégâts infligés à la cible. */
    unsigned int damage;

    /** @brief Durée de vie restante du projectile (en secondes). */
    float lifetime;

    /** @brief Identifiant de la tour qui a tiré ce projectile. */
    unsigned int sourceTowerId;

    /** @brief Référence faible vers le minion ciblé (pour le guidage). */
    std::weak_ptr<Minion> target;

    /** @brief Vecteur de direction actuel du projectile. */
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
     * @param rotation Rotation initiale.
     * @param color Couleur du projectile.
     */
    Projectile(unsigned int id, unsigned int sourceTowerId, std::shared_ptr<Minion> target,
        sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f), unsigned int damage = 10,
        float lifetime = 20, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f),
        float rotation = 0.0f, sf::Color color = sf::Color::White);

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
     * @brief Appelé quand le projectile est détruit (fin de vie ou impact).
     */
    void onDestroy() override;

    // Getters

    /**
     * @brief Récupère la durée de vie restante.
     * @return La durée en secondes.
     */
    float getLifetime() const
    {
        return lifetime;
    }

    /**
     * @brief Récupère le vecteur vitesse.
     * @return Le vecteur vitesse.
     */
    sf::Vector2f getVelocity() const
    {
        return velocity;
    }

    /**
     * @brief Récupère les dégâts du projectile.
     * @return La valeur des dégâts.
     */
    int getDamage() const
    {
        return damage;
    }

    /**
     * @brief Récupère l'ID de la tour source.
     * @return L'identifiant de la tour.
     */
    int getSourceTowerId() const
    {
        return sourceTowerId;
    }

    /**
     * @brief Récupère la cible du projectile.
     * @return Un weak_ptr vers le minion cible.
     */
    std::weak_ptr<Minion> getTarget() const
    {
        return target;
    }

    /**
     * @brief Récupère l'identifiant unique de l'entité.
     * @return L'ID de l'entité.
     */
    int getId() const
    {
        return Entity::getId();
    }

    /**
     * @brief Vérifie si le projectile a expiré (durée de vie <= 0).
     * @return true si expiré, false sinon.
     */
    bool isExpired() const
    {
        return lifetime <= 0;
    }
};