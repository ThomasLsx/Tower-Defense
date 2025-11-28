// Game.cpp

#include "Game.h"

/**
 * @brief Constructeur de Game
 */
Game::Game()
    : m_eGameMode(Menu),
      m_bRequestStartWave(false),
      m_bAutoStartWaves(false)
{
    window = std::make_unique<Window>();
    window->create();
    window->getRenderWindow().setFramerateLimit(60);

    map = std::make_unique<TileMap>(window->getRenderWindow());
    map->loadLevel("assets/map1.txt");
    map->loadTile("assets/TileMap.png", map->getLevel().data());

    m_economySystem = std::make_unique<EconomySystem>(this);

    m_castle = std::make_unique<Castle>(map.get(), m_economySystem.get(), 100);

    waveManager = std::make_unique<WaveManager>("assets/wave.txt", map.get(), m_castle.get());

    m_projectileSystem = std::make_unique<ProjectileSystem>();
    

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
            ui->updatePlayUI();

            if (m_bRequestStartWave)
            {
                waveManager->startOrNextWave();
                m_bRequestStartWave = false;
            }

            if (m_castle->isDefeated())
            {
				break;
            }
            
            waveManager->update(sec);
            m_projectileSystem->update(sec);

            // Auto start wave logic
            if (m_bAutoStartWaves)
            {

                waveManager->startOrNextWave();
            }

            if (waveManager->getCurrentWave())
            {
                const auto& minions = waveManager->getCurrentWave()->getMinions();
                towerManager.updateTowers(minions, sec, *m_projectileSystem);
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
		m_castle->draw(window->getRenderWindow());
        waveManager->draw(window->getRenderWindow());

        towerManager.drawTowers(window->getRenderWindow());

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


/* Getters resources */

/**
 * @brief Retourne la quantité de cuivre actuelle.
 */
int Game::getCopper() const
{
    return m_economySystem->getCopper();
}

/**
 * @brief Retourne la quantité d'argent actuelle.
 */
int Game::getSilver() const
{
    return m_economySystem->getSilver();
}

/**
 * @brief Retourne la quantité d'or actuelle.
 */

int Game::getGold() const
{
    return m_economySystem->getGold();
}
