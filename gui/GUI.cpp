#include "GUI.h"

GUI::GUI()
{

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

void GUI::SetPosition(double x, double y) {
    SetPosition(sf::Vector2f(x, y));
}

void GUI::Draw() {
    //m_text.setFont(m_f); // Workaround for an obnoxious bug
    //m_cap.setFont(m_f);

    m_w->draw(m_rectangle);
    m_w->draw(m_text);
    m_w->draw(m_cap);
}
