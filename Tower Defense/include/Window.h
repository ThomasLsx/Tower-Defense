#pragma once

#include <optional>
#include <string>

#include <SFML/Graphics.hpp>

/**
 * @brief Classe wrapper gérant la fenêtre SFML de l'application.
 */
class Window
{
public:
    /**
     * @brief Constructeur de la fenêtre.
     * @param width Largeur initiale de la fenêtre.
     * @param height Hauteur initiale de la fenêtre.
     * @param title Titre de la fenêtre.
     */
    Window(unsigned int width = 1920, unsigned int height = 1056, const std::string& title = "Tower Defense");

    /**
     * @brief Crée la fenêtre SFML avec les paramètres définis.
     */
    void create();

    /**
     * @brief Ferme la fenêtre.
     */
    void close();

    /**
     * @brief Vérifie si la fenêtre est ouverte.
     * @return true si la fenêtre est ouverte, false sinon.
     */
    bool isOpen() const;

    /**
     * @brief Efface le contenu de la fenêtre avec une couleur donnée.
     * @param color Couleur de remplissage (noir par défaut).
     */
    void clear(const sf::Color& color = sf::Color::Black);

    /**
     * @brief Affiche le contenu rendu à l'écran.
     */
    void display();

    /**
     * @brief Récupère la taille actuelle de la fenêtre (vecteur).
     * @return Référence constante vers le vecteur de taille.
     */
    const sf::Vector2u& getWindowSize() const;

    /**
     * @brief Récupère la largeur de la fenêtre.
     * @return Référence constante vers la largeur.
     */
    const int& getWidth() const;

    /**
     * @brief Récupère la hauteur de la fenêtre.
     * @return Référence constante vers la hauteur.
     */
    const int& getHeight() const;

    /**
     * @brief Récupère le prochain événement de la file d'attente, s'il existe.
     * @return Un std::optional contenant l'événement si présent, sinon std::nullopt.
     */
    std::optional<sf::Event> pollEvent();

    /**
     * @brief Accède à l'objet interne sf::RenderWindow.
     * @return Référence vers la fenêtre SFML.
     */
    sf::RenderWindow& getRenderWindow();

private:
    /** @brief Instance principale de la fenêtre de rendu SFML. */
    sf::RenderWindow window;

    /** @brief Largeur configurée de la fenêtre. */
    unsigned int width;

    /** @brief Hauteur configurée de la fenêtre. */
    unsigned int height;

    /** @brief Titre de la fenêtre. */
    std::string title;

    /** @brief Stocke les dimensions actuelles de la fenêtre (vecteur). */
    sf::Vector2u windowSize;

    /** @brief Largeur de l'écran (cache pour accesseur). */
    int widthScreen;

    /** @brief Hauteur de l'écran (cache pour accesseur). */
    int heightScreen;
};