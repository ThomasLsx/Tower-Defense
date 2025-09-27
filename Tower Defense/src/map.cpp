// map.cpp
#include "map.h"
#include <iostream>

Map::Map(sf::RenderWindow& window, UI* ui)
    : window(window)
    , ui(ui)
    , m_TileOptionsIndex(0)
{
	LoadTileMap();
}

Map::~Map()
{
}

void Map::LoadTileMap()
{
    m_TileMapTexture.loadFromFile("assets/TileMap.png");
    if (!m_TileMapTexture.loadFromFile("assets/TileMap.png"))
    {
        std::cerr << "Texture non chargée !" << std::endl;
        return;
    }

    for (int y = 0; y < 2; ++y)
    {
        for (int x = 0; x < 4; ++x)
        {
            auto tile = std::make_unique<sf::Sprite>(m_TileMapTexture);
            tile->setTextureRect(sf::IntRect(sf::Vector2i(x * 32, y * 32), sf::Vector2i(32, 32)));
            tile->setOrigin(sf::Vector2f(16, 16)); // Centrer l'origine de la tuile
            tile->setScale(sf::Vector2f(3, 3));
            m_TileOptions.push_back(std::move(tile));
        }
    }
}

void Map::UpdateLevelEditor(const std::vector<sf::Event>& events)
{

}

void Map::DrawLevelEditor()
{
    sf::Vector2f vMousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    vMousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    auto& tileSprite = *m_TileOptions[m_TileOptionsIndex];
    tileSprite.setPosition(vMousePosition);
    window.draw(tileSprite);
}

void Map::HandleLevelEditorInput(const std::vector<sf::Event>& events)
{
    for (const auto& event : events)
    {
        ui->handleEvent(event);
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            auto mouse = event.getIf<sf::Event::MouseButtonPressed>();
            sf::Vector2f vMousePosition(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));
            vMousePosition = window.mapPixelToCoords(mouse->position);
            if (mouse->button == sf::Mouse::Button::Left)
                CreateTileAtPosition(vMousePosition);
            std::cout << "vMouse :" << vMousePosition.x << ", " << vMousePosition.y << ")\n";
            if (mouse->button == sf::Mouse::Button::Right)
                DeleteTileAtPosition(vMousePosition);
        }
    }
}

void Map::CreateTileAtPosition(const sf::Vector2f& position)
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

void Map::DeleteTileAtPosition(const sf::Vector2f& position)
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


