#pragma once
#include "Window.h"
#include "UI.h"
#include <SFML/Graphics.hpp>

// Structure to hold the state of keys
struct KeyState {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool a = false;
    bool e = false;
};

// Main game class: handles window, GUI, and game logic
class Game {
public:
    Game(); // Setup game objects
    void run(); // Main loop
private:
    void processEvents(); // Handle input/events
    void update();        // Update game state
    void render();        // Draw everything

    Window window; // Utilisation de la classe Window
    UI* ui; // Utilisation de la classe UI
    sf::RectangleShape block; // Movable block
    sf::Vector2f blockPosition; // Block position
    sf::FloatRect detectionZone; // Mouse detection zone
    bool isRunning;              // Main loop flag

    KeyState keyState;           // Touches
    float currentSpeed = 0.f;    // Vitesse du bloc

    // Variables de configuration chargées depuis config.json
    int playerHealth = 100;
    float enemySpeed = 1.5f;
    int towerCost = 50;
};
