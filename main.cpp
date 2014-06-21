#include "runner.h"

int main() {
    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    sf::RenderWindow window(sf::VideoMode(1200, 724), "Complex Paint Revamped");
    window.setPosition(sf::Vector2i(0, 0));

    Runner run = Runner(&window, &inFont);

    while(window.isOpen()) {
        run.HandleEvents();
        run.Draw();
    }
    return 0;
}
