#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>

/**
* This class represents the graph onto which you might draw functions.
* It consists of an x (horizontal) and y (vertical) axis, with either evenly spaced tick marks or full lines,
* as well as labels on the axes to indicate scale.
**/

class Grid
{
private:
    /// The window to which we draw
    sf::RenderWindow* m_w;
    /// The font we use for the axis labels
    sf::Font m_f;


    /** The following are all relative to the WINDOW, not the graph **/
    /// The coordinates of the top-left corner of the grid on the window
    sf::Vector2i m_position;
    /// Height and width of the grid
    sf::Vector2i m_size;


    /** The following are all relative to the GRAPH **/
    /// The coordinates of the center of the grid (usually (0,0))
    sf::Vector2f m_center;
    /// How far from the center we go in the x and y directions
    double m_xRange;
    double m_yRange;
    /// Scale on the axes; how much space is between each lines
    double m_xScale;
    double m_yScale;


    /// If true, display all the lines (an actual grid; else, just display tick marks on the axes
    bool m_dispLines;
    /// If true, display number labels on the axes. Otherwise, do not display anything
    bool m_dispNumbers;

    /// The lines (or ticks) that we display
    sf::VertexArray m_lines;

    /// The labels on the axes (what number each line represents)
    std::vector<sf::Text> m_numberLabels;
public:
    /// Default constructor: Will not create a usable grid!
    Grid();

    Grid(sf::RenderWindow* w, sf::Font font, double xLoc, double yLoc, double width, double height,
         double xRange, double yRange, double xScale, double yScale,
         bool dispLines = false, bool dispNumbers = true, double centerX = 0, double centerY = 0);


    /// Generate the lines and labels
    void MakeGrid();
    /// Move the grid to a new position on the window
    void SetPosition(sf::Vector2i pos);
    void SetPosition(int xPos, int yPos);


    /** Functions for modifying the window and changing settings -- Primarily for the user **/

    /// Set the range shown in the window for x and y; 0 or negative means "use current range" for that axis
    void SetRange(double xRange, double yRange);
    /// Set the distance between tick marks/lines in the x and y direction; 0 or negative means "use current scale"
    void SetScale(double xScale, double yScale);
    /// Set the local (graph) coordinates of the center point of the graph -- Does not change position on the window;
    void SetCenter(sf::Vector2f centerCoords);
    /// Toggle between full lines or just tick marks on the axes
    void ToggleLines();
    /// Set whether or not we have full lines directly
    void SetLines(bool hasLines);
    /// Toggle between displaying numbers on the axes or not displaying any numbers
    void ToggleNumbers();
    /// Set whether or not we display numbers directly
    void SetNumbers(bool showNums);


    /** Functions for converting from graph coordinates to window coordinates and vice-versa **/

    /// Given the location on the window, returns the coordinates of the point on the graph
    sf::Vector2f WindowToGraph(sf::Vector2f wLoc);
    sf::Vector2f WindowToGraph(double xPos, double yPos);
    /// Given the location on the graph, returns the coordinates of the pixel on the window
    sf::Vector2f GraphToWindow(sf::Vector2f gLoc);
    sf::Vector2f GraphToWindow(double xPos, double yPos);
    /// Given the location on the graph, returns the coordinates of the pixel on the canvas
    sf::Vector2f GraphToPic(sf::Vector2f gLoc);
    sf::Vector2f GraphToPic(double xPos, double yPos);

    /// Draw everything to the window
    void Draw();
    /// Draw everything except number labels to the window
    void DrawTextless();
};

/// Converts (possibly double) coordinates to strings to be displayed near the axes; also rounds to precision places
std::string DoubleToString(double d, int precision = 1);

#endif // GRID_H
