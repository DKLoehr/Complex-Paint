#ifndef DOUBLEGRID_H
#define DOUBLEGRID_H

#include <SFML/Graphics.hpp>

class DoubleGrid
{
private:
    sf::RenderWindow* m_w;
    sf::Font m_f;

    int m_width;
    int m_height;

    double m_xRange;
    double m_yRange;
    double m_xScale;
    double m_yScale;

    bool m_dispGrid;

    sf::VertexArray m_lGrid;
    sf::VertexArray m_rGrid;

    std::vector<sf::Text> m_xLabels;
    std::vector<sf::Text> m_yLabels;

    sf::RectangleShape m_divider;
public:
    DoubleGrid(sf::RenderWindow* w, sf::Font font, double xRange, double yRange, double xScale, double yScale,
               bool dispGrid);

    // Generate the grids that we display on
    void MakeGrid();
    // Toggle between full lines or just tick marks on the axes
    void ToggleGrid();

    // Set the range shown in the window for x and y; 0 or negative means "use current range"
    void SetRange(double xRange, double yRange);
    // Set the distance between tick marks/lines in the x and y direction; 0 or negative means "use current scale"
    void SetScale(double xScale, double yScale);

    // Draw everything to the window
    void Draw();
};

// Converts (possibly double) coordinates to strings to be displayed near the axes; also rounds to precision places
std::string DoubleToString(double d, int precision);

#endif // DOUBLEGRID_H
