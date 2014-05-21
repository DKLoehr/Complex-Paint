#ifndef DOUBLEGRID_H
#define DOUBLEGRID_H

#include "Grid.h"

struct DoubleGrid
{
private:
    sf::RenderWindow* w;

public:
    /// Constructs the rectangle only, at heightOffset pixels from the top of the screen. Construct the grids separately.
    DoubleGrid(sf::RenderWindow* w, int heightOffset);

    Grid lGrid;
    Grid rGrid;

    sf::RectangleShape divider;

    /// Toggle between lines or tick marks on the graphs
    void ToggleLines();

    /// Draw everything to the window
    void Draw();
};

#endif // DOUBLEGRID_H
