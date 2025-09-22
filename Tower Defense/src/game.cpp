#include "game.h"

#include "wave.h"
#include "tower.h"
#include "map.h"
#include "player.h"
#include "time.h"
#include "collisionSystem.h"
#include "economySystem.h"
#include "projectileSystem.h"

Game::Game()
    : currentWaveIndex(0), running(false)
{
    map = std::make_unique<Map>();
    player = std::make_unique<Player>();
    time = std::make_unique<Time>();

    collisionSystem = std::make_unique<CollisionSystem>();
    economySystem = std::make_unique<EconomySystem>(player.get());
    projectileSystem = std::make_unique<ProjectileSystem>();
}

Game::~Game() {
    end();
}

void Game::start() {
    running = true;
    currentWaveIndex = 0;
    // Init première vague si nécessaire
    if (!waves.empty()) {
        waves[currentWaveIndex]->start();
    }
}

void Game::update(float dt) {
    if (!running) return;

    time->tick(dt);

    // Update des tours (ciblage, tirs)
    for (auto& tower : towers) {
        // TODO: implémenter tower.update()
    }

    // Update des systèmes
    projectileSystem->update(dt);
    collisionSystem->update(dt); 
    economySystem->update(dt);

    // Update de la vague en cours
    if (currentWaveIndex < waves.size()) {
        waves[currentWaveIndex]->update(dt);

        if (waves[currentWaveIndex]->isFinished()) {
            currentWaveIndex++;
            if (currentWaveIndex < waves.size()) {
                waves[currentWaveIndex]->start();
            }
            else {
                // Victoire !
                end();
            }
        }
    }

    // TODO: nettoyage des entités mortes si nécessaire
}

void Game::spawnWave(int index) {
    if (index >= 0 && index < waves.size()) {
        waves[index]->start();
        currentWaveIndex = index;
    }
}

bool Game::placeTower(std::unique_ptr<Tower> tower, int x, int y) {
    if (!map->isWalkable(x, y)) return false;
    // Vérifier ressources joueur via EconomySystem
    if (!economySystem->purchase(100)) return false; // ex coût fixe
    towers.push_back(std::move(tower));
    return true;
}

void Game::end() {
    running = false;
    // Nettoyage si nécessaire
}

Player* Game::getPlayer() const { return player.get(); }
Map* Game::getMap() const { return map.get(); }
Time* Game::getTime() const { return time.get(); }
