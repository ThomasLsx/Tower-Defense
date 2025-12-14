// map.h
#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "path.h"

class TowerManager;
class Window;
class UI;
class EconomySystem;

/**
 * @brief TileMap gère la grille, le chargement des niveaux, l'affichage et l'édition de niveau.
 */
class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    /**
     * @brief Constructeur de la TileMap.
     * @param window Référence vers la fenêtre de rendu SFML.
     */
    TileMap(sf::RenderWindow& window);

    /**
     * @brief Destructeur.
     */
    ~TileMap()
    {
    }

    /**
     * @brief Charge le tileset et initialise les vertices.
     * @param tileset Chemin vers l'image du tileset.
     * @param tiles Tableau d'entiers représentant les tuiles.
     * @return true si le chargement a réussi, false sinon.
     */
    bool loadTile(const std::filesystem::path& tileset, const int* tiles);

    /**
     * @brief Charge un niveau depuis un fichier texte.
     * @param levelFilePath Chemin vers le fichier de niveau.
     * @return true si le chargement a réussi.
     */
    bool loadLevel(const std::filesystem::path& levelFilePath);

    /**
     * @brief Sauvegarde le niveau actuel dans un fichier texte.
     * @param levelFilePath Chemin vers le fichier de sauvegarde.
     * @return true si la sauvegarde a réussi.
     */
    bool saveLevel(const std::filesystem::path& levelFilePath);

    /**
     * @brief Met à jour une tuile spécifique de la carte.
     * @param x Coordonnée X de la tuile.
     * @param y Coordonnée Y de la tuile.
     * @param newTile Nouvel index de la tuile.
     * @param tileSize Taille de la tuile.
     */
    void updateTile(int x, int y, int newTile, sf::Vector2u tileSize);

    /**
     * @brief Trouve une case accessible proche pour débloquer une entité.
     */
    std::optional<Position> getNearestAccessibleTile(Position currentPos) const
    {
        return m_pathfinding.getNearestValidPosition(currentPos, 5);
    }

    /**
     * @brief Vérifie si la carte a été modifiée.
     * @return true si modifiée, false sinon.
     */
    inline bool hasMapChanged() const
    {
        return mapChanged;
    }

    /**
     * @brief Dessine la carte sur la cible de rendu.
     * @param target Cible de rendu.
     * @param states États de rendu.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /**
     * @brief Dessine le chemin (pathfinding) pour le débogage ou la visualisation.
     */
    void drawPath();

    /**
     * @brief Convertit les coordonnées de grille en position pixel (monde).
     * @param tile Coordonnées de la tuile (x, y).
     * @return Position centrale de la tuile en pixels.
     */
    sf::Vector2f Tile2Position(const sf::Vector2u& tile) const;

    /**
     * @brief Permet à un minion de savoir vers quelle case aller.
     * Appelle en interne la méthode getNextMove du pathfinding.
     * @param currentPos Position actuelle du minion.
     * @return La prochaine position recommandée, si elle existe.
     */
    std::optional<Position> getNextDirection(Position currentPos) const;

    /**
     * @brief Initialise ou met à jour le champ de vecteurs (Flow Field) pour le pathfinding.
     */
    void updateFlowField();

    // Getters

    /**
     * @brief Récupère la largeur de la carte en nombre de tuiles.
     * @return Largeur de la carte.
     */
    inline unsigned int getWidth() const
    {
        return width;
    }

    /**
     * @brief Récupère la hauteur de la carte en nombre de tuiles.
     * @return Hauteur de la carte.
     */
    inline unsigned int getHeight() const
    {
        return height;
    }

    /**
     * @brief Récupère l'échelle d'affichage de la carte.
     * @return L'échelle.
     */
    inline float getScale() const
    {
        return scale;
    }

    /**
     * @brief Récupère la taille d'une tuile (en pixels).
     * @return Dimensions de la tuile.
     */
    inline sf::Vector2u getTileSize() const
    {
        return tileSize;
    }

    /**
     * @brief Récupère le vecteur brut du niveau (visuel).
     * @return Référence constante vers le vecteur de tuiles.
     */
    inline const std::vector<int>& getLevel() const
    {
        return m_level;
    }

    /**
     * @brief Récupère le niveau sous forme de tableau 2D (visuel).
     * @return Vecteur de vecteurs d'entiers.
     */
    const std::vector<std::vector<int>> getLevel2D() const;

    /**
     * @brief Récupère le vecteur brut du niveau logique (collisions/tours).
     * @return Référence constante vers le vecteur.
     */
    inline const std::vector<int>& getTowerLevel() const
    {
        return m_towerLevel;
    }

    /**
     * @brief Récupère le niveau logique sous forme de tableau 2D.
     * @return Vecteur de vecteurs d'entiers.
     */
    const std::vector<std::vector<int>> getTowerLevel2D() const;

    /**
     * @brief Récupère les coordonnées de la tuile sous une position monde donnée.
     * @param position Position en pixels.
     * @return Coordonnées de la tuile (x, y).
     */
    const sf::Vector2u getCurentTile(sf::Vector2f position) const;

    /**
     * @brief Trouve la première tuile correspondant à une valeur donnée.
     * @param value Valeur de la tuile à chercher.
     * @return Coordonnées de la tuile trouvée.
     */
    sf::Vector2u findEdgeTile(int value) const;

    /**
     * @brief Récupère la position du château (tuile de valeur 3).
     * @return Coordonnées de la tuile du château.
     */
    inline const sf::Vector2u getCastleTile() const
    {
        return findEdgeTile(3);
    }

    /**
     * @brief Récupère la position du spawn (tuile de valeur 0).
     * @return Coordonnées de la tuile de spawn.
     */
    inline const sf::Vector2u getSpawnTile() const
    {
        return findEdgeTile(0);
    }

    /**
     * @brief Récupère les coordonnées de la dernière tuile modifiée.
     * @return Coordonnées de la tuile.
     */
    inline const sf::Vector2u& getLastModifiedTile() const
    {
        return lastModifiedTile;
    }

    // Setters

    /**
     * @brief Définit le niveau visuel.
     * @param newLevel Nouveau vecteur de tuiles.
     */
    inline void setLevel(const std::vector<int>& newLevel)
    {
        m_level = newLevel;
    }

    /**
     * @brief Définit l'état de modification de la carte.
     * @param changed true si la carte a changé.
     */
    inline void setMapChanged(bool changed)
    {
        mapChanged = changed;
    }

    // Debug

    /**
     * @brief Affiche les tuiles dans la console pour le débogage.
     */
    void printTiles() const;

