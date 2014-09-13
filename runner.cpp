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
    mode = single;

    pic->clear(sf::Color::White);
    graphs.setPosition(0, HEIGHT_OFFSET);
    graphs.setTexture(pic->getTexture());

    fct = new parser::Tree("Z");

    grid = DoubleGrid(window, *inFont, HEIGHT_OFFSET);
    grid.Draw();

    points = std::vector<sf::CircleShape>(0);

    loc = sf::CircleShape(1, 30);
    loc.setFillColor(sf::Color::Black);

    locPos = std::complex<double>(0, 0);
    graphCoords = sf::Vector2f(0, 0);

    lTitle = sf::Text("Left Graph", *inFont, 15);
    lTitle.setPosition(window->getSize().x * 13 / 16 - 50, 5);
    lTitle.setColor(sf::Color::Black);

    rTitle = sf::Text("Right Graph", *inFont, 15);
    rTitle.setPosition(window->getSize().x * 15 / 16 - 55, 5);
    rTitle.setColor(sf::Color::Black);

    elements = std::vector<GUI*>(0);

    /** Equation-related elements **/
    equation = InputBox(window, inFont, 5, 5, 350, 15); // 25
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
    modeSingle = Button(window, inFont, window->getSize().x / 2 - 105, 93, 45, 15, "Point");  // 5
    modeSingle.SetOutlineColor(sf::Color::Green);
    elements.push_back(&modeSingle);

    modeIterate = Button(window, inFont, window->getSize().x / 2 + 45, modeSingle.GetPosition().y,
                                65, 15, "Iterate"); // 6
    elements.push_back(&modeIterate);

    /// Clear graph button
    clearGraphs = Button(window, inFont, window->getSize().x / 2 - 23, 175, 46, 15, "Clear"); // 7
    elements.push_back(&clearGraphs);

    /** Graph-related elements **/
    /// Button to copy changes from the right side to the left
    mirrorL = Button(window, inFont, lTitle.getPosition().x + ((std::string)lTitle.getString()).length() * 10 + 5,
                     lTitle.getPosition().y, 15, 15, "<"); // 8
    elements.push_back(&mirrorL);

    mirrorR = Button(window, inFont, mirrorL.GetPosition().x + 20, mirrorL.GetPosition().y, 15, 15, ">"); // 9
    elements.push_back(&mirrorR);

    /// Left-side elements
    xRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 30, 55, 15, "x Range"); // 10
    xRangeL.SetText("2");
    elements.push_back(&xRangeL);

    yRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 50, 55, 15, "y Range"); // 11
    yRangeL.SetText("2");
    elements.push_back(&yRangeL);

    xScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 70, 55, 15, "x Scale"); // 12
    xScaleL.SetText("1");
    elements.push_back(&xScaleL);

    yScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 90, 55, 15, "y Scale"); // 13
    yScaleL.SetText("1");
    elements.push_back(&yScaleL);

    centerL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 110, 55, 15, "Center "); // 14
    centerL.SetText("(0,0)");
    elements.push_back(&centerL);

    numbersL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 130, "Numbers", true); // 15
    elements.push_back(&numbersL);

    linesL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 150, "Lines", false);    // 16
    elements.push_back(&linesL);

    /// Right-side elements
    xRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 30, 55, 15, "x Range"); // 17
    xRangeR.SetText("2");
    elements.push_back(&xRangeR);

    yRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 50, 55, 15, "y Range"); // 18
    yRangeR.SetText("2");
    elements.push_back(&yRangeR);

    xScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 70, 55, 15, "x Scale"); // 19
    xScaleR.SetText("1");
    elements.push_back(&xScaleR);

    yScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 90, 55, 15, "y Scale"); // 20
    yScaleR.SetText("1");
    elements.push_back(&yScaleR);

    centerR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 110, 55, 15, "Center "); // 21
    centerR.SetText("(0,0)");
    elements.push_back(&centerR);

    numbersR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 130, "Numbers", true); // 22
    elements.push_back(&numbersR);

    linesR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 150, "Lines", false); // 23
    elements.push_back(&linesR);

    /// General graph-related elements
    okGraph = Button(window, inFont, window->getSize().x * 7 / 8 - 54, 175, // 24
                                108, 15, "Save Changes");
    elements.push_back(&okGraph);

    elements.push_back(&equation); // Finally push back equation so that it's at the end of the vector


    /** End GUI element creation **/

    for(int iii = 0; iii < elements.size(); iii++) {
        elements[iii]->SetActive(false);
    }
    activeBox = 25; // Start out highlighting the equation entering box
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
            if(event.text.unicode != 9 && activeBox >= 10 && activeBox != 24) { // One of the inputBoxes, wasn't tab
                if(activeBox >= 25) // Equation or variables
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
                if(activeBox == 14)
                    UpdateGraphs();
                else if(activeBox == 16)
                    UpdateEquation();
                else
                    elements[activeBox]->OnClick(event.mouseButton.x, event.mouseButton.y);
            } else { // In one of the graphs
                if(event.mouseButton.x < window->getSize().x / 2) {
                    graphCoords = grid.lGrid.WindowToGraph(event.mouseButton.x, event.mouseButton.y);
                } else {
                    graphCoords = grid.rGrid.WindowToGraph(event.mouseButton.x, event.mouseButton.y);
                }
                Iterate(!isIterating, new cx(graphCoords.x, graphCoords.y)); // Stop iterating if we were iterating, or start if we weren't
            }
        }
    }
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
        }
        for(int iii = 0; iii < numIterations + 1; iii++) { // Iterate 30 points at once, or just one
            fct->setVar("Z", locPos); // Feed our current location into parser as the variable Z
            try {
                if(iii != 0)
                    locPos = fct->eval(); // Don't change position, so we can make it black first
            }
            catch (std::invalid_argument) { // Should mean we've "reached infinity", so we can stop
                isIterating = false;
                return;
            }

            graphCoords = sf::Vector2f(locPos.real(), locPos.imag());
            sf::CircleShape newLoc = sf::CircleShape(circRad, 30);
            if(iii == numIterations)
                newLoc.setFillColor(sf::Color::Red);
            else
                newLoc.setFillColor(sf::Color::Black);
            newLoc.setPosition(grid.rGrid.GraphToPic(graphCoords) - sf::Vector2f(circRad, circRad));
            pic->draw(newLoc);
            newLoc.setPosition(grid.lGrid.GraphToPic(graphCoords) - sf::Vector2f(circRad, circRad));
            pic->draw(newLoc);

            if(iii == 0 && newPos != NULL)
                locPos = *newPos;
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
                elements[iii]->OnClick(x, y);           // For checkboxes; toggle them back
            }
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
    grid.lGrid.SetRange(xRangeL.GetStringAsDouble(),
                        yRangeL.GetStringAsDouble());
    grid.lGrid.SetScale(xScaleL.GetStringAsDouble(),
                        yScaleL.GetStringAsDouble());
    if(centerL.GetText() != "")
        grid.lGrid.SetCenter(centerL.GetStringAsVector());
    grid.lGrid.SetNumbers(numbersL.GetText() == "x");
    grid.lGrid.SetLines(linesL.GetText() == "x");

    grid.rGrid.SetRange(xRangeR.GetStringAsDouble(),
                        yRangeR.GetStringAsDouble());
    grid.rGrid.SetScale(xScaleR.GetStringAsDouble(),
                        yScaleR.GetStringAsDouble());
    if(centerR.GetText() != "")
        grid.rGrid.SetCenter(centerR.GetStringAsVector());
    grid.rGrid.SetNumbers(numbersR.GetText() == "x");
    grid.rGrid.SetLines(linesR.GetText() == "x");
    if(okGraph.GetOutlineColor() != sf::Color::Black) {
        okGraph.SetOutlineColor(sf::Color::Black);
        clearPic();
    }
}

