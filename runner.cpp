#include "runner.h"

Runner::Runner(sf::RenderWindow* w, sf::Font* font, sf::RenderTexture* p) :
    window(w),
    inFont(font),
    pic(p)
{
    Init();
}


void Runner::Init() {
    isIterating = false;
    isDrawing = false;
    leftToRight = true;
    mode = single;

    pic->clear(sf::Color::White);
    graphs.setPosition(0, HEIGHT_OFFSET);
    graphs.setTexture(pic->getTexture());

    fct = new parser::Tree("Z");

    grid = DoubleGrid(window, *inFont, HEIGHT_OFFSET);
    grid.Draw();

    loc = sf::CircleShape(1, 30);
    loc.setFillColor(sf::Color::Black);

    lastPoint = std::complex<double>(0, 0);
    graphCoords = sf::Vector2f(0, 0);

    lTitle = sf::Text("Left Graph", *inFont, 15);
    lTitle.setPosition(window->getSize().x * 13 / 16 - 50, 5);
    lTitle.setColor(sf::Color::Black);

    rTitle = sf::Text("Right Graph", *inFont, 15);
    rTitle.setPosition(window->getSize().x * 15 / 16 - 55, 5);
    rTitle.setColor(sf::Color::Black);

    elements = std::vector<GUI*>(0);

    /** Equation-related elements **/
    equation = InputBox(window, inFont, 5, 5, 350, 15); // 26
    // Equation must be at the end of the vector so it precedes variable boxes, so we'll push it back later

    okEquation = Button(window, inFont, equation.GetPosition().x + equation.GetSize().x + 7, equation.GetPosition().y,
                        108, 15, "Save Changes"); // 0
    elements.push_back(&okEquation);


    /** Preset-related elements **/
    presetLin = Button(window, inFont, window->getSize().x * .35, 30, 56, 15, "Linear");  // 1 -- Az + B
    elements.push_back(&presetLin);

    presetPol = Button(window, inFont, presetLin.GetPosition().x + presetLin.GetSize().x + window->getSize().x/50, 30,
                       110, 15, "Polar Linear");        // 2 -- Polar equation
    elements.push_back(&presetPol);

    presetQuad = Button(window, inFont, presetPol.GetPosition().x + presetPol.GetSize().x + window->getSize().x/50, 30,
                        81, 15, "Quadratic");   // 3 -- z*2 + c
    elements.push_back(&presetQuad);

    presetInv = Button(window, inFont, presetQuad.GetPosition().x + presetQuad.GetSize().x + window->getSize().x/50, 30,
                       109, 15, "Inverse Quad"); // 4 -- sqrt(z*2 - c)
    elements.push_back(&presetInv);


    /** Drawing mode-related elements **/
    modeSingle = Button(window, inFont, window->getSize().x / 2 - 135, 93, 45, 15, "Point");  // 5
    modeSingle.SetOutlineColor(sf::Color::Green);
    elements.push_back(&modeSingle);

    modeIterate = Button(window, inFont, window->getSize().x / 2 - 5, modeSingle.GetPosition().y,
                                65, 15, "Iterate"); // 6
    elements.push_back(&modeIterate);

    modeGrid = Button(window, inFont, window->getSize().x / 2 + 135, modeSingle.GetPosition().y,
                                40, 15, "Grid"); // 7
    elements.push_back(&modeGrid);

    /// Clear graph button
    clearGraphs = Button(window, inFont, window->getSize().x / 2 - 23, 175, 46, 15, "Clear"); // 8
    elements.push_back(&clearGraphs);

    /** Graph-related elements **/
    /// Button to copy changes from the right side to the left
    mirrorL = Button(window, inFont, lTitle.getPosition().x + ((std::string)lTitle.getString()).length() * 10 + 5,
                     lTitle.getPosition().y, 15, 15, "<"); // 9
    elements.push_back(&mirrorL);

    mirrorR = Button(window, inFont, mirrorL.GetPosition().x + 20, mirrorL.GetPosition().y, 15, 15, ">"); // 10
    elements.push_back(&mirrorR);

    /// Left-side elements
    xRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 30, 55, 15, "x Range"); // 11
    xRangeL.SetText("2");
    elements.push_back(&xRangeL);

    yRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 50, 55, 15, "y Range"); // 12
    yRangeL.SetText("2");
    elements.push_back(&yRangeL);

    xScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 70, 55, 15, "x Scale"); // 13
    xScaleL.SetText("1");
    elements.push_back(&xScaleL);

    yScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 90, 55, 15, "y Scale"); // 14
    yScaleL.SetText("1");
    elements.push_back(&yScaleL);

    centerL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 110, 55, 15, "Center "); // 15
    centerL.SetText("(0,0)");
    elements.push_back(&centerL);

    numbersL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 130, "Numbers", true); // 16
    elements.push_back(&numbersL);

    linesL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 150, "Lines", false);    // 17
    elements.push_back(&linesL);

    /// Right-side elements
    xRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 30, 55, 15, "x Range"); // 18
    xRangeR.SetText("2");
    elements.push_back(&xRangeR);

    yRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 50, 55, 15, "y Range"); // 19
    yRangeR.SetText("2");
    elements.push_back(&yRangeR);

    xScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 70, 55, 15, "x Scale"); // 20
    xScaleR.SetText("1");
    elements.push_back(&xScaleR);

    yScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 90, 55, 15, "y Scale"); // 21
    yScaleR.SetText("1");
    elements.push_back(&yScaleR);

    centerR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 110, 55, 15, "Center "); // 22
    centerR.SetText("(0,0)");
    elements.push_back(&centerR);

    numbersR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 130, "Numbers", true); // 23
    elements.push_back(&numbersR);

    linesR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 150, "Lines", false); // 24
    elements.push_back(&linesR);

    /// General graph-related elements
    okGraph = Button(window, inFont, window->getSize().x * 7 / 8 - 54, 175, // 25
                                108, 15, "Save Changes");
    elements.push_back(&okGraph);

    elements.push_back(&equation); // Finally push back equation so that it's at the end of the vector


    /** End GUI element creation **/

    for(int iii = 0; iii < elements.size(); iii++) {
        elements[iii]->SetActive(false);
    }
    activeBox = 26; // Start out highlighting the equation entering box
    elements[activeBox]->SetActive(true);
    UpdateGraphs();
}

