#include "DoubleGrid.h"
#include <iostream>

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

    m_lGrid.append(sf::Vertex(sf::Vector2f(0, m_height / 2)));
    m_lGrid.append(sf::Vertex(sf::Vector2f(m_width / 2, m_height / 2)));
    if(m_dispGrid) {
        double delta = m_height / 2 / m_yRange * m_yScale;
        for(int iii = 1; iii <= m_yRange / m_yScale; iii ++) {
            m_lGrid.append(sf::Vertex(m_lGrid[0].position + sf::Vector2f(0, iii * delta)));
            m_lGrid.append(sf::Vertex(m_lGrid[1].position + sf::Vector2f(0, iii * delta)));

            m_lGrid.append(sf::Vertex(m_lGrid[0].position + sf::Vector2f(0, iii * -delta)));
            m_lGrid.append(sf::Vertex(m_lGrid[1].position + sf::Vector2f(0, iii * -delta)));
        }
    }

    m_lGrid.append(sf::Vertex(sf::Vector2f(m_width / 4, 0)));
    m_lGrid.append(sf::Vertex(sf::Vector2f(m_width / 4, m_height)));
    std::cout << m_dispGrid << "\n";
    if(m_dispGrid) {
        double delta = m_width / 4 / m_xRange * m_xScale;
        int initialIndex = m_lGrid.getVertexCount() - 2;
        for(int iii = 1; iii <= m_xRange / m_xScale; iii++) {
            m_lGrid.append(sf::Vertex(m_lGrid[initialIndex].position + sf::Vector2f(iii * delta, 0)));
            m_lGrid.append(sf::Vertex(m_lGrid[initialIndex + 1].position + sf::Vector2f(iii * delta, 0)));

            m_lGrid.append(sf::Vertex(m_lGrid[initialIndex].position + sf::Vector2f(iii * -delta, 0)));
            m_lGrid.append(sf::Vertex(m_lGrid[initialIndex + 1].position + sf::Vector2f(iii * -delta, 0)));
        }
    }

    for(int iii = 0; iii < m_lGrid.getVertexCount(); iii++) {
        m_rGrid.append(sf::Vertex(m_lGrid[iii].position + sf::Vector2f(m_width / 2, 0)));

        m_lGrid[iii].color = sf::Color::White;
        m_rGrid[iii].color = sf::Color::White;
    }
}

void DoubleGrid::ToggleGrid() {
    m_dispGrid = !m_dispGrid;
    std::cout << m_dispGrid << "\n";
    MakeGrid();
}

void DoubleGrid::Draw() {
    m_w->draw(m_lGrid);
    m_w->draw(m_rGrid);
    m_w->draw(m_divider);
}
