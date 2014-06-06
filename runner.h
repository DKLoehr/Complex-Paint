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
    int activeBox;
    bool isIterating;
    bool leftToRight;

    sf::RenderWindow* window;
    sf::Font* inFont;

    std::vector<GUI*> elements; // An alternate way of accessing each gui element

    DoubleGrid grid;
    sf::CircleShape loc;

    sf::Text lTitle;
    sf::Text rTitle;

    InputBox xRangeL;   // 0
    InputBox yRangeL;   // 1
    InputBox xScaleL;   // 2
    InputBox yScaleL;   // 3
    InputBox centerL;   // 4
    InputBox xRangeR;   // 5
    InputBox yRangeR;   // 6
    InputBox xScaleR;   // 7
    InputBox yScaleR;   // 8
    InputBox centerR;   // 9
    Checkbox numbersL;  // 10
    Checkbox linesL;    // 11
    Checkbox numbersR;  // 12
    Checkbox linesR;    // 13
    Button okGraph;     // 14

    InputBox equation;  // 15
    Button okEquation;  // 16

    std::vector<sf::CircleShape> points;
    std::complex<double> locPos;
    sf::Vector2f graphCoords;

    void Init();
public:
    Runner(sf::RenderWindow* w, sf::Font* font);

    void HandleEvents();
    void Iterate(bool keepIterating);

    void SetActiveElement(double x, double y); // Determines what activeBox should be based on the mouse coordinates
    void StepActiveElement(bool increment); // Increased activeBox by 1 if true, decreases if false, keeping it in valid bounds
    void UpdateGraphs(); // Apply changes from the graph input elements to the two graphs
    void UpdateEquation();

    void Draw();
};

#endif // RUNNER_H
