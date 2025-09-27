// map.h
#pragma once
#include "Window.h"
#include "UI.h"
#include <SFML/Graphics.hpp>
#include <vector>

// Main map class
class Map
{
public:
	Map(sf::RenderWindow& window, UI* ui);
	~Map();

	// Getters
	const int& GetTileOptionsIndex() const { return m_TileOptionsIndex; }
	const sf::Texture& GetTileMapTexture() const { return m_TileMapTexture; }
	const std::vector<std::unique_ptr<sf::Sprite>>& GetTiles() { return m_Tiles; }
	const std::vector<std::unique_ptr<sf::Sprite>>& GetTileOptions() const { return m_TileOptions; }

	// Setters
	int& SetTileOptionsIndex() { return m_TileOptionsIndex; }

	// Level Editor methods
	void UpdateLevelEditor(const std::vector<sf::Event>& events);
	void DrawLevelEditor();
	void HandleLevelEditorInput(const std::vector<sf::Event>& events);

private:
	sf::RenderWindow& window;
	UI* ui;

	void LoadTileMap();

	void CreateTileAtPosition(const sf::Vector2f& position);
	void DeleteTileAtPosition(const sf::Vector2f& position);

	// Level Editor members
	int m_TileOptionsIndex; // Index of the selected tile option
	sf::Texture m_TileMapTexture; // Tileset texture
	std::vector<std::unique_ptr<sf::Sprite>> m_TileOptions; // Tile option for the level editor
	std::vector<std::unique_ptr<sf::Sprite>> m_Tiles; // Tiles for the level
};
