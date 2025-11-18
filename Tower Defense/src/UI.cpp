// UI.cpp
#include "UI.h"
#include <iostream>

UI::UI(Window* window, Game* game) : window(window), game(game)
{
    gui.setTarget(window->getRenderWindow());

	initMenuUI();
	initPlayUI();
	initEditorUI();

	prevWindowWidth = window->getWidth();
	prevWindowHeight = window->getHeight();
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
	int winW = window->getWidth();
	int winH = window->getHeight();
    float scaleX = winW / prevWindowWidth;
    float scaleY = winH / prevWindowHeight;

    // Menu UI
    if (menuUI) {
        auto picture = menuUI->get<tgui::Picture>("MenuBackground");
        if (picture)
            picture->setSize(winW, winH);
        auto boutonPlay = menuUI->get<tgui::Button>("PlayButton");
        auto boutonEditor = menuUI->get<tgui::Button>("EditorButton");
        if (boutonPlay && boutonEditor) {
            boutonPlay->setSize(boutonPlay->getSize().x * scaleX, boutonPlay->getSize().y * scaleY);
            boutonPlay->setTextSize(boutonPlay->getTextSize() * scaleX);
            boutonEditor->setSize(boutonEditor->getSize().x * scaleX, boutonEditor->getSize().y * scaleY);
            boutonEditor->setTextSize(boutonEditor->getTextSize() * scaleX);
            float spacing = boutonPlay->getSize().y * scaleY * 0.6f;
            float startY = winH / 2 - (boutonPlay->getSize().y * scaleY * 2 + spacing) / 2;
            centerWidget(boutonPlay, startY);
            centerWidget(boutonEditor, startY + boutonPlay->getSize().y * scaleY + spacing);
        }
    }
    // Play UI
    if (playUI) {
        auto waveLabel = playUI->get<tgui::Label>("WaveLabel");
        if (waveLabel) {
            waveLabel->setTextSize(waveLabel->getTextSize()* scaleX);
        }
        auto startWaveButton = playUI->get<tgui::Button>("StartWaveButton");
        auto pauseButton = playUI->get<tgui::Button>("PauseButton");
        auto autoStartButton = playUI->get<tgui::Button>("AutoStartButton");
        auto quitButton = playUI->get<tgui::Button>("QuitButton");
        std::vector<tgui::Button::Ptr> buttons = {startWaveButton, pauseButton, autoStartButton, quitButton};
        for (auto& btn : buttons) {
            if (btn) {
                btn->setSize(btn->getSize().x * scaleX, btn->getSize().y * scaleY);
                btn->setTextSize(btn->getTextSize() * scaleX);
            }
        }
        float buttonMargin = 10.f;
        float spacing = startWaveButton->getSize().y * scaleY * 0.3f;
        float startY = 20.f;
        centerWidget(waveLabel, startY);
        placeButtonsStacked(buttons, startY /*+ (waveLabel ? waveLabel->getSize().y : 0) + spacing*/, buttonMargin, spacing, ButtonAlign::Left);
    }
    // Editor UI
    if (editorUI) {
        auto editorLabel = editorUI->get<tgui::Label>("EditorLabel");
        if (editorLabel) {
            editorLabel->setTextSize(editorLabel->getTextSize() * scaleX + 6);
            centerWidget(editorLabel, editorLabel->getSize().y);
        }
    }
    
    prevWindowWidth = window->getWidth();
    prevWindowHeight = window->getHeight();
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
    waveLabel->setTextSize(30);
    waveLabel->getRenderer()->setTextColor(tgui::Color::White);
    waveLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    centerWidget(waveLabel, 20.f);
    playUI->add(waveLabel);

    float buttonMargin = 10.f;
    float spacing = 10.f;
    float startY = 20.f;

    // Bouton Start Wave
    auto startWaveButton = tgui::Button::create("Start Wave");
    startWaveButton->setSize(150, 40);
    startWaveButton->setWidgetName("StartWaveButton");
    startWaveButton->onPress([this]() {
        std::cout << "Start Wave button pressed!" << std::endl;
        game->m_bRequestStartWave = true;
    });
    playUI->add(startWaveButton);

	// Bouton Pause/Resume
    auto pauseButton = tgui::Button::create("Pause");
    pauseButton->setSize(100, 40);
    pauseButton->setWidgetName("PauseButton");
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

	// Bouton Auto Start Wave
	auto autoStartButton = tgui::Button::create("Auto Start Wave");
	autoStartButton->setSize(150, 40);
	autoStartButton->setWidgetName("AutoStartButton");
	autoStartButton->onPress([this, autoStartButton]() {
		game->m_bAutoStartWaves = !game->m_bAutoStartWaves;
		if (game->m_bAutoStartWaves) {
			autoStartButton->setText("Auto Start: ON");
		}
		else {
			autoStartButton->setText("Auto Start: OFF");
		}
		});
	playUI->add(autoStartButton);

    // Bouton Quitter vers le Menu
    auto quitButton = tgui::Button::create("Quit to Menu");
    quitButton->setSize(150, 40);
    quitButton->setWidgetName("QuitButton");
    quitButton->onPress([this]() {
        game->setGameMode(Game::GameMode::Menu);
    });
    playUI->add(quitButton);

    // Placement des boutons à gauche, empilés
    placeButtonsStacked({startWaveButton, pauseButton, autoStartButton, quitButton}, startY, buttonMargin, spacing, ButtonAlign::Left);

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

void UI::centerWidget(tgui::Widget::Ptr widget, float y) {
    if (widget) {
        tgui::Vector2f size = widget->getSize();
        widget->setPosition(window->getWidth() / 2 - size.x / 2, y);
    }
}

void UI::placeButtonsStacked(const std::vector<tgui::Button::Ptr>& buttons, float startY, float margin, float spacing, ButtonAlign align) {
    float y = startY;
    for (const auto& button : buttons) {
        if (button) {
            tgui::Vector2f size = button->getSize();
            float x = margin;
            if (align == ButtonAlign::Center) {
                x = window->getWidth() / 2 - size.x / 2;
            } else if (align == ButtonAlign::Right) {
                x = window->getWidth() - size.x - margin;
            }
            button->setPosition(x, y);
            y += size.y + spacing;
        }
    }
}

void UI::updatePlayUI() {
    if (playUI) {
        auto waveLabel = playUI->get<tgui::Label>("WaveLabel");
        if (waveLabel) {
            waveLabel->setText("Wave: " + std::to_string(game->getCurrentWaveId()));
        }
    }
}


