// Game.h
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

class Entity;
struct Position;
class TileMap;
class Window;
class UI;


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

    void mv_minion(void);

	void HandleInput(const std::vector<sf::Event>& events);
	void HandlePlayInput(const std::vector<sf::Event>& events);

	void showMenu();

    Window* window; // Utilisation de la classe Window
    UI* ui; // Utilisation de la classe UI
    TileMap* map; // Utilisation de la classe Map
	Minion* minion; // Utilisation de la classe Minion
    GameMode m_eGameMode;

    Entity* mimi;

    bool menuInitialized = false;
};
