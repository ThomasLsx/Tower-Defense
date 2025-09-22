#pragma once
#include "Window.h"
#include "UI.h"
#include <SFML/Graphics.hpp>
#include <vector>

// Structure to hold the state of keys
struct KeyState {
    bool z = false;
    bool s = false;
    bool q = false;
    bool d = false;
    bool a = false;
    bool e = false;
};

// Main game class: handles window, GUI, and game logic
class Game {
public:
    Game(); // Setup game objects
	~Game(); // Cleanup
	void run(); // Main loop

    enum GameMode
    {
        Play,
		LevelEditor
    };

private:
    void UpdatePlay(const std::vector<sf::Event>& events);
	void UpdateLevelEditor(const std::vector<sf::Event>& events);

	void Render();
	void DrawLevelEditor();

	void HandleInput(const std::vector<sf::Event>& events);
	void HandlePlayInput(const std::vector<sf::Event>& events);

	// Level Editor functions
	void HandleLevelEditorInput(const std::vector<sf::Event>& events);
	void CreateTileAtPosition(const sf::Vector2f& position);
    void DeleteTileAtPosition(const sf::Vector2f& position);

    Window window; // Utilisation de la classe Window
    UI* ui; // Utilisation de la classe UI
    GameMode m_eGameMode;



	// Level Editor members
	int m_TileOptionsIndex ; // Index of the selected tile option
	sf::Texture m_TileMapTexture; // Tileset texture
	std::vector<std::unique_ptr<sf::Sprite>> m_TileOptions; // Tile option for the level editor
	std::vector<std::unique_ptr<sf::Sprite>> m_Tiles; // Tiles for the level




    sf::RectangleShape block; // Movable block
    sf::Vector2f blockPosition; // Block position
    sf::FloatRect detectionZone; // Mouse detection zone

    KeyState keyState;           // Touches
    float currentSpeed = 0.f;    // Vitesse du bloc
};
