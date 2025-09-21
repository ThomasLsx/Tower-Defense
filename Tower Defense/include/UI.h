#pragma once

#include <TGUI/Core.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

class UI {
  public:
    UI(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);
    void draw();
    void setSpeed(float speed);

  private:
    tgui::Gui gui;
    tgui::Label::Ptr speedLabel;
    tgui::Button::Ptr button;
};
