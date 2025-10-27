// Game.h
#pragma once
#include "Window.h"
#include "UI.h"
#include "map.h"
#include <SFML/Graphics.hpp>
#include <vector>

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
        Editor
    };

private:
    void UpdatePlay(const std::vector<sf::Event>& events);

    void Render();

    void HandleInput(const std::vector<sf::Event>& events);
    void HandlePlayInput(const std::vector<sf::Event>& events);

    void showMenu();

    Window* window; // Utilisation de la classe Window
    UI* ui; // Utilisation de la classe UI
    TileMap* map; // Utilisation de la classe Map
    GameMode m_eGameMode;

    bool menuInitialized = false;
};