public:
    // Level editor accessors and methods

    /**
     * @brief Récupère l'index de la tuile sélectionnée dans l'éditeur.
     * @return Index de la tuile.
     */
    inline const int& GetTileIndex() const
    {
        return m_TileIndex;
    }

    /**
     * @brief Récupère le nombre d'options de tuiles disponibles.
     * @return Nombre d'options.
     */
    inline const int& GetTileOptions() const
    {
        return m_TileOptions;
    }

    /**
     * @brief Récupère une référence modifiable de l'index de tuile (pour l'éditeur).
     * @return Référence vers l'index.
     */
    inline int& SetTileIndex()
    {
        return m_TileIndex;
    }

    /**
     * @brief Dessine un aperçu de la tuile sous la souris en mode éditeur.
     */
    void DrawMouseHover();

    /**
     * @brief Gère les entrées utilisateur spécifiques à l'éditeur de niveau.
     * @param events Liste des événements SFML.
     */
    void HandleLevelEditorInput(const std::vector<sf::Event>& events);

    // Tower placement methods

    /**
     * @brief Récupère l'index du type de tour sélectionné.
     * @return Index de la tour.
     */
    inline const int& GetTowerIndex() const
    {
        return m_TowerIndex;
    }

    /**
     * @brief Récupère une référence modifiable de l'index de tour.
     * @return Référence vers l'index.
     */
    inline int& SetTowerIndex()
    {
        return m_TowerIndex;
    }

    /**
     * @brief Gère les entrées utilisateur pour le placement des tours.
     * @param events Liste des événements SFML.
     * @param towerManager Gestionnaire de tours.
     * @param economySystem Système économique.
     */
    void HandleTowerInput(const std::vector<sf::Event>& events, TowerManager& towerManager, EconomySystem& economySystem);

    /**
     * @brief Place une tour à la position donnée.
     * @param position Position de la souris (monde).
     * @param towerManager Gestionnaire de tours.
     * @param economySystem Système économique.
     */
    void PlaceTower(const sf::Vector2f& position, TowerManager& towerManager, EconomySystem& economySystem);

    /**
     * @brief Supprime une tour à la position donnée.
     * @param position Position de la souris (monde).
     * @param towerManager Gestionnaire de tours.
     * @param economySystem Système économique.
     */
    void RemoveTower(const sf::Vector2f& position, TowerManager& towerManager, EconomySystem& economySystem);

public:
    /** @brief Index de la tour sélectionnée (Basic, Sniper, etc.). */
    int m_TowerIndex;

    /** @brief Nombre de types de tours disponibles. */
    int m_TowerOptions;

private:
    /** @brief Largeur de la carte (en tuiles). */
    unsigned int width;

    /** @brief Hauteur de la carte (en tuiles). */
    unsigned int height;

    /** @brief Échelle d'affichage. */
    float scale;

    /** @brief Dimensions d'une tuile (en pixels). */
    sf::Vector2u tileSize;

    /** @brief Tableau de sommets pour le rendu optimisé des tuiles. */
    sf::VertexArray m_vertices;

    /** @brief Texture contenant l'ensemble des tuiles (tileset). */
    sf::Texture m_tileset;

    /** @brief Vecteur stockant les données du niveau (visuel). */
    std::vector<int> m_level;

    /** @brief Vecteur stockant les données logiques (collisions, tours). */
    std::vector<int> m_towerLevel;

    /** @brief Indique si la carte a été modifiée depuis le dernier update. */
    bool mapChanged = false;

    /** @brief Référence vers la fenêtre de rendu. */
    sf::RenderWindow& window;

    /** @brief Instance du pathfinding pour la gestion des chemins. */
    Pathfinding m_pathfinding;

    /**
     * @brief Crée (ou modifie) une tuile à la position donnée (Mode Éditeur).
     * @param position Position de la souris.
     */
    void CreateTileAtPosition(const sf::Vector2f& position);

    /**
     * @brief Supprime (met à 0) une tuile à la position donnée (Mode Éditeur).
     * @param position Position de la souris.
     */
    void DeleteTileAtPosition(const sf::Vector2f& position);

    /** @brief Index de la tuile sélectionnée pour le mode éditeur. */
    int m_TileIndex;

    /** @brief Nombre total d'options de tuiles dans le tileset. */
    int m_TileOptions;

    /** @brief Coordonnées de la dernière tuile modifiée (pour optimiser le recalcul). */
    sf::Vector2u lastModifiedTile;
};