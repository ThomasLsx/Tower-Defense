// Game.h
#pragma once
#include "Window.h"
#include "UI.h"
#include "map.h"
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

	void Render();

	void HandleInput(const std::vector<sf::Event>& events);
	void HandlePlayInput(const std::vector<sf::Event>& events);

    Window* window; // Utilisation de la classe Window
    UI* ui; // Utilisation de la classe UI
    Map* map; // Utilisation de la classe Map
    GameMode m_eGameMode;

    sf::RectangleShape block; // Movable block
    sf::Vector2f blockPosition; // Block position
    sf::FloatRect detectionZone; // Mouse detection zone

    KeyState keyState;           // Touches
    float currentSpeed = 0.f;    // Vitesse du bloc
};
