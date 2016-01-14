#include "runner.h"
#ifdef __APPLE__
#include "ResourcePath.hpp"
#endif

int main() {
    sf::Font inFont;
    #ifdef __APPLE__
    if(!inFont.loadFromFile(resourcePath() + "VeraMono.ttf"))
        return -1;
    #else
    if(!inFont.loadFromFile("VeraMono.ttf"))
        return -1;
    #endif

    sf::RenderWindow window(sf::VideoMode(1200, 724), "Complex Paint Revamped", sf::Style::Titlebar | sf::Style::Close);
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
