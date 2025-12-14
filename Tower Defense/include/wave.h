#pragma once

#include <future>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "map.h"
#include "threadPool.h"

class Minion;
class TileMap;
class Castle;
struct Position;

/**
 * @brief Structure contenant le résultat d'une mise à jour de chemin asynchrone.
 */
struct PathUpdateResult
{
    /** @brief Identifiant du minion concerné. */
    unsigned int minionID;

    /** @brief Nouveau chemin calculé (optionnel). */
    std::optional<std::vector<Position>> newPath;
};

/**
 * @brief Représente une vague d'ennemis spécifique.
 *
 * La Wave possède ses minions mais expose des `std::shared_ptr<Minion>` afin que
 * d'autres systèmes (tours, projectiles, UI) puissent observer ou référencer
 * les minions en toute sécurité.
 */
class Wave
{
private:
    /**
     * @brief Configuration d'un groupe de minions au sein de la vague.
     */
    struct MinionGroupConfig
    {
        /** @brief Type de minion (ex: "Normal", "Fast"). */
        std::string type;

        /** @brief Nombre de minions dans ce groupe. */
        int count;
    };

    /** @brief Pool de threads pour les calculs asynchrones (si utilisé). */
    ThreadPool m_threadPool;

    /** @brief Liste des futures pour les mises à jour de chemin en attente. */
    std::vector<std::future<PathUpdateResult>> m_pendingUpdates;

    /** @brief Identifiant unique de la vague. */
    int id;

    /** @brief Nombre total d'ennemis dans la vague. */
    int nb_enemies;

    /** @brief Indique si la vague a commencé. */
    bool started;

    /** @brief Indique si la vague est terminée. */
    bool finished;

    /** @brief Minions détenus par la vague (partagés avec d'autres systèmes). */
    std::vector<std::shared_ptr<Minion>> minions;

    /** @brief Timer écoulé depuis le dernier spawn. */
    float spawnTimer;

    /** @brief Délai entre deux spawns. */
    float spawnDelay;

    /** @brief Nombre de minions déjà apparus. */
    size_t minionsSpawned;

    /** @brief Pointeur vers la carte de jeu. */
    TileMap* map;

    /** @brief Pointeur vers le château (cible des minions). */
    Castle* castle;

    /** @brief Liste des configurations de groupes de minions. */
    std::vector<MinionGroupConfig> minionGroups;

    /** @brief Index du groupe de minions actuel. */
    size_t minionGroupIndex = 0;

    /** @brief Nombre de minions apparus dans le groupe actuel. */
    size_t minionInGroupSpawned = 0;

    /** @brief File d'attente pour la mise à jour des chemins des minions. */
    std::queue<std::shared_ptr<Minion>> minionPathUpdateQueue;

public:
    /**
     * @brief Constructeur explicite de Wave.
     * @param id Identifiant de la vague.
     * @param number Nombre d'ennemis (estimation ou total).
     * @param map Pointeur vers la carte.
     * @param castle Pointeur vers le château.
     */
    explicit Wave(int id, int number, TileMap* map, Castle* castle);

    /**
     * @brief Destructeur par défaut.
     */
    ~Wave() = default;

    /**
     * @brief Démarre la vague et initialise le timer.
     */
    void startWave();

    /**
     * @brief Crée un minion selon la configuration courante du groupe.
     */
    void spwanMinion();

    /**
     * @brief Met à jour la vague et ses minions.
     * @param dt Temps écoulé depuis la dernière frame.
     */
    void update(float dt);

    /**
     * @brief Dessine les minions actifs.
     * @param window Fenêtre de rendu SFML.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Marque la vague comme terminée.
     */
    void waveFinish();

    /**
     * @brief Ajoute un groupe de minions à la vague.
     * @param type Type de minion (chaîne de caractères).
     * @param count Nombre de minions de ce type.
     */
    void addMinionGroup(const std::string& type, int count);

    /**
     * @brief Ajoute au nombre total d'ennemis prévus.
     * @param count Nombre à ajouter.
     */
    void addEnemies(int count);

    /**
     * @brief Retourne la liste des minions actifs.
     * @return Référence constante vers le vecteur de pointeurs partagés.
     */
    const std::vector<std::shared_ptr<Minion>>& getMinions() const
    {
        return minions;
    }

    /**
     * @brief Récupère l'ID de la vague.
     * @return L'identifiant.
     */
    inline int getWaveId() const
    {
        return id;
    }

    /**
     * @brief Vérifie si la vague a commencé.
     * @return true si démarrée.
     */
    inline bool isStarted() const
    {
        return started;
    }

    /**
     * @brief Vérifie si la vague est terminée.
     * @return true si terminée.
     */
    inline bool isFinished() const
    {
        return finished;
    }
};

/**
 * @brief Gestionnaire de vagues.
 *
 * Contient les vagues (propriétaires via unique_ptr) et gère la progression du jeu.
 */
class WaveManager
{
private:
    /** @brief Liste des vagues gérées. */
    std::vector<std::unique_ptr<Wave>> waves;

    /** @brief Index de la vague actuelle. */
    int currentWaveIndex;

    /** @brief Chemin vers le fichier de configuration des vagues. */
    std::string waveFile;

    /** @brief Pointeur vers la carte. */
    TileMap* map;

    /** @brief Pointeur vers le château. */
    Castle* castle;

public:
    /**
     * @brief Constructeur de WaveManager.
     * @param waveFile Fichier de configuration des vagues.
     * @param map Pointeur vers la carte.
     * @param castle Pointeur vers le château.
     */
    WaveManager(std::string waveFile, TileMap* map, Castle* castle);

    /**
     * @brief Destructeur par défaut.
     */
    ~WaveManager() = default;

    /**
     * @brief Charge la configuration des vagues depuis un fichier.
     * @param filename Chemin du fichier.
     * @param map Pointeur vers la carte.
     * @param castle Pointeur vers le château.
     */
    void loadWavesFromFile(const std::string& filename, TileMap* map, Castle* castle);

    /**
     * @brief Récupère un pointeur vers la vague actuelle.
     * @return Pointeur vers Wave, ou nullptr si invalide.
     */
    Wave* getCurrentWave();

    /**
     * @brief Passe à la vague suivante.
     */
    void nextWave();

    /**
     * @brief Démarre la vague actuelle si elle n'est pas déjà lancée.
     */
    void startCurrentWave();

    /**
     * @brief Logique pour démarrer la vague courante ou passer à la suivante si finie.
     */
    void startOrNextWave();

    /**
     * @brief Récupère l'ID de la vague courante.
     * @return L'ID de la vague.
     */
    int getCurrentWaveId();

    /**
     * @brief Met à jour le gestionnaire et la vague courante.
     * @param dt Temps écoulé.
     */
    void update(float dt);

    /**
     * @brief Dessine la vague courante.
     * @param window Fenêtre de rendu.
     */
    void draw(sf::RenderWindow& window);
};
