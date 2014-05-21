#include "DoubleGrid.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

DoubleGrid::DoubleGrid(sf::RenderWindow* w, sf::Font font, double xRange, double yRange, double xScale, double yScale,
                       bool dispGrid) :
                       m_f(font)
{
    m_w = w;
    m_width = m_w->getSize().x;
    m_height = m_w->getSize().y;
    m_xRange = xRange;
    m_yRange = yRange;
    m_xScale = xScale;
    m_yScale = yScale;
    m_dispGrid = dispGrid;

    m_lGrid = sf::VertexArray(sf::Lines, 0);
    m_rGrid = sf::VertexArray(sf::Lines, 0);

    m_divider = sf::RectangleShape(sf::Vector2f(4, m_height));
    m_divider.setPosition((m_width - m_divider.getSize().x) / 2, 0);
    m_divider.setFillColor(sf::Color::White);
}

void DoubleGrid::MakeGrid() {
    m_lGrid = sf::VertexArray(sf::Lines, 0);
    m_rGrid = sf::VertexArray(sf::Lines, 0);

    double length = m_width / 4;
    if(!m_dispGrid)
        length *= .02;

    m_lGrid.append(sf::Vertex(sf::Vector2f(0, m_height / 2)));
    m_lGrid.append(sf::Vertex(sf::Vector2f(m_width / 2, m_height / 2)));

    m_yLabels = std::vector<sf::Text>(0);
    m_yLabels.push_back(sf::Text("0", m_f, 10));
    m_yLabels[0].setPosition(m_width / 4 + 2, m_height / 2);
    m_yLabels[0].setColor(sf::Color::White);

    double delta = m_height / 2 / m_yRange * m_yScale;
    for(int iii = 1; iii <= m_yRange / m_yScale; iii ++) {
        m_lGrid.append(sf::Vertex(m_lGrid[0].position + sf::Vector2f(m_width / 4 - length, iii * delta)));
        m_lGrid.append(sf::Vertex(m_lGrid[1].position + sf::Vector2f(-m_width / 4 + length, iii * delta)));

        sf::Text num1 = sf::Text("", m_f, 10);
        num1.setString(DoubleToString(iii * m_yScale, 1));
        num1.setPosition(m_width / 4 + 2, m_height / 2 - iii * delta);
        num1.setColor(sf::Color::White);
        m_yLabels.push_back(num1);

        m_lGrid.append(sf::Vertex(m_lGrid[0].position + sf::Vector2f(m_width / 4 - length, iii * -delta)));
        m_lGrid.append(sf::Vertex(m_lGrid[1].position + sf::Vector2f(-m_width / 4 + length, iii * -delta)));

        sf::Text num2 = sf::Text("", m_f, 10);
        num2.setString(DoubleToString(-iii * m_yScale, 1));
        num2.setPosition(m_width / 4 + 2, m_height / 2 + iii * delta);
        num2.setColor(sf::Color::White);
        m_yLabels.push_back(num2);
    }

    length = m_height / 2;
    if(!m_dispGrid)
        length *= .025;

    m_lGrid.append(sf::Vertex(sf::Vector2f(m_width / 4, 0)));
    m_lGrid.append(sf::Vertex(sf::Vector2f(m_width / 4, m_height)));

    m_xLabels = std::vector<sf::Text>(0);
    m_xLabels.push_back(sf::Text("0", m_f, 10));
    m_xLabels[0].setPosition(m_width / 4 + 2, m_height / 2);
    m_xLabels[0].setColor(sf::Color::White);

    delta = m_width / 4 / m_xRange * m_xScale;
    int initialIndex = m_lGrid.getVertexCount() - 2;
    for(int iii = 1; iii <= m_xRange / m_xScale; iii++) {
        m_lGrid.append(sf::Vertex(m_lGrid[initialIndex].position + sf::Vector2f(iii * delta, m_height / 2 - length)));
        m_lGrid.append(sf::Vertex(m_lGrid[initialIndex + 1].position + sf::Vector2f(iii * delta, -m_height / 2 + length)));

        sf::Text num1 = sf::Text("", m_f, 10);
        num1.setString(DoubleToString(iii * m_xScale, 1));
        num1.setPosition(m_width / 4 + 2 + iii * delta, m_height / 2);
        num1.setColor(sf::Color::White);
        if(num1.getPosition().x < m_width / 2)
            m_xLabels.push_back(num1);

        m_lGrid.append(sf::Vertex(m_lGrid[initialIndex].position + sf::Vector2f(iii * -delta, m_height / 2 - length)));
        m_lGrid.append(sf::Vertex(m_lGrid[initialIndex + 1].position + sf::Vector2f(iii * -delta, -m_height / 2 + length)));

        sf::Text num2 = sf::Text("", m_f, 10);
        num2.setString(DoubleToString(-iii * m_xScale, 1));
        num2.setPosition(m_width / 4 + 2 - iii * delta, m_height / 2);
        num2.setColor(sf::Color::White);
        m_xLabels.push_back(num2);
    }

    for(int iii = 0; iii < m_lGrid.getVertexCount(); iii++) {
        m_rGrid.append(sf::Vertex(m_lGrid[iii].position + sf::Vector2f(m_width / 2, 0)));

        sf::Text xLabel = m_xLabels[iii];
        xLabel.setPosition(xLabel.getPosition().x + m_width / 2, xLabel.getPosition().y);
        m_xLabels.push_back(xLabel);

        sf::Text yLabel = m_yLabels[iii];
        yLabel.setPosition(yLabel.getPosition().x + m_width / 2, yLabel.getPosition().y);
        m_yLabels.push_back(yLabel);

        m_lGrid[iii].color = sf::Color::White;
        m_rGrid[iii].color = sf::Color::White;
    }
}

void DoubleGrid::ToggleGrid() {
    m_dispGrid = !m_dispGrid;
    MakeGrid();
}

void DoubleGrid::SetRange(double xRange, double yRange) {
    if(xRange > 0)
        m_xRange = xRange;
    if(yRange > 0)
        m_yRange = yRange;
    MakeGrid();
}

void DoubleGrid::SetScale(double xScale, double yScale) {
    if(xScale > 0)
        m_xScale = xScale;
    if(yScale > 0)
        m_yScale = yScale;
    MakeGrid();
}

void DoubleGrid::Draw() {
    m_w->draw(m_lGrid);
    m_w->draw(m_rGrid);
    m_w->draw(m_divider);
    for(int iii = 0; iii < m_yLabels.size(); iii++) {
        m_w->draw(m_yLabels[iii]);
    }
    for(int iii = 0; iii < m_xLabels.size(); iii++) {
        m_w->draw(m_xLabels[iii]);
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
