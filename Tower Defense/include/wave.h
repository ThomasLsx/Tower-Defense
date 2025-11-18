 #ifndef WAVE_H
#define WAVE_H

#include "map.h"
#include <vector>
#include <memory>
#include <string>

class Minion;
class TileMap;

/**
 * @brief Représente une vague d'ennemis.
 *
 * La Wave possède ses minions mais expose des `std::shared_ptr<Minion>` afin que
 * d'autres systèmes (tours, projectiles, UI) puissent observer ou référencer
 * les minions en toute sécurité.
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

    /// Minions détenus par la vague (partagés avec d'autres systèmes)
    std::vector<std::shared_ptr<Minion>> minions;

    float spawnTimer;
    float spawnDelay;
    size_t minionsSpawned;
    TileMap* map;
    std::vector<MinionGroupConfig> minionGroups;
    size_t minionGroupIndex = 0;
    size_t minionInGroupSpawned = 0;

public:
    explicit Wave(int id, int number, TileMap* map);
    ~Wave() = default;

    /** Démarre la vague et initialise le timer. */
    void startWave();

    /** Crée un minion selon la configuration courante du groupe. */
    void spwanMinion();

    /** Met à jour la vague et ses minions. */
    void update(float dt);

    /** Dessine les minions actifs. */
    void draw(sf::RenderWindow& window);

    /** Marque la vague comme terminée. */
    void waveFinish();

    void addMinionGroup(const std::string& type, int count);
    void addEnemies(int count);

    /** Retourne la liste des minions. */
    const std::vector<std::shared_ptr<Minion>>& getMinions() const { return minions; }

    inline int getWaveId() const { return id; }
    inline bool isStarted() const { return started; }
    inline bool isFinished() const { return finished; }
};

/**
 * @brief Gestionnaire de vagues.
 *
 * Contient les vagues (propriétaires via unique_ptr) et gère la progression.
 */
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