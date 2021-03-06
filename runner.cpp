#include "runner.h"

Runner::Runner(sf::RenderWindow* w, sf::Font* font, sf::RenderTexture* p, sf::RenderTexture* j) :
    window(w),
    inFont(font),
    pic(p),
    jpic(j)

{
    Init();
}


void Runner::Init() {
    isIterating = false;
    isDrawing = false;
    leftToRight = true;
    drawingJulia = false;
    mode = mSingle;

    pic->clear(sf::Color::White);
    jpic->clear(sf::Color(0, 0, 0, 0)); // Transparent white
    graphs.setPosition(0, HEIGHT_OFFSET);
    graphs.setTexture(pic->getTexture());
    jgraphs.setPosition(0, HEIGHT_OFFSET);
    jgraphs.setTexture(jpic->getTexture());

    fct = new parser::Tree("Z");

    grid = DoubleGrid(window, *inFont, HEIGHT_OFFSET);
    grid.Draw();

    loc = sf::CircleShape(1.5, 30);
    loc.setFillColor(sf::Color::Red);
    loc.setPosition(-2, -2); // Make loc invisible until it's moved onto the screen

    lastPoint = std::complex<double>(0, 0);
    lastStartPos = NULL;
    graphCoords = sf::Vector2f(0, 0);

    lTitle = sf::Text("Left Graph", *inFont, 15);
    lTitle.setPosition(window->getSize().x * 13 / 16 - 50, 5);
    lTitle.setColor(sf::Color::Black);

    rTitle = sf::Text("Right Graph", *inFont, 15);
    rTitle.setPosition(window->getSize().x * 15 / 16 - 55, 5);
    rTitle.setColor(sf::Color::Black);

    elements = std::vector<GUI*>(0);

    /** Equation-related elements **/
    equation = InputBox(window, inFont, 5, 5, 350, 15); // 32
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
    modeSingle = Button(window, inFont, presetLin.GetPosition().x, 85, 45, 15, "Point");  // 5
    modeSingle.SetOutlineColor(sf::Color::Green);
    elements.push_back(&modeSingle);

    modeIterate = Button(window, inFont, modeSingle.GetPosition().x + modeSingle.GetSize().x + BUTTON_DELTA, modeSingle.GetPosition().y,
                                65, 15, "Iterate"); // 6
    elements.push_back(&modeIterate);

    modeLine = Button(window, inFont, modeIterate.GetPosition().x + modeIterate.GetSize().x + BUTTON_DELTA, modeSingle.GetPosition().y,
                                38, 15, "Line");    // 7
    elements.push_back(&modeLine);

    modeRect = Button(window, inFont, modeLine.GetPosition().x + modeLine.GetSize().x + BUTTON_DELTA, modeSingle.GetPosition().y,
                                82, 15, "Rectangle"); // 8
    elements.push_back(&modeRect);

    modeGrid = Button(window, inFont,  modeRect.GetPosition().x + modeRect.GetSize().x + BUTTON_DELTA, modeSingle.GetPosition().y,
                                40, 15, "Grid"); // 9
    elements.push_back(&modeGrid);

    // Line break

    modeCirc1 = Button(window, inFont,  modeSingle.GetPosition().x + 23, modeSingle.GetPosition().y + BUTTON_DELTA,
                                62, 15, "Circle1"); // 10
    elements.push_back(&modeCirc1);

    modeCirc2 = Button(window, inFont,  modeCirc1.GetPosition().x + modeCirc1.GetSize().x + BUTTON_DELTA, modeCirc1.GetPosition().y,
                                63, 15, "Circle2"); // 11
    elements.push_back(&modeCirc2);

    modeCirc3 = Button(window, inFont,  modeCirc2.GetPosition().x + modeCirc2.GetSize().x + BUTTON_DELTA, modeCirc1.GetPosition().y,
                                63, 15, "Circle3"); // 12
    elements.push_back(&modeCirc3);

    modeFree = Button(window, inFont,  modeCirc3.GetPosition().x + modeCirc3.GetSize().x + BUTTON_DELTA, modeCirc1.GetPosition().y,
                                75, 15, "Freedraw"); // 13
    elements.push_back(&modeFree);


    /// Clear graph button
    clearGraphs = Button(window, inFont, window->getSize().x / 2 - 125, 175, 100, 15, "Clear Graph"); // 14
    elements.push_back(&clearGraphs);

    clearJulia = Button(window, inFont, window->getSize().x / 2 + 25, 175, 100, 15, "Clear Julia");  // 15
    elements.push_back(&clearJulia);

    /** Graph-related elements **/
    /// Button to copy changes from the right side to the left
    mirrorL = Button(window, inFont, lTitle.getPosition().x + ((std::string)lTitle.getString()).length() * 10 + 5,
                     lTitle.getPosition().y, 15, 15, "<"); // 16
    elements.push_back(&mirrorL);

    mirrorR = Button(window, inFont, mirrorL.GetPosition().x + 20, mirrorL.GetPosition().y, 15, 15, ">"); // 17
    elements.push_back(&mirrorR);

    /// Left-side elements
    xRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 30, 55, 15, "x Range"); // 18
    xRangeL.SetText("2");
    elements.push_back(&xRangeL);

    yRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 50, 55, 15, "y Range"); // 19
    yRangeL.SetText("2");
    elements.push_back(&yRangeL);

    xScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 70, 55, 15, "x Scale"); // 20
    xScaleL.SetText("1");
    elements.push_back(&xScaleL);

    yScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 90, 55, 15, "y Scale"); // 21
    yScaleL.SetText("1");
    elements.push_back(&yScaleL);

    centerL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 110, 55, 15, "Center "); // 22
    centerL.SetText("(0,0)");
    elements.push_back(&centerL);

    numbersL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 130, "Numbers", true); // 23
    elements.push_back(&numbersL);

    linesL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 150, "Lines", false);    // 24
    elements.push_back(&linesL);

    /// Right-side elements
    xRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 30, 55, 15, "x Range"); // 25
    xRangeR.SetText("2");
    elements.push_back(&xRangeR);

    yRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 50, 55, 15, "y Range"); // 26
    yRangeR.SetText("2");
    elements.push_back(&yRangeR);

    xScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 70, 55, 15, "x Scale"); // 27
    xScaleR.SetText("1");
    elements.push_back(&xScaleR);

    yScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 90, 55, 15, "y Scale"); // 28
    yScaleR.SetText("1");
    elements.push_back(&yScaleR);

    centerR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 110, 55, 15, "Center "); // 29
    centerR.SetText("(0,0)");
    elements.push_back(&centerR);

    numbersR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 130, "Numbers", true); // 30
    elements.push_back(&numbersR);

    linesR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 150, "Lines", false); // 31
    elements.push_back(&linesR);

    /// General graph-related elements
    okGraph = Button(window, inFont, window->getSize().x * 7 / 8 - 54, 175, // 32
                                108, 15, "Save Changes");
    elements.push_back(&okGraph);

    elements.push_back(&equation); // Finally push back equation so that it's at the end of the vector


    /** End GUI element creation **/

    for(int iii = 0; iii < elements.size(); iii++) {
        elements[iii]->SetActive(false);
    }
    activeBox = elementsSize - 1; // Start out highlighting the equation entering box
    elements[activeBox]->SetActive(true);
    UpdateGraphs();
}

