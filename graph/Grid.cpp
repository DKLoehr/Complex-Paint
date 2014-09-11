#include "Grid.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

Grid::Grid() {

}

Grid::Grid(sf::RenderWindow* w, sf::Font font, double xLoc, double yLoc, double width, double height,
           double xRange, double yRange, double xScale, double yScale,
           bool dispLines, bool dispNumbers, double centerX, double centerY) :
           m_w(w),
           m_f(font)
{
    m_position = sf::Vector2i(xLoc, yLoc);
    m_size = sf::Vector2i(width, height);

    m_center = sf::Vector2f(centerX, centerY);
    m_xRange = xRange;
    m_yRange = yRange;
    m_xScale = xScale;
    m_yScale = yScale;

    m_dispLines = dispLines;
    m_dispNumbers = dispNumbers;

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
    m_numberLabels[0].setString("(" + DoubleToString(m_center.x) + "," + DoubleToString(m_center.y) + ")");
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

        num.setString(DoubleToString(-iii * m_yScale + m_center.y, 1));
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
        m_numberLabels.push_back(num);

        m_lines.append(sf::Vertex(m_lines[initialIndex].position + sf::Vector2f(iii * -delta, m_size.y / 2 - length)));
        m_lines.append(sf::Vertex(m_lines[initialIndex + 1].position + sf::Vector2f(iii * -delta, -m_size.y / 2 + length)));

        num.setString(DoubleToString(-iii * m_xScale + m_center.x, 1));
        num.setPosition(m_position.x + m_size.x / 2 + 2 - iii * delta, m_position.y + m_size.y / 2);
        m_numberLabels.push_back(num);
    }

    for(int iii = 0; iii < m_lines.getVertexCount(); iii++) {
        m_lines[iii].color = sf::Color::Black;
    }
}

void Grid::SetPosition(sf::Vector2i pos) {
    m_position = pos;
    MakeGrid();
}

void Grid::SetPosition(int xPos, int yPos) {
    m_position = sf::Vector2i(xPos, yPos);
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

void Grid::SetCenter(sf::Vector2f centerCoords) {
    m_center = centerCoords;
    MakeGrid();
}

void Grid::ToggleLines() {
    m_dispLines = !m_dispLines;
    MakeGrid();
}

void Grid::SetLines(bool hasLines) {
    m_dispLines = hasLines;
    MakeGrid();
}

void Grid::ToggleNumbers() {
    m_dispNumbers = !m_dispNumbers;
    MakeGrid();
}

void Grid::SetNumbers(bool showNums) {
    m_dispNumbers = showNums;
    MakeGrid();
}

sf::Vector2f Grid::WindowToGraph(sf::Vector2f wLoc) {
    double x = wLoc.x, y = wLoc.y;
    if(x < m_position.x) x = m_position.x;
    if(x > m_position.x + m_size.x) x = m_position.x + m_size.x;
    if(y < m_position.y) y = m_position.y;
    if(y > m_position.y + m_size.y) x = m_position.y + m_size.y;
    x -= (m_position.x + m_size.x / 2);
    y = -(y - (m_position.y + m_size.y / 2));
    x /= m_size.x / 2 / m_xRange;
    y /= m_size.y / 2 / m_yRange;
    x += m_center.x;
    y += m_center.y;
    return sf::Vector2f(x, y);
}

sf::Vector2f Grid::WindowToGraph(double xPos, double yPos) {
    return WindowToGraph(sf::Vector2f(xPos, yPos));
}

sf::Vector2f Grid::GraphToWindow(sf::Vector2f gLoc) {
    double x = gLoc.x - m_center.x;
    double y = gLoc.y - m_center.y;
    x *= m_size.x / 2 / m_xRange;
    y *= m_size.y / 2 / m_yRange;
    x += m_position.x + m_size.x / 2;
    y += m_position.y + m_size.y / 2;
    y += 2 * (m_size.y / 2 + m_position.y - y);
    if(x < m_position.x) x = m_position.x;
    if(x > m_position.x + m_size.x) x = m_position.x + m_size.x;
    if(y < m_position.y) y = m_position.y;
    if(y > m_position.y + m_size.y) x = m_position.y + m_size.y;
    return sf::Vector2f(x, y);
}

sf::Vector2f Grid::GraphToWindow(double xPos, double yPos) {
    return GraphToWindow(sf::Vector2f(xPos, yPos));
}

sf::Vector2f Grid::GraphToPic(sf::Vector2f gLoc) {
    return GraphToWindow(gLoc) - sf::Vector2f(0, m_position.y);
}

sf::Vector2f Grid::GraphToPic(double xPos, double yPos) {
    return GraphToPic(sf::Vector2f(xPos, yPos));
}

void Grid::Draw() {
    m_w->draw(m_lines);
    if(m_dispNumbers) {
        for(int iii = 0; iii < m_numberLabels.size(); iii++) {
            m_w->draw(m_numberLabels[iii]);
        }
    }
}

void Grid::DrawTextless() {
    m_w->draw(m_lines);
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
