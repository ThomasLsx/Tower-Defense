// Window.cpp
#include "Window.h"

Window::Window(unsigned int width , unsigned int height , const std::string& title)
    : width(width), height(height), title(title) 
{
}

void Window::create() {
    window.create(sf::VideoMode(sf::Vector2u(width, height)), title, sf::Style::Default, sf::State::Windowed);
}

void Window::close() {
    window.close();
}

bool Window::isOpen() const {
    return window.isOpen();
}

void Window::clear(const sf::Color& color) {
    window.clear(color);
}

void Window::display() {
    window.display();
}

const sf::Vector2u& Window::getWindowSize() const
{
	return window.getSize();
}

const int& Window::getWidth() const
{
	return window.getSize().x;
}

const int& Window::getHeight() const
{
	return window.getSize().y;
}

std::optional<sf::Event> Window::pollEvent() {
    return window.pollEvent();
}

sf::RenderWindow& Window::getRenderWindow() {
    return window;
}
