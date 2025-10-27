/* MIT License

Copyright (c) 2022 Pierre Lefebvre

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "constants.h"
#include <cmath>

struct Utils {
    /**
     * Compute square distance between 2 points
     */
    static float sd(const sf::Vector2f& a, const sf::Vector2f& b) {
        auto x = a.x - b.x;
        auto y = a.y - b.y;
        return x * x + y * y;
    }

    /**
     * Compute vector coords from 2 points
     */
    static sf::Vector2f v(const sf::Vector2f& a, const sf::Vector2f& b) {
        return b - a;
    }

    static sf::Vector2f nv(const sf::Vector2f& a, const sf::Vector2f& b) {
        sf::Vector2f u = v(a, b);
        float n = hypotf(u.x, u.y);

        // Avoid division by zero
        if (n < 1e-6f) {
            return sf::Vector2f(0.0f, 0.0f);
        }

        return u / n;
    }
};


struct Random {
    /**
     * Init randomizer
     */
    static void init() {
        srand(time(0));
    }

    /**
     * Creates a random position in [0,xMax] x [0,yMax]
     * @param maximum X coordinate
     * @param maximum Y coordinate
     */
    static sf::Vector2f position(int xMax, int yMax) {
        int x = rand() % xMax;
        int y = rand() % yMax;

        return sf::Vector2f(x, y);
    }

    /**
     * Creates a random velocity
     */
    static sf::Vector2f velocity(float maxSpeed = 100.f) {
        float angle = value(0.f, 2.f * 3.1415926535f); // Angle aléatoire en radians
        float speed = value(20.f, maxSpeed); // Vitesse aléatoire
        return sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
    }

    /**
     *
     */
    static float value(float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

};

#endif