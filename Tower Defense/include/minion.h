#pragma once
#include "entity.h"

class Path;
struct Position;
/**
 * @class Minion
 * @brief Représente un minion dans un jeu de type Tower Defense.
 *
 * Un minion suit un chemin prédéfini, peut subir des dégâts et récompense le joueur à sa mort.
 */
class Minion : public Entity {
private:
    unsigned int health;          ///< Points de vie actuels du minion.
    size_t currentTargetIndex; ///< Progression le long du chemin (0.0 à 1.0).
    Path* targetPath;    ///< Chemin que le minion doit suivre.
    Path* targetPath;    ///< Chemin que le minion doit suivre.
    unsigned int rewardOnDeath;   ///< Récompense accordée à la mort du minion.

    std::vector<sf::Vector2f> targetPath;

public:
    /**
     * @brief Constructeur de Minion.
     * @param id Identifiant unique du minion.
     * @param pos Position initiale du minion.
     * @param path Chemin que le minion doit suivre.
     * @param health Points de vie initiaux du minion.
     * @param velocity Vitesse initiale du minion.
     * @param reward Récompense accordée à la mort du minion.
     */
    Minion(int id, /*Path* path, */ unsigned int healt = 100, unsigned int reward = 10, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), float rotation = 0.0f, sf::Color color = sf::Color::White);

    /**
     * @brief Déplace le minion en fonction de sa vitesse et du temps écoulé.
     * @param dt Temps écoulé depuis la dernière frame (en secondes).
     */
    void move(float dt);

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
     * @brief Retourne la récompense accordée à la mort du minion.
     * @return Récompense en points ou monnaie.
     */
    int getRewardOnDeath(void) const { return rewardOnDeath; }

    /**
     * @brief Retourne le chemin suivi par le minion.
     * @return Pointeur vers le chemin suivi.
     */
	Path* getTargetPath(void) const { return targetPath; }

     /**
      * @brief Appelé quand le minion est détruit (mort ou arrivée à destination).
      */
    void onDestroy(void) override;

    /**
     * @brief Met à jour le minion (déplacement, suivi de chemin, etc.).
     * @param dt Temps écoulé depuis la dernière frame (en secondes).
     */
    void update(float dt) override;
};