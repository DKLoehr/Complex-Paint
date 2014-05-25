#include <SFML/Graphics.hpp>
#include <string>
#include "text.h"
#include <iostream>

InputBox::InputBox() {

}

InputBox::InputBox(sf::RenderWindow* window, sf::Font font, int x, int y, int charWidth, int charHeight,
                   std::string cap):
    m_stored(),
    m_rectangle(sf::Vector2f(charWidth, charHeight))
{
    m_w = window;
    m_f = font;
    m_x = x;
    m_y = y;
    m_width = charWidth;
    m_height = charHeight;
    m_isFocused = false;

    m_stored.setString("");
    m_stored.setFont(m_f);
    m_stored.setCharacterSize(15);
    m_stored.setColor(sf::Color::Black);

    m_cap.setString(cap);
    m_cap.setFont(m_f);
    m_cap.setCharacterSize(15);
    m_cap.setColor(sf::Color::Black);

    m_rectangle.setOutlineThickness(2);
    m_rectangle.setOutlineColor(sf::Color(100,100,100));

    m_cap.setPosition(m_x, m_y - 2);
    m_rectangle.setPosition(m_x + cap.length() * 10, m_y);
    m_stored.setPosition(m_x + cap.length() * 10, m_y - 2);
};

void InputBox::EnterText(char n) {
    if(n == '0' || n == '1' || n == '2' || n == '3' || n == '4' || n == '5' ||
       n == '6' || n == '7' || n == '8' || n == '9' || n == ',' || n == '/' ||
       n == '(' || n == ')' ||n == 8) { // n is a digit, comma, slash, or backspace
        std::string temp = m_stored.getString();
        if(n != 8) { // Some character
            if(m_stored.getString().getSize() * 10 < m_width)
                m_stored.setString(temp.substr(0, temp.length()) + n);
        } else if(n == 8) { // Backspace
            m_stored.setString(temp.substr(0, temp.length() - 1));
        }
    }
}

std::string InputBox::GetStoredString() {
    return m_stored.getString();
}

int InputBox::GetStringAsInt() {
    int ret = 0;
    std::string str = m_stored.getString();
    for(int i = str.length() - 1; i >= 0; i--) {
        ret += pow(10, str.length() - 1 - i) * (str[i] - '0');
    }
    return ret;

}

sf::Vector2f InputBox::GetStringAsVector() {
    std::string str = m_stored.getString();
    std::string xStr = str.substr(1, str.find(',') - 1);
    std::string yStr = str.substr(str.find(',') +  1, str.length() - 4);
    std::cout << yStr << "\n";
    int x = 0, y = 0;
    for(int i = xStr.length() - 1; i >= 0; i--) {
        x += pow(10, xStr.length() - 1 - i) * (xStr[i] - '0');
    }
    for(int i = yStr.length() - 1; i >= 0; i--) {
        y += pow(10, yStr.length() - 1 - i) * (yStr[i] - '0');
    }
    return sf::Vector2f(x, y);
}

void InputBox::SetBoxColor(sf::Color c) {
    m_rectangle.setFillColor(c);
}

void InputBox::Draw() {
    m_w->draw(m_rectangle);
    m_w->draw(m_stored);
    m_w->draw(m_cap);
}
