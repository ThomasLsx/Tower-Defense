#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>


class Wave;
class Tower;
class Map;
class Player;
class Time;
class CollisionSystem;
class EconomySystem;
class ProjectileSystem;
class HealthSystem;

class Game {
private:
    std::vector<std::unique_ptr<Wave>> waves;
    std::vector<std::unique_ptr<Tower>> towers;
    std::unique_ptr<Map> map;
    std::unique_ptr<Player> player;
    std::unique_ptr<Time> time;

    std::unique_ptr<CollisionSystem> collisionSystem;
    std::unique_ptr<EconomySystem> economySystem;
    std::unique_ptr<ProjectileSystem> projectileSystem;
    std::unique_ptr<HealthSystem> healthSystem;

    int currentWaveIndex;
    bool running;

public:
    Game();
    ~Game();

    void start();
    void update(float dt);
    void spawnWave(int index);
    bool placeTower(std::unique_ptr<Tower> tower, int x, int y);
    void end();

    Player* getPlayer() const;
    Map* getMap() const;
    Time* getTime() const;
};

#endif // GAME_H
