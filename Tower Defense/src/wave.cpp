
#include "Wave.h"
#include "Minion.h"

/**
 * @brief Constructeur de Wave
 * @param number Numéro de la vague
 * @param delay Délai entre chaque spawn de Minion (en secondes)
 */
Wave::Wave(int number, float delay)
    : waveNumber(number), finished(false),
    spawnTimer(0.0f), spawnDelay(delay), minionsSpawned(0)
{
}

/**
 * @brief Essaye de spawner un Minion si le délai est écoulé
 */
void Wave::trySpawnMinion()
{
    if (minionsSpawned < 5) { // Exemple : 5 Minions par vague
        minions.push_back(std::make_unique<Minion>(waveNumber));
        ++minionsSpawned;
    }
}

/**
 * @brief Démarre la vague (initialise le timer)
 */
void Wave::start()
{
    spawnTimer = 0.0f;
    minionsSpawned = 0;
    minions.clear();
}

/**
 * @brief Met à jour l'état de la vague et des Minions
 * @param dt Delta time (temps écoulé depuis la dernière frame)
 */
void Wave::update(float dt)
{
    // Mise à jour du timer de spawn
    spawnTimer += dt;
    if (spawnTimer >= spawnDelay) {
        spawnTimer = 0.0f;
        trySpawnMinion();
    }

    // Mise à jour des Minions existants
    for (auto& minion : minions) {
        minion->update(dt);
    }

    // Vérifie si tous les Minions sont morts ou arrivés à destination
    finished = (minionsSpawned >= 5); // Exemple : 5 Minions par vague
    for (const auto& minion : minions) {
        if (minion->getIsAlive()) {
            finished = false;
            break;
        }
    }
}

/**
 * @brief Indique si la vague est terminée
 * @return true si tous les Minions sont morts ou arrivés, false sinon
 */
bool Wave::isFinished() const
{
    return finished;
}

/**
 * @brief Retourne la liste des Minions de la vague
 * @return Référence constante vers le vecteur de Minions
 */
const std::vector<std::unique_ptr<Minion>>& Wave::getMinions() const
{
    return minions;
}
