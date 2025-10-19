#include "map.h"
#include "path.h"

Map::Map(int w, int h) : width(w), height(h) {}

Map::~Map() {
    for (auto* p : paths) {
        delete p;
    }
}

bool Map::isWalkable(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    for (auto& obs : obstacles) {
        if (obs.first == x && obs.second == y) return false;
    }
    return true;
}

void Map::addPath(Path* path) {
    paths.push_back(path);
}

void Map::addObstacle(int x, int y) {
    obstacles.push_back({ x, y });
}
