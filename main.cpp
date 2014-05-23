#include <SFML/Graphics.hpp>
#include "gui/button.h"
#include "gui/text.h"
#include "graph/DoubleGrid.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1265, 480), "Complex Paint Revamped");
    window.setPosition(sf::Vector2i(0, 240));

    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    InputBox equation = InputBox(&window, inFont, 1, 1, 200, 15);

    DoubleGrid grid = DoubleGrid(&window, inFont, 20);

    sf::CircleShape loc = sf::CircleShape(2, 30);
    loc.setFillColor(sf::Color::Black);

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                window.close();
            } else if(event.type == sf::Event::MouseMoved) {
                loc.setPosition(window.getSize().x / 2 + event.mouseMove.x - 1, event.mouseMove.y - 1);
            } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                grid.ToggleLines();
            }
        }
        window.clear(sf::Color::White);

        grid.Draw();
        window.draw(loc);

        equation.Draw();

        window.display();
    }
    return 0;
}
