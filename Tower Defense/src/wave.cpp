#include "Wave.h"
#include "Minion.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

/**
 * @brief Constructeur de Wave
 *
 * Initialise la configuration de spawn de la vague.
 */
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

/**
 * @brief Crée un minion selon le groupe courant et l'ajoute à la liste.
 */
void Wave::spwanMinion()
{
    if (minionsSpawned < nb_enemies && minionGroupIndex < minionGroups.size()) {
        const auto& group = minionGroups[minionGroupIndex];
        std::string type = group.type;

        // Création du minion selon le type
        if (type == "Normal") {
            minions.push_back(std::make_shared<MinionNormal>(minionsSpawned, map));
        }
        else if (type == "Fast" || type == "fast") {
            minions.push_back(std::make_shared<MinionFast>(minionsSpawned, map));
        }
        else if (type == "Tank") {
            minions.push_back(std::make_shared<MinionTank>(minionsSpawned, map));
        }
        else if (type == "Boss") {
            minions.push_back(std::make_shared<MinionBoss>(minionsSpawned, map));
        }
        else {
            minions.push_back(std::make_shared<MinionNormal>(minionsSpawned, map));
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
 * @brief Met à jour la vague et ses minions.
 */
void Wave::update(float dt)
{
    // Gérer le spawn des minions
    if (isStarted() && minionsSpawned < nb_enemies) {
        spawnTimer += dt;
        if (spawnTimer >= spawnDelay) {
            spwanMinion();
            spawnTimer = 0.0f;
        }
    }

    // Met à jour chaque minion
    for (auto& minion : minions) {
        minion->update(dt);
        // Si la carte a changé, forcer le recalcul / repositionnement
        if (map->hasMapChanged()) {
            minion->move();
            map->setMapChanged(false);
        }
    }

    // Nettoyage des minions morts
    minions.erase(
        std::remove_if(minions.begin(), minions.end(),
            [](const std::shared_ptr<Minion>& minion) {
                return !minion->getIsAlive();
            }),
        minions.end()
    );

    // Vérifie si la wave est terminée
    if (!isFinished()) {
        if (minionsSpawned == nb_enemies && minions.empty()) {
            waveFinish();
        }
    }
}

/**
 * @brief Dessine les Minions de la vague sur la fenêtre
 */
void Wave::draw(sf::RenderWindow& window)
{
    for (const auto& minion : minions) {
        minion->draw(window);
    }
}

/**
 * @brief Indique que la vague est terminée
 */
void Wave::waveFinish()
{
    started = false;
    finished = true;
    std::cout << "[DEBUG] waveFinish called for wave id=" << id << "\n";
}

void Wave::addMinionGroup(const std::string& type, int count) {
    minionGroups.push_back({ type, count });
}

void Wave::addEnemies(int count)
{
    nb_enemies += count;
}


/**
 * WaveManager
 */
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
    }
    else if (!currentWave->isStarted()) {
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