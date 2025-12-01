// map.cpp
#include "map.h"
#include "towerManager.h"
#include "Window.h"
#include "UI.h"
#include "path.h"
#include <sstream> 
#include <SFML/Graphics/View.hpp>

TileMap::TileMap(sf::RenderWindow& window) : window(window)
{
    width = 40;
    height = 22;
    scale = 1.5;
    tileSize = sf::Vector2u(32, 32);
    m_level = m_towerLevel = std::vector<int>(width * height, 1);

    // Pour l'édition de tiles
    m_TileIndex = 0;
    m_TileOptions = 8;

    m_TowerIndex = -1;   // Type de tour sélectionné (aucune sélection au départ)
    m_TowerOptions = 4; // Nombre de types de tours (Basic, Sniper, Speed, slow)
    // Suppression du système de vue
}

bool TileMap::loadTile(const std::filesystem::path& tileset, const int* tiles)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset.string()))
        return false;

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(width * height * 6);

    // populate the vertex array, with two triangles per tile
    for (unsigned int i = 0; i < width; ++i)
    {
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            const int tileNumber = tiles[i + j * width];

            // find its position in the tileset texture
            const int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            const int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            // get a pointer to the triangles' vertices of the current tile
            sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

            // define the 6 corners of the two triangles
            triangles[0].position = sf::Vector2f(i * tileSize.x * scale, j * tileSize.y * scale);
            triangles[1].position = sf::Vector2f((i + 1) * tileSize.x * scale, j * tileSize.y * scale);
            triangles[2].position = sf::Vector2f(i * tileSize.x * scale, (j + 1) * tileSize.y * scale);
            triangles[3].position = sf::Vector2f(i * tileSize.x * scale, (j + 1) * tileSize.y * scale);
            triangles[4].position = sf::Vector2f((i + 1) * tileSize.x * scale, j * tileSize.y * scale);
            triangles[5].position = sf::Vector2f((i + 1) * tileSize.x * scale, (j + 1) * tileSize.y * scale);

            // define the 6 matching texture coordinates
            triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
        }
    }

    return true;
}

bool TileMap::loadLevel(const std::filesystem::path& levelFilePath)
{
    std::vector<int> level;
    std::ifstream file(levelFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open level file: " << levelFilePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int tile;
        while (iss >> tile) {
            level.push_back(tile);
        }
    }
    setLevel(level);

    loadTile("assets/TileMap.png", m_level.data());
    m_towerLevel = m_level;


    return true;
}

bool TileMap::saveLevel(const std::filesystem::path& levelFilePath)
{
    std::ofstream file(levelFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open level file for writing: " << levelFilePath << std::endl;
        return false;
    }
    for (size_t i = 0; i < m_level.size(); ++i) {
        file << m_level[i];
        if ((i + 1) % getWidth() == 0) {
            file << '\n';
        }
        else {
            file << ' ';
        }
    }
    return true;
}

void TileMap::updateTile(int x, int y, const int index, sf::Vector2u tileSize)
{
    if (x >= 0 && x < static_cast<int>(width) && y >= 0 && y < static_cast<int>(height)) {
        m_level[x + y * width] = index;

        // find its position in the tileset texture
        const int tu = index % (m_tileset.getSize().x / tileSize.x);
        const int tv = index / (m_tileset.getSize().x / tileSize.x);

        // get a pointer to the triangles' vertices of the current tile
        sf::Vertex* triangles = &m_vertices[(x + y * width) * 6];

        // define the 6 matching texture coordinates
        triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
        triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
        triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
        triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);

        mapChanged = true;
    }
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Suppression du système de vue
    states.transform *= getTransform();
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
}

sf::Vector2f TileMap::Tile2Position(const sf::Vector2u& tile) const
{
    float x = tile.x * tileSize.x * scale + tileSize.x * scale / 2.f;
    float y = tile.y * tileSize.y * scale + tileSize.y * scale / 2.f;
    return sf::Vector2f(x, y);
}

