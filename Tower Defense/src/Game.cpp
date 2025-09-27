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
    map = new Map(window->getRenderWindow(), ui);
    
    
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
            else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if (!map->GetTileOptions().empty()) {
                    if (mouseWheelScrolled->wheel == sf::Mouse::Wheel::Vertical ||
                        mouseWheelScrolled->wheel == sf::Mouse::Wheel::Horizontal)
                    {
                        int delta = static_cast<int>(mouseWheelScrolled->delta);
                        map->SetTileOptionsIndex() = (map->GetTileOptionsIndex() + delta + map->GetTileOptions().size()) % map->GetTileOptions().size();
                    }
                }
                events.push_back(*event);
            }
            else if (event->is<sf::Event::KeyPressed>())
            {
                auto key = event->getIf<sf::Event::KeyPressed>();
                if (!map->GetTileOptions().empty()) {
                    if (key->code == sf::Keyboard::Key::Up)
                        map->SetTileOptionsIndex() = (map->GetTileOptionsIndex() + 1) % map->GetTileOptions().size();
                    else if (key->code == sf::Keyboard::Key::Down)
                        map->SetTileOptionsIndex() = (map->GetTileOptionsIndex() - 1 + map->GetTileOptions().size()) % map->GetTileOptions().size();
                }
                events.push_back(*event);
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
        case LevelEditor:
            map->UpdateLevelEditor(events);
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

    // Dessine toutes les tuiles placées
    for(const auto& tile : map->GetTiles())
    {
        window->getRenderWindow().draw(*tile);
    }

    window->getRenderWindow().draw(block);
    ui->draw();

    // Affiche la tile sélectionnée sous la souris uniquement en mode LevelEditor
    if (m_eGameMode == LevelEditor && !map->GetTileOptions().empty())
    {
        map->DrawLevelEditor();
    }

    window->display(); // doit être le dernier appel
}

void Game::HandleInput(const std::vector<sf::Event>& events)
{
    static bool bTWasPressedLastUpdate = false;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T))
    {
        if(!bTWasPressedLastUpdate)
        {
            if (m_eGameMode == Play) {
                m_eGameMode = LevelEditor;
                ui->setMode("LevelEditor");
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
    if (m_eGameMode == LevelEditor)
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

