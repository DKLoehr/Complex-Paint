#include <SFML/Graphics.hpp>
#include <string>
#include "text.h"
#include <iostream>

InputBox::InputBox():
    m_stored(),
    m_cap(),
    m_rectangle(),
    m_f()
{
    if(!m_f.loadFromFile("VeraMono.ttf")){/*error handling*/}
    m_width = 1;
    m_height = 1;
    m_isFocused = false;

    m_stored.setString("");
    m_stored.setFont(m_f);
    m_stored.setCharacterSize(15);
    m_stored.setColor(sf::Color::Black);

    m_cap.setString("");
    m_stored.setFont(m_f);
    m_cap.setCharacterSize(15);
    m_cap.setColor(sf::Color::Black);

    m_rectangle.setOutlineThickness(2);
    m_rectangle.setOutlineColor(sf::Color(100,100,100));
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
    if(IsValid(n)) { // n is a valid character
        std::string temp = m_stored.getString();
        if(n != 8) { // Some character
            m_stored.setString(temp.substr(0, temp.length()) + n);
        } else if(n == 8) { // Backspace
            m_stored.setString(temp.substr(0, temp.length() - 1));
        }
    }
}

void InputBox::SetString(std::string str) {
    m_stored.setString(str);
}

bool InputBox::IsValid(char n) {
    if(n == 8 || n == 32 || n == 33 || (n >= 40 && n <= 57) || (n >= 65 && n <= 90) || n == 94 ||
       (n >= 97 && n <= 122) || n == 127)
        return true;
    else
        return false;
}

std::string InputBox::GetStoredString() {
    return m_stored.getString();
}

double InputBox::GetStringAsDouble() {
    double ret = 0;
    std::string str = m_stored.getString();
    for(int i = str.length() - 1; i >= 0; i--) {
        ret += pow(10, str.length() - 1 - i) * (str[i] - '0');
    }
    return ret;

}

sf::Vector2f InputBox::GetStringAsVector() {
    std::string str = m_stored.getString();
    if(str.find('(') != std::string::npos)
        str = str.substr(1, std::string::npos);
    if(str.find(')') != std::string::npos)
        str = str.substr(0, str.length() - 1);
    std::string xStr = str.substr(0, str.find(','));
    std::string yStr = str.substr(str.find(',') +  1, str.length());
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
    std::string str = m_stored.getString();
    if((str.length()) * 10 > m_width) {
        std::string strShown = str.substr(str.length() - m_width / 10 - 1, std::string::npos);
        m_stored.setString(strShown);
    }
    m_stored.setFont(m_f); // Dealing with an incredibly weird and annoying bug
    m_w->draw(m_stored);
    m_stored.setString(str);

    m_cap.setFont(m_f); // Dealing with an incredibly weird and annoying bug
    m_w->draw(m_cap);
}
