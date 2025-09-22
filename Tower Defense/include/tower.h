#pragma once
#include "entity.h"
#include "projectileSystem.h"
#include <vector>

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
    int level;              ///< Niveau actuel de la tour
    int damage;             ///< Dégâts infligés par projectile
    float fireAccumulator;  ///< Accumulateur pour gérer la fréquence de tir
    std::vector<int> targetIds; ///< Liste des IDs des cibles actuelles

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
    Tower(int id, Vector2 pos, float range, float fireRate, int damage, int level = 1);

    /**
     * @brief Trouve les cibles à portée de la tour.
     * @param minions Liste des minions à vérifier.
     */
    void findTargets(const std::vector<Entity*>& minions);

    /**
     * @brief Tente de tirer sur les cibles si possible.
     * @param dt Temps écoulé depuis la dernière frame.
     * @param minions Liste des minions.
     * @param projectileSystem Système de gestion des projectiles.
     */
    void tryFire(float dt, const std::vector<Entity*>& minions, ProjectileSystem& projectileSystem);

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
    void update(float dt, const std::vector<Entity*>& minions, ProjectileSystem& projectileSystem) override;

    // Getters
    float getRange() const { return range; }          ///< Retourne la portée de la tour.
    float getFireRate() const { return fireRate; }    ///< Retourne la fréquence de tir.
    int getLevel() const { return level; }            ///< Retourne le niveau de la tour.
    int getDamage() const { return damage; }          ///< Retourne les dégâts infligés par projectile.
    const std::vector<int>& getTargetIds() const { return targetIds; } ///< Retourne la liste des IDs des cibles.
};
