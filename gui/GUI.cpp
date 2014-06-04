#include "GUI.h"

GUI::GUI()
{

}

void GUI::SetBoxColor(sf::Color c) {
    m_rectangle.setFillColor(c);
}

void GUI::SetPosition(double x, double y) {
    SetPosition(sf::Vector2f(x, y));
}