void Runner::HandleEvents() {
    sf::Event event;
    while(window->pollEvent(event)) {
        switch(event.type) {
        case sf::Event::Closed:
            window->close();
            break;
        case sf::Event::TextEntered:
            elements[activeBox]->OnTextEntered(event.text.unicode);
            if(event.text.unicode != 9 && activeBox >= 16 && activeBox != 27) { // One of the inputBoxes, wasn't tab
                if(activeBox >= 33) // Equation or variables
                    okEquation.SetOutlineColor(sf::Color::Red);
                else // Graph box changed
                    okGraph.SetOutlineColor(sf::Color::Red);
            }
            break;
        case sf::Event::KeyPressed:
            switch(event.key.code) { // Begin event.key.code switch statement
            case sf::Keyboard::Escape:
                window->close();
                break;
            case sf::Keyboard::Return:
                ActivateButtons(event);
                break;
            case sf::Keyboard::Tab:
                StepActiveElement(!(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                                    sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)));
                break;
            case sf::Keyboard::Delete:
                elements[activeBox]->OnTextEntered(127);
            default:
                elements[activeBox]->OnKeyPressed(event.key.code);
                break;
            } // End event.key.code switch statement
            break;
        case sf::Event::MouseMoved:
            if(event.mouseMove.y > HEIGHT_OFFSET) {
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
            break;
        case sf::Event::MouseButtonPressed:
            if(event.mouseButton.y < HEIGHT_OFFSET) { // Above the graphs
                SetActiveElement(event.mouseButton.x, event.mouseButton.y);
                ActivateButtons(event);
            } else { // In one of the graphs
                leftToRight = (event.mouseButton.x < window->getSize().x / 2); // True iff the click was in the left graph
                if(mode == mSingle || mode == mIterative) { // Single or Iterate mode
                    if(leftToRight) {
                        graphCoords = grid.lGrid.WindowToGraph(event.mouseButton.x, event.mouseButton.y);
                    } else {
                        graphCoords = grid.rGrid.WindowToGraph(event.mouseButton.x, event.mouseButton.y);
                    }
                    Iterate(!isIterating, new cx(graphCoords.x, graphCoords.y)); // Stop iterating if we were iterating, or start if we weren't
                }
                else { // Other mode
                    DrawShape(true);
                }
            }
            break;
            default:
                continue;
        }
    }
    DrawShape(false);
    Iterate(isIterating);
}

