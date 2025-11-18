// UI.h
#pragma once

#include <TGUI/Core.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include "Window.h"
#include "Game.h"

class Window;
class Game;
class WaveManager;

class UI {
public:
    UI(Window* window, Game* game);
    tgui::Gui gui;
     
    void handleEvent(const sf::Event& event);
    void updateUILayout();
    void draw();

    // UI Menu Modes
    void initMenuUI();
    void showMenuUI();

    // UI Play Modes
    void initPlayUI();
    void updatePlayUI();
    void showPlayUI();

    // UI Editor Modes
    void initEditorUI();
    void showEditorUI();

private:
    tgui::Group::Ptr menuUI;
    tgui::Group::Ptr playUI;
    tgui::Group::Ptr editorUI;

    bool isPaused = false;

	float prevWindowWidth;
	float prevWindowHeight;

    Window* window; // Pointeur vers l'instance de Window
    Game* game; // Pointeur vers l'instance de Game

    // Centre horizontalement un widget à la position verticale y
    void centerWidget(tgui::Widget::Ptr widget, float y);
    // Place une liste de boutons verticalement à partir du coin gauche (x = margin)
    enum class ButtonAlign { Left, Center, Right };
    void placeButtonsStacked(const std::vector<tgui::Button::Ptr>& buttons, float startY, float margin, float spacing, ButtonAlign align);
};
