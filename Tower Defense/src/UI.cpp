#include "UI.h"
#include <iostream>

static void onButtonClick()
{
    std::cout << "OK button pressed!" << std::endl;
}

UI::UI(sf::RenderWindow& window)
{
    gui.setTarget(window);
    speedLabel = tgui::Label::create();
    speedLabel->setText("Vitesse: 0");
    speedLabel->setPosition(10, 10);
    speedLabel->setTextSize(18);
    speedLabel->getRenderer()->setTextColor(tgui::Color::White);
    gui.add(speedLabel, "speedLabel");
    button = tgui::Button::create("OK");
    button->setPosition(350, 500);
    button->setSize(100, 40);
    button->onPress(onButtonClick);
    gui.add(button);
}

void UI::handleEvent(const sf::Event& event)
{
    gui.handleEvent(event);
}

void UI::draw()
{
    gui.draw();
}

void UI::setSpeed(float speed)
{
    if (speedLabel)
        speedLabel->setText("Vitesse: " + std::to_string(speed));
}
