#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

/**
 * @class Entity
 * @brief Classe de base pour tous les objets du jeu, gère la position et le sprite.
 */
class Entity {
protected:
    /**
     * @brief Position de l'entité dans le monde.
     */
    sf::Vector2f position;

    /**
     * @brief Sprite SFML représentant l'entité.
     */
    std::unique_ptr<sf::Sprite> sprite; // Utilisation d'un pointeur intelligent

public:
    /**
     * @brief Constructeur de l'entité.
     * @param x Position X.
     * @param y Position Y.
     * @param texture Texture SFML à utiliser.
     */
    Entity(float x, float y, const sf::Texture& texture);

    /**
     * @brief Constructeur par défaut.
     */
    Entity();

    /**
     * @brief Destructeur virtuel.
     */
    virtual ~Entity() = default;

    /**
     * @brief Met à jour l'entité.
     * @param deltaTime Temps écoulé depuis la dernière mise à jour.
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Dessine l'entité dans la fenêtre.
     * @param window Fenêtre SFML où dessiner.
     */
    virtual void draw(sf::RenderWindow& window);

    /**
     * @brief Obtient la position de l'entité.
     * @return Position sous forme de sf::Vector2f.
     */
    sf::Vector2f getPosition() const;

    /**
     * @brief Définit la position de l'entité.
     * @param x Nouvelle position X.
     * @param y Nouvelle position Y.
     */
    void setPosition(float x, float y);
};