#ifndef RUNNER_H
#define RUNNER_H

#include <SFML/Graphics.hpp>
#include "guts/Parser.h"
#include "gui/button.h"
#include "gui/text.h"
#include "gui/checkbox.h"
#include "graph/DoubleGrid.h"

#define HEIGHT_OFFSET 200   // The number of pixels between the top of the window and the top of the graphs
#define GRID_LINES_DELTA 20       // The number of lines in each direction on a grid drawn in grid mode
#define GRID_POINT_DELTA 3      // The number of points between each grid line that we iterate

enum drawMode {
    single,
    iterative,
    grid
};

class Runner
{
private:
    int activeBox;
    bool isIterating;
    bool leftToRight;   // True if we're drawing on the right graph (meaning input was on the left); false if vice-versa
    bool isDrawing;     // True if we're in the middle of drawing a grid/circle/etc. shape, false otherwise
    drawMode mode;

    sf::RenderWindow* window;   // The window to which we draw
    sf::Font* inFont;           // The font to use for all text (VeraMono)
    sf::RenderTexture* pic;     // The canvas to which we draw graphed points
    sf::Sprite graphs;          // The sprite which we use to draw pic to the screen

    parser::Tree* fct;          // The tree we use to evaluate our expression

    DoubleGrid grid;            // The two sets of axes which appear on the bottom of the screen
    sf::CircleShape loc;        // The point which appears opposite the cursor
    sf::VertexArray shape;      // A special shape that we're drawing (grid, etc.)

    sf::Text lTitle;            // The title of the left column of graph settings: "Left Graph"
    sf::Text rTitle;            // The title of the right column of graph settings: "Right Graph"


    std::vector<GUI*> elements;  // An alternate way of accessing each gui element
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
    Checkbox numbersL;  // 15
    Checkbox linesL;    // 16
    InputBox xRangeR;   // 17
    InputBox yRangeR;   // 18
    InputBox xScaleR;   // 19
    InputBox yScaleR;   // 20
    InputBox centerR;   // 21
    Checkbox numbersR;  // 22
    Checkbox linesR;    // 23
    Button okGraph;     // 24

    /// Elements to do with changing equation settings
    InputBox equation;  // 25

    cx lastPoint;               // The position of the most recently-drawn point
    bool lastGraph;             // Which graph lastPoint was drawn on; true = left, false = right
    sf::Vector2f graphCoords;   // The position of the point on the graph in sf::Vector form

    void Init(); // Initialize the class

    void Iterate(bool keepIterating, cx* newPos = NULL); // Iterates our function if true, stops iterating if false;
                                                         // If newPos != NULL, will begin iterating at newPos's coordinates; else uses previous coords.

    void SetActiveElement(double x, double y);  // Determines what activeBox should be based on the mouse coordinates; x and y are window coords to test
    void StepActiveElement(bool increment);     // Increases activeBox by 1 if true, decreases if false, keeping it in valid bounds
    void UpdateGraphs();                        // Apply changes from the graph input elements to the two graphs
    void UpdateEquation();                      // Apply changes from the equation inputbox and the parameter inputboxes
    void ActivateButtons(sf::Event event);      // Activate buttons depending on activeBox and/or the event; event is the event that activated a button

    void DrawShape(bool toggleDrawing = false); // Draws a shape based on mode; toggles whether or not it's drawing anything if passed true

    void ClearPic(); // Clear all points drawn to pic

public:
    Runner(sf::RenderWindow* w, sf::Font* font, sf::RenderTexture* p); // Constructor

    void HandleEvents();    // Perform all the tasks necessary to run

    void Draw();            // Draw everything to the screen
};

#endif // RUNNER_H
