///tower.h

#pragma once

#include <vector>
#include <memory>
#include "entity.h"

class ProjectileSystem;
class Minion;

/**
 * @class Tower
 * @brief Représente une tour dans un jeu de type Tower Defense.
 *
 * Une tour peut cibler des minions, tirer des projectiles et être améliorée.
 */
class Tower : public Entity {
protected:
    float range;            ///< Portée de la tour
    float fireRate;         ///< Fréquence de tir en tirs par seconde
    float fireCooldown;     ///< timer de rechargement
    unsigned int level;              ///< Niveau actuel de la tour
    unsigned int damage;  ///< Dégâts infligés par projectile

    std::vector<std::weak_ptr<Minion>> targets;

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
    Tower(unsigned int id, float range = 5, float fireRate = 10, unsigned int level = 1, unsigned int damage = 10, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), sf::Color color = sf::Color::White);


    /**
     * @brief Tente de tirer sur les cibles si possible.
     * @param projectileSystem Système de gestion des projectiles.
     */
    void tryFire(ProjectileSystem& projectileSystem);

    /**
     * @brief Améliore la tour (augmente portée et dégâts).
     */
    virtual void upgrade();

    /**
     * @brief Appelé quand la tour est détruite.
     */
    void onDestroy() override;

    /**
     * @brief Met à jour la tour (ciblage et tir).
     * @param dt Temps écoulé depuis la dernière frame.
     * @param minions Liste de TOUS les minions (pour le ciblage).
     * @param projectileSystem Système de gestion des projectiles.
     */
    void update(float dt, const std::vector<std::shared_ptr<Minion>>& minions, ProjectileSystem& projectileSystem);

    /**
     * @brief Met à jour la tour (sans cibles).
     * @param dt Temps écoulé depuis la dernière frame.
     */
    void update(float dt) override; // Cette version override bien Entity

    void SearchTargets(const std::vector<std::shared_ptr<Minion>>& allMinions);

    // Getters
    float getRange() const { return range; }
    float getFireRate() const { return fireRate; }
    int getLevel() const { return level; }
    int getDamage() const { return damage; }

    const std::vector<std::weak_ptr<Minion>>& getTargets() const { return targets; }
};

// --- Tours Dérivées ---

/**
 * @class BasicTower
 * @brief La tour de base standard.
 */
class BasicTower : public Tower {
public:
    BasicTower(unsigned int id, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f));

    // Override de l'upgrade pour des stats spécifiques
    void upgrade() override;
};

/**
 * @class SpeedTower
 * @brief Tour rapide avec une cadence de tir élevée.
 */
class SpeedTower : public Tower {
public:
    SpeedTower(unsigned int id, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f));

    void upgrade() override;
};

/**
 * @class SniperTower
 * @brief Tour à longue portée et gros dégâts, mais tir lent.
 */
class SniperTower : public Tower {
public:
    SniperTower(unsigned int id, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f));

    void upgrade() override;
};

/**
 * @class SlowTower
 * @brief Tour qui applique un effet de ralentissement.
 */
class SlowTower : public Tower {
public:
    SlowTower(unsigned int id, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f));

    void upgrade() override;
};