void Runner::HandleEvents() {
    sf::Event event;
    while(window->pollEvent(event)) {
        if(event.type == sf::Event::Closed ||
           (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
        {
            window->close();
        } else if(event.type == sf::Event::TextEntered) {
            elements[activeBox]->OnTextEntered(event.text.unicode);
            if(event.text.unicode != 9 && activeBox >= 11 && activeBox != 25) { // One of the inputBoxes, wasn't tab
                if(activeBox >= 26) // Equation or variables
                    okEquation.SetOutlineColor(sf::Color::Red);
                else // Graph box changed
                    okGraph.SetOutlineColor(sf::Color::Red);
            }
        } else if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
                  (event.type == sf::Event::MouseButtonPressed && event.mouseButton.y < 200)) {
            ActivateButtons(event);
        } else if(event.type == sf::Event::MouseMoved) {
            if(event.mouseMove.y < 200) // Upper part of the screen
                SetActiveElement(event.mouseMove.x, event.mouseMove.y);
            else { // In one of the graphs
                loc.setFillColor(sf::Color::Black);
                if(event.mouseMove.x < window->getSize().x / 2) { // Left Graph
                    sf::Vector2f graphLoc(grid.lGrid.WindowToGraph(event.mouseMove.x, event.mouseMove.y));
                    fct->setVar("Z", cx(graphLoc.x, graphLoc.y));
                    cx newLoc(fct->eval());
                    loc.setPosition(grid.rGrid.GraphToWindow(newLoc.real(), newLoc.imag()));
                } else { // Right Graph
                    sf::Vector2f graphLoc(grid.rGrid.WindowToGraph(event.mouseMove.x, event.mouseMove.y));
                    fct->setVar("Z", cx(graphLoc.x, graphLoc.y));
                    cx newLoc(fct->eval());
                    loc.setPosition(grid.lGrid.GraphToWindow(newLoc.real(), newLoc.imag()));
                }
            }
        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
            StepActiveElement(!(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                                sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)));
        } else if(event.type == sf::Event::MouseButtonPressed) {
            if(event.mouseButton.y < 200) { // Above the graphs
                if(activeBox == 15)
                    UpdateGraphs();
                else if(activeBox == 17)
                    UpdateEquation();
                else
                    elements[activeBox]->OnClick(event.mouseButton.x, event.mouseButton.y);
            } else { // In one of the graphs
                leftToRight = (event.mouseButton.x < window->getSize().x / 2); // True iff the click was in the left graph
                if((int)mode <= 1 ) { // Single or Iterate
                    if(leftToRight) {
                        graphCoords = grid.lGrid.WindowToGraph(event.mouseButton.x, event.mouseButton.y);
                    } else {
                        graphCoords = grid.rGrid.WindowToGraph(event.mouseButton.x, event.mouseButton.y);
                    }
                    Iterate(!isIterating, new cx(graphCoords.x, graphCoords.y)); // Stop iterating if we were iterating, or start if we weren't
                }
                else { // Grid mode
                    DrawShape(true);
                }
            }
        }
    }
    DrawShape(false);
    Iterate(isIterating);
}

