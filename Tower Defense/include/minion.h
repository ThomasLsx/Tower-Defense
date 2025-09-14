#pragma once

#include "entity.h"

/**
 * @class Minion
 * @brief Représente un sbire (minion) dans le jeu, hérite de Entity.
 */
class Minion : public Entity {
private:
    /**
     * @brief Dégâts infligés par le minion.
     */
    float damage;

    /**
     * @brief Direction du déplacement du minion.
     */
    sf::Vector2f direction;

    /**
     * @brief Vitesse de déplacement du minion.
     */
    float speed;

public:
    /**
     * @brief Constructeur du minion.
     * @param x Position X.
     * @param y Position Y.
     * @param damage Dégâts.
     * @param speed Vitesse.
     * @param direction Direction du déplacement.
     * @param texture Texture SFML à utiliser.
     */
    Minion(int x, int y, float damage, float speed, sf::Vector2f direction, const sf::Texture& texture);

    /**
     * @brief Constructeur par défaut.
     */
    Minion(void) = default;

    /**
     * @brief Destructeur.
     */
    ~Minion(void) override = default;

    /**
     * @brief Obtient les dégâts du minion.
     * @return Dégâts.
     */
    float getDamage(void);

    /**
     * @brief Définit les dégâts du minion.
     * @param damage Nouvelle valeur des dégâts.
     */
    void setDamage(float damage);

    /**
     * @brief Obtient la direction du minion.
     * @return Direction.
     */
    sf::Vector2f getDirection(void);

    /**
     * @brief Définit la direction du minion.
     * @param direction Nouvelle direction.
     */
    void setDirection(sf::Vector2f direction);

    /**
     * @brief Obtient la vitesse du minion.
     * @return Vitesse.
     */
    float getSpeed(void);

    /**
     * @brief Définit la vitesse du minion.
     * @param speed Nouvelle valeur de la vitesse.
     */
    void setSpeed(float speed);
};