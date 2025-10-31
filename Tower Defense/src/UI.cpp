// UI.cpp
#include "UI.h"
#include <iostream>

UI::UI(Window* window, Game* game, WaveManager* waveManager) : window(window), game(game), waveManager(waveManager)
{
    gui.setTarget(window->getRenderWindow());

	initMenuUI();
	initPlayUI();
	initEditorUI();
}

void UI::handleEvent(const sf::Event& event)
{
    gui.handleEvent(event);
    // Gestion du redimensionnement de la fenêtre
    if (event.is<sf::Event::Resized>()) {
        const auto* resized = event.getIf<sf::Event::Resized>();
        if (resized) {
            updateUILayout();
        }
    }
}

void UI::updateUILayout()
{
    // Menu UI
    if (menuUI) {
        auto picture = menuUI->get<tgui::Picture>("MenuBackground");
        if (picture)
            picture->setSize(window->getWidth(), window->getHeight());
        auto boutonPlay = menuUI->get<tgui::Button>("PlayButton");
        if (boutonPlay) {
            tgui::Vector2f size = boutonPlay->getSize();
            boutonPlay->setPosition(window->getWidth() / 2 - size.x / 2, window->getHeight() / 2 - size.y);
        }
        auto boutonEditor = menuUI->get<tgui::Button>("EditorButton");
        if (boutonEditor) {
            tgui::Vector2f size = boutonEditor->getSize();
            boutonEditor->setPosition(window->getWidth() / 2 - size.x / 2, window->getHeight() / 2 + size.y);
        }
    }
    // Play UI
    if (playUI) {
        auto waveLabel = playUI->get<tgui::Label>("WaveLabel");
        if (waveLabel) {
            tgui::Vector2f size = waveLabel->getSize();
            waveLabel->setPosition(window->getWidth() / 2 - size.x, size.y);
        }
        auto pauseButton = playUI->get<tgui::Button>("PauseButton");
        if (pauseButton) {
            tgui::Vector2f size = pauseButton->getSize();
            pauseButton->setPosition(window->getWidth() - size.x - 10, 10);
        }
    }
    // Editor UI
    if (editorUI) {
        auto editorLabel = editorUI->get<tgui::Label>("EditorLabel");
        if (editorLabel) {
            tgui::Vector2f size = editorLabel->getSize();
            editorLabel->setPosition(window->getWidth() / 2 - size.x, size.y);
        }
    }
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
    picture->setWidgetName("MenuBackground");
    menuUI->add(picture);

    // Bouton Play
    auto boutonPlay = tgui::Button::create("Play");
    boutonPlay->setSize(200, 50);
    boutonPlay->setWidgetName("PlayButton");
    tgui::Vector2f boutonPlaySize = boutonPlay->getSize();
    boutonPlay->setPosition(window->getWidth() / 2 - boutonPlaySize.x / 2, window->getHeight() / 2 - boutonPlaySize.y);
    boutonPlay->onPress([this]() {
        game->setGameMode(Game::GameMode::Play);
        });
    menuUI->add(boutonPlay);

    // Bouton Level Editor
    auto boutonEditor = tgui::Button::create("Level Editor");
    boutonEditor->setSize(200, 50);
    boutonEditor->setWidgetName("EditorButton");
    tgui::Vector2f boutonEditorSize = boutonEditor->getSize();
    boutonEditor->setPosition(window->getWidth() / 2 - boutonEditorSize.x / 2, window->getHeight() / 2 + boutonEditorSize.y);
    boutonEditor->onPress([this]() {
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
    waveLabel->setWidgetName("WaveLabel");
    tgui::Vector2f waveLabelSize = waveLabel->getSize();
	waveLabel->setPosition(window->getWidth() / 2 - waveLabelSize.x, waveLabelSize.y);
	waveLabel->setTextSize(30);
	waveLabel->getRenderer()->setTextColor(tgui::Color::White);
	waveLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
	playUI->add(waveLabel);

	// Bouton Start Wave
	auto startWaveButton = tgui::Button::create("Start Wave");
	startWaveButton->setSize(150, 40);
	startWaveButton->setWidgetName("StartWaveButton");
	tgui::Vector2f startWaveButtonSize = startWaveButton->getSize();
	startWaveButton->setPosition(10, 10);
	startWaveButton->onPress([this, waveLabel]() {
		std::cout << "Start Wave button pressed!" << std::endl;
		game->m_bRequestStartWave = true;
		waveLabel->setText("Wave: " + std::to_string(game->getCurrentWaveId()));
		});
	playUI->add(startWaveButton);

	// Bouton Pause
	auto pauseButton = tgui::Button::create("Pause");
	pauseButton->setSize(100, 40);
	pauseButton->setWidgetName("PauseButton");
	tgui::Vector2f pauseButtonSize = pauseButton->getSize();
	pauseButton->setPosition(window->getWidth() - pauseButtonSize.x - 10, 10);
	pauseButton->onPress([this, pauseButton]() {
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
    editorLabel->setWidgetName("EditorLabel");
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


