#include "runner.h"
#ifdef __APPLE__
#include "ResourcePath.hpp"
#endif

#include <iostream>
#include <fstream>

struct ini_data {
  std::string font;
  unsigned int height;
  unsigned int width;
};

// Primitive parser to read options from the ini file.
// TODO: Change this to use regexes instead
ini_data parse_ini(std::string ini_name) {
    ini_data options;
    options.font = "VeraMono.ttf";
    options.height = 1200;
    options.width = 724;

    ifstream ini_file;
    ini_file.open(ini_name);
    if(!ini_file.is_open()) {
        std::cout << "Failed to open ini file. Using default parameters\n";
        return options;
    }

    std::string line;
    while(std::getline(ini_file, line)) {
        if(line.length() == 0) // Empty line, ignore
            continue;
        if(line[0] == '#') // This is a comment, so ignore it
            continue;
        else if(!line.substr(0, 5).compare("font="))
            options.font = line.substr(5, std::string::npos);
        else if(!line.substr(0, 7).compare("height="))
            options.height = atoi(line.substr(7, std::string::npos).c_str());
        else if(!line.substr(0, 6).compare("width="))
            options.width = atoi(line.substr(6, std::string::npos).c_str());
        else // Unrecognized line, let the user know
            std::cout << "Unrecognized line in the ini file:\n" << line << "\n";
  }
  ini_file.close();
  return options;
}

// Temporary method to signal that an error has occurred.
// Hopefully will upgrade to a proper error message.
void signal_error(std::string msg) {
  std::cout << msg;
}

int main() {
    ini_data options = parse_ini("CPaint.ini");
    std::cout << options.font << ", " << options.height << "x" << options.width << "\n";

    sf::Font inFont;
    std::string font_file = options.font;
    #ifdef __APPLE__
    if !options.font.compare("VeraMono.ttf")
        font_file = resourcePath() + "VeraMono.ttf";
    #endif
    if(!inFont.loadFromFile(font_file)) {
        signal_error("Unable to load font from file:" + options.font + "\n");
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
