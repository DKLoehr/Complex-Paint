#include "runner.h"
#include "iohelpers.h"
#ifdef __APPLE__
#include "ResourcePath.hpp"
#endif


int main() {
    ini_data options = parse_ini("CPaint.ini");
    std::cout << "Options: " << options.font << ", " << options.height << "x" << options.width << "\n";

    sf::Font inFont;
    std::string font_file = options.font;
    #ifdef __APPLE__
    if !options.font.compare("VeraMono.ttf")
        font_file = resourcePath() + "VeraMono.ttf";
    #endif
    if(!inFont.loadFromFile(font_file)) {
        emit_error("Unable to load font from file:" + options.font + "\n");
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(options.height, options.width), "Complex Paint Revamped", sf::Style::Titlebar | sf::Style::Close);
    window.setPosition(sf::Vector2i(0, 0));

    sf::RenderTexture pic;
    if(!pic.create(window.getSize().x, window.getSize().y - HEIGHT_OFFSET))
        return -2;
    sf::RenderTexture jpic;
    if(!jpic.create(window.getSize().x, window.getSize().y - HEIGHT_OFFSET))
        return -3;

    Runner run = Runner(&window, &inFont, &pic, &jpic);

    while(window.isOpen()) {
        run.HandleEvents();
        run.Draw();
    }
    return 0;
}
