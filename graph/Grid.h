#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>

class Grid
{
private:
    /// The window to which we draw
    sf::RenderWindow* m_w;
    /// The font we use for the axis labels
    sf::Font m_f;


    /* The following are all relative to the WINDOW, not the graph */
    /// The coordinates of the top-left corner of the grid on the window
    sf::Vector2f m_position;
    /// Height and width of the grid
    sf::Vector2f m_size;


    /* The following are all relative to the GRAPH */
    /// The coordinates of the center of the grid (usually (0,0))
    sf::Vector2f m_center;
    /// How far from the center we go in the x and y directions
    double m_xRange;
    double m_yRange;
    /// Scale on the axes; how much space is between each lines
    double m_xScale;
    double m_yScale;


    /// Display all the lines (an actual grid) or just tick marks on the axes
    bool m_dispLines;

    /// The lines (or ticks) that we display
    sf::VertexArray m_lines;

    /// The labels on the axes (what number each line represents)
    std::vector<sf::Text> m_numberLabels;
public:
    /// Default constructor: Will not create a usable grid!
    Grid();

    Grid(sf::RenderWindow* w, sf::Font font, double xLoc, double yLoc, double width, double height,
         double xRange, double yRange, double xScale, double yScale,
         bool dispLines = false, double centerX = 0, double centerY = 0);

    /// Generate the lines and labels
    void MakeGrid();
    /// Toggle between full lines or just tick marks on the axes
    void ToggleLines();

    /// Set the range shown in the window for x and y; 0 or negative means "use current range" for that axis
    void SetRange(double xRange, double yRange);
    /// Set the distance between tick marks/lines in the x and y direction; 0 or negative means "use current scale"
    void SetScale(double xScale, double yScale);

    /// Draw everything to the window
    void Draw();
};

/// Converts (possibly double) coordinates to strings to be displayed near the axes; also rounds to precision places
std::string DoubleToString(double d, int precision = 1);

#endif // GRID_H
