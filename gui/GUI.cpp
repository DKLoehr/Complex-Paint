#include "GUI.h"

GUI::GUI()
{

}

GUI::GUI(sf::RenderWindow* window, sf::Font font, double x, double y, double width, double height) :
    m_w(window),
    m_f(font),
    m_position(x, y),
    m_size(width, height),
    m_wSize(window->getSize()),
    m_text("", font),
    m_cap("", font),
    m_rectangle(sf::Vector2f(width, height))
{
    m_text.setColor(sf::Color::Black);
    m_text.setCharacterSize(15);

    m_cap.setColor(sf::Color::Black);
    m_cap.setCharacterSize(15);

    m_rectangle.setOutlineThickness(2);
    m_rectangle.setOutlineColor(sf::Color::Black);
    m_rectangle.setFillColor(sf::Color::White);

    m_cap.setPosition(x, y - 2);
    m_rectangle.setPosition(x, y);
    m_text.setPosition(x, y - 2);
}

void GUI::SetBoxColor(sf::Color c) {
    m_rectangle.setFillColor(c);
}

void GUI::SetText(std::string str) {
    m_text.setString(str);
}

std::string GUI::GetText() {
    return m_text.getString();
}

void GUI::Draw() {
    m_text.setFont(m_f); // Workaround for an obnoxious bug
    m_cap.setFont(m_f);

    m_w->draw(m_rectangle);
    m_w->draw(m_text);
    m_w->draw(m_cap);
}