void Runner::Iterate(bool keepIterating, cx* newPos) {
    isIterating = keepIterating;
    if(!keepIterating)
        return;

    /** Setup: Size of the points, how many times to iterate **/
    double circRad = 1;
    int numIterations = 1;
    if(mode == mSingle) {
        isIterating = false;
        circRad = 1.5;
    }
    else if (mode == mIterative) {
        numIterations = 90;
        circRad = .75;
    } else if(mode == mGrid) { // Grid mode
        isIterating = false;
        circRad = 1.0;
    }
    sf::CircleShape point = sf::CircleShape(circRad, 30); // Point that we draw to the screen
    point.setFillColor(sf::Color::Black);

    /** Draw special points:
        First point of an iteration is green,
        Draw over previous green ones
        Draw over the last drawn point (which was drawn in red) **/

    /// Draw over last drawn point, so it's no longer red
    graphCoords = sf::Vector2f(lastPoint.real(), lastPoint.imag()); // Location of the last drawn point (currently red)
    if(mode == mIterative) {                   // Iterate mode draws to both windows
        DrawPoint(point, graphCoords, true);   // Draw over it in black
        DrawPoint(point, graphCoords, false);
    } else {
        DrawPoint(point, graphCoords, lastGraph);
    }
    if(newPos != NULL) { // We've been given a new starting position
        /// Draw over the previous first point, so it's no longer green
        if(lastStartPos != NULL) { // We have a green point currently on the screen
            point.setFillColor(sf::Color::Black); // Draw over previous first point in black
            graphCoords = sf::Vector2f(lastStartPos->real(), lastStartPos->imag()); // Coordinates of previous first point
            DrawPoint(point, graphCoords, true); // Draw over the previous first point (is still green) on both graphs
            DrawPoint(point, graphCoords, false);
        }
        if(mode == mIterative) {
            if(lastStartPos == NULL) {      // Store our current first point in the appropriate manner
                lastStartPos = new cx(*newPos);
            } else {
                *lastStartPos = *newPos;
            }
        }

        /// Draw the first point of an iteration in green
        if(mode == mIterative) {
            point.setFillColor(sf::Color::Green); // Draw the first point in green
            graphCoords = sf::Vector2f(newPos->real(), newPos->imag()); // Coordinates of our new starting point
            DrawPoint(point, graphCoords, true); // Draw the first point to both graphs
            DrawPoint(point, graphCoords, false);
        }
        lastPoint = *newPos; // Mark the last point drawn
    }
    delete newPos; // Drawn this point, so free its memory to prevent leaks


    /** Draw regular points **/
    for(int iii = 0; iii < numIterations; iii++) { // Iterate many points at once, or just one
        fct->setVar("Z", lastPoint); // Feed our current location into parser as the variable Z
        try {
            lastPoint = fct->eval(); // Don't change position, so we can make it black first
        }
        catch (std::invalid_argument) { // Should mean we've "reached infinity", so we can stop
            isIterating = false;
            return;
        }

        graphCoords = sf::Vector2f(lastPoint.real(), lastPoint.imag()); // Coordinates of the last drawn point
        if(iii == numIterations - 1)
            point.setFillColor(sf::Color::Red);    // Last point, so draw it in red
        else
            point.setFillColor(sf::Color::Black);  // Otherwise, it's black
        if(mode == mIterative) {                    // Iterate mode draws to both windows; other modes only draw to one
            DrawPoint(point, graphCoords, true);   // Draw to both windows
            DrawPoint(point, graphCoords, false);
        } else {
            DrawPoint(point, graphCoords, !leftToRight);
            lastGraph = !leftToRight; // Mark which graph we most recently drew to
        }
        leftToRight = !leftToRight;
    }
}


