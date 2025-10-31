/* MIT License

Copyright (c) 2022 Pierre Lefebvre

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */
#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "utils.h"

/**
 * @brief Classe de base pour toutes les entités du jeu.
 *
 * Fournit position, identifiant, la forme graphique et des utilitaires de collision.
 */
class Entity {
public:
    /**
     * @brief Constructeur
     * @param id Identifiant unique
     * @param position Position initiale
     */
    Entity(unsigned int id = 0, sf::Vector2f position = sf::Vector2f(0, 0));

    virtual ~Entity() = default;

    /**
     * @brief Appelé lors de la destruction de l'entité
     */
    virtual void onDestroy() {}

    /**
     * @brief Initialise une forme circulaire pour l'entité.
     */
    void init(int radius, const sf::Color& color = sf::Color::Red , const sf::Color& outline = sf::Color::Black, int thickness = 1);

    /**
     * @brief Initialise une forme rectangulaire pour l'entité.
     */
    void init(int l, int L, const sf::Color& color = sf::Color::Red, const sf::Color& outline = sf::Color::Black, int thickness = 1);

    /**
     * @brief Met à jour l'entité chaque frame.
     * @param dt Temps écoulé depuis la dernière frame
     */
    virtual void update(float dt) = 0;

    /**
     * @brief Dessine l'entité.
     * @param window Fenêtre de rendu
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Retourne la position courante.
     */
    inline const sf::Vector2f& getPosition() const {
        return _position;
    }

    /**
     * @brief Définit la position et met à jour la forme.
     */
    inline void setPosition(const sf::Vector2f& position) {
        _position = position;
        _shape->setPosition(_position);
    }

    inline void setPosition(float x, float y) {
        _position = sf::Vector2f(x, y);
        _shape->setPosition(_position);
    }

    /**
     * @brief Retourne l'identifiant.
     */
    inline unsigned int getId() const {
        return _id;
    }

    /**
     * @brief Vérifie la collision avec une autre entité.
     */
    bool isColliding(const Entity& e) const;

    /**
     * @brief Accès en lecture à la forme SFML.
     */
    inline const sf::Shape& getShape() const {
        return *_shape;
    }

    inline void setIsAlive(bool alive) {
        _isAlive = alive;
    }

    inline bool getIsAlive() {
        return _isAlive;
    }

    inline sf::FloatRect getGlobalBounds() const {
        return _shape->getGlobalBounds();
    }

protected:
    sf::Shape* _shape = nullptr;    ///< Forme graphique (allouée dynamiquement)
    sf::Vector2f _position;         ///< Position
    unsigned int _squaredRadius;    ///< Rayon au carré pour tests rapides

    bool _isAlive;

    unsigned int _id;
};

#endif
