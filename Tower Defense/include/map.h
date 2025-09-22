#ifndef MAP_H
#define MAP_H

#include <vector>
#include <utility> // for std::pair

class Path;

class Map {
private:
    int width;
    int height;
    std::vector<std::pair<int /*x*/, int /*y*/>> obstacles;

public:
    Map(int w = 20, int h = 20);
    ~Map();

    bool isWalkable(int x, int y) const;

    void addObstacle(int x, int y);
};

#endif // MAP_H
