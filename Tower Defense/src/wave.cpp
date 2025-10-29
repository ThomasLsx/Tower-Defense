#include "Wave.h"
#include "Minion.h"

/**
 * @brief Constructeur de Wave
 * @param id Numéro de la vague
 * @param nb_enemies Nombre d'ennemis dans la vague
 * @param map Pointeur vers la carte du jeu
 * @param delay Délai entre chaque spawn de Minion (en secondes)
 */
Wave::Wave(int id, int nb_enemies, TileMap* map)
    : id(id), nb_enemies(nb_enemies), started(false), finished(true),
      spawnTimer(0.0f), spawnDelay(1.0f), minionsSpawned(0), map(map)
{
}

/**
 * @brief Démarre la vague (initialise le timer)
 */
void Wave::start()
{
	started = true;
	finished = false;
    spawnTimer = 0.0f; // On attend le délai complet avant le premier spawn
    minionsSpawned = 0;
    minions.clear();
}

void Wave::spwanMinion()
{
    // Spawner les minions un par un avec un délai
    if (minionsSpawned < nb_enemies) {
        minions.push_back(std::make_unique<Minion>(minionsSpawned, map));
        minions.back()->init(15, sf::Color::Green, sf::Color::Black, 2);
        float tile = map->getTileSize().x * map->getScale();
        // Trouver la position de spawn (valeur 5 sur le bord)
        sf::Vector2u spawnTile = map->findEdgeTile(7);
        std::cout << "Spawn Minion " << minionsSpawned 
            << " at tile (" << spawnTile.x 
			<< ", " << spawnTile.y << ")" << std::endl;
        minions.back()->setPosition(sf::Vector2f(spawnTile.x * tile + tile / 2, spawnTile.y * tile + tile / 2));
		minions.back()->move(); // Initialiser le chemin du minion
        ++minionsSpawned;
    }
}

/**
 * @brief Met à jour l'état de la vague et des Minions
 * @param dt Delta time (temps écoulé depuis la dernière frame)
 */
void Wave::update(float dt)
{
	// Gérer le spawn des Minions
    if (started) {
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