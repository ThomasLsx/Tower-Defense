// Game.cpp
#include "Game.h"
#include <iostream>

Game::Game()
    : m_eGameMode(Menu)

{
    window = new Window();
    window->create();
    window->getRenderWindow().setFramerateLimit(60);

    ui = new UI(window, this);
    map = new TileMap(window->getRenderWindow());

    map->loadLevel("assets/map1.txt");
    map->loadTile("assets/TileMap.png", map->getLevel().data());

    wave = new Wave(1, 5, map);

}

Game::~Game()
{
}

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
            UpdatePlay(events);
            if (startNextWave)
				wave->update(sec);
            break;
        case Editor:
            ui->showEditorUI();
            break;
        }

        Render();

    }
}


void Game::UpdatePlay(const std::vector<sf::Event>& events)
{
    HandlePlayInput(events);
}



void Game::Render()
{
    window->clear(sf::Color(50, 50, 50));

    // Affiche le menu
    if (m_eGameMode == Menu)
    {
        //window->clear(sf::Color(100, 100, 100));
    }

    // Affiche la map de jeux
    if (m_eGameMode == Play  or m_eGameMode == Pause)
    {
        map->draw(window->getRenderWindow(), sf::RenderStates::Default);
        wave->draw(window->getRenderWindow());
    }

    // Affiche la tuile sélectionnée sous la souris uniquement en mode Editor
    if (m_eGameMode == Editor)
    {
        map->draw(window->getRenderWindow(), sf::RenderStates::Default);
        map->DrawMouseHover();
    }
    ui->draw();
    window->display();
}

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

    // N'appeler HandlePlayInput() que dans UpdatePlay()
    if (m_eGameMode == Editor)
        map->HandleLevelEditorInput(events);
}

void Game::HandlePlayInput(const std::vector<sf::Event>& events)
{
    for (const auto& event : events)
    {
        ui->handleEvent(event);
    }
}