void Runner::SetActiveElement(double x, double y) {
    for(int iii = 0; iii < elements.size(); iii++) {
        if(elements[iii]->IsClicked(x, y)) {            // True iff x and y are in that element's box
            if(activeBox != iii) {                      // If the active box has changed
                elements[activeBox]->SetActive(false);  // Deactivate the previously-active box
                activeBox = iii;                        // Update which box is active
                elements[activeBox]->SetActive(true);   // Activate the newly-active box
            }
            break;                                      // Figured out which box is active, so we can stop looking now
        }
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

    /// Left Grid
    fct->setVar("Z", xRangeL.GetText());    // Set the variable Z to be our text to evaluate (Z is reset before use)
    x = fct->evalVar("Z").real();           // Store the value of Z in x
    fct->setVar("Z", yRangeL.GetText());    // Repeat for y
    y = fct->evalVar("Z").real();
    grid.lGrid.SetRange(x, y);              // Set the left grid's range to be that specified in the left range boxes

    fct->setVar("Z", xScaleL.GetText());    // Repeat for other values
    x = fct->evalVar("Z").real();
    fct->setVar("Z", yScaleL.GetText());
    y = fct->evalVar("Z").real();
    grid.lGrid.SetScale(x, y);              // Set the left grid's scale to be that specified in the left scale boxes

    if(centerL.GetText() != "") {           // If there's no entry, leave the center as it is
        fct->setVar("Z", centerL.GetOrderedPairElement(true));
        x = fct->evalVar("Z").real();
        fct->setVar("Z", centerL.GetOrderedPairElement(false));
        y = fct->evalVar("Z").real();
        grid.lGrid.SetCenter(sf::Vector2f(x, y));  // Set the left grid's center to be that specified in the left center box
    }

    grid.lGrid.SetNumbers(numbersL.GetText() == "x");       // Set numbers on the axes to be on or off as specified by the left "numbers" checkbox
    grid.lGrid.SetLines(linesL.GetText() == "x");           // Set tick marks or whole grid lines to be on or off as specified by the left "lines" checkbox

    /// Right Grid
    fct->setVar("Z", xRangeR.GetText()); // Repeat for the right grid
    x = fct->evalVar("Z").real();
    fct->setVar("Z", yRangeR.GetText());
    y = fct->evalVar("Z").real();
    grid.rGrid.SetRange(x, y);

    fct->setVar("Z", xScaleR.GetText());
    x = fct->evalVar("Z").real();
    fct->setVar("Z", yScaleR.GetText());
    y = fct->evalVar("Z").real();
    grid.rGrid.SetScale(x, y);

    if(centerR.GetText() != "") {
        fct->setVar("Z", centerR.GetOrderedPairElement(true));
        x = fct->evalVar("Z").real();
        fct->setVar("Z", centerR.GetOrderedPairElement(false));
        y = fct->evalVar("Z").real();
        grid.rGrid.SetCenter(sf::Vector2f(x, y));  // Set the left grid's center to be that specified in the left center box
    }

    grid.rGrid.SetNumbers(numbersR.GetText() == "x");
    grid.rGrid.SetLines(linesR.GetText() == "x");

    if(okGraph.GetOutlineColor() != sf::Color::Black) {     // If anything (besides numbers or lines) was changed
        okGraph.SetOutlineColor(sf::Color::Black);          // Mark "Save changes" button as up-to-date
        ClearPic();                                         // Get rid of old points which are no longer accurate
        ClearJPic();
    }
}

void Runner::UpdateEquation() {
    Iterate(false); // Stop iterating

    /// Update the tree with the new equation
    delete fct;                                             // Get rid of our old, outdated equation tree
    std::string func = SanitizeString(equation.GetText());  // Get our updated equation and sanitize it
    fct = new parser::Tree(func);                           // Create a new tree with our new, sanitized equation

    /** Replace the old parameter inputBoxes with the new ones **/

    /// Update fct with new values and remove old inputBoxes
    while(elements.size() > elementsSize) { // While we have more elements than we started with (all extra ones are parameter boxes)
        fct->setVar(elements[elements.size() - 1]->GetCap().substr(0, 1),       // Figure out which param the bottom inputBox corresponds to...
                    SanitizeString(elements[elements.size() - 1]->GetText()));  // And set that parameter with the string in that box
        delete elements[elements.size() - 1];   // Remove the last inputBox from memory to prevent memory leaks
        elements.pop_back();                    // Remove the last inputBox from the array
    }

    /// Create the new inputBoxes for each parameter that appears in equation
    // Loop through all possible parameter values: 'A' through 'Z'
    for(int iii = 'A'; iii < 'Z' && elements.size() < elementsSize + 8; iii++) { // Max 8 vars for now due to screen size constraints
        if(func.find(iii) != std::string::npos) { // If this parameter appears in the equation...
            elements.push_back(new InputBox(window, inFont, 3, (elements.size() - elementsSize + 1.25) * 20, // Add a new inputBox
                                        150, 15, std::string(1, (char)iii) + " "));                          // for this parameter
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
        drawingJulia = false;
        UpdateEquation();
        break;
    case 2: // Polar Preset
        equation.SetText("(R*cos(2*pi*T)+R*sin(2*pi*T)*i)*z+B");
        UpdateEquation();
        drawingJulia = false;
        break;
    case 3: // Quadratic Preset
        equation.SetText("z^2 + C");
        drawingJulia = false;
        UpdateEquation();
        break;
    case 4: // Inverse Quadratic Preset
        equation.SetText("rpm() * sqrt(z - C)");
        drawingJulia = true;
        UpdateEquation();
        break;
    case 5: // Changing the current mode
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        Iterate(false);
        elements[(int)mode + 5]->SetOutlineColor(sf::Color::Black); // Reset the coloring of the last-selected mode button
        mode = (drawMode)(activeBox - 5);                           // Update our current draw mode
        elements[(int)mode + 5]->SetOutlineColor(sf::Color::Green); // Color the active mode green
        break;
    case 14: // Clear
        ClearPic();
        break;
    case 15:
        ClearJPic(); // Clear julia set
        break;
    case 16: // Mirror R->L
        okGraph.SetOutlineColor(sf::Color::Red);
        xRangeL.SetText(xRangeR.GetText());
        yRangeL.SetText(yRangeR.GetText());
        xScaleL.SetText(xScaleR.GetText());
        yScaleL.SetText(yScaleR.GetText());
        centerL.SetText(centerR.GetText());
        if(numbersL.GetText() != numbersR.GetText()) numbersL.Toggle();
        if(linesL.GetText() != linesR.GetText()) linesL.Toggle();
        break;
    case 17: // Mirror L->R
        okGraph.SetOutlineColor(sf::Color::Red);
        xRangeR.SetText(xRangeL.GetText());
        yRangeR.SetText(yRangeL.GetText());
        xScaleR.SetText(xScaleL.GetText());
        yScaleR.SetText(yScaleL.GetText());
        centerR.SetText(centerL.GetText());
        if(numbersR.GetText() != numbersL.GetText()) numbersR.Toggle();
        if(linesR.GetText() != linesL.GetText()) linesR.Toggle();
        break;
    case 32: // Save Changes for graphs
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
    if(mode != mFree)
        shape = sf::VertexArray(sf::Lines, 0); // VertexArray to hold shapes

    if(!isDrawing && toggleDrawing)
        position = sf::Mouse::getPosition(*window); // Get our initial position from where the mouse cursor was when it was clicked
    if(sf::Mouse::getPosition(*window).y < 200 || // In the upper part of the window, out of the graphs, or
        position.x < window->getSize().x / 2 && sf::Mouse::getPosition(*window).x > window->getSize().x / 2 || // In a different grid from the
        position.x > window->getSize().x / 2 && sf::Mouse::getPosition(*window).x < window->getSize().x / 2)   // one we clicked in originally
            return; // Our picture would cross a boundary, so don't draw a new one

    switch(mode) {
    case mLine:
        DrawLine(position);
        break;
    case mRect:
        DrawRect(position);
        break;
    case mGrid:
        DrawGrid(position);
        break;
    case mCirc1:
        DrawCirc1(position);
        break;
    case mCirc2:
        DrawCirc2(position);
        break;
    case mCirc3:
        DrawCirc3(position);
        break;
    case mFree:
        DrawFree(position, toggleDrawing);
        break;
    }

    for(int iii = 0; iii < shape.getVertexCount(); iii++) { // Make every vertex black for drawing
        shape[iii].color = sf::Color::Black;
    }

    /// Draw the shape to the other grid if we're done drawing
    if(isDrawing && toggleDrawing) { // We were actively drawing, but are about to stop
        bool leftToRightLocal = leftToRight; // Local copy of leftToRight so that we can reset it when it is flipped by Iterate

        // Convert the shape's coordinates to be those of the graph
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

        for(int iii = 0; iii < shape.getVertexCount(); iii++) {
            leftToRight = leftToRightLocal;
            Iterate(true, new cx(shape[iii].position.x, shape[iii].position.y));
            if(iii % 3 == 0) // Every third vertex is a repeat of the previous one, so don't waste time redrawing it
                iii++;
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
        Iterate(false);
    }

    if(toggleDrawing)           // Toggle whether or not we're actively creating our shape
        isDrawing = !isDrawing;
}

void Runner::DrawLine(sf::Vector2i position) {
    sf::Vector2i endPoint = sf::Mouse::getPosition(*window);

    double numPoints = sqrt((endPoint.x - position.x) * (endPoint.x - position.x) +
                         (endPoint.y - position.y) * (endPoint.y - position.y)) /
                         LINE_POINT_DELTA;
    for(int iii = 0; iii <= numPoints; iii++) {
        double scale = iii / numPoints;
        shape.append(sf::Vertex(sf::Vector2f((1 - scale) * position.x + scale * endPoint.x,
                                             (1 - scale) * position.y + scale * endPoint.y)));
        scale += 1 / numPoints;
        shape.append(sf::Vertex(sf::Vector2f((1 - scale) * position.x + scale * endPoint.x,
                                             (1 - scale) * position.y + scale * endPoint.y)));
    }
}

void Runner::DrawRect(sf::Vector2i position) {
    sf::Vector2i shapeSize = sf::Mouse::getPosition(*window) - position;

    double pointDeltaX = (double)shapeSize.x / (shapeSize.x / RECT_POINT_DELTA),
           pointDeltaY = (double)shapeSize.y / (shapeSize.y / RECT_POINT_DELTA);
    if(shapeSize.x < 0) pointDeltaX *= -1;
    if(shapeSize.y < 0) pointDeltaY *= -1;
    double numPointsX = abs(shapeSize.x / RECT_POINT_DELTA),
           numPointsY = abs(shapeSize.y / RECT_POINT_DELTA);

    for(int iii = 0; iii < 2; iii++) { // Iterate for each pair of lines
        for(int jjj = 0; jjj < numPointsX; jjj++) { // Horizontal lines
            shape.append(sf::Vertex(sf::Vector2f(position.x + jjj * pointDeltaX, position.y + iii * shapeSize.y)));
            shape.append(sf::Vertex(sf::Vector2f(position.x + (jjj + 1) * pointDeltaX, position.y + iii * shapeSize.y)));
        }
        for(int jjj = 0; jjj < numPointsY; jjj++) { // Vertical lines
            shape.append(sf::Vertex(sf::Vector2f(position.x + iii * shapeSize.x, position.y + jjj * pointDeltaY)));
            shape.append(sf::Vertex(sf::Vector2f(position.x + iii * shapeSize.x, position.y + (jjj + 1) * pointDeltaY)));
        }
    }
}

void Runner::DrawGrid(sf::Vector2i position) {
    sf::Vector2i shapeSize = sf::Mouse::getPosition(*window) - position;
    int xLines = shapeSize.y / GRID_LINES_DELTA,    // Number of horizontal lines. Note: integer division to remove extra pixels
        yLines = shapeSize.x / GRID_LINES_DELTA;    // Number of vertical lines; also integer division
    shapeSize.y = xLines * GRID_LINES_DELTA;        // Remove extra pixels that aren't enough to constitute a line
    shapeSize.x = yLines * GRID_LINES_DELTA;
    xLines = abs(xLines) + 1;   // Want a positive number of lines;
    yLines = abs(yLines) + 1;   // Increment by one because we've only counted one of the extreme lines (far left/right or top/bottom)

    /// Create the grid
    // Add our horizontal lines from top to bottom (if in standard orientation)
    int delta = GRID_LINES_DELTA; // Distance between lines in pixels
    double pointDelta = (double)GRID_LINES_DELTA / GRID_POINT_NUMBER;
    if(shapeSize.y < 0) delta *= -1;
    if(shapeSize.x < 0) pointDelta *= -1;

    for(int iii = 0; iii < xLines; iii++) { // Iterate between lines
        for(int jjj = 0; jjj < (yLines - 1) * GRID_POINT_NUMBER; jjj++) {
            shape.append(sf::Vertex(sf::Vector2f(position.x + jjj * pointDelta, position.y + iii * delta)));
            shape.append(sf::Vertex(sf::Vector2f(position.x + (jjj + 1) * pointDelta, position.y + iii * delta)));
        }
    }

    delta = abs(delta);
    pointDelta = abs(pointDelta);
    if(shapeSize.x < 0) delta *= -1;
    if(shapeSize.y < 0) pointDelta *= -1;

    // Add vertical lines from left to right (if in standard orientation)
    for(int iii = 0; iii < yLines; iii++) { // Iterate between lines
        for(int jjj = 0; jjj < (xLines - 1) * GRID_POINT_NUMBER; jjj++) {
            shape.append(sf::Vertex(sf::Vector2f(position.x + iii * delta, position.y + jjj * pointDelta)));
            shape.append(sf::Vertex(sf::Vector2f(position.x + iii * delta, position.y + (jjj + 1) * pointDelta)));
        }
    }
}

void Runner::DrawCirc1(sf::Vector2i position) {
    sf::Vector2i endPoint = sf::Mouse::getPosition(*window);
    if(endPoint.x < window->getSize().x / 2) // In left graph
        position = sf::Vector2i(grid.lGrid.GraphToWindow(0,0));
    else // In right graph
        position = sf::Vector2i(grid.rGrid.GraphToWindow(0,0));
    double radius = sqrt((endPoint.x - position.x) * (endPoint.x - position.x) +
                         (endPoint.y - position.y) * (endPoint.y - position.y));
    double theta = acos((2 * radius * radius - CIRC1_POINT_DELTA * CIRC1_POINT_DELTA) / (2 * radius * radius));
    for(int iii = 0; iii < 2 * PI / theta + 1; iii++) {
        shape.append(sf::Vertex(sf::Vector2f(position) + sf::Vector2f(radius * cos(theta * iii), radius * sin(theta * iii))));
        shape.append(sf::Vertex(sf::Vector2f(position) + sf::Vector2f(radius * cos(theta * (iii + 1)), radius * sin(theta * (iii + 1)))));
    }
}

void Runner::DrawCirc2(sf::Vector2i position) {
    sf::Vector2i endPoint = sf::Mouse::getPosition(*window);
    double radius = sqrt((endPoint.x - position.x) * (endPoint.x - position.x) +
                         (endPoint.y - position.y) * (endPoint.y - position.y));
    double theta = acos((2 * radius * radius - CIRC2_POINT_DELTA * CIRC2_POINT_DELTA) / (2 * radius * radius));
    for(int iii = 0; iii < 2 * PI / theta + 1; iii++) {
        shape.append(sf::Vertex(sf::Vector2f(position) + sf::Vector2f(radius * cos(theta * iii), radius * sin(theta * iii))));
        shape.append(sf::Vertex(sf::Vector2f(position) + sf::Vector2f(radius * cos(theta * (iii + 1)), radius * sin(theta * (iii + 1)))));
    }
}

void Runner::DrawCirc3(sf::Vector2i position) {
    sf::Vector2i endPoint = sf::Mouse::getPosition(*window);
    double radius = sqrt((endPoint.x - position.x) * (endPoint.x - position.x) +
                         (endPoint.y - position.y) * (endPoint.y - position.y));
    int numCircles = radius / CIRC3_RAD_DELTA; // Integer division to get rid of extra pixels
    for(int iii = 0; iii < numCircles; iii++) {
        radius = iii * CIRC3_RAD_DELTA;
        double theta = acos((2 * radius * radius - CIRC3_POINT_DELTA * CIRC3_POINT_DELTA) / (2 * radius * radius));
        for(int jjj = 0; jjj < 2 * PI / theta + 1; jjj++) {
            shape.append(sf::Vertex(sf::Vector2f(position) + sf::Vector2f(radius * cos(theta * jjj), radius * sin(theta * jjj))));
            shape.append(sf::Vertex(sf::Vector2f(position) + sf::Vector2f(radius * cos(theta * (jjj + 1)), radius * sin(theta * (jjj + 1)))));
        }
    }
}

void Runner::DrawFree(sf::Vector2i position, bool toggleDrawing) {
    if(!isDrawing && toggleDrawing) { // We're starting to draw a new figure
        shape.resize(0);
        shape.append(sf::Vertex(sf::Vector2f(position)));
    }
    shape.append(sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(*window))));
    shape.append(sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(*window))));
}

