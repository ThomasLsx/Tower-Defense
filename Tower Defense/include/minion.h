#pragma once

#include <memory>
#include <vector>

#include "castle.h"
#include "entity.h"
#include "map.h"
#include "threadPool.h"

struct Position;

/**
 * @class Minion
 * @brief Représente un minion (ennemi) dans le jeu.
 *
 * Un minion suit un chemin prédéfini, peut subir des dégâts et récompense le joueur à sa mort.
 */
class Minion : public Entity
{
private:
    /** @brief Pointeur vers la carte du jeu. */
    TileMap* map;

    /** @brief Pointeur vers le château (objectif du minion). */
    Castle* castle;

    /** @brief Points de vie actuels du minion. */
    unsigned int health;

    /** @brief Vitesse de déplacement du minion (pixels par seconde). */
    float speed;

    /** @brief Index de la cible actuelle dans le chemin (progression). */
    size_t currentTargetIndex;

    /** @brief Quantité de cuivre accordée à la mort du minion. */
    unsigned int copper;

    /** @brief Quantité d'argent accordée à la mort du minion. */
    unsigned int silver;

    /** @brief Quantité d'or accordée à la mort du minion. */
    unsigned int gold;

    /** @brief Chemin actuel à suivre en coordonnées du monde (pixels). */
    std::vector<sf::Vector2f> targetPath;

    /** @brief Chemin sauvegardé en coordonnées du monde (pixels). */
    std::vector<sf::Vector2f> savedPath;

    /** @brief Points de vie maximum du minion. */
    unsigned int maxHealth;

    /** @brief Forme graphique de l'arrière-plan de la barre de vie. */
    sf::RectangleShape healthBarBack;

    /** @brief Forme graphique de la barre de vie. */
    sf::RectangleShape healthBar;

    /** @brief Timer pour gérer les états spéciaux (ex: ralentissement). */
    float specialStateTimer;

    /** @brief Indique si le minion nécessite une mise à jour de son chemin. */
    bool needPathUpdate = false;

public:
    /**
     * @brief Constructeur de Minion.
     * @param id Identifiant unique du minion.
     * @param map Pointeur vers la map du jeu.
     * @param castle Pointeur vers le château.
     * @param health Points de vie initiaux du minion.
     * @param speed Vitesse de déplacement.
     * @param reward Récompense de base (non utilisée si copper/silver/gold sont hardcodés).
     * @param pos Position initiale du minion.
     * @param rotation Rotation initiale.
     * @param color Couleur du minion.
     */
    Minion(int id, TileMap* map = nullptr, Castle* castle = nullptr, unsigned int health = 100, float speed = 50.0f, unsigned int reward = 10, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), float rotation = 0.0f, sf::Color color = sf::Color::White);

    /**
     * @brief Prépare le mouvement du minion (réinitialise le chemin).
     */
    void move();

    /**
     * @brief Logique de mouvement interne pour suivre le chemin.
     * @param dt Temps écoulé (delta time).
     */
    void followPath(float dt);

    /**
     * @brief Assigne un nouveau chemin au minion depuis le pathfinding.
     * @param gridPath Le chemin en coordonnées de GRILLE.
     * @param tileSize La taille d'une tuile en pixels.
     */
    void setPath(const std::vector<Position>& gridPath, float tileSize);

    /**
     * @brief Recalcule ou définit le chemin par défaut.
     */
    void setPath();

    /**
     * @brief Définit si le minion a besoin d'une mise à jour de chemin.
     * @param needUpdate true pour demander une mise à jour.
     */
    void setNeedPathUpdate(bool needUpdate)
    {
        needPathUpdate = needUpdate;
    }

    /**
     * @brief Sauvegarde le chemin actuel.
     * @param gridPath Chemin en coordonnées de grille.
     * @param tileSize Taille d'une tuile.
     */
    void savePath(const std::vector<Position>& gridPath, float tileSize);

    /**
     * @brief Inflige des dégâts au minion (gestion mort et récompenses).
     * @param amount Montant des dégâts infligés.
     */
    void takeDamage(int amount);

    /**
     * @brief Applique des dégâts (alias ou logique alternative).
     * @param amount Montant des dégâts.
     */
    void makeDamage(int amount);

    /**
     * @brief Retourne la progression du minion le long de son chemin.
     * @return L'index de la cible actuelle dans le vecteur chemin.
     */
    size_t getPathProgress(void) const
    {
        return currentTargetIndex;
    }

    /**
     * @brief Retourne les points de vie actuels du minion.
     * @return Points de vie restants.
     */
    int getHealth(void) const
    {
        return health;
    }

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
     * @brief Surcharge de update avec contexte de carte explicite.
     * @param dt Temps écoulé.
     * @param map Pointeur vers la carte.
     */
    void update(float dt, TileMap* map);

    /**
     * @brief Dessine le minion et sa barre de vie.
     * @param window Fenêtre de rendu.
     */
    void draw(sf::RenderWindow& window) override;

    /**
     * @brief Définit la vitesse de déplacement du minion.
     * @param newSpeed Nouvelle vitesse en pixels par seconde.
     */
    void setSpeed(float newSpeed)
    {
        speed = newSpeed;
    }

    /**
     * @brief Récupère la vitesse actuelle du minion.
     * @return Vitesse en pixels par seconde.
     */
    float getSpeed() const
    {
        return speed;
    }

    /**
     * @brief Retourne le chemin cible actuel du minion.
     * @return Référence constante vers le vecteur de positions cibles.
     */
    const std::vector<sf::Vector2f>& getTargetPath() const
    {
        return targetPath;
    }

    /**
     * @brief Récupère le temps restant pour l'état spécial.
     * @return Temps en secondes.
     */
    float getSpecialStateTimer() const
    {
        return specialStateTimer;
    }

    /**
     * @brief Définit le timer d'état spécial (ex: durée de ralentissement).
     * @param time Temps en secondes.
     */
    void setSpecialStateTimer(float time)
    {
        specialStateTimer = time;
    }

    /**
     * @brief Récupère la carte associée au minion.
     * @return Pointeur vers TileMap.
     */
    TileMap* getMap() const
    {
        return map;
    }
};

