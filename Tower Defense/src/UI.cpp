// UI.cpp
#include "UI.h"
#include <iostream>

UI::UI(sf::RenderWindow& window)
{
    gui.setTarget(window);

    modeLabel = createLabel("Mode: ", 10, 10);
	gui.add(modeLabel, "modeLabel");
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

void UI::setMode(const std::string& mode)
{
    if (modeLabel)
        modeLabel->setText("Mode: " + mode);
}

void UI::createMenu()
{
    if (!menuInitialized) {
        auto menu_ui = tgui::Group::create();
        // Bouton Play
        auto boutonPlay = ui->createButton("Play", window->getWidth() / 2 - 100, window->getHeight() / 2 - 25, 200, 50);
        boutonPlay->onPress([this]() {
            std::cout << "Play button pressed!" << std::endl;
            m_eGameMode = Play;
            ui->setMode("Play");
            });
        // Bouton Level Editor
        auto boutonEditor = ui->createButton("Level Editor", window->getWidth() / 2 - 100, window->getHeight() / 2 + 25, 200, 50);
        boutonEditor->onPress([this]() {
            std::cout << "Level Editor button pressed!" << std::endl;
            m_eGameMode = Editor;
            ui->setMode("Level Editor");
            });

        menu_ui->add(boutonPlay);
        menu_ui->add(boutonEditor);

        ui->gui.add(menu_ui);

        menu_ui->setVisible(true);

        menuInitialized = true;
    }
}
