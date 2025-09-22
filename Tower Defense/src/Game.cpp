#include "Game.h"
#include <iostream>

Game::Game()
    : window(800, 600, "Tower Defense")
    , block(sf::Vector2f(50, 50))
    , blockPosition(100, 100)
    , detectionZone(sf::Vector2f(200, 150), sf::Vector2f(400, 200))
	, m_eGameMode(Play)
	, m_TileOptionsIndex(0)

{
    window.create();
    window.getRenderWindow().setFramerateLimit(60);
    ui = new UI(window.getRenderWindow());


    block.setFillColor(sf::Color::Blue);
    block.setPosition(blockPosition);
    // Initialise le label mode au démarrage
    ui->setMode("Play");

	m_TileMapTexture.loadFromFile("assets/TileMap.png");
    if (!m_TileMapTexture.loadFromFile("assets/TileMap.png"))
    {
        std::cerr << "Texture non chargée !" << std::endl;
        return;
    }

    for (int x = 0; x < 4; ++x)
    {
        for(int y = 0; y < 1; ++y)
        {
            auto tile = std::make_unique<sf::Sprite>(m_TileMapTexture);
            tile->setTextureRect(sf::IntRect(sf::Vector2i(x * 32, y * 32), sf::Vector2i(32, 32)));
            tile->setOrigin(sf::Vector2f(16, 16)); // Centrer l'origine de la tuile
            m_TileOptions.push_back(std::move(tile));
		}
    }
}

Game::~Game()
{
}

void Game::run()
{
    while (window.isOpen())
    {
        std::vector<sf::Event> events;
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if (!m_TileOptions.empty()) {
                    if (mouseWheelScrolled->wheel == sf::Mouse::Wheel::Vertical ||
                        mouseWheelScrolled->wheel == sf::Mouse::Wheel::Horizontal)
                    {
                        int delta = static_cast<int>(mouseWheelScrolled->delta);
                        m_TileOptionsIndex = (m_TileOptionsIndex + delta + m_TileOptions.size()) % m_TileOptions.size();
                    }
                }
                events.push_back(*event);
            }
            else if (event->is<sf::Event::KeyPressed>())
            {
                auto key = event->getIf<sf::Event::KeyPressed>();
                if (!m_TileOptions.empty()) {
                    if (key->code == sf::Keyboard::Key::Up)
                        m_TileOptionsIndex = (m_TileOptionsIndex + 1) % m_TileOptions.size();
                    else if (key->code == sf::Keyboard::Key::Down)
                        m_TileOptionsIndex = (m_TileOptionsIndex - 1 + m_TileOptions.size()) % m_TileOptions.size();
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
            UpdateLevelEditor(events);
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
    if (blockPosition.x > 800 - 50) blockPosition.x = 800 - 50;
    if (blockPosition.y > 600 - 50) blockPosition.y = 600 - 50;

    block.setPosition(blockPosition);
    ui->setSpeed(currentSpeed);

}

void Game::UpdateLevelEditor(const std::vector<sf::Event>& events)
{

}

void Game::Render()
{
    window.clear(sf::Color(50, 50, 50));

    // Dessine toutes les tuiles placées
    for(const auto& tile : m_Tiles)
    {
        window.getRenderWindow().draw(*tile);
    }

    window.getRenderWindow().draw(block);
    ui->draw();

    // Affiche la tile sélectionnée sous la souris uniquement en mode LevelEditor
    if (m_eGameMode == LevelEditor && !m_TileOptions.empty())
    {
        DrawLevelEditor();
    }

    window.display(); // doit être le dernier appel
}

void Game::DrawLevelEditor()
{
    sf::Vector2f vMousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window.getRenderWindow()));
    auto& tileSprite = *m_TileOptions[m_TileOptionsIndex];
    tileSprite.setPosition(vMousePosition);
    window.getRenderWindow().draw(tileSprite);
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
        HandleLevelEditorInput(events);
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

void Game::HandleLevelEditorInput(const std::vector<sf::Event>& events)
{
    for (const auto& event : events)
    {
        ui->handleEvent(event);
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            auto mouse = event.getIf<sf::Event::MouseButtonPressed>();
            sf::Vector2f vMousePosition(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));
            if (mouse->button == sf::Mouse::Button::Left)
                CreateTileAtPosition(vMousePosition);
            if (mouse->button == sf::Mouse::Button::Right)
                DeleteTileAtPosition(vMousePosition);
        }
    }
}

void Game::CreateTileAtPosition(const sf::Vector2f& position)
{
    // Supprime d'abord la tuile existante à la position
    DeleteTileAtPosition(position);

    // Récupère la taille de la tuile sélectionnée
    auto& refTile = *m_TileOptions[m_TileOptionsIndex];
    auto rect = refTile.getTextureRect();
    float tileWidth = static_cast<float>(rect.size.x) * refTile.getScale().x;
    float tileHeight = static_cast<float>(rect.size.y) * refTile.getScale().y;
    sf::Vector2f tileSize(tileWidth, tileHeight);

    // Calcule la position alignée sur la grille
    sf::Vector2f gridPos;
    gridPos.x = std::floor(position.x / tileSize.x) * tileSize.x + tileSize.x / 2.f;
    gridPos.y = std::floor(position.y / tileSize.y) * tileSize.y + tileSize.y / 2.f;

    auto tile = std::make_unique<sf::Sprite>(m_TileMapTexture);
    tile->setTextureRect(rect);
    tile->setScale(refTile.getScale());
    tile->setOrigin(refTile.getOrigin());
    tile->setPosition(gridPos); // Place à la position alignée sur la grille
    m_Tiles.push_back(std::move(tile));
    std::cout << "Tile created at (" << gridPos.x << ", " << gridPos.y << ")" << std::endl;
}

void Game::DeleteTileAtPosition(const sf::Vector2f& position)
{
    // Récupère la taille de la tuile sélectionnée
    auto& refTile = *m_TileOptions[m_TileOptionsIndex];
    auto rect = refTile.getTextureRect();
    float tileWidth = static_cast<float>(rect.size.x) * refTile.getScale().x;
    float tileHeight = static_cast<float>(rect.size.y) * refTile.getScale().y;
    sf::Vector2f tileSize(tileWidth, tileHeight);

    // Calcule la position alignée sur la grille
    sf::Vector2f gridPos;
    gridPos.x = std::floor(position.x / tileSize.x) * tileSize.x + tileSize.x / 2.f;
    gridPos.y = std::floor(position.y / tileSize.y) * tileSize.y + tileSize.y / 2.f;

    // Supprime la tuile à cette position si elle existe
    for (auto it = m_Tiles.begin(); it != m_Tiles.end(); ++it) {
        sf::Vector2f existingPos = (*it)->getPosition();
        if (std::abs(existingPos.x - gridPos.x) < 0.1f && std::abs(existingPos.y - gridPos.y) < 0.1f) {
            m_Tiles.erase(it);
            std::cout << "Tile deleted at (" << gridPos.x << ", " << gridPos.y << ")" << std::endl;
            break;
        }
    }
}