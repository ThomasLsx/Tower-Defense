#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Window {
public:
    Window(unsigned int width = 1920, unsigned int height = 1056, const std::string& title = "Tower Defense");
    void create();
    void close();
    bool isOpen() const;
    void clear(const sf::Color& color = sf::Color::Black);
    void display();
    const sf::Vector2u& getWindowSize() const; 
    const int& getWidth() const;
    const int& getHeight() const;
    std::optional<sf::Event> pollEvent();
    sf::RenderWindow& getRenderWindow();

private:
    sf::RenderWindow window;
    unsigned int width;
    unsigned int height;
    std::string title;
	sf::Vector2u windowSize;
	int widthScreen;
	int heightScreen;
};
