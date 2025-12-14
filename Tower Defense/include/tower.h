#pragma once

#include <memory>
#include <vector>

#include "entity.h"

class ProjectileSystem;
class Minion;

/**
 * @class Tower
 * @brief Représente une tour dans un jeu de type Tower Defense.
 *
 * Une tour peut cibler des minions, tirer des projectiles et être améliorée.
 */
class Tower : public Entity
{
protected:
    /** @brief Portée de la tour. */
    float range;

    /** @brief Fréquence de tir en tirs par seconde. */
    float fireRate;

    /** @brief Timer de rechargement actuel. */
    float fireCooldown;

    /** @brief Niveau actuel de la tour. */
    unsigned int level;

    /** @brief Dégâts infligés par projectile. */
    unsigned int damage;

    /** @brief Coût en copper pour placer/upgrader. */
    unsigned int costCopper;

    /** @brief Coût en silver pour placer/upgrader. */
    unsigned int costSilver;

    /** @brief Coût en gold pour placer/upgrader. */
    unsigned int costGold;

    /** @brief Liste des cibles potentielles (Minions). */
    std::vector<std::weak_ptr<Minion>> targets;

public:
    /**
     * @brief Constructeur de Tower.
     * @param id Identifiant unique de la tour.
     * @param range Portée de la tour.
     * @param fireRate Fréquence de tir en tirs par seconde.
     * @param level Niveau initial de la tour (par défaut 1).
     * @param damage Dégâts infligés par projectile.
     * @param costCopper Coût en copper.
     * @param costSilver Coût en silver.
     * @param costGold Coût en gold.
     * @param pos Position initiale de la tour.
     * @param color Couleur de la tour.
     */
    Tower(unsigned int id, float range = 5, float fireRate = 10, unsigned int level = 1, unsigned int damage = 10,
        unsigned int costCopper = 10, unsigned int costSilver = 0, unsigned int costGold = 0,
        sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), sf::Color color = sf::Color::White);

    /**
     * @brief Met à jour l'état interne de la tour (surcharge simple).
     * @param dt Temps écoulé depuis la dernière frame.
     */
    void update(float dt) override;

    /**
     * @brief Applique un effet spécial lors de l'attaque (ex: ralentissement).
     * @param target La cible visée.
     */
    virtual void attackSpecialEffect(std::weak_ptr<Minion> target)
    {
    }

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
     * @brief Recherche les cibles à portée parmi la liste des minions.
     * @param allMinions Liste de tous les minions actifs.
     */
    void SearchTargets(const std::vector<std::shared_ptr<Minion>>& allMinions);

    // Getters

    /**
     * @brief Récupère la portée de la tour.
     * @return La portée en float.
     */
    float getRange() const
    {
        return range;
    }

    /**
     * @brief Récupère la cadence de tir.
     * @return La cadence en tirs/seconde.
     */
    float getFireRate() const
    {
        return fireRate;
    }

    /**
     * @brief Récupère le niveau actuel.
     * @return Le niveau de la tour.
     */
    int getLevel() const
    {
        return level;
    }

    /**
     * @brief Récupère les dégâts de la tour.
     * @return Les dégâts par tir.
     */
    int getDamage() const
    {
        return damage;
    }

    // Getters pour les coûts

    /**
     * @brief Récupère le coût de base en cuivre.
     * @return Le coût en cuivre.
     */
    unsigned int getCostCopper() const
    {
        return costCopper;
    }

    /**
     * @brief Récupère le coût de base en argent.
     * @return Le coût en argent.
     */
    unsigned int getCostSilver() const
    {
        return costSilver;
    }

    /**
     * @brief Récupère le coût de base en or.
     * @return Le coût en or.
     */
    unsigned int getCostGold() const
    {
        return costGold;
    }

    // Getters pour les coûts d'upgrade

    /**
     * @brief Calcule le prix en cuivre pour la prochaine amélioration.
     * @return Le prix calculé (1.5x le coût de base).
     */
    unsigned int getUpgradeCopperPrice() const
    {
        return static_cast<unsigned int>(costCopper * 1.5f);
    }

    /**
     * @brief Calcule le prix en argent pour la prochaine amélioration.
     * @return Le prix calculé (1.5x le coût de base).
     */
    unsigned int getUpgradeSilverPrice() const
    {
        return static_cast<unsigned int>(costSilver * 1.5f);
    }

    /**
     * @brief Calcule le prix en or pour la prochaine amélioration.
     * @return Le prix calculé (1.5x le coût de base).
     */
    unsigned int getUpgradeGoldPrice() const
    {
        return static_cast<unsigned int>(costGold * 1.5f);
    }

    // Getters pour le remboursement

    /**
     * @brief Calcule le montant de cuivre remboursé à la vente.
     * @return Le montant remboursé (70% de la valeur totale investie).
     */
    unsigned int getRefundCopper() const
    {
        return static_cast<unsigned int>((costCopper + getUpgradeCopperPrice() * (level - 1)) * 0.7f);
    }

    /**
     * @brief Calcule le montant d'argent remboursé à la vente.
     * @return Le montant remboursé (70% de la valeur totale investie).
     */
    unsigned int getRefundSilver() const
    {
        return static_cast<unsigned int>((costSilver + getUpgradeSilverPrice() * (level - 1)) * 0.7f);
    }

    /**
     * @brief Calcule le montant d'or remboursé à la vente.
     * @return Le montant remboursé (70% de la valeur totale investie).
     */
    unsigned int getRefundGold() const
    {
        return static_cast<unsigned int>((costGold + getUpgradeGoldPrice() * (level - 1)) * 0.7f);
    }

    /**
     * @brief Récupère la liste des cibles actuelles.
     * @return Référence constante vers le vecteur de cibles.
     */
    const std::vector<std::weak_ptr<Minion>>& getTargets() const
    {
        return targets;
    }
};

