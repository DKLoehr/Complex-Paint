#include <SFML/Graphics.hpp>
#include "gui/button.h"
#include "gui/text.h"
#include "graph/DoubleGrid.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1265, 620), "Complex Paint Revamped");
    window.setPosition(sf::Vector2i(0, 100));

    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    InputBox equation = InputBox(&window, inFont, 1, 1, 200, 15);

    DoubleGrid grid = DoubleGrid(&window, inFont, 160);

    Button graphModify = Button(&window, inFont, window.getSize().x / 2 - 54, 140, 117, 15, "Modify Graphs");
    Button eqModify = Button(&window, inFont, 210, 2, 135, 15, "Modify Equation");

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

        equation.Draw();
        grid.Draw();

        graphModify.Draw();
        eqModify.Draw();

        window.draw(loc);

        window.display();
    }
    return 0;
}
