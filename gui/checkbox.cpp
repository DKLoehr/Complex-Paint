#include "Checkbox.h"

Checkbox::Checkbox() {

}

Checkbox::Checkbox(sf::RenderWindow* window, sf::Font font, int x, int y, std::string cap, bool isToggled)
    : GUI(window, font, x, y, 11, 11)
{
    m_isToggled = !isToggled;
    Toggle(); // Toggle so we update our string

    m_cap.setString(cap);

    m_rectangle.setPosition(x + cap.length() * 10, y + 5);
    m_text.setPosition(x + cap.length() * 10 + 1, y);
    m_cap.setPosition(x, y);
}

void Checkbox::Toggle() {
    m_isToggled = !m_isToggled;
    if(m_isToggled)
        m_text.setString("x");
    else
        m_text.setString("");
}

void Checkbox::SetPosition(sf::Vector2f newPos) {
    SetPosition(newPos.x, newPos.y);
}

void Checkbox::SetPosition(double x, double y) {
    std::string str = m_cap.getString();
    m_rectangle.setPosition(x + str.length() * 10, y + 5);
    m_text.setPosition(x + str.length() * 10 + 1, y);
    m_cap.setPosition(x, y);
}
bool Checkbox::OnEnter() {
    Toggle();
    return m_isToggled;
}

bool Checkbox::OnClick(double xP, double yP) {
    double xScale = m_w->getSize().x / m_wSize.x, yScale = m_w->getSize().y / m_wSize.y;

    if((m_position.x - 3) * xScale < xP && xP < (m_position.x + m_size.x + 3) * xScale &&
       (m_position.y - 3) * yScale < yP && yP < (m_position.y + m_size.y + 2) * yScale)
        Toggle();

    return m_isToggled;
}

void Checkbox::OnTextEntered(char n) {
    return;
}
