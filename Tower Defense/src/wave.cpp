#include "Wave.h"
#include "Minion.h"
#include "path.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>


/**
 * @brief Constructeur de Wave
 *
 * Initialise la configuration de spawn de la vague.
 */
Wave::Wave(int id, int nb_enemies, TileMap* map, Castle* castle)
    : id(id), nb_enemies(nb_enemies), started(false), finished(false),
      spawnTimer(0.0f), spawnDelay(1.0f), minionsSpawned(0), map(map), castle(castle), minionGroupIndex(0), minionInGroupSpawned(0), m_threadPool(1)
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
            minions.push_back(std::make_shared<MinionNormal>(minionsSpawned, map, castle));
        }
        else if (type == "Fast") {
            minions.push_back(std::make_shared<MinionFast>(minionsSpawned, map, castle));
        }
        else if (type == "Tank") {
            minions.push_back(std::make_shared<MinionTank>(minionsSpawned, map, castle));
        }
        else if (type == "Boss") {
            minions.push_back(std::make_shared<MinionBoss>(minionsSpawned, map, castle));
        }
        else {
            minions.push_back(std::make_shared<MinionNormal>(minionsSpawned, map, castle));
        }

        float tile = map->getTileSize().x * map->getScale();
        sf::Vector2u spawnTile = map->findEdgeTile(0);
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

    // 1. Détection des changements (Reste sur le Main Thread)
    if (map->hasMapChanged()) {
        for (auto& minion : minions) {
            bool needsUpdate = false;
            for (const auto& pathPos : minion->getTargetPath()) {
                if (map->getCurentTile(pathPos) == map->getLastModifiedTile()) {
                    needsUpdate = true;
                    break;
                }
            }

            if (needsUpdate) {
                sf::Vector2u pos = map->getCurentTile(minion->getPosition());
                Position startPos = { pos.y, pos.x };

                // Trouver la position de fin 
                sf::Vector2u endTile = map->findEdgeTile(3);
                Position targetPos = { endTile.y, endTile.x };

                auto towerLevel2D = map->getTowerLevel2D();
                auto tilesize = map->getTileSize().x * map->getScale();

                unsigned int minionID = minion->getId();

                // 2. Envoi des tâches de recalcul (Main Thread)
                m_pendingUpdates.push_back(
                    m_threadPool.enqueue([towerLevel2D, startPos, targetPos, tilesize, minionID]() -> PathUpdateResult {
                        // Ici on fait le calcul A*.
						// Note : On ne modifie pas l'objet dans le thread, on retourne juste le résultat.
                        Pathfinding pf(towerLevel2D);

                        std::optional<std::vector<Position>> pathOpt = pf.findPath(startPos, targetPos);

                        return { minionID, pathOpt };

                        })
                );
            }
        }

        map->setMapChanged(false);
    }

    // 3. Récupération des résultats (À chaque frame)
    // On parcourt la liste des futures pour voir si certains ont fini.
    auto it = m_pendingUpdates.begin();
    while (it != m_pendingUpdates.end()) {
        if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            // Le calcul est fini !
            PathUpdateResult result = it->get(); // Récupère le résultat

            // On trouve le minion correspondant et on applique le chemin
            auto minionIt = std::find_if(minions.begin(), minions.end(),
                [id = result.minionID](auto& m) { return m->getId() == id; });

            if (minionIt != minions.end()) {
                if (result.newPath.has_value() && !result.newPath->empty()) {
                    static_cast<Minion*>(minionIt->get())->setPath(*result.newPath, map->getTileSize().x * map->getScale());
                }
                else {
                    std::cout << "Aucun chemin valide trouve !" << std::endl;
                }
            }

            it = m_pendingUpdates.erase(it); // On retire la tâche finie
        }
        else {
            ++it;
        }
    }


    // Met à jour chaque Minion
    for (auto& minion : minions) {
        minion->update(dt);
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
WaveManager::WaveManager(std::string waveFile, TileMap* map, Castle* castle)
	: currentWaveIndex(0), waveFile(waveFile), map(map), castle(castle)
{
    loadWavesFromFile(waveFile, map, castle);
}

void WaveManager::loadWavesFromFile(const std::string& filename, TileMap* map, Castle* castle)
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
            waves.push_back(std::make_unique<Wave>(waveId, 0, map, castle));
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