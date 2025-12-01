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

        // Effet de survol pour le placement de tour
        if (ui->getSelectedTurretButtonIndex() != -1 && !selectedTower)
        {
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window->getRenderWindow());
            sf::Vector2f mouseWorld = window->getRenderWindow().mapPixelToCoords(mousePixel);
            // Vérifier si la souris est dans la zone du panel UI
            float panelStartX = window->getWidth() * 0.875f;
            if (mouseWorld.x < panelStartX) {
                sf::Vector2u tSize = map->getTileSize();
                float scale = map->getScale();
                unsigned int i = static_cast<unsigned int>(mouseWorld.x / (tSize.x * scale));
                unsigned int j = static_cast<unsigned int>(mouseWorld.y / (tSize.y * scale));
                float centerX = (i * tSize.x * scale) + (tSize.x * scale / 2.0f);
                float centerY = (j * tSize.y * scale) + (tSize.y * scale / 2.0f);
                sf::Vector2f center(centerX, centerY);

                // Apparence réelle de la tour en survol
                std::unique_ptr<Tower> previewTower;
                int type = ui->getSelectedTurretButtonIndex();
                if (type == 0)
                    previewTower = std::make_unique<BasicTower>(-1, center);
                else if (type == 1)
                    previewTower = std::make_unique<SniperTower>(-1, center);
                else if (type == 2)
                    previewTower = std::make_unique<SpeedTower>(-1, center);
                else if (type == 3)
                    previewTower = std::make_unique<SlowTower>(-1, center);

                if (previewTower)
                {
                    float range = previewTower->getRange();
                    // Cercle de portée
                    sf::CircleShape previewCircle;
                    previewCircle.setRadius(range);
                    previewCircle.setOrigin(sf::Vector2f(range, range));
                    previewCircle.setPosition(center);
                    previewCircle.setFillColor(sf::Color(0, 255, 0, 40)); // Vert transparent
                    previewCircle.setOutlineThickness(2.f);
                    previewCircle.setOutlineColor(sf::Color(0, 255, 0, 120));
                    window->getRenderWindow().draw(previewCircle);
                    
                    // Dessin de la tour
                    previewTower->draw(window->getRenderWindow());
                }
            }
        }

        // Affichage du cercle de portée autour de la tour sélectionnée
        if (selectedTower)
        {
            sf::CircleShape rangeCircle;
            float radius = selectedTower->getRange();
            rangeCircle.setRadius(radius);
            rangeCircle.setOrigin(sf::Vector2f(radius, radius)); // Centrer le cercle
            rangeCircle.setPosition(selectedTower->getPosition());
            rangeCircle.setFillColor(sf::Color(0, 0, 255, 40)); // Bleu transparent
            rangeCircle.setOutlineThickness(2.f);
            rangeCircle.setOutlineColor(sf::Color(0, 0, 255, 120));
            window->getRenderWindow().draw(rangeCircle);
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

    // Sélection de tour avec clic gauche en mode Play
    if (m_eGameMode == Play)
    {
        for (const auto& event : events)
        {
            if (event.is<sf::Event::MouseButtonPressed>())
            {
                auto mouse = event.getIf<sf::Event::MouseButtonPressed>();
                if (mouse->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos = window->getRenderWindow().mapPixelToCoords(mouse->position);
                    // Vérifier si le clic est dans la zone du panel UI (12.5% à droite)
                    float panelStartX = window->getWidth() * 0.875f;
                    if (mousePos.x < panelStartX) {
                        selectedTower = towerManager.getTowerAtPosition(mousePos, map->getTileSize(), map->getScale());
                        if (selectedTower) {
                            ui->deselectTurretButtons();
                        }
                    }
                    // Sinon, clic ignoré pour la sélection de tour sur la map
                }
            }
        }
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

TowerManager& Game::getTowerManager() { return towerManager; }
TileMap* Game::getMap() { return map.get(); }