// --- Tours Dérivées ---

/**
 * @class BasicTower
 * @brief La tour de base standard.
 */
class BasicTower : public Tower
{
public:
    /**
     * @brief Constructeur de BasicTower.
     * @param id Identifiant unique.
     * @param pos Position initiale.
     */
    BasicTower(unsigned int id, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f));

    /**
     * @brief Améliore la tour de base (statistiques spécifiques).
     */
    void upgrade() override;
};

/**
 * @class SpeedTower
 * @brief Tour rapide avec une cadence de tir élevée.
 */
class SpeedTower : public Tower
{
public:
    /**
     * @brief Constructeur de SpeedTower.
     * @param id Identifiant unique.
     * @param pos Position initiale.
     */
    SpeedTower(unsigned int id, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f));

    /**
     * @brief Améliore la tour rapide.
     */
    void upgrade() override;
};

/**
 * @class SniperTower
 * @brief Tour à longue portée et gros dégâts, mais tir lent.
 */
class SniperTower : public Tower
{
public:
    /**
     * @brief Constructeur de SniperTower.
     * @param id Identifiant unique.
     * @param pos Position initiale.
     */
    SniperTower(unsigned int id, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f));

    /**
     * @brief Améliore la tour sniper.
     */
    void upgrade() override;
};

/**
 * @class SlowTower
 * @brief Tour qui applique un effet de ralentissement.
 */
class SlowTower : public Tower
{
public:
    /**
     * @brief Constructeur de SlowTower.
     * @param id Identifiant unique.
     * @param pos Position initiale.
     */
    SlowTower(unsigned int id, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f));

    /**
     * @brief Améliore la tour de ralentissement.
     */
    void upgrade() override;

    /**
     * @brief Applique l'effet de ralentissement à la cible.
     * @param target La cible touchée.
     */
    void attackSpecialEffect(std::weak_ptr<Minion> target) override;

private:
    /** @brief Pourcentage de ralentissement appliqué aux minions (0.0 à 1.0). */
    float slowEffect;
};