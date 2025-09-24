#ifndef WAVE_H
#define WAVE_H

#include <vector>
#include <memory>

class Minion;
class Spawner;

class Wave {
private:
    int waveNumber;
    bool finished;
    std::vector<std::unique_ptr<Minion>> minions;

public:
    Wave(int number);
    ~Wave();

    void spawn();
    void start();
    void update(float dt);
    bool isFinished() const;

    const std::vector<std::unique_ptr<Minion>>& getMinions() const;
};

#endif // WAVE_H