void Runner::ClearPic() {
    Iterate(false);                 // Stop iterating
    pic->clear(sf::Color::White);   // Clear the canvas (pic) to be fully white
    lastPoint = cx(0, 0);           // Move our last drawn point to the origin
    delete(lastStartPos);
    lastStartPos = NULL;
}

void Runner::ClearJPic() {
    Iterate(false);                         // Stop iterating
    jpic->clear(sf::Color(0, 0, 0, 0));   // Clear the julia set canvas (jpic) to be transparent white
    lastPoint = cx(0, 0);                   // Move our last drawn point to the origin
    delete(lastStartPos);
    lastStartPos = NULL;
}

void Runner::DrawPoint(sf::CircleShape& point, sf::Vector2f pos, bool left) {
    if(left) {
        point.setPosition(grid.lGrid.GraphToPic(pos) - sf::Vector2f(point.getRadius(), point.getRadius()));
    } else {
        point.setPosition(grid.rGrid.GraphToPic(pos) - sf::Vector2f(point.getRadius(), point.getRadius()));
    }
    if(drawingJulia)
        jpic->draw(point);
    else
        pic->draw(point);
}

std::string Runner::SanitizeString(std::string str){
    for(int iii = 0; iii < str.length(); iii++) {
        if(str[iii] == 'z')                // Equate user input of 'z' with 'Z'
            str.replace(iii, 1, "Z");      // Replace instances of 'z' with 'Z', since parser requires uppercase parameters
        else if(iii != 0 && str[iii] == 'i' && str[iii - 1] == '-')
            str.replace(iii - 1, 1, "-1");     ///Workaround for a bug: replace -i with -1i to ensure it's parsed correctly
    }
    return str;
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
    window->draw(graphs); // Draw the updated graph of the first layer to the screen
    jpic->display();
    window->draw(jgraphs); // Draw the second layer
    grid.Draw(); // Draw the axes over the graph

    if (isDrawing) { // If we're actively drawing a shape
        window->draw(shape); // Draw that shape
    }

    window->draw(loc); // Draw the cursor's position after one application of the current equation

    window->display(); // Display everything we've drawn on the screen
}
