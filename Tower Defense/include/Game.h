#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>


#include "UI.h"
#include "Window.h"
#include "castle.h"
#include "economySystem.h"
#include "map.h"
#include "path.h"
#include "projectileSystem.h"
#include "tower.h"
#include "towerManager.h"
#include "wave.h"


/**
 * @brief Classe principale du jeu : gère la fenêtre, l'UI, les systèmes et la boucle de jeu.
 */
class Game
{
public:
    /**
     * @brief Modes de jeu possibles.
     */
    enum GameMode
    {
        Menu,   /**< Menu principal. */
        Play,   /**< En cours de jeu. */
        Pause,  /**< Jeu en pause. */
        Editor  /**< Éditeur de niveau. */
    };

    /**
     * @brief Constructeur de Game.
     */
    Game();

    /**
     * @brief Destructeur de Game.
     */
    ~Game();

    /**
     * @brief Lance la boucle principale du jeu.
     */
    void run();

    /**
     * @brief Retourne le mode courant du jeu.
     * @return Le mode de jeu actuel.
     */
    GameMode getGameMode() const
    {
        return m_eGameMode;
    }

    /**
     * @brief Retourne l'ID de la vague courante.
     * @return L'ID de la vague, ou -1 si aucune.
     */
    int getCurrentWaveId() const;

    /**
     * @brief Change le mode de jeu.
     * @param mode Le nouveau mode.
     */
    void setGameMode(GameMode mode)
    {
        m_eGameMode = mode;
    }

    /** @brief Drapeau utilisé par l'UI pour demander le démarrage d'une vague. */
    bool m_bRequestStartWave;

    /** @brief Drapeau pour l'auto-démarrage des vagues (utilisé par l'UI). */
    bool m_bAutoStartWaves = false;

    /* Getters Resources */

    /**
     * @brief Récupère la quantité de cuivre disponible.
     * @return Quantité de cuivre.
     */
    int getCopper() const;

    /**
     * @brief Récupère la quantité d'argent disponible.
     * @return Quantité d'argent.
     */
    int getSilver() const;

    /**
     * @brief Récupère la quantité d'or disponible.
     * @return Quantité d'or.
     */
    int getGold() const;

    /* Tower Placement */

    /**
     * @brief Récupère l'index du type de tour sélectionné pour le placement.
     * @return L'index de la tour.
     */
    int getTowerIndex() const
    {
        return map->GetTowerIndex();
    }

    /**
     * @brief Définit l'index du type de tour à placer.
     * @param index L'index de la tour.
     */
    void setTowerIndex(int index)
    {
        map->SetTowerIndex() = index;
    }

    /**
     * @brief Vérifie si une tour est présente aux coordonnées données.
     * @param i Index X de la grille.
     * @param j Index Y de la grille.
     * @return true si une tour est présente.
     */
    bool isTowerAt(unsigned int i, unsigned int j) const
    {
        return towerManager.isTowerAt(i, j, map->getTileSize(), map->getScale());
    }

    /** @brief Pointeur vers la tour actuellement sélectionnée (via clic). */
    Tower* selectedTower = nullptr;

    /**
     * @brief Accède au gestionnaire de tours.
     * @return Référence vers TowerManager.
     */
    TowerManager& getTowerManager();

    /**
     * @brief Accède à la carte du jeu.
     * @return Pointeur vers TileMap.
     */
    TileMap* getMap();

    /**
     * @brief Accède au système économique.
     * @return Pointeur vers EconomySystem.
     */
    EconomySystem* getEconomySystem()
    {
        return m_economySystem.get();
    }

private:
    /**
     * @brief Gère le rendu graphique de la frame courante.
     */
    void Render();

    /**
     * @brief Traite les entrées utilisateur (clavier/souris).
     * @param events Liste des événements SFML capturés.
     */
    void HandleInput(const std::vector<sf::Event>& events);

    /** @brief Mode de jeu actuel. */
    GameMode m_eGameMode;

    /** @brief Gestionnaire des tours (instance directe). */
    TowerManager towerManager;

    /** @brief Pointeur unique vers la fenêtre de jeu. */
    std::unique_ptr<Window> window;

    /** @brief Pointeur unique vers l'interface utilisateur. */
    std::unique_ptr<UI> ui;

    /** @brief Pointeur unique vers la carte de tuiles. */
    std::unique_ptr<TileMap> map;

    /** @brief Pointeur unique vers le gestionnaire de vagues. */
    std::unique_ptr<WaveManager> waveManager;

    /** @brief Pointeur unique vers le système de projectiles. */
    std::unique_ptr<ProjectileSystem> m_projectileSystem;

    /** @brief Vecteur stockant les tours (Obsolète si TowerManager gère le stockage, mais présent dans la classe). */
    std::vector<std::unique_ptr<Tower>> m_towers;

    /** @brief Pointeur unique vers le château (base du joueur). */
    std::unique_ptr<Castle> m_castle;

    /** @brief Pointeur unique vers le système économique. */
    std::unique_ptr<EconomySystem> m_economySystem;
};