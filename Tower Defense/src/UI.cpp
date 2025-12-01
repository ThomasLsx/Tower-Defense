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
        // Panel de sélection
        auto selectionPanel = playUI->get<tgui::Panel>("SelectionPanel");
        if (selectionPanel) {
            selectionPanel->setSize(winW * 0.125f, winH);
            selectionPanel->setPosition(winW * 0.875f, 0);
            float panelWidth = winW * 0.125f;
            float buttonWidth = 120.f * scaleX;
            float buttonHeight = 40.f * scaleY;
            float buttonSpacing = 15.f * scaleY;
            float xCenter = panelWidth / 2.f - buttonWidth / 2.f;
            float y = 40.f * scaleY;
            // Boutons d'action
            std::vector<std::string> btnNames = {"StartWaveButton", "PauseButton", "AutoStartButton", "QuitButton"};
            for (size_t idx = 0; idx < btnNames.size(); ++idx) {
                auto btn = selectionPanel->get<tgui::Button>(btnNames[idx]);
                if (btn) {
                    btn->setSize(buttonWidth, buttonHeight);
                    btn->setPosition(xCenter, y + idx * (buttonHeight + buttonSpacing));
                    btn->setTextSize(btn->getTextSize() * scaleX);
                }
            }
            // Boutons tourelles
            float turretSpacing = 20.f * scaleY;
            float turretStartY = y + btnNames.size() * (buttonHeight + buttonSpacing) + buttonSpacing;
            for (int i = 0; i < 4; ++i) {
                auto turretButton = selectionPanel->get<tgui::Button>("TurretButton" + std::to_string(i + 1));
                if (turretButton) {
                    turretButton->setSize(buttonWidth, buttonHeight);
                    turretButton->setPosition(xCenter, turretStartY + i * (buttonHeight + turretSpacing));
                }
            }
        }
        // Labels centrés dans la zone de jeu
        auto waveLabel = playUI->get<tgui::Label>("WaveLabel");
        if (waveLabel) {
            waveLabel->setTextSize(waveLabel->getTextSize() * scaleX);
            centerWidget(waveLabel, 20.f * scaleY);
        }
        auto copperLabel = playUI->get<tgui::Label>("CopperLabel");
        if (copperLabel) {
            copperLabel->setTextSize(copperLabel->getTextSize() * scaleX);
            centerWidget(copperLabel, 60.f * scaleY);
        }
        auto silverLabel = playUI->get<tgui::Label>("SilverLabel");
        if (silverLabel) {
            silverLabel->setTextSize(silverLabel->getTextSize() * scaleX);
            centerWidget(silverLabel, 90.f * scaleY);
        }
        auto goldLabel = playUI->get<tgui::Label>("GoldLabel");
        if (goldLabel) {
            goldLabel->setTextSize(goldLabel->getTextSize() * scaleX);
            centerWidget(goldLabel, 120.f * scaleY);
        }
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

	// Panel de sélection à droite
    float panelWidth = window->getWidth() * 0.125f;
    float panelHeight = window->getHeight();
    auto selectionPanel = tgui::Panel::create({ panelWidth, panelHeight });
    selectionPanel->setPosition(window->getWidth() * 0.875f, 0);
    selectionPanel->getRenderer()->setBackgroundColor(tgui::Color(50, 50, 50));
    selectionPanel->setWidgetName("SelectionPanel");

    // Boutons principaux du jeu (actions)
    float buttonWidth = 120.f;
    float buttonHeight = 40.f;
    float buttonSpacing = 15.f;
    float xCenter = panelWidth / 2.f - buttonWidth / 2.f;
    float y = 40.f;

    auto startWaveButton = tgui::Button::create("Start Wave");
    startWaveButton->setSize(buttonWidth, buttonHeight);
    startWaveButton->setPosition(xCenter, y);
    startWaveButton->setWidgetName("StartWaveButton");
    startWaveButton->onPress([this]() {
        std::cout << "Start Wave button pressed!" << std::endl;
        game->m_bRequestStartWave = true;
    });
    selectionPanel->add(startWaveButton);
    y += buttonHeight + buttonSpacing;

    auto pauseButton = tgui::Button::create("Pause");
    pauseButton->setSize(buttonWidth, buttonHeight);
    pauseButton->setPosition(xCenter, y);
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
    selectionPanel->add(pauseButton);
    y += buttonHeight + buttonSpacing;

    auto autoStartButton = tgui::Button::create("Auto Start Wave");
    autoStartButton->setSize(buttonWidth, buttonHeight);
    autoStartButton->setPosition(xCenter, y);
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
    selectionPanel->add(autoStartButton);
    y += buttonHeight + buttonSpacing;

    auto quitButton = tgui::Button::create("Quit to Menu");
    quitButton->setSize(buttonWidth, buttonHeight);
    quitButton->setPosition(xCenter, y);
    quitButton->setWidgetName("QuitButton");
    quitButton->onPress([this]() {
        game->setGameMode(Game::GameMode::Menu);
    });
    selectionPanel->add(quitButton);
    y += buttonHeight + buttonSpacing * 2; // Un peu plus d'espace avant les tourelles

    // Boutons tourelles
    const char* turretNames[4] = {"Basic", "Long", "Rapide", "Slow"};
    float turretSpacing = 20.f;
    turretButtons.clear();
    for (int i = 0; i < 4; ++i) {
        auto turretButton = tgui::Button::create(turretNames[i]);
        turretButton->setSize(buttonWidth, buttonHeight);
        turretButton->setPosition(xCenter, y + i * (buttonHeight + turretSpacing));
        turretButton->setWidgetName("TurretButton" + std::to_string(i + 1));
        turretButton->onPress([this, i, turretButton]() {
            if (selectedTurretButtonIndex == i) {
                selectedTurretButtonIndex = -1;
                game->setTowerIndex(-1);
                // Remettre tous les boutons en blanc
                for (int j = 0; j < turretButtons.size(); ++j) {
                    if (turretButtons[j])
                        turretButtons[j]->getRenderer()->setBackgroundColor(tgui::Color(255, 255, 255));
                }
            } else {
                selectedTurretButtonIndex = i;
                game->setTowerIndex(i);
                for (int j = 0; j < turretButtons.size(); ++j) {
                    if (turretButtons[j]) {
                        if (j == i)
                            turretButtons[j]->getRenderer()->setBackgroundColor(tgui::Color::Green);
                        else
                            turretButtons[j]->getRenderer()->setBackgroundColor(tgui::Color(255, 255, 255));
                    }
                }
            }
        });
        selectionPanel->add(turretButton);
        turretButtons.push_back(turretButton);
    }

    // Panel d'infos tourelle
    float infoPanelWidth = panelWidth * 0.9f;
    float infoPanelHeight = panelHeight / 2.f;
    float infoPanelX = (panelWidth - infoPanelWidth) / 2.f;
    float infoPanelY = panelHeight / 2.f;
    auto turretInfoPanel = tgui::Panel::create({infoPanelWidth, infoPanelHeight});
    turretInfoPanel->setPosition(infoPanelX, infoPanelY);
    turretInfoPanel->setWidgetName("TurretInfoPanel");
    turretInfoPanel->getRenderer()->setBackgroundColor(tgui::Color(70, 70, 70));

    // Labels d'infos centrés et en blanc
    auto nameLabel = tgui::Label::create("Nom : -");
    nameLabel->setWidgetName("TurretNameLabel");
    nameLabel->setTextSize(18);
    nameLabel->getRenderer()->setTextColor(tgui::Color::White);
    nameLabel->setPosition(infoPanelWidth / 2.f - 60, 10);
    turretInfoPanel->add(nameLabel);

    auto rangeLabel = tgui::Label::create("Portée : -");
    rangeLabel->setWidgetName("TurretRangeLabel");
    rangeLabel->setTextSize(18);
    rangeLabel->getRenderer()->setTextColor(tgui::Color::White);
    rangeLabel->setPosition(infoPanelWidth / 2.f - 60, 40);
    turretInfoPanel->add(rangeLabel);

    auto speedLabel = tgui::Label::create("Vitesse : -");
    speedLabel->setWidgetName("TurretSpeedLabel");
    speedLabel->setTextSize(18);
    speedLabel->getRenderer()->setTextColor(tgui::Color::White);
    speedLabel->setPosition(infoPanelWidth / 2.f - 60, 70);
    turretInfoPanel->add(speedLabel);

    auto damageLabel = tgui::Label::create("Dégâts : -");
    damageLabel->setWidgetName("TurretDamageLabel");
    damageLabel->setTextSize(18);
    damageLabel->getRenderer()->setTextColor(tgui::Color::White);
    damageLabel->setPosition(infoPanelWidth / 2.f - 60, 100);
    turretInfoPanel->add(damageLabel);

    // Bouton Upgrade
    auto upgradeButton = tgui::Button::create("Upgrade");
    upgradeButton->setWidgetName("UpgradeButton");
    upgradeButton->setSize(90, 32);
    upgradeButton->setPosition(infoPanelWidth / 2.f - 100, 140);
    upgradeButton->onPress([this]() {
        if (game->selectedTower) {
            game->selectedTower->upgrade();
            // Mise à jour immédiate de l'UI
            updatePlayUI();
        }
    });
    turretInfoPanel->add(upgradeButton);

    // Bouton Delete
    auto deleteButton = tgui::Button::create("Delete");
    deleteButton->setWidgetName("DeleteButton");
    deleteButton->setSize(90, 32);
    deleteButton->setPosition(infoPanelWidth / 2.f + 10, 140);
    deleteButton->onPress([this]() {
        if (game->selectedTower) {
            // Suppression de la tour via TowerManager
            game->getTowerManager().removeTowerAt(
                static_cast<unsigned int>(game->selectedTower->getPosition().x / (game->getMap()->getTileSize().x * game->getMap()->getScale())),
                static_cast<unsigned int>(game->selectedTower->getPosition().y / (game->getMap()->getTileSize().y * game->getMap()->getScale())),
                game->getMap()->getTileSize(), game->getMap()->getScale()
            );
            game->selectedTower = nullptr;
            updatePlayUI();
        }
    });
    turretInfoPanel->add(deleteButton);

    selectionPanel->add(turretInfoPanel);

    // Ajout du panel à la Play UI
    playUI->add(selectionPanel);

    // Ajout des labels d'information au centre
    auto waveLabel = tgui::Label::create("Wave: 0");
    waveLabel->setWidgetName("WaveLabel");
    waveLabel->setTextSize(30);
    waveLabel->getRenderer()->setTextColor(tgui::Color::White);
    waveLabel->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
    centerWidget(waveLabel, 20.f);
    playUI->add(waveLabel);

    auto copperLabel = tgui::Label::create("Copper: 0");
    copperLabel->setWidgetName("CopperLabel");
    copperLabel->setTextSize(20);
    copperLabel->getRenderer()->setTextColor(tgui::Color::White);
    centerWidget(copperLabel, 60.f);
    playUI->add(copperLabel);

    auto silverLabel = tgui::Label::create("Silver: 0");
    silverLabel->setWidgetName("SilverLabel");
    silverLabel->setTextSize(20);
    silverLabel->getRenderer()->setTextColor(tgui::Color::White);
    centerWidget(silverLabel, 90.f);
    playUI->add(silverLabel);

    auto goldLabel = tgui::Label::create("Gold: 0");
    goldLabel->setWidgetName("GoldLabel");
    goldLabel->setTextSize(20);
    goldLabel->getRenderer()->setTextColor(tgui::Color::White);
    centerWidget(goldLabel, 120.f);
    playUI->add(goldLabel);

    gui.add(playUI);
}