void Runner::UpdateEquation() {
    Iterate(false);
    delete fct;
    std::string func = equation.GetText();
    for(int iii = 0; iii < func.length(); iii ++) {
        if(func[iii] == 'z')
            func.replace(iii, 1, "Z");
    }
    fct = new parser::Tree(func);
    while(elements.size() > elementsSize) {
        fct->setVar(elements[elements.size() - 1]->GetCap().substr(0, 1), elements[elements.size() - 1]->GetText());
        //delete elements[elements.size() - 1];
        elements.pop_back();
    }
    for(int iii = 'A'; iii < 'Z' && elements.size() < elementsSize + 8; iii++) { // Max 8 vars for now
        if(func.find(iii) != std::string::npos) {
            elements.push_back(new InputBox(window, inFont, 3, (elements.size() - elementsSize + 1.25) * 20,
                                        150, 15, std::string(1, (char)iii) + " "));
            elements[elements.size() - 1]->SetActive(false);
            elements[elements.size() - 1]->SetText(fct->getVarFct(std::string(1, (char)iii)));
        }
    }
    okEquation.SetOutlineColor(sf::Color::Black);
    Iterate(false);
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
        break;
    case 6: // Iterate mode
        Iterate(false);
        mode = iterative;
        modeIterate.SetOutlineColor(sf::Color::Green);
        modeSingle.SetOutlineColor(sf::Color::Black);
        break;
    case 7: // Clear
        clearPic();
        break;
    case 8: // Mirror R->L
        okGraph.SetOutlineColor(sf::Color::Red);
        xRangeL.SetText(xRangeR.GetText());
        yRangeL.SetText(yRangeR.GetText());
        xScaleL.SetText(xScaleR.GetText());
        yScaleL.SetText(yScaleR.GetText());
        centerL.SetText(centerR.GetText());
        if(numbersL.GetText() != numbersR.GetText()) numbersL.Toggle();
        if(linesL.GetText() != linesR.GetText()) linesL.Toggle();
        break;
    case 9: // Mirror L->R
        okGraph.SetOutlineColor(sf::Color::Red);
        xRangeR.SetText(xRangeL.GetText());
        yRangeR.SetText(yRangeL.GetText());
        xScaleR.SetText(xScaleL.GetText());
        yScaleR.SetText(yScaleL.GetText());
        centerR.SetText(centerL.GetText());
        if(numbersR.GetText() != numbersL.GetText()) numbersR.Toggle();
        if(linesR.GetText() != linesL.GetText()) linesR.Toggle();
        break;
    case 15: // One of the checkboxes
    case 16:
    case 22:
    case 23:
        okGraph.SetOutlineColor(sf::Color::Red);
        if(event.type == sf::Event::MouseButtonPressed)
            elements[activeBox]->OnClick(event.mouseButton.x, event.mouseButton.y);
        else
            elements[activeBox]->OnEnter();
        break;
    case 24: // Save Changes for graphs
        UpdateGraphs();
        break;
    default:
        if(event.type == sf::Event::MouseButtonPressed)
            elements[activeBox]->OnClick(event.mouseButton.x, event.mouseButton.y);
        else
            elements[activeBox]->OnEnter();
    }
}

void Runner::clearPic() {
    Iterate(false);                 // Stop iterating
    pic->clear(sf::Color::White);   // Clear the canvas (pic) to be fully white
    locPos = cx(0, 0);              // Move our last drawn point to the origin
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

    window->draw(loc); // Draw the cursor's position after one application of the current equation

    window->display(); // Display everything we've drawn on the screen
}
