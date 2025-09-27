// UI.cpp
#include "UI.h"
#include <iostream>

static void onButtonClick()
{
    std::cout << "OK button pressed!" << std::endl;
}

UI::UI(sf::RenderWindow& window)
{
    gui.setTarget(window);

    modeLabel = createLabel("Mode: Play", 10, 10);
	gui.add(modeLabel, "modeLabel");

    speedLabel = createLabel("Vitesse: 0", 10, 40);
    gui.add(speedLabel, "speedLabel");

    button = createButton("OK", 350, 500, 100, 40);
    button->onPress(onButtonClick);
    gui.add(button);
}

tgui::Label::Ptr UI::createLabel(const std::string& text, int x, int y)
{
    auto label = tgui::Label::create();
    label->setText(text);
    label->setPosition(x, y);
    label->setTextSize(18);
    label->getRenderer()->setTextColor(tgui::Color::White);
    return label;
}

tgui::Button::Ptr UI::createButton(const std::string& text, int x, int y, int width, int height)
{
    auto btn = tgui::Button::create(text);
    btn->setPosition(x, y);
    btn->setSize(width, height);
    return btn;
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

void UI::setMode(const std::string& mode)
{
    if (modeLabel)
        modeLabel->setText("Mode: " + mode);
}
