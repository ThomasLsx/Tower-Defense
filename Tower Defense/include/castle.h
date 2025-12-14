#pragma once

#include <SFML/Graphics.hpp>

#include "economySystem.h"
#include "map.h"

class TileMap;
class EconomySystem;

/**
 * @brief Représente le château (la base) que le joueur doit défendre.
 */
class Castle
{
private:
    /** @brief Points de vie actuels du château. */
    unsigned int health;

    /** @brief Points de vie maximum du château. */
    unsigned int maxHealth;

    /** @brief Pointeur vers la carte de tuiles. */
    TileMap* map;

    /** @brief Pointeur vers le système économique pour l'ajout de ressources. */
    EconomySystem* economySystem;

    /** @brief Forme graphique représentant la barre de vie (remplissage). */
    sf::RectangleShape healthBar;

    /** @brief Forme graphique représentant l'arrière-plan de la barre de vie. */
    sf::RectangleShape healthBarBack;

    /** @brief Coordonnées de la tuile sur laquelle se trouve le château. */
    sf::Vector2u castleTile;

    /** @brief Indique si le château est détruit (Game Over). */
    bool destroyed;

public:
    /**
     * @brief Constructeur de Castle.
     * @param map Pointeur vers la carte.
     * @param economySystem Pointeur vers le système économique.
     * @param maxHealth Points de vie initiaux et maximums.
     */
    Castle(TileMap* map = nullptr, EconomySystem* economySystem = nullptr, unsigned int maxHealth = 100);

    /**
     * @brief Destructeur par défaut.
     */
    ~Castle() = default;

    /**
     * @brief Inflige des dégâts au château.
     * @param amount Quantité de dégâts à infliger.
     */
    void takeDamage(unsigned int amount);

    /**
     * @brief Dessine le château (barre de vie) sur la fenêtre.
     * @param window Fenêtre de rendu SFML.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Vérifie si le château est détruit.
     * @return true si le château est détruit, false sinon.
     */
    inline bool isDefeated() const
    {
        return destroyed;
    }

    /**
     * @brief Récupère les points de vie actuels.
     * @return Les PV actuels.
     */
    inline unsigned int getHealth() const
    {
        return health;
    }

    /**
     * @brief Récupère les points de vie maximums.
     * @return Les PV max.
     */
    inline unsigned int getMaxHealth() const
    {
        return maxHealth;
    }

    /**
     * @brief Force la destruction du château (déclenche l'état Game Over).
     */
    void destroy();

    /**
     * @brief Ajoute des ressources au joueur (gain passif ou récompense).
     * @param copper Quantité de cuivre.
     * @param silver Quantité d'argent.
     * @param gold Quantité d'or.
     */
    void addResource(int copper, int silver, int gold);

private:
    /**
     * @brief Initialise la position de la tuile du château à partir de la carte.
     */
    void setCastleTile();
};