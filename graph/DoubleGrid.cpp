#include "DoubleGrid.h"

DoubleGrid::DoubleGrid(sf::RenderWindow* window, sf::Font f, int heightOffset) :
    lGrid(Grid(window, f, 0, heightOffset, window->getSize().x / 2, window->getSize().y - heightOffset,
               10, 10, 2, 2)),
    rGrid(Grid(window, f, window->getSize().x / 2, heightOffset, window->getSize().x / 2,
                 window->getSize().y - heightOffset, 10, 10, 2, 2)),
    horDivider(sf::RectangleShape(sf::Vector2f(window->getSize().x, 2))),
    vertDivider(sf::RectangleShape(sf::Vector2f(6, window->getSize().y - heightOffset)))
{
    w = window;

    horDivider.setFillColor(sf::Color::Black);
    horDivider.setPosition(0, heightOffset - horDivider.getSize().y);

    vertDivider.setFillColor(sf::Color::Black);
    vertDivider.setPosition((w->getSize().x - vertDivider.getSize().x) / 2, heightOffset);
}

void DoubleGrid::SetYPosition(int yPos) {
    horDivider.setPosition(0, yPos);
    vertDivider.setPosition((w->getSize().x - vertDivider.getSize().x) / 2, yPos);
    lGrid.SetPosition(0, yPos);
    rGrid.SetPosition(w->getSize().x /2, yPos);
}

void DoubleGrid::Draw() {
    lGrid.Draw();
    rGrid.Draw();
    w->draw(horDivider);
    w->draw(vertDivider);
}