const std::vector<std::vector<int>> TileMap::getLevel2D() const
{
    std::vector<std::vector<int>>  m_level2D;
    for (int j = 0; j < static_cast<int>(height); ++j)
    {
        std::vector<int> row;
        for (unsigned int i = 0; i < width; ++i)
        {
            // get the current tile number
            const int tileNumber = m_level[i + j * width];
            row.push_back(tileNumber);
        }
        m_level2D.push_back(row);
    }
    return m_level2D;
}

const std::vector<std::vector<int>> TileMap::getTowerLevel2D() const
{
    std::vector<std::vector<int>>  m_towerLevel2D;
    for (int j = 0; j < static_cast<int>(height); ++j)
    {
        std::vector<int> row;
        for (unsigned int i = 0; i < width; ++i)
        {
            // get the current tile number
            const int tileNumber = m_towerLevel[i + j * width];
            row.push_back(tileNumber);
        }
        m_towerLevel2D.push_back(row);
    }
    return m_towerLevel2D;
}

const sf::Vector2u TileMap::getCurentTile(sf::Vector2f position) const
{
    // donne la valeur de la tuile à la position (x, y)
    unsigned int x = static_cast<unsigned int>(position.x / (tileSize.x * scale));
    unsigned int y = static_cast<unsigned int>(position.y / (tileSize.y * scale));
    if (x < width && y < height)
    {
        return sf::Vector2u(x, y);
    }
    std::cout << "getCurentTile: Coordinates (" << x << ", " << y << ") sont hors limites.\n";
    return sf::Vector2u(0, 0); // Retourne (0,0) si les coordonnées sont hors limites
}

/**
* @brief Trouve la première case d'une valeur donnée sur le bord de la grille
* @param value La valeur à chercher
* @return sf::Vector2u (x, y) de la case trouvée, ou (0,0) si non trouvée
*/
sf::Vector2u TileMap::findEdgeTile(int value) const {
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            if (m_level[x + y * width] == value)
                return sf::Vector2u(x, y);
        }
    }
    // Si rien trouvé, retourne (0,0)
    return sf::Vector2u(0, 0);
}

void TileMap::printTiles() const
{
    std::cout << "Contenu de m_level :\n";

    const int* level = getLevel().data();
    for (unsigned int j = 0; j < height; ++j)
    {
        for (unsigned int i = 0; i < width; ++i)
        {
            // get the current tile number
            const int tileNumber = level[i + j * width];
            std::cout << tileNumber << " ";
        }
        std::cout << "\n"; // Nouvelle ligne pour chaque rangée
    }
}


/// Level Editor methods

void TileMap::DrawMouseHover()
{
    sf::Vector2f vMousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2u tSize = getTileSize();
    float scale = getScale();

    // Calcule la position alignée sur la grille
    unsigned int i = static_cast<unsigned int>(vMousePosition.x / (tSize.x * scale));
    unsigned int j = static_cast<unsigned int>(vMousePosition.y / (tSize.y * scale));
    sf::Vector2f gridPos(i * tSize.x * scale, j * tSize.y * scale);

    // Prépare le sprite de la tuile sélectionnée
    sf::Sprite tileSprite(m_tileset);

    // Calcule le rectangle de texture à partir de l'index sélectionné
    int tu = m_TileIndex % (m_tileset.getSize().x / tSize.x);
    int tv = m_TileIndex / (m_tileset.getSize().x / tSize.x);
    sf::IntRect rect;
    rect.position = sf::Vector2i(tu * static_cast<int>(tSize.x), tv * static_cast<int>(tSize.y));
    rect.size = sf::Vector2i(static_cast<int>(tSize.x), static_cast<int>(tSize.y));
    tileSprite.setTextureRect(rect);
    tileSprite.setPosition(gridPos);
    tileSprite.setScale(sf::Vector2f(scale, scale));
    tileSprite.setColor(sf::Color(255, 255, 255));

    window.draw(tileSprite);

    // Dessine une bordure autour de la tuile
    sf::RectangleShape border(sf::Vector2f(tSize.x * scale, tSize.y * scale));
    border.setPosition(gridPos);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(3.f);
    border.setOutlineColor(sf::Color::Black);
    window.draw(border);
}

