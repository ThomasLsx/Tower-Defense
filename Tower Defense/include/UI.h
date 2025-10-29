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
class Wave;

class UI {
public:
    UI(Window* window, Game* game, Wave* wave);
    tgui::Gui gui;
     
    void handleEvent(const sf::Event& event);
    void draw();
    
	// UI Menu Modes
    void initMenuUI();
	void showMenuUI();

	// UI Play Modes
	void initPlayUI();
	void showPlayUI();

	// UI Editor Modes
	void initEditorUI();
	void showEditorUI();

private:
    tgui::Group::Ptr menuUI;
	tgui::Group::Ptr playUI;
	tgui::Group::Ptr editorUI;

	bool isPaused = false;

    Window* window; // Pointeur vers l'instance de Window
    Game* game; // Pointeur vers l'instance de Game
	Wave* wave; // Pointeur vers l'instance de Wave

};
