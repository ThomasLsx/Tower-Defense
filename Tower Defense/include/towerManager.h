#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

class Tower;
class Minion;
class ProjectileSystem;
class EconomySystem;

/**
 * @brief Gère l'ensemble des tours du jeu (création, mise à jour, affichage, suppression).
 */
class TowerManager
{
public:
    /**
     * @brief Énumération des différents types de tours disponibles.
     */
    enum class TowerType
    {
        Basic,  /* Tour basique. */
        Sniper, /* Tour de précision (longue portée). */
        Speed,  /* Tour rapide. */
        Slow    /* Tour de ralentissement. */
    };

    /**
     * @brief Constructeur par défaut.
     */
    TowerManager() = default;

    /**
     * @brief Destructeur par défaut.
     */
    ~TowerManager() = default;

    /**
     * @brief Ajoute une nouvelle tour à la position donnée.
     * @param position Position 2D (pixels) où placer la tour.
     * @param type Type de la tour (entier correspondant à TowerType).
     */
    void addTower(const sf::Vector2f& position, int type);

    /**
     * @brief Met à jour toutes les tours (recherche de cibles, tir, cooldowns).
     * @param minions Liste des minions ennemis présents sur la carte.
     * @param deltaTime Temps écoulé depuis la dernière frame.
     * @param projectileSystem Système de gestion des projectiles pour générer les tirs.
     */
    void updateTowers(const std::vector<std::shared_ptr<Minion>>& minions, float deltaTime, ProjectileSystem& projectileSystem);

    /**
     * @brief Dessine toutes les tours sur la fenêtre.
     * @param window La fenêtre de rendu SFML.
     */
    void drawTowers(sf::RenderWindow& window) const;

    /**
     * @brief Supprime une tour aux coordonnées de grille spécifiées et rembourse le joueur.
     * @param i Index X de la tuile sur la grille.
     * @param j Index Y de la tuile sur la grille.
     * @param tileSize Dimensions d'une tuile (en pixels).
     * @param scale Échelle de la carte.
     * @param economySystem Système économique pour effectuer le remboursement.
     */
    void removeTowerAt(unsigned int i, unsigned int j, sf::Vector2u tileSize, float scale, EconomySystem& economySystem);

    /**
     * @brief Vérifie si une tour existe déjà aux coordonnées de grille spécifiées.
     * @param i Index X de la tuile sur la grille.
     * @param j Index Y de la tuile sur la grille.
     * @param tileSize Dimensions d'une tuile (en pixels).
     * @param scale Échelle de la carte.
     * @return true si une tour est présente, false sinon.
     */
    bool isTowerAt(unsigned int i, unsigned int j, sf::Vector2u tileSize, float scale) const;

    /**
     * @brief Récupère un pointeur vers la tour située à la position donnée (en pixels).
     * @param position Position en pixels (généralement celle de la souris).
     * @param tileSize Dimensions d'une tuile (en pixels).
     * @param scale Échelle de la carte.
     * @return Pointeur vers la tour trouvée, ou nullptr si aucune tour n'est présente.
     */
    Tower* getTowerAtPosition(const sf::Vector2f& position, sf::Vector2u tileSize, float scale) const;

    /**
     * @brief Accesseur pour la liste des tours gérées.
     * @return Référence constante vers le vecteur de pointeurs uniques de tours.
     */
    const std::vector<std::unique_ptr<Tower>>& getTowers() const
    {
        return towers;
    }

private:
    /** @brief Conteneur principal stockant les tours (pointeurs uniques). */
    std::vector<std::unique_ptr<Tower>> towers;

    /** @brief Compteur utilisé pour générer des identifiants uniques pour les tours. */
    unsigned int towerIDCounter = 0;
};