void TileMap::HandleLevelEditorInput(const std::vector<sf::Event>& events)
{
    for (const sf::Event& event : events)
    {
        // Gestion de la molette de la souris
        if (event.is<sf::Event::MouseWheelScrolled>())
        {
            auto mouseWheel = event.getIf<sf::Event::MouseWheelScrolled>();
            int delta = static_cast<int>(mouseWheel->delta);
            SetTileIndex() = (GetTileIndex() + delta + GetTileOptions()) % GetTileOptions();
        }
        // Gestion des touches clavier
        if (event.is<sf::Event::KeyPressed>())
        {
            auto key = event.getIf<sf::Event::KeyPressed>();
            if (key->code == sf::Keyboard::Key::Up)
                SetTileIndex() = (GetTileIndex() + 1) % GetTileOptions();
            else if (key->code == sf::Keyboard::Key::Down)
                SetTileIndex() = (GetTileIndex() - 1 + GetTileOptions()) % GetTileOptions();
            // Enregistrement du niveau
            if (key->code == sf::Keyboard::Key::S)
            {
                std::filesystem::path levelFilePath = "assets/map1.txt";
                if (saveLevel(levelFilePath))
                    std::cout << "Level saved to " << levelFilePath << std::endl;
                else
                    std::cerr << "Failed to save level to " << levelFilePath << std::endl;
            }
        }
        // Gestion des clics souris
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            sf::Event::MouseButtonPressed mouse = *event.getIf<sf::Event::MouseButtonPressed>();
            sf::Vector2f vMousePosition(static_cast<float>(mouse.position.x), static_cast<float>(mouse.position.y));
            vMousePosition = window.mapPixelToCoords(mouse.position);
            if (mouse.button == sf::Mouse::Button::Left)
                CreateTileAtPosition(vMousePosition);
            if (mouse.button == sf::Mouse::Button::Right)
                DeleteTileAtPosition(vMousePosition);
            std::cout << "Mouse Click at (" << vMousePosition.x << ", " << vMousePosition.y << ")\n";
            std::cout << "Case (" << int(vMousePosition.x / (getTileSize().x * getScale())) << ", " << int(vMousePosition.y / (getTileSize().y * getScale())) << ")\n";
            printTiles();
        }
    }
}

void TileMap::CreateTileAtPosition(const sf::Vector2f& position)
{
    unsigned int i = static_cast<unsigned int>(position.x / (getTileSize().x * getScale()));
    unsigned int j = static_cast<unsigned int>(position.y / (getTileSize().y * getScale()));

    if (i < getWidth() && j < getHeight()) {
        updateTile(i, j, m_TileIndex, getTileSize());
    }
}

void TileMap::DeleteTileAtPosition(const sf::Vector2f& position)
{
    unsigned int i = static_cast<unsigned int>(position.x / (getTileSize().x * getScale()));
    unsigned int j = static_cast<unsigned int>(position.y / (getTileSize().y * getScale()));

    if (i < getWidth() && j < getHeight()) {
        updateTile(i, j, 0, getTileSize()); // Supposer que 0 est la tuile vide (herbe)
    }
}


/// Placement des tours
void TileMap::HandleTowerInput(const std::vector<sf::Event>& events, TowerManager& towerManager)
{
    for (const sf::Event& event : events)
    {
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            sf::Event::MouseButtonPressed mouse = *event.getIf<sf::Event::MouseButtonPressed>();
            sf::Vector2f vMousePosition(static_cast<float>(mouse.position.x), static_cast<float>(mouse.position.y));
            vMousePosition = window.mapPixelToCoords(mouse.position);
            if (mouse.button == sf::Mouse::Button::Left)
                PlaceTower(vMousePosition, towerManager);
            if (mouse.button == sf::Mouse::Button::Right)
                RemoveTower(vMousePosition, towerManager);

            std::cout << "Tower Mode Click at (" << vMousePosition.x << ", " << vMousePosition.y << ")\n";
        }
    }
}

