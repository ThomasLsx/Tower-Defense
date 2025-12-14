#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

class Projectile;
class Entity;
class Minion;
class Tower;

/**
 * @class ProjectileSystem
 * @brief Système de gestion des projectiles dans le jeu.
 */
class ProjectileSystem
{
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
     * @param source Tour source qui tire le projectile.
     * @param target Cible visée (minion).
     * @param dmg Dégâts infligés par le projectile.
     * @param speed Vitesse de déplacement du projectile.
     */
    void createProjectile(const Tower& source, std::shared_ptr<Minion> target, int dmg, float speed);

    /**
     * @brief Met à jour la logique de tous les projectiles actifs.
     * @param dt Temps écoulé depuis la dernière frame.
     */
    void update(float dt);

    /**
     * @brief Dessine tous les projectiles sur la fenêtre.
     * @param window Fenêtre de rendu SFML.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Supprime un projectile spécifique par son identifiant.
     * @param id ID du projectile à supprimer.
     */
    void removeProjectile(int id);

    /**
     * @brief Retourne le nombre de projectiles actuellement actifs.
     * @return Nombre de projectiles.
     */
    int getActiveCount() const;

    /**
     * @brief Accesseur pour la liste des projectiles.
     * @return Une copie du vecteur contenant les pointeurs partagés vers les projectiles.
     */
    std::vector<std::shared_ptr<Projectile>> getProjectiles() const
    {
        return projectiles;
    }

private:
    /** @brief Liste contenant tous les projectiles actifs gérés par le système. */
    std::vector<std::shared_ptr<Projectile>> projectiles;
};