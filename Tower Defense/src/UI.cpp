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

    // Menu UI
    if (menuUI) {
        auto picture = menuUI->get<tgui::Picture>("MenuBackground");
        if (picture)
            picture->setSize(winW, winH);
        auto boutonPlay = menuUI->get<tgui::Button>("PlayButton");
        auto boutonEditor = menuUI->get<tgui::Button>("EditorButton");
        if (boutonPlay && boutonEditor) {
            boutonPlay->setSize(200, 50);
            boutonPlay->setTextSize(24);
            boutonPlay->setPosition(winW / 2 - 100, winH / 2 - 50);
            boutonEditor->setSize(200, 50);
            boutonEditor->setTextSize(24);
            boutonEditor->setPosition(winW / 2 - 100, winH / 2 + 50);
        }
    }
    // Play UI
    if (playUI) {
        auto selectionPanel = playUI->get<tgui::Panel>("SelectionPanel");
        if (selectionPanel) {
            float panelWidth = winW * 0.125f;
            float panelHeight = winH;
            selectionPanel->setSize(panelWidth, panelHeight);
            selectionPanel->setPosition(winW * 0.875f, 0);
            float buttonWidth = 120.f;
            float buttonHeight = 40.f;
            float buttonSpacing = 15.f;
            float xCenter = panelWidth / 2.f - buttonWidth / 2.f;
            float y = 40.f;
            // Boutons d'action
            std::vector<std::string> btnNames = {"StartWaveButton", "PauseButton", "AutoStartButton", "QuitButton"};
            for (size_t idx = 0; idx < btnNames.size(); ++idx) {
                auto btn = selectionPanel->get<tgui::Button>(btnNames[idx]);
                if (btn) {
                    btn->setSize(buttonWidth, buttonHeight);
                    btn->setPosition(xCenter, y + idx * (buttonHeight + buttonSpacing));
                    btn->setTextSize(18);
                }
            }
            // Boutons tourelles
            float turretSpacing = 20.f;
            float turretStartY = y + btnNames.size() * (buttonHeight + buttonSpacing) + buttonSpacing;
            for (int i = 0; i < 4; ++i) {
                auto turretButton = selectionPanel->get<tgui::Button>("TurretButton" + std::to_string(i + 1));
                if (turretButton) {
                    turretButton->setSize(buttonWidth, buttonHeight);
                    turretButton->setPosition(xCenter, turretStartY + i * (buttonHeight + turretSpacing));
                }
            }
            // Panel d'infos tourelle
            float infoPanelWidth = panelWidth * 0.9f;
            float infoPanelHeight = panelHeight / 2.f;
            float infoPanelX = (panelWidth - infoPanelWidth) / 2.f;
            float infoPanelY = panelHeight / 2.f;
            // TurretInfoPanel
            auto infoPanel = selectionPanel->get<tgui::Panel>("TurretInfoPanel");
            if (infoPanel) {
                infoPanel->setSize(infoPanelWidth, infoPanelHeight);
                infoPanel->setPosition(infoPanelX, infoPanelY);
                std::vector<std::string> labelNames = {"TurretNameLabel", "TurretRangeLabel", "TurretSpeedLabel", "TurretDamageLabel", "TurretUpgradePriceLabel", "TurretDeleteRefundLabel"};
                int labelCount = static_cast<int>(labelNames.size());
                float labelSpacing = 30.f;
                float startY = 10.f;
                for (int i = 0; i < labelCount; ++i) {
                    auto label = infoPanel->get<tgui::Label>(labelNames[i]);
                    if (label) {
                        float labelWidth = label->getSize().x;
                        float labelX = infoPanelWidth / 2.f - labelWidth / 2.f;
                        float labelY = startY + i * labelSpacing;
                        label->setPosition(labelX, labelY);
                    }
                }
                // Boutons Upgrade/Delete centrés
                auto upgradeButton = infoPanel->get<tgui::Button>("UpgradeButton");
                auto deleteButton = infoPanel->get<tgui::Button>("DeleteButton");
                if (upgradeButton && deleteButton) {
                    float totalWidth = upgradeButton->getSize().x + 10.f + deleteButton->getSize().x;
                    float startX = infoPanelWidth / 2.f - totalWidth / 2.f;
                    float buttonY = startY + labelCount * labelSpacing;
                    upgradeButton->setPosition(startX, buttonY);
                    deleteButton->setPosition(startX + upgradeButton->getSize().x + 10.f, buttonY);
                }
            }
            // TurretPreviewPanel
            auto previewPanel = selectionPanel->get<tgui::Panel>("TurretPreviewPanel");
            if (previewPanel) {
                previewPanel->setSize(infoPanelWidth, infoPanelHeight);
                previewPanel->setPosition(infoPanelX, infoPanelY);
                std::vector<std::string> previewLabelNames = {
                    "PreviewNameLabel", "PreviewRangeLabel", "PreviewSpeedLabel", "PreviewDamageLabel",
                    "PreviewCopperPriceLabel", "PreviewSilverPriceLabel", "PreviewGoldPriceLabel"
                };
                int previewLabelCount = static_cast<int>(previewLabelNames.size());
                float labelSpacing = 30.f;
                float startY = 20.f;
                for (int i = 0; i < previewLabelCount; ++i) {
                    auto label = previewPanel->get<tgui::Label>(previewLabelNames[i]);
                    if (label) {
                        float labelWidth = label->getSize().x;
                        float labelX = infoPanelWidth / 2.f - labelWidth / 2.f;
                        float labelY = startY + i * labelSpacing;
                        label->setPosition(labelX, labelY);
                    }
                }
            }
        }
        // Labels centrés dans la zone de jeu
        auto waveLabel = playUI->get<tgui::Label>("WaveLabel");
        if (waveLabel) {
            waveLabel->setTextSize(30);
            centerWidget(waveLabel, 20.f);
        }
        auto copperLabel = playUI->get<tgui::Label>("CopperLabel");
        if (copperLabel) {
            copperLabel->setTextSize(20);
            centerWidget(copperLabel, 60.f);
        }
        auto silverLabel = playUI->get<tgui::Label>("SilverLabel");
        if (silverLabel) {
            silverLabel->setTextSize(20);
            centerWidget(silverLabel, 90.f);
        }
        auto goldLabel = playUI->get<tgui::Label>("GoldLabel");
        if (goldLabel) {
            goldLabel->setTextSize(20);
            centerWidget(goldLabel, 120.f);
        }
    }
    // Editor UI
    if (editorUI) {
        auto editorLabel = editorUI->get<tgui::Label>("EditorLabel");
        if (editorLabel) {
            editorLabel->setTextSize(30);
            tgui::Vector2f editorLabelSize = editorLabel->getSize();
            editorLabel->setPosition(winW / 2 - editorLabelSize.x, editorLabelSize.y);
        }
    }
    prevWindowWidth = winW;
    prevWindowHeight = winH;
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
    rangeLabel->setPosition(infoPanelWidth / 2.f - 60, 35);
    turretInfoPanel->add(rangeLabel);

    auto speedLabel = tgui::Label::create("Vitesse : -");
    speedLabel->setWidgetName("TurretSpeedLabel");
    speedLabel->setTextSize(18);
    speedLabel->getRenderer()->setTextColor(tgui::Color::White);
    speedLabel->setPosition(infoPanelWidth / 2.f - 60, 60);
    turretInfoPanel->add(speedLabel);

    auto damageLabel = tgui::Label::create("Dégâts : -");
    damageLabel->setWidgetName("TurretDamageLabel");
    damageLabel->setTextSize(18);
    damageLabel->getRenderer()->setTextColor(tgui::Color::White);
    damageLabel->setPosition(infoPanelWidth / 2.f - 60, 85);
    turretInfoPanel->add(damageLabel);

    // Upgrade labels
    auto upgradeCopperLabel = tgui::Label::create("Up Cuivre : -");
    upgradeCopperLabel->setWidgetName("TurretUpgradeCopperLabel");
    upgradeCopperLabel->setTextSize(18);
    upgradeCopperLabel->getRenderer()->setTextColor(tgui::Color::White);
    upgradeCopperLabel->setPosition(infoPanelWidth / 2.f - 60, 110);
    turretInfoPanel->add(upgradeCopperLabel);

    auto upgradeSilverLabel = tgui::Label::create("Up Argent : -");
    upgradeSilverLabel->setWidgetName("TurretUpgradeSilverLabel");
    upgradeSilverLabel->setTextSize(18);
    upgradeSilverLabel->getRenderer()->setTextColor(tgui::Color::White);
    upgradeSilverLabel->setPosition(infoPanelWidth / 2.f - 60, 135);
    turretInfoPanel->add(upgradeSilverLabel);

    auto upgradeGoldLabel = tgui::Label::create("Up Or : -");
    upgradeGoldLabel->setWidgetName("TurretUpgradeGoldLabel");
    upgradeGoldLabel->setTextSize(18);
    upgradeGoldLabel->getRenderer()->setTextColor(tgui::Color::White);
    upgradeGoldLabel->setPosition(infoPanelWidth / 2.f - 60, 160);
    turretInfoPanel->add(upgradeGoldLabel);

    // Delete labels
    auto deleteCopperLabel = tgui::Label::create("Remb Cuivre : -");
    deleteCopperLabel->setWidgetName("TurretDeleteCopperLabel");
    deleteCopperLabel->setTextSize(18);
    deleteCopperLabel->getRenderer()->setTextColor(tgui::Color::White);
    deleteCopperLabel->setPosition(infoPanelWidth / 2.f - 60, 185);
    turretInfoPanel->add(deleteCopperLabel);

    auto deleteSilverLabel = tgui::Label::create("Remb Argent : -");
    deleteSilverLabel->setWidgetName("TurretDeleteSilverLabel");
    deleteSilverLabel->setTextSize(18);
    deleteSilverLabel->getRenderer()->setTextColor(tgui::Color::White);
    deleteSilverLabel->setPosition(infoPanelWidth / 2.f - 60, 210);
    turretInfoPanel->add(deleteSilverLabel);

    auto deleteGoldLabel = tgui::Label::create("Remb Or : -");
    deleteGoldLabel->setWidgetName("TurretDeleteGoldLabel");
    deleteGoldLabel->setTextSize(18);
    deleteGoldLabel->getRenderer()->setTextColor(tgui::Color::White);
    deleteGoldLabel->setPosition(infoPanelWidth / 2.f - 60, 235);
    turretInfoPanel->add(deleteGoldLabel);

    // Boutons Upgrade/Delete centrés
    auto upgradeButton = tgui::Button::create("Upgrade");
    upgradeButton->setWidgetName("UpgradeButton");
    upgradeButton->setSize(90, 32);
    auto deleteButton = tgui::Button::create("Delete");
    deleteButton->setWidgetName("DeleteButton");
    deleteButton->setSize(90, 32);
    // Positionnement juste après le dernier label
    float buttonsY = 260; // 235 + 25 (marge)
    float totalWidth = upgradeButton->getSize().x + 10.f + deleteButton->getSize().x;
    float startX = infoPanelWidth / 2.f - totalWidth / 2.f;
    upgradeButton->setPosition(startX, buttonsY);
    deleteButton->setPosition(startX + upgradeButton->getSize().x + 10.f, buttonsY);
    upgradeButton->onPress([this]() {
        if (game->selectedTower) {
            game->selectedTower->upgrade();
            updatePlayUI();
        }
    });
    deleteButton->onPress([this]() {
        if (game->selectedTower) {
            game->getTowerManager().removeTowerAt(
                static_cast<unsigned int>(game->selectedTower->getPosition().x / (game->getMap()->getTileSize().x * game->getMap()->getScale())),
                static_cast<unsigned int>(game->selectedTower->getPosition().y / (game->getMap()->getTileSize().y * game->getMap()->getScale())),
                game->getMap()->getTileSize(), game->getMap()->getScale(),
				*game->getEconomySystem()
            );
            game->selectedTower = nullptr;
            updatePlayUI();
        }
    });
    turretInfoPanel->add(upgradeButton);
    turretInfoPanel->add(deleteButton);

    selectionPanel->add(turretInfoPanel);

    // Panel d'aperçu tourelle (preview)
    auto turretPreviewPanel = tgui::Panel::create({infoPanelWidth, infoPanelHeight});
    turretPreviewPanel->setPosition(infoPanelX, infoPanelY);
    turretPreviewPanel->setWidgetName("TurretPreviewPanel");
    turretPreviewPanel->getRenderer()->setBackgroundColor(tgui::Color(90, 90, 90));
    // Labels preview alignés verticalement
    std::vector<std::pair<std::string, std::string>> previewLabels = {
        {"PreviewNameLabel", "Nom : -"},
        {"PreviewRangeLabel", "Portée : -"},
        {"PreviewSpeedLabel", "Vitesse : -"},
        {"PreviewDamageLabel", "Dégâts : -"},
        {"PreviewCopperPriceLabel", "Prix Cuivre : -"},
        {"PreviewSilverPriceLabel", "Prix Argent : -"},
        {"PreviewGoldPriceLabel", "Prix Or : -"}
    };
    float previewLabelSpacing = 30.f;
    float previewStartY = 20.f;
    for (int i = 0; i < previewLabels.size(); ++i) {
        auto label = tgui::Label::create(previewLabels[i].second);
        label->setWidgetName(previewLabels[i].first);
        label->setTextSize(18);
        label->getRenderer()->setTextColor(tgui::Color::White);
        float labelWidth = label->getSize().x;
        float labelX = infoPanelWidth / 2.f - labelWidth / 2.f;
        float labelY = previewStartY + i * previewLabelSpacing;
        label->setPosition(labelX, labelY);
        turretPreviewPanel->add(label);
    }
    selectionPanel->add(turretPreviewPanel);

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
            auto infoPanel = selectionPanel->get<tgui::Panel>("TurretInfoPanel");
            auto previewPanel = selectionPanel->get<tgui::Panel>("TurretPreviewPanel");
            if (game->selectedTower) {
                // Afficher le panel info, masquer le preview
                if (infoPanel) infoPanel->setVisible(true);
                if (previewPanel) previewPanel->setVisible(false);
                // Labels d'info tourelle
                std::vector<std::string> labelNames = {
                    "TurretNameLabel", "TurretRangeLabel", "TurretSpeedLabel", "TurretDamageLabel"
                };
                if (infoPanel) {
                    for (int i = 0; i < labelNames.size(); ++i) {
                        auto label = infoPanel->get<tgui::Label>(labelNames[i]);
                        if (label) {
                            if (i == 0) label->setText("Nom : Tour " + std::to_string(game->selectedTower->getId()));
                            if (i == 1) label->setText("Portée : " + std::to_string(static_cast<int>(game->selectedTower->getRange())));
                            if (i == 2) label->setText("Vitesse : " + std::to_string(game->selectedTower->getFireRate()));
                            if (i == 3) label->setText("Dégâts : " + std::to_string(game->selectedTower->getDamage()));
                        }
                    }
                    // Upgrade labels
                    auto upgradeCopperLabel = infoPanel->get<tgui::Label>("TurretUpgradeCopperLabel");
                    auto upgradeSilverLabel = infoPanel->get<tgui::Label>("TurretUpgradeSilverLabel");
                    auto upgradeGoldLabel = infoPanel->get<tgui::Label>("TurretUpgradeGoldLabel");
                    // Delete labels
                    auto deleteCopperLabel = infoPanel->get<tgui::Label>("TurretDeleteCopperLabel");
                    auto deleteSilverLabel = infoPanel->get<tgui::Label>("TurretDeleteSilverLabel");
                    auto deleteGoldLabel = infoPanel->get<tgui::Label>("TurretDeleteGoldLabel");
                    // Utilisation des getters pour les coûts
                    if (upgradeCopperLabel) upgradeCopperLabel->setText("Up Cuivre : " + std::to_string(game->selectedTower->getUpgradeCopperPrice()));
                    if (upgradeSilverLabel) upgradeSilverLabel->setText("Up Argent : " + std::to_string(game->selectedTower->getUpgradeSilverPrice()));
                    if (upgradeGoldLabel) upgradeGoldLabel->setText("Up Or : " + std::to_string(game->selectedTower->getUpgradeGoldPrice()));
                    if (deleteCopperLabel) deleteCopperLabel->setText("Remb Cuivre : " + std::to_string(game->selectedTower->getRefundCopper()));
                    if (deleteSilverLabel) deleteSilverLabel->setText("Remb Argent : " + std::to_string(game->selectedTower->getRefundSilver()));
                    if (deleteGoldLabel) deleteGoldLabel->setText("Remb Or : " + std::to_string(game->selectedTower->getRefundGold()));
                }
            } else if (selectedTurretButtonIndex != -1) {
                // Afficher le panel preview, masquer le info
                if (infoPanel) infoPanel->setVisible(false);
                if (previewPanel) previewPanel->setVisible(true);
                // Mise à jour des labels preview selon le bouton sélectionné
                auto nameLabel = previewPanel->get<tgui::Label>("PreviewNameLabel");
                auto rangeLabel = previewPanel->get<tgui::Label>("PreviewRangeLabel");
                auto speedLabel = previewPanel->get<tgui::Label>("PreviewSpeedLabel");
                auto damageLabel = previewPanel->get<tgui::Label>("PreviewDamageLabel");
                auto copperLabel = previewPanel->get<tgui::Label>("PreviewCopperPriceLabel");
                auto silverLabel = previewPanel->get<tgui::Label>("PreviewSilverPriceLabel");
                auto goldLabel = previewPanel->get<tgui::Label>("PreviewGoldPriceLabel");
                const char* turretNames[4] = {"Basic", "Long", "Rapide", "Slow"};
                Tower* previewTower = nullptr;
                if (selectedTurretButtonIndex == 0) previewTower = new BasicTower(-1);
                else if (selectedTurretButtonIndex == 1) previewTower = new SniperTower(-1);
                else if (selectedTurretButtonIndex == 2) previewTower = new SpeedTower(-1);
                else if (selectedTurretButtonIndex == 3) previewTower = new SlowTower(-1);
                if (previewTower) {
                    if (nameLabel) nameLabel->setText("Nom : " + std::string(turretNames[selectedTurretButtonIndex]));
                    if (rangeLabel) rangeLabel->setText("Portée : " + std::to_string(static_cast<int>(previewTower->getRange())));
                    if (speedLabel) speedLabel->setText("Vitesse : " + std::to_string(previewTower->getFireRate()));
                    if (damageLabel) damageLabel->setText("Dégâts : " + std::to_string(previewTower->getDamage()));
                    if (copperLabel) copperLabel->setText("Prix Cuivre : " + std::to_string(previewTower->getCostCopper()));
                    if (silverLabel) silverLabel->setText("Prix Argent : " + std::to_string(previewTower->getCostSilver()));
                    if (goldLabel) goldLabel->setText("Prix Or : " + std::to_string(previewTower->getCostGold()));
                    delete previewTower;
                }
            } else {
                // Rien sélectionné, masquer les deux panels
                if (infoPanel) infoPanel->setVisible(false);
                if (previewPanel) previewPanel->setVisible(false);
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
	game->setTowerIndex(-1);
    for (auto& btn : turretButtons) {
        if (btn)
            btn->getRenderer()->setBackgroundColor(tgui::Color(255, 255, 255));
    }
}
