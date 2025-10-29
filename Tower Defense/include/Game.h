// Game.h
#pragma once
#include "Window.h"
#include "UI.h"
#include "map.h"
#include "wave.h"
#include "path.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Window;
class UI;
class TileMap;
class Wave;

// Main game class: handles window, GUI, and game logic
class Game {
public:
    Game(); // Setup game objects
    ~Game(); // Cleanup
    void run(); // Main loop

    enum GameMode
    {
        Menu,
        Play,
        Pause,
        Editor
    };

	// Getters
	GameMode getGameMode() const { return m_eGameMode; }

	// Setters
	void setGameMode(GameMode mode) { m_eGameMode = mode; }

private:
    void UpdatePlay(const std::vector<sf::Event>& events);

    void Render();

    void HandleInput(const std::vector<sf::Event>& events);
    void HandlePlayInput(const std::vector<sf::Event>& events);
    
    GameMode m_eGameMode;
    Window* window; // Utilisation de la classe Window
    UI* ui; // Utilisation de la classe UI
    TileMap* map; // Utilisation de la classe Map
    Wave* wave;
};