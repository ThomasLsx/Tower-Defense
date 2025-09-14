// main.cpp

// TGUI core and SFML backend
#include <TGUI/Core.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>

// SFML graphics
#include <SFML/Graphics.hpp>

// Standard output
#include <iostream>

// Button callback function
static void onButtonClick()
{
    std::cout << "OK button pressed!" << std::endl;
}

int main()
{
    // --- Create a window ---
    sf::VideoMode mode(sf::Vector2u(800, 600)); // window size
    sf::RenderWindow window(mode, "SFML + TGUI Example"); // create window
    tgui::Gui gui{ window }; // attach TGUI GUI to the window

    // --- Create OK button ---
    auto button = tgui::Button::create("OK"); // create a button
    button->setPosition(350, 500); // set position
    button->setSize(100, 40); // set size
    button->onPress(onButtonClick); // assign callback
    gui.add(button); // add button to GUI

     // --- Invisible detection zone ---
    sf::FloatRect detectionZone(sf::Vector2f(200, 150), sf::Vector2f(400, 200));
    // Rectangle from (200,150) with width 400, height 200

// --- Main loop ---
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // Close window event
            if (event->is<sf::Event::Closed>())
                window.close();

            // Forward event to TGUI
            gui.handleEvent(*event);

            // Handle mouse clicks inside detection zone
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                auto mouse = event->getIf<sf::Event::MouseButtonPressed>();

                // Convert sf::Vector2i to sf::Vector2f
                sf::Vector2f pos(static_cast<float>(mouse->position.x),
                    static_cast<float>(mouse->position.y));

                if (detectionZone.contains(pos)) // check if click is inside the zone
                {
                    if (mouse->button == sf::Mouse::Button::Left)
                        std::cout << "Left click at ("
                        << mouse->position.x << ", "
                        << mouse->position.y << ")\n";

                    if (mouse->button == sf::Mouse::Button::Right)
                        std::cout << "Right click at ("
                        << mouse->position.x << ", "
                        << mouse->position.y << ")\n";
                }
            }
        }

        // --- Draw ---
        window.clear(sf::Color(50, 50, 50)); // clear window with dark gray
        gui.draw(); // draw TGUI widgets
        window.display(); // display the window contents
    }

    return 0; // exit program
}