/**
 * @Class MinionNormal
 * @brief Représente un minion de type normal (équilibré).
 */
class MinionNormal : public Minion
{
public:
    /**
     * @brief Constructeur de MinionNormal.
     * @param id Identifiant.
     * @param map Pointeur carte.
     * @param castle Pointeur château.
     */
    MinionNormal(int id, TileMap* map = nullptr, Castle* castle = nullptr)
        : Minion(id, map, castle, 100, 50.0f, 10)
    {
        Entity::init(15, sf::Color::Green, sf::Color::Black, 2);
    }
};

/**
 * @Class MinionFast
 * @brief Représente un minion de type rapide mais fragile.
 */
class MinionFast : public Minion
{
public:
    /**
     * @brief Constructeur de MinionFast.
     * @param id Identifiant.
     * @param map Pointeur carte.
     * @param castle Pointeur château.
     */
    MinionFast(int id, TileMap* map = nullptr, Castle* castle = nullptr)
        : Minion(id, map, castle, 75, 100.0f, 15)
    {
        Entity::init(15, sf::Color::Blue, sf::Color::Black, 2);
    }
};

/**
 * @Class MinionTank
 * @brief Représente un minion de type tank (lent et résistant).
 */
class MinionTank : public Minion
{
public:
    /**
     * @brief Constructeur de MinionTank.
     * @param id Identifiant.
     * @param map Pointeur carte.
     * @param castle Pointeur château.
     */
    MinionTank(int id, TileMap* map = nullptr, Castle* castle = nullptr)
        : Minion(id, map, castle, 200, 30.0f, 25)
    {
        Entity::init(20, sf::Color::Red, sf::Color::Black, 2);
    }
};

/**
 * @Class MinionBoss
 * @brief Représente un minion de type boss (très résistant).
 */
class MinionBoss : public Minion
{
public:
    /**
     * @brief Constructeur de MinionBoss.
     * @param id Identifiant.
     * @param map Pointeur carte.
     * @param castle Pointeur château.
     */
    MinionBoss(int id, TileMap* map = nullptr, Castle* castle = nullptr)
        : Minion(id, map, castle, 500, 20.0f, 100)
    {
        Entity::init(25, sf::Color::Magenta, sf::Color::Black, 2);
    }
};