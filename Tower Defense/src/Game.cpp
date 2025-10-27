// Game.cpp
#include "Game.h"
#include <iostream>
#include <cmath>
#include "Window.h"
#include <SFML/Graphics.hpp>

#include "entity.h"
#include "minion.h"
#include "path.h"

Game::Game()
    : m_eGameMode(Menu)

{
    window = new Window();
    window->create();
    window->getRenderWindow().setFramerateLimit(60);

    ui = new UI(window->getRenderWindow());
    map = new TileMap(window->getRenderWindow());

    map->loadLevel("assets/map1.txt");
    map->loadTile("assets/TileMap.png", map->getLevel().data());
    map->printTiles();

    // Initialise le label mode au démarrage
    ui->setMode("Menu");

	minion = new Minion(1);
	minion->setPosition({ 100.0f, 100.0f });
}

Game::~Game()
{
}

void Game::run()
{
    while (window->isOpen())
    {
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
            showMenu();
			break;
        case Play:
            ui->gui.removeAllWidgets();
            menuInitialized = false;
            UpdatePlay(events);
            break;
        case Editor:
            ui->gui.removeAllWidgets();
            menuInitialized = false;
            //map->UpdateLevelEditor(events);
            break;
        }

        Render();

    }
}


void Game::UpdatePlay(const std::vector<sf::Event>& events)
{
    HandlePlayInput(events);
	minion->setPosition({ minion->getPosition().x + 1.0f, minion->getPosition().y });
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
    if (m_eGameMode == Play)
    {
        map->draw(window->getRenderWindow(), sf::RenderStates::Default);
		minion->draw(window->getRenderWindow());
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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T))
    {
        if(!bTWasPressedLastUpdate)
        {
            if (m_eGameMode == Editor) {
                m_eGameMode = Menu;
                ui->setMode("Menu");
            }
            else if (m_eGameMode == Play) {
                m_eGameMode = Editor;
                ui->setMode("Editor");
            } 
            else {
                m_eGameMode = Play;
                ui->setMode("Play");
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

void Game::showMenu()
{
    if (!menuInitialized) {
		// Ecrant d'accueil
		auto picture = ui->createPicture("assets/menu_background.png", 0, 0, window->getWidth(), window->getHeight());
        picture->setSize({ "100%", "100%" });
        ui->gui.add(picture);

		// Bouton Play
        auto boutonPlay = ui->createButton("Play", window->getWidth()/2 - 100, window->getHeight()/2 -25, 200, 50);
        boutonPlay->onPress([this]() {
            std::cout << "Play button pressed!" << std::endl;
            m_eGameMode = Play;
            ui->setMode("Play");
        });
        ui->gui.add(boutonPlay);

		// Bouton Level Editor
        auto boutonEditor = ui->createButton("Level Editor", window->getWidth()/2 - 100, window->getHeight()/2 + 25, 200, 50);
        boutonEditor->onPress([this]() {
            std::cout << "Level Editor button pressed!" << std::endl;
            m_eGameMode = Editor;
            ui->setMode("Level Editor");
            });
		ui->gui.add(boutonEditor);

        menuInitialized = true;
    }
}

