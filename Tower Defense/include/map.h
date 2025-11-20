// map.h
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem> // Ajout pour std::filesystem::path

class TowerManager;
class Window;
class UI;

/**
 * @brief TileMap gère la grille, le chargement des niveaux et l'édition de niveau.
 */
class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap(sf::RenderWindow& window);
    ~TileMap() {};

    bool loadTile(const std::filesystem::path& tileset, const int* tiles);
    bool loadLevel(const std::filesystem::path& levelFilePath);
    bool saveLevel(const std::filesystem::path& levelFilePath);
    void updateTileEditor(int x, int y, int newTile, sf::Vector2u tileSize);
    // void updateTileTower(int x, int y, int newTile, sf::Vector2u tileSize);

    bool hasMapChanged() const { return mapChanged; }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // Getters
    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }
    float getScale() const { return scale; }
    sf::Vector2u getTileSize() const { return tileSize; }
    const std::vector<int>& getLevel() const { return m_level; }
    const std::vector<std::vector<int>> getLevel2D() const;
	const std::vector<int>& getTowerLevel() const { return m_towerLevel; }
	const std::vector<std::vector<int>> getTowerLevel2D() const;
    const sf::Vector2u getCurentTile(sf::Vector2f position) const;

    // Setters
    void setLevel(const std::vector<int>& newLevel) { m_level = newLevel; }
    void setMapChanged(bool changed) { mapChanged = changed; }

    // Debug
    void printTiles() const;

public:
    // Level editor accessors and methods
    const int& GetTileIndex() const { return m_TileIndex; }
    const int& GetTileOptions() const { return m_TileOptions; }
    sf::Vector2u findEdgeTile(int value) const;

    int& SetTileIndex() { return m_TileIndex; }

    void DrawMouseHover();
    void HandleLevelEditorInput(const std::vector<sf::Event>& events);

    void HandleTowerInput(const std::vector<sf::Event>& events, TowerManager& towerManager);
    void PlaceTower(const sf::Vector2f& position, TowerManager& towerManager);
    void RemoveTower(const sf::Vector2f& position, TowerManager& towerManager);

public:
    int m_TowerIndex; // Index de la tour sélectionnée (Basic, Sniper, etc.)
    int m_TowerOptions; // Nombre de types de tours (ex: 3)

private:
    unsigned int width;
    unsigned int height;
    float scale;
    sf::Vector2u tileSize;
    sf::VertexArray m_vertices;
    sf::Texture     m_tileset;
    std::vector<int> m_level;
	std::vector<int> m_towerLevel;
    bool mapChanged = false;

    sf::RenderWindow& window;

    void CreateTileAtPosition(const sf::Vector2f& position);
    void DeleteTileAtPosition(const sf::Vector2f& position);

    int m_TileIndex;
    int m_TileOptions;
};
