#include "Wave.h"
#include "Minion.h"

/**
 * @brief Constructeur de Wave
 * @param number Numéro de la vague
 * @param delay Délai entre chaque spawn de Minion (en secondes)
 */
Wave::Wave(int id, int nb_enemies, TileMap* map, float delay)
    : id(id), nb_enemies(nb_enemies), finished(false),
    spawnTimer(0.0f), spawnDelay(delay), minionsSpawned(0), map(map)
{
}

/**
 * @brief Démarre la vague (initialise le timer)
 */
void Wave::start()
{
	finished = false;
    spawnTimer = 1.0f;
    minionsSpawned = 0;
    minions.clear();
}

/**
 * @brief Met à jour l'état de la vague et des Minions
 * @param dt Delta time (temps écoulé depuis la dernière frame)
 */
void Wave::update(float sec)
{
    // Spawner les minions un par un avec un délai
    if (minionsSpawned < nb_enemies) {
        spawnTimer += sec;
        if (spawnTimer >= spawnDelay) {
            minions.push_back(std::make_unique<Minion>(minionsSpawned, map));
            minions.back()->init(30, sf::Color::Green, sf::Color::Black, 2);
            float tile = map->getTileSize().x * map->getScale();
            minions.back()->setPosition(sf::Vector2f(1 * tile, 5 * tile));
            minions.back()->move();
            ++minionsSpawned;
            spawnTimer = 0.0f;
        }
    }

    // Mise à jour des Minions existants
    for (auto& minion : minions) {
        minion->update(sec*120);
    }

    // Vérifie si tous les Minions sont morts ou arrivés à destination
    finished = (minionsSpawned >= nb_enemies);
    for (const auto& minion : minions) {
        if (minion->getIsAlive()) {
            finished = false;
            break;
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