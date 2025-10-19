/* MIT License

Copyright(c) 2022 Pierre Lefebvre

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include "app.h"
#include "utils.h"
#include <iostream>

App::App()
    : _window(std::make_unique<sf::RenderWindow>(
        sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT), 32),
        "Tower Defense")),
    _quadtree(std::make_unique<Node>(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(WINDOW_WIDTH, WINDOW_HEIGHT)))),
    _entities(NB_ENTITY) {
    Random::init();
    _window->setFramerateLimit(60);

    for (auto& entity : _entities) {
        entity.init(5, sf::Color(0x33CC00FF), sf::Color::Green, 1.f);
        entity.setPlayableArea(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT))));
        // Définir une position aléatoire dans la zone de jeu
        entity.setPosition(sf::Vector2f(
            Random::value(0.f, static_cast<float>(WINDOW_WIDTH)),
            Random::value(0.f, static_cast<float>(WINDOW_HEIGHT))
        ));
        entity.move(Random::velocity()); // Vélocité aléatoire
    }
}

App::~App() = default;

void App::render() {
    _window->clear();
    for (auto& entity : _entities) {
        _window->draw(entity.getShape());
    }
    _quadtree->draw(_window.get());
    _window->display();
}

void App::resolveCollisions() {
    std::vector<Node*> leaves;
    _quadtree->getLeaves(&leaves);

    for (auto leaf : leaves) {
        unsigned int* elements;
        unsigned int nbEntities = leaf->getElements(&elements);

        for (unsigned int i = 0; i < nbEntities; ++i) {
            Entity& e = _entities[elements[i]];
            for (unsigned int j = i + 1; j < nbEntities; ++j) {
                Entity& f = _entities[elements[j]];
                if (e.isColliding(f)) {
                    e.bounce(f);
                    f.bounce(e);
                }
            }
        }
    }
}

void App::handleEvents() {
    std::optional<sf::Event> event;
    while ((event = _window->pollEvent())) {
        // Fermeture de la fenêtre
        if (event->is<sf::Event::Closed>()) {
            _window->close();
        }
        // Appui sur la touche Échap
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                _window->close();
            }
        }
    }
}


void App::run() {
    sf::Clock clock;
    while (_window->isOpen()) {
        float dt = clock.restart().asSeconds();

        // Update entities and quadtree
        _quadtree->clear();
        for (size_t i = 0; i < _entities.size(); ++i) {
            _entities[i].update(dt);
            _quadtree->add<Entity>(_entities.data(), static_cast<unsigned int>(i));
        }


        resolveCollisions();
        render();
        handleEvents();
    }
}

