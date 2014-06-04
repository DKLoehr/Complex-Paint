#ifndef INPUT_BOX_H
#define INPUT_BOX_H

#include "GUI.h"

class InputBox : public GUI
{
private:
    bool IsValid(char n);
public:
    InputBox(); // Warning -- since this doesn't take a window, this is useless!
    InputBox(sf::RenderWindow* window, sf::Font font, int x, int y, int charWidth, int charHeight, std::string cap = "");

    void SetPosition(sf::Vector2f newPos);
    void SetPosition(double x, double y);

    bool OnEnter(); // Does nothing
    bool OnClick(double xP, double yP); // Does nothing
    void OnTextEntered(char n); // Adds text to string


    double GetStringAsDouble(); // Only call if you know the string is just a single number
    sf::Vector2f GetStringAsVector(); // Only call if you know the string is of the form "(int, int)"
};

#endif //INPUT_BOX_H