void Runner::Iterate(bool keepIterating, cx* newPos) {
    isIterating = keepIterating;
    if(keepIterating) {
        double circRad = 1;
        int numIterations = 1;
        if(mode == single) {
            isIterating = false;
            circRad = 1.5;
        }
        else if (mode == iterative) {
            numIterations = 90;
            circRad = .75;
        } else if(mode == (drawMode)2) { // Grid mode
            isIterating = false;
            circRad = 1.0;
        }

        graphCoords = sf::Vector2f(lastPoint.real(), lastPoint.imag());
        sf::CircleShape lastLoc = sf::CircleShape(circRad, 30);
        lastLoc.setFillColor(sf::Color::Black);
        if(lastGraph) { // lastLoc is on the left graph
            lastLoc.setPosition(grid.lGrid.GraphToPic(graphCoords) - sf::Vector2f(circRad, circRad));
            pic->draw(lastLoc);
        }
        else {          // lastLoc is on the right graph
            lastLoc.setPosition(grid.rGrid.GraphToPic(graphCoords) - sf::Vector2f(circRad, circRad));
            pic->draw(lastLoc);
        }
        if(newPos != NULL)
            lastPoint = *newPos;
        delete newPos;

        for(int iii = 0; iii < numIterations; iii++) { // Iterate 30 points at once, or just one
            fct->setVar("Z", lastPoint); // Feed our current location into parser as the variable Z
            try {
                lastPoint = fct->eval(); // Don't change position, so we can make it black first
            }
            catch (std::invalid_argument) { // Should mean we've "reached infinity", so we can stop
                isIterating = false;
                return;
            }

            graphCoords = sf::Vector2f(lastPoint.real(), lastPoint.imag());
            sf::CircleShape newLoc = sf::CircleShape(circRad, 30);
            if(iii == numIterations - 1)
                newLoc.setFillColor(sf::Color::Red);
            else
                newLoc.setFillColor(sf::Color::Black);
            if(leftToRight) { // Draw the output on the right graph
                newLoc.setPosition(grid.rGrid.GraphToPic(graphCoords) - sf::Vector2f(circRad, circRad));
                pic->draw(newLoc);
                lastGraph = false; // Mark as having been drawn on the right graph
            }
            else { // Draw the output on the left graph
                newLoc.setPosition(grid.lGrid.GraphToPic(graphCoords) - sf::Vector2f(circRad, circRad));
                pic->draw(newLoc);
                lastGraph = true; // Mark as having been draw on the left graph
            }
            leftToRight = !leftToRight;
            }
        }
    }


