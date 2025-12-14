#include "path.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <utility>
#include <vector>

const std::vector<Position> directions = 
{
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0}
};

Pathfinding::Pathfinding(const std::vector<std::vector<int>>& grid)
    : m_grid(grid)
{
    if (grid.empty())
    {
        return;
    }

    m_height = grid.size();
    m_width = grid[0].size();

    m_distanceMap.resize(m_width * m_height, std::numeric_limits<int>::max());
}

unsigned int Pathfinding::getIndex(int x, int y) const
{
    return x + y * m_width;
}

bool Pathfinding::isValid(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

bool Pathfinding::isWalkable(int x, int y) const
{
    // 9 = Tour, 2 = Mur.
    // 0 = Spawn, 1 = Herbe, 3 = Chateau sont praticables.
    int tile = m_grid[y][x];
    return tile != 9 && tile != 2;
}

void Pathfinding::generateFlowField(Position targetPos)
{
    m_vectorMap.clear();
    std::fill(m_distanceMap.begin(), m_distanceMap.end(), std::numeric_limits<int>::max());
    m_isPathBlocked = false;

    if (!isValid(targetPos.x, targetPos.y))
    {
        m_isPathBlocked = true;
        return;
    }

    // Utilisé comme une file de priorité pour Dijkstra: {distance, index}
    std::set<std::pair<int, unsigned int>> frontier;

    unsigned int startIndex = getIndex(targetPos.x, targetPos.y);
    m_distanceMap[startIndex] = 0;
    frontier.insert({ 0, startIndex });

   m_vectorMap[startIndex] = { 0, 0 };

    // Algorithme de Dijkstra Inversé
    while (!frontier.empty())
    {
        auto top = *frontier.begin();
        frontier.erase(frontier.begin());

        int currentDist = top.first;
        unsigned int currentIndex = top.second;

        int cx = currentIndex % m_width;
        int cy = currentIndex / m_width;

        for (const auto& dir : directions)
        {
            int nx = cx + dir.x;
            int ny = cy + dir.y;

            if (isValid(nx, ny) && isWalkable(nx, ny))
            {
                int newDist = currentDist + 1;
                unsigned int nIndex = getIndex(nx, ny);

                if (newDist < m_distanceMap[nIndex])
                {
                    // Met à jour la distance
                    m_distanceMap[nIndex] = newDist;
                    frontier.insert({ newDist, nIndex });

                    // Met à jour le vecteur de direction (pointe vers la case actuelle)
                    m_vectorMap[nIndex] = { cx - nx, cy - ny };
                }
            }
        }
    }

    // Vérification de blocage: si une tuile praticable n'est pas atteinte
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            if (isWalkable(x, y))
            {
                unsigned int idx = getIndex(x, y);

                if (m_distanceMap[idx] == std::numeric_limits<int>::max())
                {
                    m_isPathBlocked = true;
                    return;
                }
            }
        }
    }
}

std::optional<Position> Pathfinding::getNearestValidPosition(Position startPos, int maxRadius) const {
    // 1. Vérification immédiate
    if (isValid(startPos.x, startPos.y) && isWalkable(startPos.x, startPos.y)) {
        unsigned int idx = getIndex(startPos.x, startPos.y);
        if (m_distanceMap[idx] != std::numeric_limits<int>::max()) {
            return startPos;
        }
    }

    // 2. Recherche en couches (Layer-based BFS)
    std::vector<Position> currentLayer = { startPos };
    std::set<Position> visited = { startPos };

    for (int r = 0; r < maxRadius; ++r) {
        std::vector<Position> nextLayer;

        for (const auto& pos : currentLayer) {
            for (const auto& dir : directions) {
                Position next = { pos.x + dir.x, pos.y + dir.y };

                // Si déjà visité ou innaccessible, on ignore
                if (!isValid(next.x, next.y) || visited.count(next)) continue;

                visited.insert(next);

                if (isWalkable(next.x, next.y)) {
                    unsigned int idx = getIndex(next.x, next.y);
                    if (m_distanceMap[idx] != std::numeric_limits<int>::max()) {
                        return next; // TYrouvé
                    }
                }

                nextLayer.push_back(next);
            }
        }
        currentLayer = std::move(nextLayer);
        if (currentLayer.empty()) break; // Plus rien à explorer
    }

    return std::nullopt; // Rien trouvé
}

std::optional<std::pair<int, int>> Pathfinding::getVector(unsigned int tileIndex) const
{
    auto it = m_vectorMap.find(tileIndex);

    if (it != m_vectorMap.end())
    {
        return it->second;
    }

    return std::nullopt;
}

std::optional<Position> Pathfinding::getNextMove(Position currentPos) const
{
    if (!isValid(currentPos.x, currentPos.y))
    {
        return std::nullopt;
    }

    unsigned int idx = getIndex(currentPos.x, currentPos.y);
    auto vecOpt = getVector(idx);

    if (vecOpt.has_value())
    {
        return Position{ currentPos.x + vecOpt->first, currentPos.y + vecOpt->second };
    }

    return std::nullopt;
}