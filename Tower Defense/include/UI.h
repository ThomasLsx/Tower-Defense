// UI.h
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
    tgui::Gui gui;
	tgui::Label::Ptr createLabel(const std::string& text, int x, int y);
    tgui::Button::Ptr createButton(const std::string& text, int x, int y, int width, int height);
    
    void handleEvent(const sf::Event& event);
    void draw();
    void setMode(const std::string& mode);
    
	//void createMenu();

  private:
    tgui::Label::Ptr modeLabel;
};
