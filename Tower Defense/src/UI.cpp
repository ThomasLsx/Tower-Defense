// UI.cpp
#include "UI.h"
#include <iostream>

UI::UI(Window* window, Game* game, Wave* wave) : window(window), game(game), wave(wave)
{
    gui.setTarget(window->getRenderWindow());

	initMenuUI();
	initPlayUI();
	initEditorUI();
}

void UI::handleEvent(const sf::Event& event)
{
    gui.handleEvent(event);
}

void UI::draw()
{
    gui.draw();
}

void UI::initMenuUI()
{
    menuUI = tgui::Group::create();

    // Ecrant d'accueil
    auto picture = tgui::Picture::create("assets/menu_background.png");
	picture->setPosition(0, 0);
    picture->setSize(window->getWidth(), window->getHeight());
    menuUI->add(picture);

    // Bouton Play
    auto boutonPlay = tgui::Button::create("Play");
    boutonPlay->setSize(200, 50);
	tgui::Vector2f boutonPlaySize = boutonPlay->getSize();
	boutonPlay->setPosition(window->getWidth() / 2 - boutonPlaySize.x / 2, window->getHeight() / 2 - boutonPlaySize.y);
    boutonPlay->onPress([this]() {
        std::cout << "Play button pressed!" << std::endl;
		game->setGameMode(Game::GameMode::Play);
        });
    menuUI->add(boutonPlay);

    // Bouton Level Editor
    auto boutonEditor = tgui::Button::create("Level Editor");
    boutonEditor->setSize(200, 50);
	tgui::Vector2f boutonEditorSize = boutonEditor->getSize();
    boutonEditor->setPosition(window->getWidth() / 2 - boutonEditorSize.x / 2, window->getHeight() / 2 + boutonEditorSize.y);
    boutonEditor->onPress([this]() {
        std::cout << "Level Editor button pressed!" << std::endl;
        game->setGameMode(Game::GameMode::Editor);
        });
    menuUI->add(boutonEditor);

    gui.add(menuUI);
}

void UI::showMenuUI()
{
    menuUI->setVisible(true);
	playUI->setVisible(false);
	editorUI->setVisible(false);
}

void UI::initPlayUI()
{
	playUI = tgui::Group::create();

	// Label Wave Info
    auto waveLabel = tgui::Label::create("Wave: 0");
	tgui::Vector2f waveLabelSize = waveLabel->getSize();
	waveLabel->setPosition(window->getWidth() / 2 - waveLabelSize.x, waveLabelSize.y);
	waveLabel->setTextSize(30);
	waveLabel->getRenderer()->setTextColor(tgui::Color::White);
	waveLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
	playUI->add(waveLabel);

	// Bouton Start Wave
	auto startWaveButton = tgui::Button::create("Start Wave");
	startWaveButton->setSize(150, 40);
	tgui::Vector2f startWaveButtonSize = startWaveButton->getSize();
	startWaveButton->setPosition(10, 10);
	startWaveButton->onPress([this]() {
		std::cout << "Start Wave button pressed!" << std::endl;
		wave->start();
		});
	playUI->add(startWaveButton);

	// Bouton Pause
	auto pauseButton = tgui::Button::create("Pause");
	pauseButton->setSize(100, 40);
	tgui::Vector2f pauseButtonSize = pauseButton->getSize();
	pauseButton->setPosition(window->getWidth() - pauseButtonSize.x - 10, 10);
	pauseButton->onPress([this, pauseButton]() {
		std::cout << "Pause button pressed!" << std::endl;
		if (isPaused) {
			isPaused = false;
			game->setGameMode(Game::GameMode::Play);
			pauseButton->setText("Pause");
		}
		else {
			isPaused = true;
			game->setGameMode(Game::GameMode::Pause);
			pauseButton->setText("Resume");
		}
		});
	playUI->add(pauseButton);

	gui.add(playUI);
}

void UI::showPlayUI()
{
    menuUI->setVisible(false);
	playUI->setVisible(true);
	editorUI->setVisible(false);
}

void UI::initEditorUI()
{
    editorUI = tgui::Group::create();
    // Label Editor Info
    auto editorLabel = tgui::Label::create("Editor Mode Active");
	tgui::Vector2f editorLabelSize = editorLabel->getSize();
	editorLabel->setPosition(window->getWidth() / 2 - editorLabelSize.x, editorLabelSize.y);
	editorLabel->setTextSize(30);
	editorLabel->getRenderer()->setTextColor(tgui::Color::White);
	editorLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
	editorUI->add(editorLabel);

	gui.add(editorUI);
}

void UI::showEditorUI()
{
    menuUI->setVisible(false);
	playUI->setVisible(false);
	editorUI->setVisible(true);
}