void Runner::SetActiveElement(double x, double y) {
    for(int iii = 0; iii < elements.size(); iii++) {
        if(elements[iii]->OnClick(x, y)) {              // True iff x and y are in that element's box; will toggle checkboxes, so must toggle back later
            if(activeBox != iii) {                      // If the active box has changed
                elements[activeBox]->SetActive(false);  // Deactivate the previously-active box
                activeBox = iii;                        // Update which box is active
                elements[activeBox]->SetActive(true);   // Activate the newly-active box
            }
            elements[iii]->OnClick(x, y);           // For checkboxes; toggle them back
            break;                                      // Figured out which box is active, so we can stop looking now
        }
        elements[iii]->OnClick(x, y);                   // For checkboxes; toggle them back
    }
}

void Runner::StepActiveElement(bool increment) {
    elements[activeBox]->SetActive(false);          // Deactivate previously-active box
    if(increment)                                   // If we're going forward (down or to the right)
        activeBox = ++activeBox % elements.size();  // Increment by one modularly
    else
        activeBox = (activeBox + elements.size() - 1) % elements.size(); // Decrement by one modularly
    elements[activeBox]->SetActive(true);           // Activate the newly-active box
}

void Runner::UpdateGraphs() {
    double x, y;                            // Values which will be the x and y ranges, scales, etc.
    parser::Tree expr;                      // Tree to evaluate the expressions in the inputBoxes

    expr = parser::Tree(xRangeL.GetText()); // Set the tree to the first inputBox
    x = expr.eval().real();                 // Store the value in that box in x
    expr = parser::Tree(yRangeL.GetText()); // Repeat for y
    y = expr.eval().real();
    grid.lGrid.SetRange(x, y);              // Set the left grid's range to be that specified in the left range boxes

    expr = parser::Tree(xScaleL.GetText());
    x = expr.eval().real();
    expr = parser::Tree(yScaleL.GetText());
    y = expr.eval().real();
    grid.lGrid.SetScale(x, y);              // Set the left grid's scale to be that specified in the left scale boxes

    if(centerL.GetText() != "") {                           // If there's no entry, leave the center as it is
        expr = parser::Tree(centerL.GetOrderedPairElement(true));
        x = expr.eval().real();
        expr = parser::Tree(centerL.GetOrderedPairElement(false));
        y = expr.eval().real();
        grid.lGrid.SetCenter(sf::Vector2f(x, y));  // Set the left grid's center to be that specified in the left center box
    }

    grid.lGrid.SetNumbers(numbersL.GetText() == "x");       // Set numbers on the axes to be on or off as specified by the left "numbers" checkbox
    grid.lGrid.SetLines(linesL.GetText() == "x");           // Set tick marks or whole grid lines to be on or off as specified by the left "lines" checkbox

    expr = parser::Tree(xRangeR.GetText()); // Repeat for the right grid
    x = expr.eval().real();
    expr = parser::Tree(yRangeR.GetText());
    y = expr.eval().real();
    grid.rGrid.SetRange(x, y);

    expr = parser::Tree(xScaleR.GetText());
    x = expr.eval().real();
    expr = parser::Tree(yScaleR.GetText());
    y = expr.eval().real();
    grid.rGrid.SetScale(x, y);

    if(centerR.GetText() != "") {
        expr = parser::Tree(centerR.GetOrderedPairElement(true));
        x = expr.eval().real();
        expr = parser::Tree(centerR.GetOrderedPairElement(false));
        y = expr.eval().real();
        grid.rGrid.SetCenter(sf::Vector2f(x, y));  // Set the left grid's center to be that specified in the left center box
    }

    grid.rGrid.SetNumbers(numbersR.GetText() == "x");
    grid.rGrid.SetLines(linesR.GetText() == "x");

    if(okGraph.GetOutlineColor() != sf::Color::Black) {     // If anything (besides numbers or lines) was changed
        okGraph.SetOutlineColor(sf::Color::Black);          // Mark "Save changes" button as up-to-date
        ClearPic();                                         // Get rid of old points which are no longer accurate
    }
}

