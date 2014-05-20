#include <SFML/Graphics.hpp>
#include "gui/button.h"
#include "gui/text.h"
#include "graph/DoubleGrid.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(1265, 480), "Input");
    window.setPosition(sf::Vector2i(0, 150));

    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    InputBox input = InputBox(&window, inFont, 1, 1, window.getSize().x * .25, window.getSize().y * .035);

    DoubleGrid grid = DoubleGrid(&window, inFont, 10, 10, 2, 2, true);
    grid.MakeGrid();

    sf::CircleShape loc = sf::CircleShape(10, 30);
    loc.setFillColor(sf::Color::Blue);

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                window.close();
            } else if(event.type == sf::Event::MouseMoved) {
                loc.setPosition(window.getSize().x / 2 + event.mouseMove.x, event.mouseMove.y);
            } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                std::cout << "Tab\n";
                grid.ToggleGrid();
            }
        }
        window.clear();

        input.Draw();
        grid.Draw();
        window.draw(loc);

        window.display();
    }
    return 0;
}
