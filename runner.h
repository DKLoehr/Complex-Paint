#ifndef RUNNER_H
#define RUNNER_H

#include <SFML/Graphics.hpp>
#include <random>
#include "guts/parser.h"
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

    parser::Tree* fct;

    std::vector<GUI*> elements; // An alternate way of accessing each gui element
    const int elementsSize = 26; // Initial size of elements (without any variables)

    DoubleGrid grid;
    sf::CircleShape loc;

    sf::Text lTitle;
    sf::Text rTitle;

    /// Elements for changing graph settings
    Button mirrorL;     // 0
    Button mirrorR;     // 1

    InputBox xRangeL;   // 2
    InputBox yRangeL;   // 3
    InputBox xScaleL;   // 4
    InputBox yScaleL;   // 5
    InputBox centerL;   // 6
    InputBox xRangeR;   // 7
    InputBox yRangeR;   // 8
    InputBox xScaleR;   // 9
    InputBox yScaleR;   // 10
    InputBox centerR;   // 11
    Checkbox numbersL;  // 12
    Checkbox linesL;    // 13
    Checkbox numbersR;  // 14
    Checkbox linesR;    // 15
    Button okGraph;     // 16

    Button clearGraphs; // 17

    /// Buttons to represent different preset equations
    Button presetLin;   // 18 -- Az + B
    Button presetQuad;  // 19 -- z*2 + c
    Button presetInv;   // 20 -- sqrt(z*2 - c)
    Button presetPol;   // 21 -- Polar equation

    /// Buttons to represent different possible drawing modes
    Button modeSingle;  // 22
    Button modeIterate; // 23

    /// Elements to do with changing equation settings
    InputBox equation;  // 24
    Button okEquation;  // 25

    std::vector<sf::CircleShape> points;
    cx locPos;
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