void Runner::UpdateEquation() {
    Iterate(false); // Stop iterating

    /// Update the tree with the new equation
    delete fct;                             // Get rid of our old, outdated equation tree
    std::string func = equation.GetText();  // Get the string with the new equation
    for(int iii = 0; iii < func.length(); iii ++) {
        if(func[iii] == 'z')                // Equate user input of 'z' with 'Z'
            func.replace(iii, 1, "Z");      // Replace instances of 'z' with 'Z', since parser requires uppercase parameters
    }
    fct = new parser::Tree(func);           // Create a new tree with our modified equation

    /** Replace the old parameter inputBoxes with the new ones **/

    /// Update fct with new values and remove old inputBoxes
    while(elements.size() > elementsSize) { // While we have more elements than we started with (all extra ones are parameter boxes)
        fct->setVar(elements[elements.size() - 1]->GetCap().substr(0, 1),   // Figure out which param the bottom inputBox corresponds to...
                    elements[elements.size() - 1]->GetText());              // And set that parameter with the string in that box
        delete elements[elements.size() - 1];   // Remove the last inputBox from memory to prevent memory leaks
        elements.pop_back();                    // Remove the last inputBox from the array
    }

    /// Create the new inputBoxes for each parameter that appears in equation
    // Loop through all possible parameter values: 'A' through 'Z'
    for(int iii = 'A'; iii < 'Z' && elements.size() < elementsSize + 8; iii++) { // Max 8 vars for now due to screen size constraints
        if(func.find(iii) != std::string::npos) { // If this parameter appears in the equation...
            elements.push_back(new InputBox(window, inFont, 3, (elements.size() - elementsSize + 1.25) * 20, // Add a new inputBox
                                        150, 15, std::string(1, (char)iii) + " "));                          // for this parameer
            elements[elements.size() - 1]->SetActive(false);                                    // Don't have the box start active
            elements[elements.size() - 1]->SetText(fct->getVarFct(std::string(1, (char)iii)));  // Set the parameter to have the last value
                                                                                            // entered (0 if this is the first time it's used)
        }
    }
    okEquation.SetOutlineColor(sf::Color::Black); // Mark the "Save Changes" button as being up-to-date
}

void Runner::ActivateButtons(sf::Event event) {
    switch(activeBox) {
    case 0: // Save Changes for equation
        UpdateEquation();
        break;
    case 1: // Linear Preset
        equation.SetText("A*z + B");
        UpdateEquation();
        break;
    case 2: // Polar Preset
        equation.SetText("(R*cos(2*pi*T)+R*sin(2*pi*T)*i)*z+B");
        UpdateEquation();
        break;
    case 3: // Quadratic Preset
        equation.SetText("z^2 + C");
        UpdateEquation();
        break;
    case 4: // Inverse Quadratic Preset
        equation.SetText("rpm() * sqrt(z - C)");
        UpdateEquation();
        break;
    case 5: // Single point mode
        Iterate(false);
        mode = single;
        modeSingle.SetOutlineColor(sf::Color::Green);
        modeIterate.SetOutlineColor(sf::Color::Black);
        modeGrid.SetOutlineColor(sf::Color::Black);
        break;
    case 6: // Iterate mode
        Iterate(false);
        mode = iterative;
        modeIterate.SetOutlineColor(sf::Color::Green);
        modeSingle.SetOutlineColor(sf::Color::Black);
        modeGrid.SetOutlineColor(sf::Color::Black);
        break;
    case 7: // Grid mode
        Iterate(false);
        mode = (drawMode)2; // Grid mode
        modeGrid.SetOutlineColor(sf::Color::Green);
        modeSingle.SetOutlineColor(sf::Color::Black);
        modeIterate.SetOutlineColor(sf::Color::Black);
        break;
    case 8: // Clear
        ClearPic();
        break;
    case 9: // Mirror R->L
        okGraph.SetOutlineColor(sf::Color::Red);
        xRangeL.SetText(xRangeR.GetText());
        yRangeL.SetText(yRangeR.GetText());
        xScaleL.SetText(xScaleR.GetText());
        yScaleL.SetText(yScaleR.GetText());
        centerL.SetText(centerR.GetText());
        if(numbersL.GetText() != numbersR.GetText()) numbersL.Toggle();
        if(linesL.GetText() != linesR.GetText()) linesL.Toggle();
        break;
    case 10: // Mirror L->R
        okGraph.SetOutlineColor(sf::Color::Red);
        xRangeR.SetText(xRangeL.GetText());
        yRangeR.SetText(yRangeL.GetText());
        xScaleR.SetText(xScaleL.GetText());
        yScaleR.SetText(yScaleL.GetText());
        centerR.SetText(centerL.GetText());
        if(numbersR.GetText() != numbersL.GetText()) numbersR.Toggle();
        if(linesR.GetText() != linesL.GetText()) linesR.Toggle();
        break;
    case 14: // One of the checkboxes
    case 17:
    case 23:
    case 24:
        okGraph.SetOutlineColor(sf::Color::Red);
        if(event.type == sf::Event::MouseButtonPressed)
            elements[activeBox]->OnClick(event.mouseButton.x, event.mouseButton.y);
        else
            elements[activeBox]->OnEnter();
        break;
    case 25: // Save Changes for graphs
        UpdateGraphs();
        break;
    default:
        if(event.type == sf::Event::MouseButtonPressed)
            elements[activeBox]->OnClick(event.mouseButton.x, event.mouseButton.y);
        else
            elements[activeBox]->OnEnter();
    }
}

