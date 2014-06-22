#ifndef RUNNER_H
#define RUNNER_H

#include <SFML/Graphics.hpp>
#include "guts/Parser.h"
#include "gui/button.h"
#include "gui/text.h"
#include "gui/checkbox.h"
#include "graph/DoubleGrid.h"

enum drawMode {
    single,
    iterative
};

class Runner
{
private:
    int activeBox;
    bool isIterating;
    bool drawGUI;

    drawMode mode;

    sf::RenderWindow* window;
    sf::Font* inFont;

    parser::Tree* fct;

    DoubleGrid grid;
    sf::CircleShape loc;

    sf::Text lTitle;
    sf::Text rTitle;


    std::vector<GUI*> elements; // An alternate way of accessing each gui element
    const int elementsSize = 26; // Initial size of elements (without any variables)

    Button okEquation;  // 0

    /// Buttons to represent different preset equations
    Button presetLin;   // 1 -- Az + B
    Button presetPol;   // 2 -- Polar equation
    Button presetQuad;  // 3 -- z*2 + c
    Button presetInv;   // 4 -- sqrt(z*2 - c)

    /// Buttons to represent different possible drawing modes
    Button modeSingle;  // 5
    Button modeIterate; // 6

    /// Button to clear both graphs
    Button clearGraphs; // 7


    /// Elements for changing graph settings
    Button mirrorL;     // 8
    Button mirrorR;     // 9

    InputBox xRangeL;   // 10
    InputBox yRangeL;   // 11
    InputBox xScaleL;   // 12
    InputBox yScaleL;   // 13
    InputBox centerL;   // 14
    InputBox xRangeR;   // 15
    InputBox yRangeR;   // 16
    InputBox xScaleR;   // 17
    InputBox yScaleR;   // 18
    InputBox centerR;   // 19
    Checkbox numbersL;  // 20
    Checkbox linesL;    // 21
    Checkbox numbersR;  // 22
    Checkbox linesR;    // 23
    Button okGraph;     // 24

    /// Elements to do with changing equation settings
    InputBox equation;  // 25

    std::vector<sf::CircleShape> points;
    cx locPos;
    sf::Vector2f graphCoords;

    void Init();
public:
    Runner(sf::RenderWindow* w, sf::Font* font);

    void HandleEvents();
    void Iterate(bool keepIterating, cx* newPos = NULL);

    void SetActiveElement(double x, double y); // Determines what activeBox should be based on the mouse coordinates
    void StepActiveElement(bool increment); // Increased activeBox by 1 if true, decreases if false, keeping it in valid bounds
    void UpdateGraphs(); // Apply changes from the graph input elements to the two graphs
    void UpdateEquation();
    void ActivateButtons(sf::Event event);

    void Draw();
};

#endif // RUNNER_H
