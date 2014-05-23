#include <SFML/Graphics.hpp>
#include "gui/button.h"
#include "gui/text.h"
#include "graph/DoubleGrid.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1265, 480), "Complex Paint Revamped");
    window.setPosition(sf::Vector2i(0, 240));

    sf::RenderWindow inputWindow(sf::VideoMode(640, 200), "Complex Paint Revamped");
    inputWindow.setPosition(sf::Vector2i(0, window.getPosition().y - inputWindow.getSize().y - 37));

    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    InputBox equation = InputBox(&inputWindow, inFont, 1, 1, 200, 15);

    DoubleGrid grid = DoubleGrid(&window, 20);
    grid.lGrid = Grid(&window, inFont, 0, 20, window.getSize().x / 2, window.getSize().y - 20, 10, 10, 2, 2, true);
    grid.rGrid = Grid(&window, inFont, window.getSize().x / 2, 20, window.getSize().x / 2,
                      window.getSize().y - 20, 10, 10, 2, 2, true);
    grid.ToggleLines(); // Workaround for a very strange bug in which numbers don't appear until you toggle
    grid.ToggleLines(); // Toggle back so we get to the original toggle value

    sf::CircleShape loc = sf::CircleShape(2, 30);
    loc.setFillColor(sf::Color::Black);

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                window.close();
                inputWindow.close();
            } else if(event.type == sf::Event::MouseMoved) {
                loc.setPosition(window.getSize().x / 2 + event.mouseMove.x - 1, event.mouseMove.y - 1);
            } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                grid.ToggleLines();
            }
        }
        window.clear(sf::Color::White);
        inputWindow.clear();

        grid.Draw();
        window.draw(loc);

        equation.Draw();

        window.display();
        inputWindow.display();
    }
    return 0;
}
