#include "Wave.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "minion.h"

Wave::Wave(int id, int nb_enemies, TileMap* map, Castle* castle)
    : id(id), nb_enemies(nb_enemies), started(false), finished(false),
    spawnTimer(0.0f), spawnDelay(1.0f), minionsSpawned(0),
    map(map), castle(castle), minionGroupIndex(0), minionInGroupSpawned(0)
{
}

void Wave::startWave()
{
    if (started)
    {
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
    if (minionsSpawned < nb_enemies && minionGroupIndex < minionGroups.size())
    {
        const auto& group = minionGroups[minionGroupIndex];
        std::string type = group.type;

        if (type == "Normal")
        {
            minions.push_back(std::make_shared<MinionNormal>(minionsSpawned, map, castle));
        }
        else if (type == "Fast")
        {
            minions.push_back(std::make_shared<MinionFast>(minionsSpawned, map, castle));
        }
        else if (type == "Tank")
        {
            minions.push_back(std::make_shared<MinionTank>(minionsSpawned, map, castle));
        }
        else if (type == "Boss")
        {
            minions.push_back(std::make_shared<MinionBoss>(minionsSpawned, map, castle));
        }
        else
        {
            minions.push_back(std::make_shared<MinionNormal>(minionsSpawned, map, castle));
        }

        // Positionnement au spawn
        float tile = map->getTileSize().x * map->getScale();
        sf::Vector2u spawnTile = map->getSpawnTile();

        // Centrer le minion sur la tuile de spawn
        sf::Vector2f spawnPos(spawnTile.x * tile + tile / 2, spawnTile.y * tile + tile / 2);
        minions.back()->setPosition(spawnPos);

        // Initialisation du mouvement (reset interne du minion)
        minions.back()->move();

        ++minionsSpawned;
        ++minionInGroupSpawned;

        if (minionInGroupSpawned >= group.count)
        {
            minionGroupIndex++;
            minionInGroupSpawned = 0;
        }
    }
}

void Wave::update(float dt)
{
    // 1. Gérer le spawn des minions
    if (isStarted() && minionsSpawned < nb_enemies)
    {
        spawnTimer += dt;

        if (spawnTimer >= spawnDelay)
        {
            spwanMinion();
            spawnTimer = 0.0f;
        }
    }

    // 2. Met à jour chaque Minion
    for (auto& minion : minions)
    {
        minion->update(dt);
    }

    // 3. Nettoyage des minions morts ou arrivés
    minions.erase(
        std::remove_if(minions.begin(), minions.end(),
            [](const std::shared_ptr<Minion>& minion)
            {
                return !minion->getIsAlive();
            }),
        minions.end()
    );

    // 4. Vérifie si la wave est terminée (plus de spawn et plus de minions vivants)
    if (!isFinished())
    {
        if (minionsSpawned == nb_enemies && minions.empty())
        {
            waveFinish();
        }
    }
}

void Wave::draw(sf::RenderWindow& window)
{
    for (const auto& minion : minions)
    {
        minion->draw(window);
    }
}

void Wave::waveFinish()
{
    started = false;
    finished = true;
    std::cout << "[INFO] Wave " << id << " finished.\n";
}

void Wave::addMinionGroup(const std::string& type, int count)
{
    minionGroups.push_back({ type, count });
}

void Wave::addEnemies(int count)
{
    nb_enemies += count;
}

WaveManager::WaveManager(std::string waveFile, TileMap* map, Castle* castle)
    : currentWaveIndex(0), waveFile(waveFile), map(map), castle(castle)
{
    loadWavesFromFile(waveFile, map, castle);
}

void WaveManager::loadWavesFromFile(const std::string& filename, TileMap* map, Castle* castle)
{
    waves.clear();
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "[ERREUR] Impossible d'ouvrir le fichier de vague: " << filename << std::endl;
        return;
    }

    std::string line;
    int currentWaveId = -1;
    Wave* currentWave = nullptr;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::istringstream iss(line);

        int waveId, count;
        std::string type, sep;

        // Parsing basique du fichier wave.txt (Format: ID;Type;Count)
        // ex: 1;Normal;5
        std::string idStr, countStr;

        if (std::getline(iss, idStr, ';') && std::getline(iss, type, ';') && std::getline(iss, countStr, ';'))
        {
            waveId = std::stoi(idStr);
            count = std::stoi(countStr);

            if (waveId != currentWaveId)
            {
                waves.push_back(std::make_unique<Wave>(waveId, 0, map, castle));
                currentWave = waves.back().get();
                currentWaveId = waveId;
            }

            if (currentWave)
            {
                currentWave->addMinionGroup(type, count);
                currentWave->addEnemies(count);
            }
        }
    }

    std::cout << "[DEBUG] Waves loaded: " << waves.size() << "\n";
}

Wave* WaveManager::getCurrentWave()
{
    if (waves.empty() || currentWaveIndex < 0 || currentWaveIndex >= (int)waves.size())
    {
        return nullptr;
    }

    return waves[currentWaveIndex].get();
}

void WaveManager::nextWave()
{
    if (currentWaveIndex + 1 < (int)waves.size())
    {
        ++currentWaveIndex;
    }
    else
    {
        std::cout << "[INFO] Toutes les vagues sont terminées !\n";
    }
}

void WaveManager::startCurrentWave()
{
    Wave* wave = getCurrentWave();

    if (wave && !wave->isStarted())
    {
        wave->startWave();
    }
}

void WaveManager::startOrNextWave()
{
    Wave* currentWave = this->getCurrentWave();

    if (!currentWave)
    {
        return;
    }

    if (currentWave->isFinished())
    {
        this->nextWave();
        Wave* next = this->getCurrentWave();

        // Si la prochaine vague existe et n'a pas encore couru
        if (next && !next->isStarted() && !next->isFinished())
        {
            next->startWave();
        }
    }
    else if (!currentWave->isStarted())
    {
        this->startCurrentWave();
    }
}

int WaveManager::getCurrentWaveId()
{
    Wave* wave = getCurrentWave();
    return wave ? wave->getWaveId() : 0;
}

void WaveManager::update(float dt)
{
    Wave* wave = getCurrentWave();

    if (wave && wave->isStarted())
    {
        wave->update(dt);
    }
}

void WaveManager::draw(sf::RenderWindow& window)
{
    Wave* wave = getCurrentWave();

    if (wave && wave->isStarted())
    {
        wave->draw(window);
    }
}