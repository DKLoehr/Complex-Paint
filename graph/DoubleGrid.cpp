#include "DoubleGrid.h"

DoubleGrid::DoubleGrid(sf::RenderWindow* window, sf::Font f, int heightOffset) :
    lGrid(Grid(window, f, 0, heightOffset, window->getSize().x / 2, window->getSize().y - heightOffset,
               10, 10, 2, 2)),
    rGrid(Grid(window, f, window->getSize().x / 2, heightOffset, window->getSize().x / 2,
                 window->getSize().y - heightOffset, 10, 10, 2, 2)),
    divider(sf::RectangleShape(sf::Vector2f(6, window->getSize().y - heightOffset)))
 {
    w = window;
    divider.setFillColor(sf::Color::Black);
    divider.setPosition((w->getSize().x - divider.getSize().x) / 2, heightOffset);
}

void DoubleGrid::ToggleLines() {
    lGrid.ToggleLines();
    rGrid.ToggleLines();
}

void DoubleGrid::Draw() {
    lGrid.Draw();
    rGrid.Draw();
    w->draw(divider);
}
