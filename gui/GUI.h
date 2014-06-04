#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>

class GUI
{
protected:
    bool isActive;

    sf::RenderWindow* m_w;

    sf::Vector2f m_position; // x and y position
    sf::Vector2f m_size;     // Width and heights
    sf::Vector2f m_wSize;    // Initial width and height of the window (for dealing with resizing)

    sf::Font m_f;
    sf::Text m_text;
    sf::Text m_cap;
    sf::RectangleShape m_rectangle;
public:
    GUI();

    void SetBoxColor(sf::Color c);

    virtual void SetPosition(sf::Vector2f newPos) {}
    virtual void SetPosition(double x, double y);

    virtual void OnEnter() {}
    virtual void OnClick(int xP, int yP) {}
    virtual void OnTextEntered(char n) {}

    void SetText(std::string str);
    std::string GetText();

    virtual void Draw();
};

#endif // GUI_H
