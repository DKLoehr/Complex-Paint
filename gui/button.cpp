#include "button.h"
#include <iostream>

Button::Button(sf::RenderWindow* window, sf::Font font, int x, int y, int width, int height, std::string str) :
        m_text(),
        m_f(font),
        m_rectangle(sf::Vector2f(width, height))

{
    m_w = window;
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_wWidth = window->getSize().x;
    m_wHeight = window->getSize().y;

    m_text.setString(str);
    m_text.setFont(m_f);
    m_text.setCharacterSize(15);
    m_text.setColor(sf::Color::Black);
    m_rectangle.setOutlineThickness(2);
    m_rectangle.setOutlineColor(sf::Color::Black);
    m_rectangle.setFillColor(sf::Color::White);

    m_rectangle.setPosition(x, y);
    m_text.setPosition(x, y - 2);
}

double Button::GetHeight() {
    return m_height;
}

void Button::Draw() {
    m_w->draw(m_rectangle);
    m_text.setFont(m_f); // Dealing with an incredibly weird and annoying bug
    m_w->draw(m_text);
}

bool Button::IsPressed(int xP, int yP) {
    if((m_x - 3) * m_w->getSize().x / m_wWidth < xP && xP < (m_x + m_width + 3) * m_w->getSize().x / m_wWidth &&
       (m_y - 3) * m_w->getSize().y / m_wHeight < yP && yP < (m_y + m_height + 2) * m_w->getSize().y / m_wHeight)
        return true;
    else
        return false;
}

void Button::SetText(std::string str){
    m_text.setString(str);
}

std::string Button::GetText(){
    return m_text.getString();
}
