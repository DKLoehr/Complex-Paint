#ifndef INPUT_BOX_H
#define INPUT_BOX_H

#include <SFML/Graphics.hpp>
#include <string>

/*
class TextSettings {
public:
    int charSize;
    sf::Font f_regular;
    sf::Font f_bold;
    sf::Font f_italic;
    sf::Font f_special;

    TextSettings(int cSize, sf::Font f_r, sf::Font f_b, sf::Font f_i, sf::Font f_s) : f_regular(f_r), f_bold(f_b), f_italic(f_i), f_special(f_s){
        charSize=cSize;
    };
};
*/
class InputBox {
private:
    sf::Font m_f;
    bool m_isFocused;
    int m_x;
    int m_y;
    int m_width;
    int m_height;

    sf::Text m_stored;
    sf::Text m_cap;
    sf::RenderWindow* m_w;
    sf::RectangleShape m_rectangle;
public:
    InputBox(); // Warning -- since this doesn't take a window, this is useless!
    InputBox(sf::RenderWindow* window, sf::Font font, int x, int y, int charWidth, int charHeight, std::string cap = "");

    void EnterText(char n);
    void SetString(std::string str);
    void SetBoxColor(sf::Color c);

    bool IsValid(char n);

    std::string GetStoredString();
    int GetStringAsInt(); // Only call if you know the string is just a single integer
    sf::Vector2f GetStringAsVector(); // Only call if you know the string is of the form "(int, int)"

    void Draw();
};

#endif //INPUT_BOX_H
