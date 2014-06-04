#include "runner.h"

Runner::Runner(sf::RenderWindow* w, sf::Font font) {
    window = w;
    inFont = font;
    Init();
}


void Runner::Init() {
    equation = InputBox(window, inFont, 1, 1, 200, 15);

    grid = DoubleGrid(window, inFont, 200);

    loc = sf::CircleShape(1, 30);
    loc.setFillColor(sf::Color::Black);
}

void Runner::Draw() {
    window->clear(sf::Color::White);

    equation.Draw();
    grid.Draw();
    window->draw(loc);

    window->display();
}
