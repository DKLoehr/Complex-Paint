#ifndef DOUBLEGRID_H
#define DOUBLEGRID_H

#include "Grid.h"

/**
* This is a simple structure meant to hold the background onto which we draw;
* specifically, the input (left) and output (right) grids, as well as a thin rectangle
* between them to serve as a separator.
**/

struct DoubleGrid
{
private:
    sf::RenderWindow* w;

public:
    /// Constructs the rectangle only, at heightOffset pixels from the top of the screen. Construct the grids separately.
    DoubleGrid(sf::RenderWindow* w, sf::Font f, int heightOffset);

    Grid lGrid;
    Grid rGrid;

    sf::RectangleShape horDivider;
    sf::RectangleShape vertDivider;

    /// Change the location of everything on the screen in the Y direction
    void SetYPosition(int yPos);

    /// Draw everything to the window
    void Draw();
};

#endif // DOUBLEGRID_H
