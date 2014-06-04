#ifndef RUNNER_H
#define RUNNER_H

#include <SFML/Graphics.hpp>
#include <complex>
#include <random>
#include "gui/button.h"
#include "gui/text.h"
#include "gui/checkbox.h"
#include "graph/DoubleGrid.h"
#include <iostream>

class Runner
{
private:
    sf::RenderWindow* window;
    sf::Font inFont;

    InputBox equation;

    DoubleGrid grid;

    sf::CircleShape loc;

    void Init();
public:
    Runner(sf::RenderWindow* w, sf::Font font);

    void Draw();
};

#endif // RUNNER_H