void TileMap::PlaceTower(const sf::Vector2f& position, TowerManager& towerManager)
{
    // Ne rien faire si aucune tourelle n'est sélectionnée
    if (m_TowerIndex == -1) {
        std::cout << "Aucune tourelle sélectionnée, placement impossible.\n";
        return;
    }

    // 1. Obtenir les coordonnées de la grille (i, j)
    unsigned int i = static_cast<unsigned int>(position.x / (tileSize.x * scale));
    unsigned int j = static_cast<unsigned int>(position.y / (tileSize.y * scale));

    // 2. Vérifier si les coordonnées sont valides
    if (i >= getWidth() || j >= getHeight()) {
        std::cout << "Placement hors carte." << std::endl;
        return;
    }

    // 3. Obtenir le type de tuile à cet endroit
    const int tileType = m_level[i + j * width];

    if (tileType != 1)
    {
        std::cout << "Placement de tour interdit sur ce type de tuile (" << tileType << ")\n";
        return;
    }

    // 4.2 Vérifier s'il y a déjà une tour ici
    if (towerManager.isTowerAt(i, j, tileSize, scale)) {
        std::cout << "Une tour existe déjà à cet endroit." << std::endl;
        return;
    }

	// 6. Tester le chemin entre le château et la sortie avec la tour placée
    m_towerLevel[i + j * width] = 9;

    Pathfinding pf(getTowerLevel2D());
    sf::Vector2u pos = getCastleTile();
    Position start = { pos.y, pos.x };
    sf::Vector2u endTile = getSpawnTile();
    Position goal = { endTile.y, endTile.x };
    std::optional<std::vector<Position>> pathOpt = pf.findPath(start, goal);

    if (!pathOpt.has_value() || pathOpt->empty()) {

        m_towerLevel[i + j * width] = m_level[i + j * width];
		std::cout << "[Debug] Placement de la tour bloque le chemin entre le château et la sortie. Annulation du placement.\n";
        return;
    }

    // 7. Stocker la tuile modifiée
    lastModifiedTile = sf::Vector2u(i, j);

    // 8. Calculer la position CENTRÉE de la tuile pour la tour
    float centeredX = (i * tileSize.x * scale) + (tileSize.x * scale / 2.0f);
    float centeredY = (j * tileSize.y * scale) + (tileSize.y * scale / 2.0f);
    sf::Vector2f towerPosition(centeredX, centeredY);

	// 9. Ajouter la tour au TowerManager
    towerManager.addTower(towerPosition, m_TowerIndex);
    std::cout << "Tour de type " << m_TowerIndex << " placee sur la tuile (" << i << ", " << j << ")\n";

    mapChanged = true;
}

void TileMap::RemoveTower(const sf::Vector2f& position, TowerManager& towerManager)
{
    unsigned int i = static_cast<unsigned int>(position.x / (getTileSize().x * getScale()));
    unsigned int j = static_cast<unsigned int>(position.y / (getTileSize().y * getScale()));

    if (i < getWidth() && j < getHeight()) {
        // 1. Supprimer la tour du TowerManager
        towerManager.removeTowerAt(i, j, getTileSize(), getScale());

        // 2. Remettre la tuile à sa valeur d'origine
        m_towerLevel[i + j * width] = m_level[i + j * width];

        // 3. Stocker la tuile modifiée et déclencher le recalcul
        lastModifiedTile = sf::Vector2u(i, j);
        mapChanged = true;

        std::cout << "Tour supprimée à (" << i << ", " << j << ")\n";
    }
}
