#include "Game.h"
#include <iostream>

Game::Game()
    : window(800, 600, "SFML + TGUI Example"),
      block(sf::Vector2f(50, 50)),
      blockPosition(100, 100),
      detectionZone(sf::Vector2f(200, 150), sf::Vector2f(400, 200)),
      isRunning(true)
{
    window.create();
    window.getRenderWindow().setFramerateLimit(60);
    ui = new UI(window.getRenderWindow());
    block.setFillColor(sf::Color::Blue);
    block.setPosition(blockPosition);
}

void Game::run()
{
    while (window.isOpen() && isRunning)
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional<sf::Event> event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
        ui->handleEvent(*event);
        if (event->is<sf::Event::MouseButtonPressed>())
        {
            auto mouse = event->getIf<sf::Event::MouseButtonPressed>();
            sf::Vector2f pos(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));
            if (detectionZone.contains(pos))
            {
                if (mouse->button == sf::Mouse::Button::Left)
                    std::cout << "Left click at (" << mouse->position.x << ", " << mouse->position.y << ")\n";
                if (mouse->button == sf::Mouse::Button::Right)
                    std::cout << "Right click at (" << mouse->position.x << ", " << mouse->position.y << ")\n";
            }
        }
        if (event->is<sf::Event::KeyPressed>())
        {
            auto key = event->getIf<sf::Event::KeyPressed>();
            if (key->code == sf::Keyboard::Key::Up) keyState.up = true;
            if (key->code == sf::Keyboard::Key::Down) keyState.down = true;
            if (key->code == sf::Keyboard::Key::Left) keyState.left = true;
            if (key->code == sf::Keyboard::Key::Right) keyState.right = true;
            if (key->code == sf::Keyboard::Key::A) keyState.a = true;
            if (key->code == sf::Keyboard::Key::E) keyState.e = true;
        }
        if (event->is<sf::Event::KeyReleased>())
        {
            auto key = event->getIf<sf::Event::KeyReleased>();
            if (key->code == sf::Keyboard::Key::Up) keyState.up = false;
            if (key->code == sf::Keyboard::Key::Down) keyState.down = false;
            if (key->code == sf::Keyboard::Key::Left) keyState.left = false;
            if (key->code == sf::Keyboard::Key::Right) keyState.right = false;
            if (key->code == sf::Keyboard::Key::A) keyState.a = false;
            if (key->code == sf::Keyboard::Key::E) keyState.e = false;
        }
    }
    float moveAmount = 1.5f;
    float dx = 0.f, dy = 0.f;
    if (keyState.up)
        dy -= 1.f;
    if (keyState.down)
        dy += 1.f;
    if (keyState.left)
        dx -= 1.f;
    if (keyState.right)
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
}

void Game::update()
{
    block.setPosition(blockPosition);
    ui->setSpeed(currentSpeed);
}

void Game::render()
{
    window.clear(sf::Color(50, 50, 50));
    window.getRenderWindow().draw(block);
    ui->draw();
    window.display();
}