void Runner::DrawShape(bool toggleDrawing) {
    static sf::Vector2i position; // Starting position of our shape
    sf::Vector2i shapeSize;
    shape = sf::VertexArray(sf::Lines, 0); // VertexArray to hold rectangular-type shapes

    if(!isDrawing && toggleDrawing)
        position = sf::Mouse::getPosition(*window); // Get our initial position from where the mouse cursor was when it was clicked
    if(sf::Mouse::getPosition(*window).y < 200 || // In the upper part of the window, out of the graphs
       position.x < window->getSize().x / 2 && sf::Mouse::getPosition(*window).x > window->getSize().x / 2 || // In a different grid from the
       position.x > window->getSize().x / 2 && sf::Mouse::getPosition(*window).x < window->getSize().x / 2)   // one we clicked in originally
        return; // Our grid would cross a boundary, so don't draw a new one

    shapeSize = sf::Mouse::getPosition(*window) - position;
    int xLines = shapeSize.y / GRID_LINES_DELTA,    // Number of horizontal lines. Note: integer division to remove extra pixels
        yLines = shapeSize.x / GRID_LINES_DELTA;    // Number of vertical lines; also integer division
    shapeSize.y = xLines * GRID_LINES_DELTA;        // Remove extra pixels that aren't enough to constitute a line
    shapeSize.x = yLines * GRID_LINES_DELTA;
    xLines = abs(xLines) + 1;   // Want a positive number of lines so that for loops are easier,
    yLines = abs(yLines) + 1;   // Increment by one because we've only counted one of the extreme lines (far left/right or top/bottom)

    if(mode == (drawMode) 2) {  // Grid mode
        /// Create the grid
        // Add our horizontal lines from top to bottom
        double delta = (double)shapeSize.y / xLines; // Distance between lines in pixels
        for(int iii = 0; iii <= xLines; iii++) {
            shape.append(sf::Vertex(sf::Vector2f(position.x, position.y + iii * delta)));
            shape.append(sf::Vertex(sf::Vector2f(position.x + shapeSize.x, position.y + iii * delta)));
        }

        int yIndex = shape.getVertexCount(); // Index of the first point of the vertical lines

        delta = (double)shapeSize.x / yLines; // Distance between lines in pixels
        for(int iii = 0; iii <= yLines; iii++) {
            shape.append(sf::Vertex(sf::Vector2f(position.x + iii * delta, position.y)));
            shape.append(sf::Vertex(sf::Vector2f(position.x + iii * delta, position.y + shapeSize.y)));
        }

        for(int iii = 0; iii < shape.getVertexCount(); iii++) { // Make every vertex black for drawing
            shape[iii].color = sf::Color::Black;
        }

        /// Draw the grid to the screen
        bool leftToRightLocal = leftToRight; // Local copy of leftToRight so that we can reset it when it is flipped by Iterate
        if(isDrawing && toggleDrawing) { // We were actively drawing, but are about to stop0
            // Convert the grid's coordinates to be those of the graph
            if(position.x < window->getSize().x / 2) {    // In the left grid
                for(int iii = 0; iii < shape.getVertexCount(); iii++) {
                    shape[iii].position = grid.lGrid.WindowToGraph(shape[iii].position);
                }
            }
            else {  // In the right grid
                for(int iii = 0; iii < shape.getVertexCount(); iii++) {
                    shape[iii].position = grid.rGrid.WindowToGraph(shape[iii].position);
                }
            }

            // Iterate a bunch of points on the grid and draw them to the other side
            int vertCount = shape.getVertexCount(); // Store as its own variable since it will be used a lot
            // Iterate vertically: Start at top, iterate down, move over & repeat
            delta = ((double)(shape[vertCount - 1].position.y - shape[0].position.y) / xLines) / GRID_POINT_DELTA;
            for(int iii = 0; iii <= yLines; iii++) {
                for(int jjj = 0; jjj <= GRID_POINT_DELTA * xLines; jjj++) {
                    leftToRight = leftToRightLocal;
                    Iterate(true, new cx(shape[yIndex + iii * 2].position.x,
                                         shape[yIndex + iii * 2].position.y + jjj * delta));
                }
            }
            delta = ((double)(shape[vertCount - 1].position.x - shape[0].position.x) / yLines) / GRID_POINT_DELTA;
            for(int iii = 0; iii <= xLines; iii++) {
                for(int jjj = 0; jjj <= GRID_POINT_DELTA * yLines; jjj++) {
                    leftToRight = leftToRightLocal;
                    Iterate(true, new cx(shape[iii * 2].position.x + jjj * delta,
                                        shape[iii * 2].position.y));
                }
            }


            // Convert shape's vertices to pic coordinates so we can draw the original version
            if(position.x < window->getSize().x / 2) {    // In the left grid
                for(int iii = 0; iii < shape.getVertexCount(); iii++) {
                    shape[iii].position = grid.lGrid.GraphToPic(shape[iii].position);
                }
            }
            else {  // In the right grid
                for(int iii = 0; iii < shape.getVertexCount(); iii++) {
                    shape[iii].position = grid.rGrid.GraphToPic(shape[iii].position);
                }
            }
            pic->draw(shape);
        }
    }

    if(toggleDrawing)           // Toggle whether or not we're actively creating our shape
        isDrawing = !isDrawing;

}

void Runner::ClearPic() {
    Iterate(false);                 // Stop iterating
    pic->clear(sf::Color::White);   // Clear the canvas (pic) to be fully white
    lastPoint = cx(0, 0);              // Move our last drawn point to the origin
}

void Runner::Draw() {
    window->clear(sf::Color::White); // Clear in preparation for drawing new stuff

    /// Draw GUI elements
    window->draw(lTitle); // Draw the titles for the columns of graph settings
    window->draw(rTitle);

    for(int iii = 0; iii < elements.size(); iii++) { // Draw each GUI element (textboxes, buttons, checkboxes)
        elements[iii]->Draw();
    }

    /// Draw graph elements
    pic->display(); // Update our graph with the newest points
    window->draw(graphs); // Draw the updated graph to the screen
    grid.Draw(); // Draw the axes over the graph

    if (isDrawing) { // If we're actively drawing a shape
        window->draw(shape); // Draw that shape
    }

    window->draw(loc); // Draw the cursor's position after one application of the current equation

    window->display(); // Display everything we've drawn on the screen
}
