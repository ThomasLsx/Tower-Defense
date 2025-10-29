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
class WaveManager;

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
    void Render();
    void HandleInput(const std::vector<sf::Event>& events);
    
    GameMode m_eGameMode;
    Window* window; // Utilisation de la classe Window
    UI* ui; // Utilisation de la classe UI
    TileMap* map; // Utilisation de la classe Map
	WaveManager* waveManager; // Utilisation de la classe WaveManager
};