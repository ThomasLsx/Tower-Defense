// map.h
#pragma once
#include "Window.h"
#include "UI.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// TileMap class
class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	TileMap(sf::RenderWindow& window);
	~TileMap() {};

	bool loadTile(const std::filesystem::path& tileset, const int* tiles);
	bool loadLevel(const std::filesystem::path& levelFilePath);
	bool saveLevel(const std::filesystem::path& levelFilePath);
	void updateTile(int x, int y, int newTile, sf::Vector2u tileSize);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// Getters
	unsigned int getWidth() const { return width; }
	unsigned int getHeight() const { return height; }
	float getScale() const { return scale; }
	sf::Vector2u getTileSize() const { return tileSize; }
	const std::vector<int>& getLevel() const { return m_level; }

	// Setters
	void setLevel(const std::vector<int>& newLevel) { m_level = newLevel;  }

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

// LevelEditor

public:
	// Getters
	const int& GetTileIndex() const { return m_TileIndex; }
	const int& GetTileOptions() const { return m_TileOptions; }

	// Setters
	int& SetTileIndex() { return m_TileIndex; }

	// Level Editor methods
	void DrawMouseHover();
	void HandleLevelEditorInput(const std::vector<sf::Event>& events);

private:
	sf::RenderWindow& window;

	void CreateTileAtPosition(const sf::Vector2f& position);
	void DeleteTileAtPosition(const sf::Vector2f& position);

	// Level Editor members
	int m_TileIndex; // Index of the selected tile option
	int m_TileOptions; // Tile option for the level editor
};
