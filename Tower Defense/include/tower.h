///tower.h

#pragma once
#include "entity.h"

#include <vector>

class ProjectileSystem;

/**
 * @class Tower
 * @brief Représente une tour dans un jeu de type Tower Defense.
 *
 * Une tour peut cibler des minions, tirer des projectiles et être améliorée.
 */
class Tower : public Entity {
private:
    float range;            ///< Portée de la tour
    float fireRate;         ///< Fréquence de tir en tirs par seconde
    unsigned int level;              ///< Niveau actuel de la tour
    unsigned int damage;  ///< Dégâts infligés par projectile
    std::vector<int> targetIds; ///< Liste des IDs des cibles actuellement visées

    /**
     * @brief Vitesse d'attaque de la tour.
     */
    float attackSpeed;

public:
    /**
     * @brief Constructeur de Tower.
     * @param id Identifiant unique de la tour.
     * @param pos Position initiale de la tour.
     * @param range Portée de la tour.
     * @param fireRate Fréquence de tir en tirs par seconde.
     * @param damage Dégâts infligés par projectile.
     * @param level Niveau initial de la tour (par défaut 1).
     */

    Tower(unsigned int id, float range = 5, float fireRate = 10, unsigned int level = 1, unsigned int damage = 10, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), float rotation = 0.0f, sf::Color color = sf::Color::White);


    /**
     * @brief Tente de tirer sur les cibles si possible.
     * @param minions Liste des minions. opti avec quadtree plus tard
     * @param projectileSystem Système de gestion des projectiles.
     */
    void tryFire(const std::vector<Entity*>& minions, ProjectileSystem& projectileSystem);

    /**
     * @brief Améliore la tour (augmente portée et dégâts).
     */
    void upgrade();

    /**
     * @brief Appelé quand la tour est détruite.
     */
    void onDestroy() override;

    /**
     * @brief Met à jour la tour (ciblage et tir).
     * @param dt Temps écoulé depuis la dernière frame.
     * @param minions Liste des minions.
     * @param projectileSystem Système de gestion des projectiles.
     */
    void update(float dt, const std::vector<Entity*>& minions, ProjectileSystem& projectileSystem); // Surcharge, pas override

    /**
     * @brief Met à jour la tour (sans cibles).
     * @param dt Temps écoulé depuis la dernière frame.
     */
    void update(float dt) override; // Cette version override bien Entity

    std::vector<int>  SearchTargets(unsigned int maxTargets); ///< Recherche des cibles dans la portée de la tour

    // Getters
    float getRange() const { return range; }          ///< Retourne la portée de la tour.
    float getFireRate() const { return fireRate; }    ///< Retourne la fréquence de tir.
    int getLevel() const { return level; }            ///< Retourne le niveau de la tour.
    int getDamage() const { return damage; }          ///< Retourne les dégâts infligés par projectile.
    const std::vector<int>& getTargetIds() { return targetIds; } ///< Retourne la liste des IDs des cibles.
};