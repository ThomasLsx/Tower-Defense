// Game.cpp
#include "Game.h"
#include <iostream>

Game::Game()
    : block(sf::Vector2f(50, 50))
    , blockPosition(100, 100)
    , detectionZone(sf::Vector2f(200, 150), sf::Vector2f(400, 200))
	, m_eGameMode(Play)

{
    window = new Window();
    window->create();
    window->getRenderWindow().setFramerateLimit(60);

    ui = new UI(window->getRenderWindow());
    map = new TileMap(window->getRenderWindow(), ui);

    map->loadLevel("assets/map1.txt");
    map->loadTile("assets/TileMap.png", map->getLevel().data());
    map->printTiles();

    block.setFillColor(sf::Color::Blue);
    block.setPosition(blockPosition);
    // Initialise le label mode au démarrage
    ui->setMode("Play");

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
            }
        }

        HandleInput(events);

        switch (m_eGameMode) {
        case Play:
            UpdatePlay(events);
            break;
        case Editor:
            //map->UpdateLevelEditor(events);
            break;
        }

        Render();
    }
}


void Game::UpdatePlay(const std::vector<sf::Event>& events)
{
    HandlePlayInput(events);
    float moveAmount = 1.5f;
    float dx = 0.f, dy = 0.f;
    if (keyState.z)
        dy -= 1.f;
    if (keyState.s)
        dy += 1.f;
    if (keyState.q)
        dx -= 1.f;
    if (keyState.d)
        dx += 1.f;
    currentSpeed = 0.f;
    if (dx != 0.f || dy != 0.f) {
        float length = std::sqrt(dx * dx + dy * dy);
        dx /= length;
        dy /= length;
        blockPosition.x += dx * moveAmount;
        blockPosition.y += dy * moveAmount;
        currentSpeed = moveAmount;
    }
    if (keyState.a)
        block.rotate(sf::degrees(-2.f));
    if (keyState.e)
        block.rotate(sf::degrees(2.f));
    if (blockPosition.x < 0) blockPosition.x = 0;
    if (blockPosition.y < 0) blockPosition.y = 0;
    if (blockPosition.x > window->getWidth()) blockPosition.x = window->getWidth();
    if (blockPosition.y > window->getHeight()) blockPosition.y = window->getHeight();

    block.setPosition(blockPosition);
    ui->setSpeed(currentSpeed);

}



void Game::Render()
{
    window->clear(sf::Color(50, 50, 50));
    map->draw(window->getRenderWindow(), sf::RenderStates::Default);
    window->getRenderWindow().draw(block);
    ui->draw();

    // Affiche la tuile sélectionnée sous la souris uniquement en mode Editor
    if (m_eGameMode == Editor)
        map->DrawMouseHover();

    window->display();
}

void Game::HandleInput(const std::vector<sf::Event>& events)
{
    static bool bTWasPressedLastUpdate = false;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T))
    {
        if(!bTWasPressedLastUpdate)
        {
            if (m_eGameMode == Play) {
                m_eGameMode = Editor;
                ui->setMode("Editor");
            } else {
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
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            auto mouse = event.getIf<sf::Event::MouseButtonPressed>();
            sf::Vector2f pos(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));
            if (detectionZone.contains(pos))
            {
                if (mouse->button == sf::Mouse::Button::Left)
                    std::cout << "Left click at (" << mouse->position.x << ", " << mouse->position.y << ")\n";
                if (mouse->button == sf::Mouse::Button::Right)
                    std::cout << "Right click at (" << mouse->position.x << ", " << mouse->position.y << ")\n";
            }
        }
        if (event.is<sf::Event::KeyPressed>())
        {
            auto key = event.getIf<sf::Event::KeyPressed>();
            if (key->code == sf::Keyboard::Key::Z) keyState.z = true;
            if (key->code == sf::Keyboard::Key::S) keyState.s = true;
            if (key->code == sf::Keyboard::Key::Q) keyState.q = true;
            if (key->code == sf::Keyboard::Key::D) keyState.d = true;
            if (key->code == sf::Keyboard::Key::A) keyState.a = true;
            if (key->code == sf::Keyboard::Key::E) keyState.e = true;
        }
        if (event.is<sf::Event::KeyReleased>())
        {
            auto key = event.getIf<sf::Event::KeyReleased>();
            if (key->code == sf::Keyboard::Key::Z) keyState.z = false;
            if (key->code == sf::Keyboard::Key::S) keyState.s = false;
            if (key->code == sf::Keyboard::Key::Q) keyState.q = false;
            if (key->code == sf::Keyboard::Key::D) keyState.d = false;
            if (key->code == sf::Keyboard::Key::A) keyState.a = false;
            if (key->code == sf::Keyboard::Key::E) keyState.e = false;
        }
    }
}

