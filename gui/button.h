#ifndef BUTTON_H
#define BUTTON_H

#include "GUI.h"

class Button : public GUI
{
public:
    Button(sf::RenderWindow* window, sf::Font font, int x, int y, int width, int height, std::string str);

    void SetPosition(sf::Vector2f newPos);
    void SetPosition(double x, double y);

    bool OnEnter(); // Always returns true
    bool OnClick(double xP, double yP); // Returns true if the button was clicked
    void OnTextEntered(char n);
};

#endif //BUTTON_H
