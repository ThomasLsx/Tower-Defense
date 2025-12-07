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

    // --- Ajout des coûts de la tour ---
    unsigned int costCopper; ///< Coût en copper pour placer/upgrader
    unsigned int costSilver; ///< Coût en silver pour placer/upgrader
    unsigned int costGold;   ///< Coût en gold pour placer/upgrader

    std::vector<std::weak_ptr<Minion>> targets;

public:
    /**
     * @brief Constructeur de Tower.
     * @param id Identifiant unique de la tour.
     * @param range Portée de la tour.
     * @param fireRate Fréquence de tir en tirs par seconde.
     * @param level Niveau initial de la tour (par défaut 1).
     * @param damage Dégâts infligés par projectile.
     * @param costCopper Coût en copper
     * @param costSilver Coût en silver
     * @param costGold Coût en gold
     * @param pos Position initiale de la tour.
     * @param color Couleur de la tour.
     */
    Tower(unsigned int id, float range = 5, float fireRate = 10, unsigned int level = 1, unsigned int damage = 10, 
          unsigned int costCopper = 10, unsigned int costSilver = 0, unsigned int costGold = 0,
          sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), sf::Color color = sf::Color::White);

    void update(float dt) override;

	virtual void attackSpecialEffect(std::weak_ptr<Minion> target) {}

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

    void SearchTargets(const std::vector<std::shared_ptr<Minion>>& allMinions);

    // Getters
    float getRange() const { return range; }
    float getFireRate() const { return fireRate; }
    int getLevel() const { return level; }
    int getDamage() const { return damage; }

    // Getters pour les coûts
    unsigned int getCostCopper() const { return costCopper; }
    unsigned int getCostSilver() const { return costSilver; }
    unsigned int getCostGold() const { return costGold; }

    // Getters pour les coûts d'upgrade
    unsigned int getUpgradeCopperPrice() const { return static_cast<unsigned int>(costCopper * 1.5f); }
    unsigned int getUpgradeSilverPrice() const { return static_cast<unsigned int>(costSilver * 1.5f); }
    unsigned int getUpgradeGoldPrice() const { return static_cast<unsigned int>(costGold * 1.5f); }

    // Getters pour le remboursement
    unsigned int getRefundCopper() const { return static_cast<unsigned int>((costCopper + getUpgradeCopperPrice() * (level - 1)) * 0.7f); }
    unsigned int getRefundSilver() const { return static_cast<unsigned int>((costSilver + getUpgradeSilverPrice() * (level - 1)) * 0.7f); }
    unsigned int getRefundGold() const { return static_cast<unsigned int>((costGold + getUpgradeGoldPrice() * (level - 1)) * 0.7f); }

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
	void attackSpecialEffect(std::weak_ptr<Minion> target) override;
private:
	float slowEffect; ///< Pourcentage de ralentissement appliqué aux minions (0.0 à 1.0)
};