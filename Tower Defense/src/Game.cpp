#include "Game.h"

#include "Window.h"
#include "UI.h"
#include "map.h"
#include "wave.h"
#include "path.h"
#include "projectileSystem.h"
#include "tower.h"

/**
 * @brief Constructeur de Game
 */
Game::Game()
    : m_eGameMode(Menu),
      m_bRequestStartWave(false)
{
    window = std::make_unique<Window>();
    window->create();
    window->getRenderWindow().setFramerateLimit(60);

    map = std::make_unique<TileMap>(window->getRenderWindow());
    map->loadLevel("assets/map1.txt");
    map->loadTile("assets/TileMap.png", map->getLevel().data());

    waveManager = std::make_unique<WaveManager>("assets/wave.txt", map.get());

    m_projectileSystem = std::make_unique<ProjectileSystem>();

    // initialize towers
    m_towers.push_back(std::make_unique<BasicTower>(1, sf::Vector2f(300.f, 300.f)));
    m_towers.push_back(std::make_unique<SpeedTower>(2, sf::Vector2f(500.f, 400.f)));

    ui = std::make_unique<UI>(window.get(), this);
}

Game::~Game()
{
}

/**
 * @brief Retourne l'ID de la vague courante, ou 0 si aucune.
 */
int Game::getCurrentWaveId() const
{
    if (waveManager && waveManager->getCurrentWave())
    {
        return waveManager->getCurrentWaveId();
    }
    return 0;
}

/**
 * @brief Boucle principale du jeu.
 */
void Game::run()
{
    sf::Clock clock;
    while (window->isOpen())
    {
        float sec = clock.restart().asSeconds();
        std::vector<sf::Event> events;
        while (const std::optional<sf::Event> event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window->close();
            }
            else
            {
                events.push_back(*event);
                ui->handleEvent(*event);
            }
        }

        HandleInput(events);

        switch (m_eGameMode) {
        case Menu:
            ui->showMenuUI();
            break;
        case Play: 
            ui->showPlayUI();

            if (m_bRequestStartWave)
            {
                waveManager->startOrNextWave();
                m_bRequestStartWave = false;
            }

            waveManager->update(sec);
            m_projectileSystem->update(sec);

            if (waveManager->getCurrentWave())
            {
                const auto& minions = waveManager->getCurrentWave()->getMinions();
                for (auto& tower : m_towers)
                {
                    tower->update(sec, minions, *m_projectileSystem);
                }
            }
            break;
        case Editor:
            ui->showEditorUI();
            break;
        }

        Render();
    }
}

/**
 * @brief Rendu de la scène selon le mode de jeu.
 */
void Game::Render()
{
    window->clear(sf::Color(50, 50, 50));

    if (m_eGameMode == Play or m_eGameMode == Pause)
    {
        map->draw(window->getRenderWindow(), sf::RenderStates::Default);
        waveManager->draw(window->getRenderWindow());

        for (auto& tower : m_towers)
        {
            tower->draw(window->getRenderWindow());
        }

        m_projectileSystem->draw(window->getRenderWindow());
    }

    if (m_eGameMode == Editor)
    {
        map->draw(window->getRenderWindow(), sf::RenderStates::Default);
        map->DrawMouseHover();
    }
    ui->draw();
    window->display();
}

/**
 * @brief Gère les entrées utilisateur globales.
 */
void Game::HandleInput(const std::vector<sf::Event>& events)
{
    static bool bTWasPressedLastUpdate = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T))
    {
        if (!bTWasPressedLastUpdate)
        {
            if (m_eGameMode == Menu) {
                m_eGameMode = Play;
            }
            else if (m_eGameMode == Play) {
                m_eGameMode = Pause;
            }
            else if (m_eGameMode == Pause) {
                m_eGameMode = Editor;
            }
            else if (m_eGameMode == Editor) {
                m_eGameMode = Menu;
            }
        }
        bTWasPressedLastUpdate = true;
    }
    else
    {
        bTWasPressedLastUpdate = false;
    }

    if (m_eGameMode == Editor)
        map->HandleLevelEditorInput(events);

    else if (m_eGameMode == Play)
        map->HandleTowerInput(events, towerManager);
}
