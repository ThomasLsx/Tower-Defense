#include "Wave.h"
#include "Minion.h"
#include <fstream>
#include <sstream>
#include <iostream>


Wave::Wave(int id, int nb_enemies, TileMap* map)
    : id(id), nb_enemies(nb_enemies), started(false), finished(false),
      spawnTimer(0.0f), spawnDelay(1.0f), minionsSpawned(0), map(map), minionGroupIndex(0), minionInGroupSpawned(0)
{
}

/**
 * @brief Démarre la vague (initialise le timer)
 */
void Wave::startWave()
{
    if (started) {
        return;
    }
    started = true;
    finished = false;
    minionsSpawned = 0;
    minions.clear();
    minionGroupIndex = 0;
    minionInGroupSpawned = 0;
}

void Wave::spwanMinion()
{
    if (minionsSpawned < nb_enemies && minionGroupIndex < minionGroups.size()) {
        const auto& group = minionGroups[minionGroupIndex];
        std::string type = group.type;
        // Création du bon type de minion selon le type
        if (type == "Normal") {
            minions.push_back(std::make_unique<MinionNormal>(minionsSpawned, map));
        } else if (type == "Fast" || type == "fast") {
            minions.push_back(std::make_unique<MinionFast>(minionsSpawned, map));
        } else if (type == "Tank") {
            minions.push_back(std::make_unique<MinionTank>(minionsSpawned, map));
        } else if (type == "Boss") {
            minions.push_back(std::make_unique<MinionBoss>(minionsSpawned, map));
        } else {
            minions.push_back(std::make_unique<MinionNormal>(minionsSpawned, map));
        }
        float tile = map->getTileSize().x * map->getScale();
        sf::Vector2u spawnTile = map->findEdgeTile(7);
        minions.back()->setPosition(sf::Vector2f(spawnTile.x * tile + tile / 2, spawnTile.y * tile + tile / 2));
        minions.back()->move();
        ++minionsSpawned;
        ++minionInGroupSpawned;
        if (minionInGroupSpawned >= group.count) {
            minionGroupIndex++;
            minionInGroupSpawned = 0;
        }
    }
}

/**
 * @brief Met à jour l'état de la vague et des Minions
 * @param dt Delta time (temps écoulé depuis la dernière frame)
 */
void Wave::update(float dt)
{
	// Gérer le spawn des Minions
    if (isStarted()) {
        spawnTimer += dt;
        if (spawnTimer >= spawnDelay) {
            spwanMinion();
            spawnTimer = 0.0f;
        }
    }
        
	// Met à jour chaque Minion 
    for (auto& minion : minions) {
        minion->update(dt * minion->getSpeed());
        if (map->hasMapChanged()) {
            minion->move();
            map->setMapChanged(false);
        }
    }

	// Vérifie si la wave est terminée
    if (!isFinished()) {
        if (minionsSpawned == nb_enemies) {
            bool allDeadOrArrived = true;
            for (const auto& minion : minions) {
                if (minion->getIsAlive()) {
                    allDeadOrArrived = false;
                    break;
                }
            }
            if (allDeadOrArrived) {
                waveFinish();
            }
        }
    }
}

/**
 * @brief Dessine les Minions de la vague sur la fenêtre
 * @param window Référence vers la fenêtre de rendu SFML
 */
void Wave::draw(sf::RenderWindow& window)
{
    for (const auto& minion : minions) {
        minion->draw(window);
    }
}

/**
 * @brief Indique si la vague est terminée
 * @return true si tous les Minions sont morts ou arrivés, false sinon
 */
void Wave::waveFinish() 
{
    started = false;
    finished = true;
    std::cout << "[DEBUG] waveFinish called for wave id=" << id << "\n";
    return;
}

void Wave::addMinionGroup(const std::string& type, int count) {
    minionGroups.push_back({ type, count });
}

void Wave::addEnemies(int count)
{
    nb_enemies += count;
}


WaveManager::WaveManager(std::string waveFile, TileMap* map) 
	: currentWaveIndex(0), waveFile(waveFile), map(map)
{
	loadWavesFromFile(waveFile, map);
}

void WaveManager::loadWavesFromFile(const std::string& filename, TileMap* map)
{
    waves.clear();
    std::ifstream file(filename);
    std::string line;
    int currentWaveId = -1;
    Wave* currentWave = nullptr;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        int waveId, count;
        std::string type, sep;
        std::getline(iss, sep, ';'); waveId = std::stoi(sep);
        std::getline(iss, type, ';');
        std::getline(iss, sep, ';'); count = std::stoi(sep);
        if (waveId != currentWaveId) {
            waves.push_back(std::make_unique<Wave>(waveId, 0, map));
            currentWave = waves.back().get();
            currentWaveId = waveId;
        }
        currentWave->addMinionGroup(type, count);
        currentWave->addEnemies(count);
    }
    std::cout << "[DEBUG] Total waves loaded: " << waves.size() << "\n";
}

Wave* WaveManager::getCurrentWave() {
    if (waves.empty() || currentWaveIndex < 0 || currentWaveIndex >= (int)waves.size())
        return nullptr;
    return waves[currentWaveIndex].get();
}

void WaveManager::nextWave() {
    std::cout << "[DEBUG] nextWave: currentWaveIndex before=" << currentWaveIndex << "\n";
    if (currentWaveIndex + 1 < (int)waves.size())
        ++currentWaveIndex;
    std::cout << "[DEBUG] nextWave: currentWaveIndex after=" << currentWaveIndex << "\n";
}

void WaveManager::startCurrentWave()
{
    Wave* wave = getCurrentWave();
    if (wave && !wave->isStarted()) {
        wave->startWave();
	}
}

void WaveManager::startOrNextWave() {
    Wave* currentWave = this->getCurrentWave();
    if (!currentWave) return;
    std::cout << "[DEBUG] startOrNextWave: waveId=" << currentWave->getWaveId()
              << " started=" << currentWave->isStarted()
              << " finished=" << currentWave->isFinished() << "\n";
    if (currentWave->isFinished()) {
        this->nextWave();
        Wave* next = this->getCurrentWave();
        if (next && !next->isStarted() && !next->isFinished())
            next->startWave();
    } else if (!currentWave->isStarted()) {
        this->startCurrentWave();
    }
}

int WaveManager::getCurrentWaveId() 
{
    Wave* wave = getCurrentWave();
    int id = wave ? wave->getWaveId() : -1;
    std::cout << "[DEBUG] getCurrentWaveId: " << id << "\n";
    return id;
}

void WaveManager::update(float dt)
{
    Wave* wave = getCurrentWave();
    if (wave && wave->isStarted()) {
        wave->update(dt);
    }
}

void WaveManager::draw(sf::RenderWindow& window)
{
    Wave* wave = getCurrentWave();
    if (wave && wave->isStarted()) {
        wave->draw(window);
    }
}