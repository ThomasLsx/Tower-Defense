#ifndef PROJECTILSYSTEM_H
#define PROJECTILSYSTEM_H
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Projectile;
class Entity;
class Minion;
class Tower;

/**
 * @class ProjectileSystem
 * @brief Système de gestion des projectiles dans le jeu.
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
     * @param source Tour source.
     * @param target Cible (minion).
     * @param dmg Dégâts infligés par le projectile.
     * @param speed Vitesse du projectile.
     */
    void createProjectile(const Tower& source, std::shared_ptr<Minion> target, int dmg, float speed);

    /**
     * @brief Met à jour tous les projectiles.
     * @param dt Temps écoulé depuis la dernière frame.
     */
    void update(float dt);

    /**
     * @brief Dessine tous les projectiles.
     * @param window Fenêtre de rendu.
     */
    void draw(sf::RenderWindow& window);

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