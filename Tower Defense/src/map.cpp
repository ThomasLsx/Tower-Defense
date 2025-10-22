// map.cpp
#include "map.h"

TileMap::TileMap(sf::RenderWindow& window) : window(window)
{
    width = 20;
    height = 11;
	scale = 3.0f;
	tileSize = sf::Vector2u(32, 32);
	m_level = std::vector<int>(width * height, 0);
    m_TileIndex = 0;
    m_TileOptions = 8;
}

bool TileMap::loadTile(const std::filesystem::path& tileset, const int* tiles)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
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
        } else {
            file << ' ';
        }
    }
	return true;
}

void TileMap::updateTile(int x, int y, const int index, sf::Vector2u tileSize)
{
    if (x >= 0 && x < width && y >= 0 && y < height) {
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
    }
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
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
        std::cout << "\n"; // Nouvelle ligne pour chaque rang�e
    }
}

// Level Editor methods

void TileMap::DrawMouseHover()
{
    sf::Vector2f vMousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2u tSize = getTileSize();
    float scale = getScale();

    // Calcule la position align�e sur la grille
    unsigned int i = static_cast<unsigned int>(vMousePosition.x / (tSize.x * scale));
    unsigned int j = static_cast<unsigned int>(vMousePosition.y / (tSize.y * scale));
    sf::Vector2f gridPos(i * tSize.x * scale, j * tSize.y * scale);

    // Pr�pare le sprite de la tuile s�lectionn�e
    sf::Sprite tileSprite(m_tileset);

    // Calcule le rectangle de texture � partir de l'index s�lectionn�
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
    border.setOutlineThickness(5.f);
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
        updateTile(i, j, 9, getTileSize());
    }
}
