#pragma once
#include "Window.h"
#include "UI.h"

#include <SFML/Graphics.hpp>
#include <optional>

class Minion; // Forward declaration
struct Position;

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
    Game();
    void run();
    void processEvents();
    void update();
    void render();
    sf::Vector2f gridToPixel(Position gridPos);
    Position pixelToGrid(sf::Vector2f pixelPos);
    void drawGrid(const std::vector<std::vector<int>>& adjacencyMatrix);
private:

    Window window; // Utilisation de la classe Window
    UI* ui; // Utilisation de la classe UI
    Minion* entity;
	Minion* entity2;
    sf::Vector2f entityPosition;
    sf::FloatRect detectionZone;
    bool isRunning;

    KeyState keyState;           // Touches
    float currentSpeed = 0.f;    // Vitesse du bloc

    std::vector<std::vector<int>> adjacencyMatrix;

    // Variables de configuration chargées depuis config.json
    int playerHealth = 100;
    float enemySpeed = 1.5f;
    int towerCost = 50;
};