void UI::updatePlayUI() {
    if (playUI) {
        auto waveLabel = playUI->get<tgui::Label>("WaveLabel");
        if (waveLabel) {
            waveLabel->setText("Wave: " + std::to_string(game->getCurrentWaveId()));
        }
        auto copperLabel = playUI->get<tgui::Label>("CopperLabel");
        if (copperLabel) {
            copperLabel->setText("Copper: " + std::to_string(game->getCopper()));
        }
        auto silverLabel = playUI->get<tgui::Label>("SilverLabel");
        if (silverLabel) {
            silverLabel->setText("Silver: " + std::to_string(game->getSilver()));
        }
        auto goldLabel = playUI->get<tgui::Label>("GoldLabel");
        if (goldLabel) {
            goldLabel->setText("Gold: " + std::to_string(game->getGold()));
        }
        // Mise à jour des infos tourelle sélectionnée
        auto selectionPanel = playUI->get<tgui::Panel>("SelectionPanel");
        if (selectionPanel) {
            // Panel d'infos tourelle responsive
            float panelWidth = selectionPanel->getSize().x;
            float scaleY = window->getHeight() / prevWindowHeight;
            auto infoPanel = selectionPanel->get<tgui::Panel>("TurretInfoPanel");
            if (infoPanel) {
                float infoPanelWidth = panelWidth * 0.9f;
                float infoPanelHeight = window->getHeight() / 2.f;
                float infoPanelX = (panelWidth - infoPanelWidth) / 2.f;
                float infoPanelY = window->getHeight() / 2.f;
                infoPanel->setSize(infoPanelWidth, infoPanelHeight);
                infoPanel->setPosition(infoPanelX, infoPanelY);
                // Labels responsive
                std::vector<std::string> labelNames = {"TurretNameLabel", "TurretRangeLabel", "TurretSpeedLabel", "TurretDamageLabel"};
                int labelCount = static_cast<int>(labelNames.size());
                float labelSpacing = infoPanelHeight / (labelCount + 1);
                std::vector<tgui::Label::Ptr> labels;
                for (int i = 0; i < labelCount; ++i) {
                    auto label = infoPanel->get<tgui::Label>(labelNames[i]);
                    if (label) {
                        float labelWidth = label->getSize().x;
                        float labelX = infoPanelWidth / 2.f - labelWidth / 2.f;
                        float labelY = (i + 1) * labelSpacing - label->getSize().y / 2.f;
                        label->setPosition(labelX, labelY);
                        label->setTextSize(18 * scaleY);
                        label->getRenderer()->setTextColor(tgui::Color::White);
                        labels.push_back(label);
                    }
                }
                // Mise à jour du texte des labels après le responsive
                if (labels.size() == 4) {
                    if (game->selectedTower) {
                        // Affichage des caractéristiques de la tour sélectionnée sur la map
                        labels[0]->setText("Nom : Tour " + std::to_string(game->selectedTower->getId()));
                        labels[1]->setText("Portée : " + std::to_string(static_cast<int>(game->selectedTower->getRange())));
                        labels[2]->setText("Vitesse : " + std::to_string(game->selectedTower->getFireRate()));
                        labels[3]->setText("Dégâts : " + std::to_string(game->selectedTower->getDamage()));
                        // Afficher les boutons Upgrade et Delete
                        auto upgradeButton = infoPanel->get<tgui::Button>("UpgradeButton");
                        auto deleteButton = infoPanel->get<tgui::Button>("DeleteButton");
                        if (upgradeButton) upgradeButton->setVisible(true);
                        if (deleteButton) deleteButton->setVisible(true);
                    } else {
                        // Masquer les boutons si aucune tour sélectionnée
                        auto upgradeButton = infoPanel->get<tgui::Button>("UpgradeButton");
                        auto deleteButton = infoPanel->get<tgui::Button>("DeleteButton");
                        if (upgradeButton) upgradeButton->setVisible(false);
                        if (deleteButton) deleteButton->setVisible(false);
                        // Modèle de tour (avant placement)
                        if (selectedTurretButtonIndex != -1) {
                            if (selectedTurretButtonIndex == 0) { labels[0]->setText("Nom : Basic"); labels[1]->setText("Portée : 100"); labels[2]->setText("Vitesse : 1.0"); labels[3]->setText("Dégâts : 10"); }
                            else if (selectedTurretButtonIndex == 1) { labels[0]->setText("Nom : Long"); labels[1]->setText("Portée : 180"); labels[2]->setText("Vitesse : 0.7"); labels[3]->setText("Dégâts : 15"); }
                            else if (selectedTurretButtonIndex == 2) { labels[0]->setText("Nom : Rapide"); labels[1]->setText("Portée : 80"); labels[2]->setText("Vitesse : 2.0"); labels[3]->setText("Dégâts : 7"); }
                            else if (selectedTurretButtonIndex == 3) { labels[0]->setText("Nom : Slow"); labels[1]->setText("Portée : 120"); labels[2]->setText("Vitesse : 0.5"); labels[3]->setText("Dégâts : 20"); }
                        } else {
                            labels[0]->setText("Nom : -"); labels[1]->setText("Portée : -"); labels[2]->setText("Vitesse : -"); labels[3]->setText("Dégâts : -");
                        }
                    }
                }
            }
        }
    }
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
        float gameZoneWidth = window->getWidth() * 0.875f;
        widget->setPosition(gameZoneWidth / 2 - size.x / 2, y);
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

int UI::getSelectedTurretButtonIndex() const {
    return selectedTurretButtonIndex;
}

void UI::deselectTurretButtons() {
    selectedTurretButtonIndex = -1;
    for (auto& btn : turretButtons) {
        if (btn)
            btn->getRenderer()->setBackgroundColor(tgui::Color(255, 255, 255));
    }
}
