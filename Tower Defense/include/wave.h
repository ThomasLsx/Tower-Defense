#ifndef WAVE_H
#define WAVE_H

#include <vector>
#include <memory>

class Minion;

class Wave {
private:
    int waveNumber;
    bool finished;
    std::vector<std::unique_ptr<Minion>> minions;
    float spawnTimer;       // Timer pour g�rer le d�lai entre chaque spawn
    float spawnDelay;       // D�lai entre chaque spawn (en secondes)
    size_t minionsSpawned;  // Nombre de Minions d�j� spawn�s

public:
    Wave(int number, float delay = 1.0f);
    ~Wave() = default;
    void spawn();
    void start();
    void update(float dt);
    bool isFinished() const;
    const std::vector<std::unique_ptr<Minion>>& getMinions() const;

private:
    void trySpawnMinion();
};

#endif // WAVE_H
