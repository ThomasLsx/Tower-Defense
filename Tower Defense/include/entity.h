#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

/**
 * @brief Classe de base pour toutes les entités du jeu.
 *
 * Fournit position, identifiant, la forme graphique et des utilitaires de collision.
 */
class Entity
{
public:
    /**
     * @brief Constructeur.
     * @param id Identifiant unique.
     * @param position Position initiale.
     */
    Entity(unsigned int id = 0, sf::Vector2f position = sf::Vector2f(0, 0));

    /**
     * @brief Destructeur virtuel par défaut.
     */
    virtual ~Entity() = default;

    /**
     * @brief Appelé lors de la destruction de l'entité.
     */
    virtual void onDestroy()
    {
    }

    /**
     * @brief Initialise une forme circulaire pour l'entité.
     * @param radius Rayon du cercle.
     * @param color Couleur de remplissage.
     * @param outline Couleur du contour.
     * @param thickness Épaisseur du contour.
     */
    void init(int radius, const sf::Color& color = sf::Color::Red, const sf::Color& outline = sf::Color::Black, int thickness = 1);

    /**
     * @brief Initialise une forme rectangulaire pour l'entité.
     * @param l Largeur.
     * @param L Longueur (Hauteur).
     * @param color Couleur de remplissage.
     * @param outline Couleur du contour.
     * @param thickness Épaisseur du contour.
     */
    void init(int l, int L, const sf::Color& color = sf::Color::Red, const sf::Color& outline = sf::Color::Black, int thickness = 1);

    /**
     * @brief Met à jour l'entité chaque frame.
     * @param dt Temps écoulé depuis la dernière frame.
     */
    virtual void update(float dt) = 0;

    /**
     * @brief Dessine l'entité.
     * @param window Fenêtre de rendu.
     */
    virtual void draw(sf::RenderWindow& window);

    /**
     * @brief Retourne la position courante.
     * @return Référence constante vers le vecteur position.
     */
    inline const sf::Vector2f& getPosition() const
    {
        return _position;
    }

    /**
     * @brief Définit la position et met à jour la forme.
     * @param position Nouvelle position.
     */
    inline void setPosition(const sf::Vector2f& position)
    {
        _position = position;
        _shape->setPosition(_position);
    }

    /**
     * @brief Définit la position et met à jour la forme (surcharge XY).
     * @param x Coordonnée X.
     * @param y Coordonnée Y.
     */
    inline void setPosition(float x, float y)
    {
        _position = sf::Vector2f(x, y);
        _shape->setPosition(_position);
    }

    /**
     * @brief Retourne l'identifiant.
     * @return L'ID de l'entité.
     */
    inline unsigned int getId() const
    {
        return _id;
    }

    /**
     * @brief Vérifie la collision avec une autre entité.
     * @param e L'autre entité.
     * @return true si collision, false sinon.
     */
    bool isColliding(const Entity& e) const;

    /**
     * @brief Accès en lecture à la forme SFML.
     * @return Référence constante vers la forme.
     */
    inline const sf::Shape& getShape() const
    {
        return *_shape;
    }

    /**
     * @brief Définit l'état de vie de l'entité.
     * @param alive true pour vivant, false pour mort.
     */
    inline void setIsAlive(bool alive)
    {
        _isAlive = alive;
    }

    /**
     * @brief Récupère l'état de vie de l'entité.
     * @return true si vivant, false sinon.
     */
    inline bool getIsAlive()
    {
        return _isAlive;
    }

    /**
     * @brief Récupère les limites globales de la forme (AABB).
     * @return Le rectangle englobant.
     */
    inline sf::FloatRect getGlobalBounds() const
    {
        return _shape->getGlobalBounds();
    }

protected:
    /** @brief Forme graphique gérée par un pointeur intelligent. */
    std::unique_ptr<sf::Shape> _shape;

    /** @brief Position actuelle de l'entité. */
    sf::Vector2f _position;

    /** @brief Rayon au carré pour tests rapides de distance. */
    unsigned int _squaredRadius;

    /** @brief État de vie de l'entité (true = active). */
    bool _isAlive;

    /** @brief Identifiant unique de l'entité. */
    unsigned int _id;
};