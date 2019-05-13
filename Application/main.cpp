#include <TGUI/TGUI.hpp>

int main(int argc, char * argv[])
{
    sf::RenderWindow window({800, 600}, "Window");
    tgui::Gui gui(window); // Create the gui and attach it to the window

    while (window.isOpen())
    {
        sf::Event event = {};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            gui.handleEvent(event); // Pass the event to the widgets
        }

        window.clear(sf::Color(0xFF, 0xFF, 0xFF));
        gui.draw(); // Draw all widgets
        window.display();
    }

    return 0;
}