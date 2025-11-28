// Game.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <utility>

#include "Window.h"
#include "UI.h"
#include "map.h"
#include "wave.h"
#include "path.h"
#include "projectileSystem.h"
#include "tower.h"
#include "towerManager.h"
#include "castle.h"
#include "economySystem.h"

class Window;
class UI;
class TileMap;
class WaveManager;
class ProjectileSystem;
class Tower;
class TowerManager;
class Castle;
class EconomySystem;

/**
 * @brief Classe principale du jeu : fenêtre, UI et boucle de jeu.
 */
class Game {
public:
    Game();
    ~Game();
    void run();

    enum GameMode
    {
        Menu,
        Play,
        Pause,
        Editor
    };

    /** Retourne le mode courant du jeu. */
    GameMode getGameMode() const { return m_eGameMode; }

    /** Retourne l'ID de la vague courante (utilis� par l'UI).
     *  -1 si aucune vague.
     */
    int getCurrentWaveId() const;

    void setGameMode(GameMode mode) { m_eGameMode = mode; }

    /** Drapeau utilis� par l'UI pour demander le d�marrage d'une vague. */
    bool m_bRequestStartWave;
    /** Drapeau pour l'auto-d�marrage des vagues (utilis� par l'UI) */
    bool m_bAutoStartWaves = false;

    /* Getters Resources */
	int getCopper() const;
	int getSilver() const;
	int getGold() const;

private:
    void Render();
    void HandleInput(const std::vector<sf::Event>& events);

    GameMode m_eGameMode;
    TowerManager towerManager;

    std::unique_ptr<Window> window;
    std::unique_ptr<UI> ui;
    std::unique_ptr<TileMap> map;
    std::unique_ptr<WaveManager> waveManager;
    std::unique_ptr<ProjectileSystem> m_projectileSystem;
    std::vector<std::unique_ptr<Tower>> m_towers;
    std::unique_ptr<Castle> m_castle;
	std::unique_ptr<EconomySystem> m_economySystem;
};