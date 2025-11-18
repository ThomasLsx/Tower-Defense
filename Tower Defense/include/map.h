// map.h
#pragma once
#include "Window.h"
#include "UI.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

/**
 * @brief TileMap g�re la grille, le chargement des niveaux et l'�dition de niveau.
 */
class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap(sf::RenderWindow& window);
    ~TileMap() {};

    bool loadTile(const std::filesystem::path& tileset, const int* tiles);
    bool loadLevel(const std::filesystem::path& levelFilePath);
    bool saveLevel(const std::filesystem::path& levelFilePath);
    void updateTile(int x, int y, int newTile, sf::Vector2u tileSize);
    inline bool hasMapChanged() const { return mapChanged; }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Vector2f Tile2Position(const sf::Vector2u& tile) const;


    // Getters
    inline unsigned int getWidth() const { return width; }
    inline unsigned int getHeight() const { return height; }
    inline float getScale() const { return scale; }
    inline sf::Vector2u getTileSize() const { return tileSize; }
    inline const std::vector<int>& getLevel() const { return m_level; }
    const std::vector<std::vector<int>> getLevel2D() const;
    const sf::Vector2u getCurentTile(sf::Vector2f position) const;
    sf::Vector2u findEdgeTile(int value) const;
    inline const sf::Vector2u getCastleTile() const { return findEdgeTile(7);}
    inline const sf::Vector2u getSpawnTile() const { return findEdgeTile(4); }

    // Setters
    inline void setLevel(const std::vector<int>& newLevel) { m_level = newLevel; }
    inline void setMapChanged(bool changed) { mapChanged = changed; }

    // Debug
    void printTiles() const;

private:
    unsigned int width;
    unsigned int height;
    float scale;
    sf::Vector2u tileSize;
    sf::VertexArray m_vertices;
    sf::Texture     m_tileset;
    std::vector<int> m_level;
    bool mapChanged = false;

public:
    // Level editor accessors and methods
    inline const int& GetTileIndex() const { return m_TileIndex; }
    inline const int& GetTileOptions() const { return m_TileOptions; }

    inline int& SetTileIndex() { return m_TileIndex; }

    void DrawMouseHover();
    void HandleLevelEditorInput(const std::vector<sf::Event>& events);

private:
    sf::RenderWindow& window;

    void CreateTileAtPosition(const sf::Vector2f& position);
    void DeleteTileAtPosition(const sf::Vector2f& position);

    int m_TileIndex;
    int m_TileOptions;
};
