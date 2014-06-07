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

enum drawMode {
    single,
    iterative
};

class Runner
{
private:
    int activeBox;
    bool isIterating;
    bool leftToRight;
    bool drawGUI;

    drawMode mode;

    sf::RenderWindow* window;
    sf::Font* inFont;

    std::vector<GUI*> elements; // An alternate way of accessing each gui element

    DoubleGrid grid;
    sf::CircleShape loc;

    sf::Text lTitle;
    sf::Text rTitle;

    /// Elements for changing graph settings
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

    Button clearGraphs; // 15

    /// Buttons to represent different preset equations
    Button presetLin;   // 16 -- Az + B
    Button presetQuad;  // 17 -- z*2 + c
    Button presetInv;   // 18 -- sqrt(z*2 - c)
    Button presetPol;   // 19 -- Polar equation

    /// Buttons to represent different possible drawing modes
    Button modeSingle;  // 20
    Button modeIterate; // 21

    /// Elements to do with changing equation settings
    InputBox equation;  // 22
    Button okEquation;  // 23

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
    void ActivateButtons(sf::Event event);

    void Draw();
};

#endif // RUNNER_H
