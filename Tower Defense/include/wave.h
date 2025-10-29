#ifndef WAVE_H
#define WAVE_H

#include "map.h"
#include <vector>
#include <memory>
#include <string>

class Minion;
class TileMap;

/**
 * @brief Constructeur de Wave
 * @param id Numéro de la vague
 * @param nb_enemies Nombre d'ennemis dans la vague
 * @param map Pointeur vers la carte du jeu
 * @param delay Délai entre chaque spawn de Minion (en secondes)
 */
class Wave {
private:
    struct MinionGroupConfig {
        std::string type;
        int count;
    };
    int id;
    int nb_enemies;
    bool started;
    bool finished;
    std::vector<std::unique_ptr<Minion>> minions;
    float spawnTimer;       // Timer pour gérer le délai entre chaque spawn
    float spawnDelay;       // Délai entre chaque spawn (en secondes)
    size_t minionsSpawned;  // Nombre de Minions déjà spawnés
    TileMap* map;         // Pointeur vers la carte du jeu
    std::vector<MinionGroupConfig> minionGroups; // Ajouté pour la config
    size_t minionGroupIndex = 0; // Pour suivre le groupe courant
    size_t minionInGroupSpawned = 0; // Pour suivre le nombre spawné dans le groupe courant

public:
    Wave(int id, int number, TileMap* map);
    ~Wave() = default;
    void startWave();
    void spwanMinion();
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void waveFinish();
    void addMinionGroup(const std::string& type, int count);
    void addEnemies(int count); 

    // Getters
    const std::vector<std::unique_ptr<Minion>>& getMinions() const;
    inline int getWaveId() const { return id; }
    inline bool isStarted() const { return started; }
    inline bool isFinished() const { return finished; }
};

class WaveManager {
private:
    std::vector<std::unique_ptr<Wave>> waves;
    int currentWaveIndex;
	std::string waveFile;
	TileMap* map;

public:
    WaveManager(std::string waveFile, TileMap* map);
    ~WaveManager() = default;

    void loadWavesFromFile(const std::string& filename, TileMap* map);
    Wave* getCurrentWave();
    void nextWave();
    void startCurrentWave();
    void startOrNextWave();
    int getCurrentWaveId();
    void update(float dt);
    void draw(sf::RenderWindow& window);
};

#endif // WAVE_H