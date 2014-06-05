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

    std::vector<GUI*> elements; // An alternate way of accessing each gui element

    DoubleGrid grid;
    sf::CircleShape loc;

    InputBox xRangeL; // 0
    InputBox yRangeL; // 1
    InputBox xScaleL; // 2
    InputBox yScaleL; // 3
    InputBox centerL; // 4
    InputBox xRangeR; // 5
    InputBox yRangeR; // 6
    InputBox xScaleR; // 7
    InputBox yScaleR; // 8
    InputBox centerR; // 9

    Checkbox numbersL;  // 10
    Checkbox linesL;    // 11
    Checkbox numbersR;  // 12
    Checkbox linesR;    //13

    Button okGraph; // 14

    InputBox equation;

    void Init();
public:
    Runner(sf::RenderWindow* w, sf::Font font);

    void HandleEvents();

    void Draw();
};

#endif // RUNNER_H
