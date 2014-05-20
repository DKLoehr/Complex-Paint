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

    void MakeGrid();
    void ToggleGrid();

    void Draw();
};

std::string DoubleToString(double d, int precision);

#endif // DOUBLEGRID_H
