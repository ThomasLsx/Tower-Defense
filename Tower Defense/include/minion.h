#pragma once
#include "entity.h"

class Path;

/**
 * @class Minion
 * @brief Représente un minion dans un jeu de type Tower Defense.
 *
 * Un minion suit un chemin prédéfini, peut subir des dégâts et récompense le joueur à sa mort.
 */
class Minion : public Entity {
private:
    unsigned int health;          ///< Points de vie actuels du minion.
    sf::Vector2f velocity;    ///< Vitesse et direction actuelle du minion.
    float pathProgress;  ///< Progression le long du chemin (0.0 à 1.0).
    Path* targetPath;    ///< Chemin que le minion doit suivre.
    unsigned int rewardOnDeath;   ///< Récompense accordée à la mort du minion.

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
    Minion(int id, Path* path, unsigned int healt = 100, sf::Vector2f velocity = sf::Vector2f(0.0f,0.0f), unsigned int reward = 10, sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f), float rotation = 0.0f, sf::Color color = sf::Color::White, sf::IntRect textureRect = sf::IntRect(), const sf::Texture& texture = sf::Texture());

    /**
     * @brief Déplace le minion en fonction de sa vitesse et du temps écoulé.
     * @param dt Temps écoulé depuis la dernière frame (en secondes).
     */
    void move(float dt);

    /**
     * @brief Met à jour la position du minion pour qu'il suive son chemin.
     */
    void followPath(void);

    /**
     * @brief Inflige des dégâts au minion.
     * @param amount Montant des dégâts infligés.
     * @param sourceId ID de l'entité source des dégâts (ex: ID de la tour).
     */
    void takeDamage(int amount);

    void makeDamage(int amount);

    /**
     * @brief Retourne la vitesse actuelle du minion.
     * @return Vecteur de vitesse du minion.
     */
    sf::Vector2f getVelocity(void) const { return velocity; }

    /**
     * @brief Retourne la progression du minion le long de son chemin.
     * @return Valeur entre 0.0 et 1.0 représentant la progression.
     */
    float getPathProgress(void) const { return pathProgress; }

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
