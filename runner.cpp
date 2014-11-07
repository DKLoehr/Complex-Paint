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
    mode = mSingle;

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
    clearGraphs = Button(window, inFont, window->getSize().x / 2 - 23, 175, 46, 15, "Clear"); // 14
    elements.push_back(&clearGraphs);

    /** Graph-related elements **/
    /// Button to copy changes from the right side to the left
    mirrorL = Button(window, inFont, lTitle.getPosition().x + ((std::string)lTitle.getString()).length() * 10 + 5,
                     lTitle.getPosition().y, 15, 15, "<"); // 15
    elements.push_back(&mirrorL);

    mirrorR = Button(window, inFont, mirrorL.GetPosition().x + 20, mirrorL.GetPosition().y, 15, 15, ">"); // 16
    elements.push_back(&mirrorR);

    /// Left-side elements
    xRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 30, 55, 15, "x Range"); // 17
    xRangeL.SetText("2");
    elements.push_back(&xRangeL);

    yRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 50, 55, 15, "y Range"); // 18
    yRangeL.SetText("2");
    elements.push_back(&yRangeL);

    xScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 70, 55, 15, "x Scale"); // 19
    xScaleL.SetText("1");
    elements.push_back(&xScaleL);

    yScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 90, 55, 15, "y Scale"); // 20
    yScaleL.SetText("1");
    elements.push_back(&yScaleL);

    centerL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 110, 55, 15, "Center "); // 21
    centerL.SetText("(0,0)");
    elements.push_back(&centerL);

    numbersL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 130, "Numbers", true); // 22
    elements.push_back(&numbersL);

    linesL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 150, "Lines", false);    // 23
    elements.push_back(&linesL);

    /// Right-side elements
    xRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 30, 55, 15, "x Range"); // 24
    xRangeR.SetText("2");
    elements.push_back(&xRangeR);

    yRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 50, 55, 15, "y Range"); // 25
    yRangeR.SetText("2");
    elements.push_back(&yRangeR);

    xScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 70, 55, 15, "x Scale"); // 26
    xScaleR.SetText("1");
    elements.push_back(&xScaleR);

    yScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 90, 55, 15, "y Scale"); // 27
    yScaleR.SetText("1");
    elements.push_back(&yScaleR);

    centerR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 110, 55, 15, "Center "); // 28
    centerR.SetText("(0,0)");
    elements.push_back(&centerR);

    numbersR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 130, "Numbers", true); // 29
    elements.push_back(&numbersR);

    linesR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 150, "Lines", false); // 30
    elements.push_back(&linesR);

    /// General graph-related elements
    okGraph = Button(window, inFont, window->getSize().x * 7 / 8 - 54, 175, // 31
                                108, 15, "Save Changes");
    elements.push_back(&okGraph);

    elements.push_back(&equation); // Finally push back equation so that it's at the end of the vector


    /** End GUI element creation **/

    for(int iii = 0; iii < elements.size(); iii++) {
        elements[iii]->SetActive(false);
    }
    activeBox = 32; // Start out highlighting the equation entering box
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
        if(newPos != NULL) {
            lastPoint = *newPos;
            if(mode == mIterative) {
                sf::CircleShape firstLoc = sf::CircleShape(circRad, 30);
                firstLoc.setFillColor(sf::Color::Green);
                firstLoc.setPosition(grid.lGrid.GraphToPic(sf::Vector2f(lastPoint.real(), lastPoint.imag())) -
                                                           sf::Vector2f(circRad, circRad));
                pic->draw(firstLoc);
                firstLoc.setPosition(grid.rGrid.GraphToPic(sf::Vector2f(lastPoint.real(), lastPoint.imag())) -
                                                           sf::Vector2f(circRad, circRad));
                pic->draw(firstLoc);
            }
        }
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
            if(mode == mIterative) { // Iterate mode draws to both windows; other modes only draw to one
                newLoc.setPosition(grid.rGrid.GraphToPic(graphCoords) - sf::Vector2f(circRad, circRad));
                pic->draw(newLoc);
                newLoc.setPosition(grid.lGrid.GraphToPic(graphCoords) - sf::Vector2f(circRad, circRad));
                pic->draw(newLoc);
            }
            else if(leftToRight) { // Draw the output on the right graph
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
    case 15: // Mirror R->L
        okGraph.SetOutlineColor(sf::Color::Red);
        xRangeL.SetText(xRangeR.GetText());
        yRangeL.SetText(yRangeR.GetText());
        xScaleL.SetText(xScaleR.GetText());
        yScaleL.SetText(yScaleR.GetText());
        centerL.SetText(centerR.GetText());
        if(numbersL.GetText() != numbersR.GetText()) numbersL.Toggle();
        if(linesL.GetText() != linesR.GetText()) linesL.Toggle();
        break;
    case 16: // Mirror L->R
        okGraph.SetOutlineColor(sf::Color::Red);
        xRangeR.SetText(xRangeL.GetText());
        yRangeR.SetText(yRangeL.GetText());
        xScaleR.SetText(xScaleL.GetText());
        yScaleR.SetText(yScaleL.GetText());
        centerR.SetText(centerL.GetText());
        if(numbersR.GetText() != numbersL.GetText()) numbersR.Toggle();
        if(linesR.GetText() != linesL.GetText()) linesR.Toggle();
        break;
    case 31: // Save Changes for graphs
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
        DrawFree(position);
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

    shape.resize(0); // Clear out the previous line, since only the fixed endpoint will still be on it

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
    std::cout << "Rect\n";
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

    // Add vertical lines from left to right (if in standard orientation)
    for(int iii = 0; iii < yLines; iii++) { // Iterate between lines
        for(int jjj = 0; jjj < (xLines - 1) * GRID_POINT_NUMBER; jjj++) {
            shape.append(sf::Vertex(sf::Vector2f(position.x + iii * delta, position.y + jjj * pointDelta)));
            shape.append(sf::Vertex(sf::Vector2f(position.x + iii * delta, position.y + (jjj + 1) * pointDelta)));
        }
    }
}

void Runner::DrawCirc1(sf::Vector2i position) {
    std::cout << "Circ1\n";
}

void Runner::DrawCirc2(sf::Vector2i position) {
    std::cout << "Circ2\n";
}

void Runner::DrawCirc3(sf::Vector2i position) {
    std::cout << "Circ3\n";
}

void Runner::DrawFree(sf::Vector2i position) {
    std::cout << "Free\n";
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
