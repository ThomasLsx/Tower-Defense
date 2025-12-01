#pragma once
#include "entity.h"
#include "map.h"
#include "castle.h"
#include <memory>
#include <vector> 

struct Position;
/**
 * @class Minion
 * @brief Représente un minion dans un jeu de type Tower Defense.
 *
 * Un minion suit un chemin prédéfini, peut subir des dégâts et récompense le joueur à sa mort.
 */
class Minion : public Entity {
private:
    TileMap* map;               ///< Pointeur vers la map du jeu.
	Castle* castle; ///< Pointeur faible vers le château.
    unsigned int health;          ///< Points de vie actuels du minion.
    float speed;        ///< Vitesse de déplacement du minion (pixels par seconde).
    size_t currentTargetIndex; ///< Progression le long du chemin (0.0 à 1.0).
    unsigned int copper;   ///< Copper accordée à la mort du minion.
	unsigned int silver;   ///< Silver accordée à la mort du minion.
	unsigned int gold;     ///< Gold accordée à la mort du minion.
    std::vector<sf::Vector2f> targetPath; ///< Chemin en coordonnées du monde (pixels).
	unsigned int maxHealth; 		///< Points de vie maximum du minion.
	sf::RectangleShape healthBarBack;   ///< Arrière-plan de la barre de vie.
	sf::RectangleShape healthBar;       ///< Barre de vie.

	float specialStateTimer;		///< Timer pour les états spéciaux (ex: gelé, enflammé).


public:
    /**
     * @brief Constructeur de Minion.
     * @param id Identifiant unique du minion.
     * @param map Pointeur vers la map du jeu.
     * @param health Points de vie initiaux du minion.
     * @param reward Récompense accordée à la mort du minion.
     * @param pos Position initiale du minion.
     * @param rotation Rotation initiale.
     * @param color Couleur du minion.
     */
    Minion(int id, TileMap* map = nullptr, Castle* castle = nullptr, unsigned int health = 100, float speed = 50.0f, unsigned int reward = 10, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), float rotation = 0.0f, sf::Color color = sf::Color::White);

    /**
     * @brief Déplace le minion en fonction de sa vitesse et du temps écoulé.
     * @param dt Temps écoulé depuis la dernière frame (en secondes).
     */
    void move();

    /**
     * @brief Logique de mouvement interne pour suivre le chemin.
     * @param dt Temps écoulé (delta time).
     */
    void followPath(float dt);

    /**
     * @brief Assigne un nouveau chemin au minion.
     * @param gridPath Le chemin en coordonnées de GRILLE (venant de Pathfinding).
     * @param tileSize La taille (largeur/hauteur) d'une tuile en pixels.
     */
    void setPath(const std::vector<Position>& gridPath, float tileSize);

    /**
     * @brief Inflige des dégâts au minion.
     * @param amount Montant des dégâts infligés.
     * @param sourceId ID de l'entité source des dégâts (ex: ID de la tour).
     */
    void takeDamage(int amount);

    void makeDamage(int amount);

    /**
     * @brief Retourne la progression du minion le long de son chemin.
     * @return Valeur entre 0.0 et 1.0 représentant la progression.
     */
    size_t getPathProgress(void) const { return currentTargetIndex; }

    /**
     * @brief Retourne les points de vie actuels du minion.
     * @return Points de vie restants.
     */
    int getHealth(void) const { return health; }

    /**
     * @brief Appelé quand le minion est détruit (mort ou arrivée à destination).
     */
    void onDestroy(void) override;

    /**
     * @brief Met à jour le minion (déplacement, suivi de chemin, etc.).
     * @param dt Temps écoulé depuis la dernière frame (en secondes).
     */
    void update(float dt) override;

    /**
	* @brief Dessine le minion.
	* @param window Fenêtre de rendu.
    */
	void draw(sf::RenderWindow& window) override;

    /**
    * @brief Définit la vitesse de déplacement du minion.
    * @param newSpeed Nouvelle vitesse en pixels par seconde.
    */
    void setSpeed(float newSpeed) { speed = newSpeed; }

    /**
    * @brief Récupère la vitesse actuelle du minion.
    * @return Vitesse en pixels par seconde.
    */
    float getSpeed() const { return speed; }

    /**
    * @brief Retourne le chemin cible actuel du minion.
    * @return Référence constante vers le vecteur de positions cibles.
    */
    const std::vector<sf::Vector2f>& getTargetPath() const { return targetPath; }

    float getSpecialStateTimer() const { return specialStateTimer; }

	void setSpecialStateTimer(float time) { specialStateTimer = time; }

    TileMap* getMap() const { return map; }
};


/**
* @Class MinionNormal
* @brief Représente un minion de type normal.
*/
class MinionNormal : public Minion {
public:
    MinionNormal(int id, TileMap* map = nullptr, Castle* castle = nullptr)
        : Minion(id, map, castle, 100, 50.0f, 10) // health=100, speed=50, reward=10
    {
        Entity::init(15, sf::Color::Green, sf::Color::Black, 2);
    }
};

/**
* @Class MinionFast
* @brief Représente un minion de type rapide.
*/
class MinionFast : public Minion {
public:
    MinionFast(int id, TileMap* map = nullptr, Castle* castle = nullptr)
        : Minion(id, map, castle, 75, 100.0f, 15) // health=75, speed=100, reward=15
    {
        Entity::init(15, sf::Color::Blue, sf::Color::Black, 2);
    }
};

/**
* @Class MinionTank
* @brief Représente un minion de type tank.
*/
class MinionTank : public Minion {
public:
    MinionTank(int id, TileMap* map = nullptr, Castle* castle = nullptr)
        : Minion(id, map, castle, 200, 30.0f, 25) // health=200, speed=30, reward=25
    {
        Entity::init(20, sf::Color::Red, sf::Color::Black, 2);
    }
};

/**
* @Class MinionBoss
* @brief Représente un minion de type boss.
*/
class MinionBoss : public Minion {
public:
    MinionBoss(int id, TileMap* map = nullptr, Castle* castle = nullptr)
        : Minion(id, map, castle, 500, 20.0f, 100) // health=500, speed=20, reward=100
    {
        Entity::init(25, sf::Color::Magenta, sf::Color::Black, 2);
    }
};