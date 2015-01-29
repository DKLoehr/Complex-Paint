#ifndef RUNNER_H
#define RUNNER_H

#include <SFML/Graphics.hpp>
#include "guts/Parser.h"
#include "gui/button.h"
#include "gui/text.h"
#include "gui/checkbox.h"
#include "graph/DoubleGrid.h"
#include "constants.h"

enum drawMode {
    mSingle,
    mIterative,
    mLine,
    mRect,
    mGrid,
    mCirc1,
    mCirc2,
    mCirc3,
    mFree
};

class Runner
{
private:
    int activeBox;
    bool isIterating;
    bool leftToRight;   // True if we're drawing on the right graph (meaning input was on the left); false if vice-versa
    bool isDrawing;     // True if we're in the middle of drawing a grid/circle/etc. shape, false otherwise
    bool drawingJulia;  // True if we're drawing a julia set (if that preset was active last time we ran UpdateEquation())
    drawMode mode;

    sf::RenderWindow* window;   // The window to which we draw
    sf::Font* inFont;           // The font to use for all text (VeraMono)
    sf::RenderTexture* pic;     // The canvas to which we draw graphed points (except julia sets)
    sf::RenderTexture* jpic;    // The canvas to which we draw julia sets
    sf::Sprite graphs;          // The sprite which we use to draw pic to the screen
    sf::Sprite jgraphs;         // The sprite which we use to draw jpic to the screen

    parser::Tree* fct;          // The tree we use to evaluate our expression

    DoubleGrid grid;            // The two sets of axes which appear on the bottom of the screen
    sf::CircleShape loc;        // The point which appears opposite the cursor
    sf::VertexArray shape;      // A special shape that we're drawing (grid, etc.)

    sf::Text lTitle;            // The title of the left column of graph settings: "Left Graph"
    sf::Text rTitle;            // The title of the right column of graph settings: "Right Graph"


    std::vector<GUI*> elements;  // An alternate way of accessing each gui element
    const int elementsSize = 34; // Initial size of elements (without any variables)

    Button okEquation;  // 0

    /// Buttons to represent different preset equations
    Button presetLin;   // 1 -- Az + B
    Button presetPol;   // 2 -- Polar equation
    Button presetQuad;  // 3 -- z*2 + c
    Button presetInv;   // 4 -- sqrt(z*2 - c)

    /// Buttons to represent different possible drawing modes
    Button modeSingle;  // 5
    Button modeIterate; // 6
    Button modeLine;    // 7
    Button modeRect;    // 8
    Button modeGrid;    // 9
    Button modeCirc1;   // 10 -- Centered at origin
    Button modeCirc2;   // 11 -- Centered at click & radius
    Button modeCirc3;   // 12 -- Concentric from click
    Button modeFree;    // 13

    /// Button to clear both graphs, except the julia set
    Button clearGraphs; // 14
    Button clearJulia;  // 15


    /// Elements for changing graph settings
    Button mirrorL;     // 16
    Button mirrorR;     // 17

    InputBox xRangeL;   // 18
    InputBox yRangeL;   // 19
    InputBox xScaleL;   // 20
    InputBox yScaleL;   // 21
    InputBox centerL;   // 22
    Checkbox numbersL;  // 23
    Checkbox linesL;    // 24
    InputBox xRangeR;   // 25
    InputBox yRangeR;   // 26
    InputBox xScaleR;   // 27
    InputBox yScaleR;   // 28
    InputBox centerR;   // 29
    Checkbox numbersR;  // 30
    Checkbox linesR;    // 31
    Button okGraph;     // 32

    /// Elements to do with changing equation settings
    InputBox equation;  // 33

    cx lastPoint;               // The position of the most recently-drawn point
    cx* lastStartPos;            // The position of the first point of the last iteration cycle
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
    /// Functions called by DrawShape
    void DrawLine(sf::Vector2i position);  // Draws a line from the previous and current mouse location
    void DrawRect(sf::Vector2i position);  // Draws a rectangle with corners at the previous and current mouse location
    void DrawGrid(sf::Vector2i position);  // Draws a grid with corners at the previous and current mouse location
    void DrawCirc1(sf::Vector2i position); // Draws a circle centered at the origin passing through the current mouse location
    void DrawCirc2(sf::Vector2i position); // Draws a circle centered at the previous mouse location passing through the current mouse location
    void DrawCirc3(sf::Vector2i position); // Draws concentric circles centered at the previous mouse location
    void DrawFree(sf::Vector2i position, bool toggleDrawing);  // Draws the current mouse location

    void ClearPic();  // Clear all points drawn to pic
    void ClearJPic(); // Clear all points drawn to jpic

    void DrawPoint(sf::CircleShape& point, sf::Vector2f pos, bool left); // Draws a given point to the right and/or left graphs on the appropriate layer
    std::string SanitizeString(std::string str); // Modify a string so that it's valid for the parser

public:
    Runner(sf::RenderWindow* w, sf::Font* font, sf::RenderTexture* p, sf::RenderTexture* j); // Constructor

    void HandleEvents();    // Perform all the tasks necessary to run

    void Draw();            // Draw everything to the screen
};

#endif // RUNNER_H
