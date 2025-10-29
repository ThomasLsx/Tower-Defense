#ifndef PROJECTILSYSTEM_H
#define PROJECTILSYSTEM_H
#include <vector>
#include <memory>

class Projectile;
class Entity;
class Minion;
class Tower;

/**
 * @class ProjectileSystem
 * @brief Système de gestion des projectiles dans le jeu.
 *
 * Crée, met à jour et supprime les projectiles, et gère les collisions avec les cibles.
 */
class ProjectileSystem {
private:
    std::vector<std::shared_ptr<Projectile>> projectiles; ///< Liste des projectiles actifs.

public:
    /**
     * @brief Constructeur par défaut.
     */
    ProjectileSystem();

    /**
     * @brief Destructeur par défaut.
     */
    ~ProjectileSystem();

    /**
     * @brief Crée un nouveau projectile.
     * @param sourceId ID de l'entité source (tour).
     * @param targetId ID de la cible (minion).
     * @param dmg Dégâts infligés par le projectile.
     * @param speed Vitesse du projectile.
     */
    void createProjectile(Tower source, Minion target, int dmg, float speed);

    /**
     * @brief Met à jour tous les projectiles.
     * @param dt Temps écoulé depuis la dernière frame.
     * @param entities Liste de toutes les entités (pour détecter les collisions).
     */
    void update(float dt);

    /**
     * @brief Supprime un projectile par son ID.
     * @param id ID du projectile à supprimer.
     */
    void removeProjectile(int id);

    /**
     * @brief Retourne le nombre de projectiles actifs.
     * @return Nombre de projectiles actifs.
     */
    int getActiveCount() const;

    std::vector<std::shared_ptr<Projectile>> getProjectiles() const {
        return projectiles;
    }
};

#endif // PROJECTILSYSTEM_H