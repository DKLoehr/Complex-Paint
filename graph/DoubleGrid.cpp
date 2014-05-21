#include "DoubleGrid.h"

DoubleGrid::DoubleGrid(sf::RenderWindow* window, int heightOffset) {
    w = window;
    divider = sf::RectangleShape(sf::Vector2f(6, w->getSize().y - heightOffset));
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
