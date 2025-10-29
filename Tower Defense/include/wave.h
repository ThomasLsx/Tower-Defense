#ifndef WAVE_H
#define WAVE_H

#include "map.h"
#include <vector>
#include <memory>

class Minion;
class TileMap;

class Wave {
private:
	int id;
    int nb_enemies;
	bool started;
    bool finished;
    std::vector<std::unique_ptr<Minion>> minions;
    float spawnTimer;       // Timer pour gérer le délai entre chaque spawn
    float spawnDelay;       // Délai entre chaque spawn (en secondes)
    size_t minionsSpawned;  // Nombre de Minions déjà spawnés
	TileMap* map;         // Pointeur vers la carte du jeu

public:
    Wave(int id, int number, TileMap* map);
    ~Wave() = default;
    void start();
	void spwanMinion();
    void update(float dt);
	void draw(sf::RenderWindow& window);
    bool isFinished() const;
    
	// Getters
    const std::vector<std::unique_ptr<Minion>>& getMinions() const;
	inline int getId() const { return id; }
};

#endif // WAVE_H