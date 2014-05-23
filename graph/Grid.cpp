#include "Grid.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

Grid::Grid() {

}

Grid::Grid(sf::RenderWindow* w, sf::Font font, double xLoc, double yLoc, double width, double height,
           double xRange, double yRange, double xScale, double yScale,
           bool dispLines, double centerX, double centerY) :
           m_w(w),
           m_f(font)
{
    m_position = sf::Vector2f(xLoc, yLoc);
    m_size = sf::Vector2f(width, height);

    m_center = sf::Vector2f(centerX, centerY);
    m_xRange = xRange;
    m_yRange = yRange;
    m_xScale = xScale;
    m_yScale = yScale;

    m_dispLines = dispLines;

    MakeGrid();
}

void Grid::MakeGrid() {
    // Clear out any existing lines and number labels
    m_lines = sf::VertexArray(sf::Lines, 0);
    m_numberLabels = std::vector<sf::Text>(0);

    double length = m_size.x / 2; // Length of the line segments from the axis to the edge
    if(!m_dispLines)
        length *= .02; // Make them much shorter if we're only displaying tick marks

    // Add our x-axis
    m_lines.append(sf::Vertex(sf::Vector2f(m_position.x, m_position.y + m_size.y / 2)));
    m_lines.append(sf::Vertex(sf::Vector2f(m_position.x + m_size.x, m_position.y + m_size.y / 2)));

    // Create our label for the "origin"
    m_numberLabels.push_back(sf::Text("", m_f, 10));
    m_numberLabels[0].setString(DoubleToString(m_center.x));
    m_numberLabels[0].setPosition(m_position.x + m_size.x / 2 + 2, m_position.y + m_size.y / 2);
    m_numberLabels[0].setColor(sf::Color::Black);

    double delta = m_size.y / 2 / m_yRange * m_yScale;
    for(int iii = 1; iii <= m_yRange / m_yScale; iii++) {
        m_lines.append(sf::Vertex(m_lines[0].position + sf::Vector2f(m_size.x / 2 - length, iii * delta)));
        m_lines.append(sf::Vertex(m_lines[1].position + sf::Vector2f(-m_size.x / 2 + length, iii * delta)));

        sf::Text num = sf::Text("", m_f, 10);
        num.setString(DoubleToString(iii * m_yScale + m_center.y, 1));
        num.setPosition(m_position.x + m_size.x / 2 + 2, m_position.y + m_size.y / 2 - iii * delta);
        num.setColor(sf::Color::Black);
        m_numberLabels.push_back(num);

        m_lines.append(sf::Vertex(m_lines[0].position + sf::Vector2f(m_size.x / 2 - length, iii * -delta)));
        m_lines.append(sf::Vertex(m_lines[1].position + sf::Vector2f(-m_size.x / 2 + length, iii * -delta)));

        num.setString(DoubleToString(-iii * m_yScale, 1));
        num.setPosition(m_position.x + m_size.x / 2 + 2, m_position.y + m_size.y / 2 + iii * delta);
        m_numberLabels.push_back(num);
    }

    length = m_size.y / 2;
    if(!m_dispLines)
        length *= .025;

    // Add our y-axis
    m_lines.append(sf::Vertex(sf::Vector2f(m_position.x + m_size.x / 2, m_position.y)));
    m_lines.append(sf::Vertex(sf::Vector2f(m_position.x + m_size.x / 2, m_position.y + m_size.y)));

    delta = m_size.x / 2 / m_xRange * m_xScale;
    int initialIndex = m_lines.getVertexCount() - 2;
    for(int iii = 1; iii <= m_xRange / m_xScale; iii++) {
        m_lines.append(sf::Vertex(m_lines[initialIndex].position + sf::Vector2f(iii * delta, m_size.y / 2 - length)));
        m_lines.append(sf::Vertex(m_lines[initialIndex + 1].position + sf::Vector2f(iii * delta, -m_size.y / 2 + length)));

        sf::Text num = sf::Text("", m_f, 10);
        num.setString(DoubleToString(iii * m_xScale + m_center.x, 1));
        num.setPosition(m_position.x + m_size.x / 2 + 2 + iii * delta, m_position.y + m_size.y / 2);
        num.setColor(sf::Color::Black);
        if(num.getPosition().x < m_size.x + m_position.x) // Don't display numbers outside of our boundary
            m_numberLabels.push_back(num);

        m_lines.append(sf::Vertex(m_lines[initialIndex].position + sf::Vector2f(iii * -delta, m_size.y / 2 - length)));
        m_lines.append(sf::Vertex(m_lines[initialIndex + 1].position + sf::Vector2f(iii * -delta, -m_size.y / 2 + length)));

        num.setString(DoubleToString(-iii * m_xScale, 1));
        num.setPosition(m_position.x + m_size.x / 2 + 2 - iii * delta, m_position.y + m_size.y / 2);
        m_numberLabels.push_back(num);
    }

    for(int iii = 0; iii < m_lines.getVertexCount(); iii++) {
        m_lines[iii].color = sf::Color::Black;
    }
}

void Grid::ToggleLines() {
    m_dispLines = !m_dispLines;
    MakeGrid();
}

void Grid::SetRange(double xRange, double yRange) {
    if(xRange > 0)
        m_xRange = xRange;
    if(yRange > 0)
        m_yRange = yRange;
    MakeGrid();
}

void Grid::SetScale(double xScale, double yScale) {
    if(xScale > 0)
        m_xScale = xScale;
    if(yScale > 0)
        m_yScale = yScale;
    MakeGrid();
}

void Grid::Draw() {
    m_w->draw(m_lines);
    for(int iii = 0; iii < m_numberLabels.size(); iii++) {
        m_w->draw(m_numberLabels[iii]);
    }
}

std::string DoubleToString(double d, int precision) {
    if(d > 0)
        d = ((int)(d * pow(10, precision) + .5)) / (double)pow(10, precision);
    else
        d = ((int)(d * pow(10, precision) - .5)) / (double)pow(10, precision);
    std::ostringstream ss;
    ss << d;
    return ss.str();
}
