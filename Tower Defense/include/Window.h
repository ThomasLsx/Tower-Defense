#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Window {
public:
    Window(unsigned int width = 800, unsigned int height = 600, const std::string& title = "SFML Window");
    void create();
    void close();
    bool isOpen() const;
    void clear(const sf::Color& color = sf::Color::Black);
    void display();
    std::optional<sf::Event> pollEvent();
    sf::RenderWindow& getRenderWindow();
private:
    sf::RenderWindow window;
    unsigned int width;
    unsigned int height;
    std::string title;
};
