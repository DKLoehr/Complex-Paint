#include "runner.h"

Runner::Runner(sf::RenderWindow* w, sf::Font* font) :
    window(w),
    inFont(font)
{
    Init();
}


void Runner::Init() {
    isIterating = false;
    mode = drawMode::single;
    drawGUI = true;
    window->clear(sf::Color::White);

    grid = DoubleGrid(window, *inFont, 200);
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

    /** Graph-related elements **/
    /// Button to copy changes from the right side to the left
    mirrorL = Button(window, inFont, lTitle.getPosition().x + ((std::string)lTitle.getString()).length() * 10 + 5,
                     lTitle.getPosition().y, 15, 15, "<");
    elements.push_back(&mirrorL);

    mirrorR = Button(window, inFont, mirrorL.GetPosition().x + 20, mirrorL.GetPosition().y, 15, 15, ">");
    elements.push_back(&mirrorR);

    /// Left-side elements
    xRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 30, 55, 15, "x Range"); // 0
    xRangeL.SetText("2");
    elements.push_back(&xRangeL);

    yRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 50, 55, 15, "y Range"); // 1
    yRangeL.SetText("2");
    elements.push_back(&yRangeL);

    xScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 70, 55, 15, "x Scale"); // 2
    xScaleL.SetText("1");
    elements.push_back(&xScaleL);

    yScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 90, 55, 15, "y Scale"); // 3
    yScaleL.SetText("1");
    elements.push_back(&yScaleL);

    centerL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 110, 55, 15, "Center "); // 4
    centerL.SetText("(0,0)");
    elements.push_back(&centerL);

    numbersL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 130, "Numbers", true); // 5
    elements.push_back(&numbersL);

    linesL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 150, "Lines", false);    // 6
    elements.push_back(&linesL);

    /// Right-side elements
    xRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 30, 55, 15, "x Range"); // 7
    xRangeR.SetText("2");
    elements.push_back(&xRangeR);

    yRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 50, 55, 15, "y Range"); // 8
    yRangeR.SetText("2");
    elements.push_back(&yRangeR);

    xScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 70, 55, 15, "x Scale"); // 9
    xScaleR.SetText("1");
    elements.push_back(&xScaleR);

    yScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 90, 55, 15, "y Scale"); // 10
    yScaleR.SetText("1");
    elements.push_back(&yScaleR);

    centerR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 110, 55, 15, "Center "); // 11
    centerR.SetText("(0,0)");
    elements.push_back(&centerR);

    numbersR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 130, "Numbers", true); // 12
    elements.push_back(&numbersR);

    linesR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 150, "Lines", false); // 13
    elements.push_back(&linesR);

    /// General graph-related elements
    okGraph = Button(window, inFont, window->getSize().x * 7 / 8 - 54, 175, // 14
                                108, 15, "Save Changes");
    elements.push_back(&okGraph);


    /** Equation-related elements **/
    equation = InputBox(window, inFont, 5, 5, 350, 15); // 15
    elements.push_back(&equation);

    okEquation = Button(window, inFont, equation.GetPosition().x + equation.GetSize().x + 7, equation.GetPosition().y,
                        108, 15, "Save Changes"); // 16
    elements.push_back(&okEquation);


    /** Preset-related elements **/
    presetLin = Button(window, inFont, window->getSize().x * .35, 30, 56, 15, "Linear");  // 17 -- Az + B
    elements.push_back(&presetLin);

    presetQuad = Button(window, inFont, presetLin.GetPosition().x + presetLin.GetSize().x + window->getSize().x/50, 30,
                        81, 15, "Quadratic");   // 18 -- z*2 + c
    elements.push_back(&presetQuad);

    presetInv = Button(window, inFont, presetQuad.GetPosition().x + presetQuad.GetSize().x + window->getSize().x/50, 30,
                       109, 15, "Inverse Quad"); // 19 -- sqrt(z*2 - c)
    elements.push_back(&presetInv);

    presetPol = Button(window, inFont, presetInv.GetPosition().x + presetInv.GetSize().x + window->getSize().x/50, 30,
                       46, 15, "Polar");        // 20 -- Polar equation
    elements.push_back(&presetPol);


    /** Drawing mode-related elements **/
    modeSingle = Button(window, inFont, window->getSize().x / 2 - 105, 93, 45, 15, "Point");  // 21
    elements.push_back(&modeSingle);

    modeIterate = Button(window, inFont, window->getSize().x / 2 + 45, modeSingle.GetPosition().y,
                                65, 15, "Iterate"); // 22
    elements.push_back(&modeIterate);

    /// Clear graph button
    clearGraphs = Button(window, inFont, window->getSize().x / 2 - 23, 175, 46, 15, "Clear"); // 23
    elements.push_back(&clearGraphs);


    /** End GUI element creation **/

    for(int iii = 0; iii < elements.size(); iii++) {
        elements[iii]->SetActive(false);
    }
    activeBox = 15;
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
            drawGUI = true;
            elements[activeBox]->OnTextEntered(event.text.unicode);
        } else if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
                  (event.type == sf::Event::MouseButtonPressed && event.mouseButton.y < 200)) {
            ActivateButtons(event);
        } else if(event.type == sf::Event::MouseMoved) {
            if(event.mouseMove.y < 200) // Upper part of the screen
                SetActiveElement(event.mouseMove.x, event.mouseMove.y);
            else { // In one of the graphs
                loc.setFillColor(sf::Color::White);
                window->draw(loc);
                loc.setFillColor(sf::Color::Black);
                if(event.mouseMove.x < window->getSize().x / 2) {
                    loc.setPosition(grid.rGrid.GraphToWindow(grid.lGrid.WindowToGraph(event.mouseMove.x,
                                                                                      event.mouseMove.y)));
                } else {
                    loc.setPosition(grid.lGrid.GraphToWindow(grid.rGrid.WindowToGraph(event.mouseMove.x,
                                                                                      event.mouseMove.y)));
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
                    leftToRight = true;
                    graphCoords = grid.lGrid.WindowToGraph(event.mouseButton.x, event.mouseButton.y);
                } else {
                    leftToRight = false;
                    graphCoords = grid.rGrid.WindowToGraph(event.mouseButton.x, event.mouseButton.y);
                }
                locPos = std::complex<double>(graphCoords.x, graphCoords.y);
                Iterate(!isIterating); // Stop iterating if we were iterating, or start if we weren't
            }
        }
    }
    Iterate(isIterating);
}

void Runner::Iterate(bool keepIterating) {
    isIterating = keepIterating;
    if(keepIterating) {
        int numIterations = 1;
        if(mode == drawMode::single) {
            numIterations = 1;
            isIterating = false;
        }
        else if (mode == drawMode::iterative)
            numIterations = 30;
        for(int iii = 0; iii < numIterations; iii++) { // Iterate 30 points at once, or just one
            int sign = 1;
            if(rand() < RAND_MAX / 2)
                sign = 1;
            else
                sign = -1;
            locPos = std::complex<double>(sign, 0) * sqrt(locPos - std::complex<double>(-1, 0));
            graphCoords = sf::Vector2f(locPos.real(), locPos.imag());
            sf::CircleShape newLoc = sf::CircleShape(1, 30);
            if(leftToRight)
                newLoc.setPosition(grid.rGrid.GraphToWindow(graphCoords) - sf::Vector2f(1, 1));
            else
                newLoc.setPosition(grid.lGrid.GraphToWindow(graphCoords) - sf::Vector2f(1, 1));
            leftToRight = !leftToRight; // Flip which grid is the input and which is the output
            newLoc.setFillColor(sf::Color::Black);
            window->draw(newLoc);
        }
    }
}

void Runner::SetActiveElement(double x, double y) {
    elements[activeBox]->SetActive(false);
    for(int iii = 0; iii < elements.size(); iii++) {
        elements[iii]->OnClick(x, y);
        if(elements[iii]->OnClick(x, y)) {
            activeBox = iii;
        }
    }
    elements[activeBox]->SetActive(true);
}

void Runner::StepActiveElement(bool increment) {
    elements[activeBox]->SetActive(false);
    if(increment)
        activeBox = ++activeBox % elements.size();
    else
        activeBox = (activeBox + elements.size() - 1) % elements.size(); // Decrement by one modularly
    elements[activeBox]->SetActive(true);
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
    Iterate(false);
    window->clear(sf::Color::White);
    grid.Draw();
}

void Runner::UpdateEquation() {
    return; // Will pass equation and variables to parser
}

void Runner::ActivateButtons(sf::Event event) {
    switch(activeBox) {
    case 0: // Mirror R->L
        xRangeL.SetText(xRangeR.GetText());
        yRangeL.SetText(yRangeR.GetText());
        xScaleL.SetText(xScaleR.GetText());
        yScaleL.SetText(yScaleR.GetText());
        centerL.SetText(centerR.GetText());
        if(numbersL.GetText() != numbersR.GetText()) numbersL.Toggle();
        if(linesL.GetText() != linesR.GetText()) linesL.Toggle();
        break;
    case 1: // Mirror L->R
        xRangeR.SetText(xRangeL.GetText());
        yRangeR.SetText(yRangeL.GetText());
        xScaleR.SetText(xScaleL.GetText());
        yScaleR.SetText(yScaleL.GetText());
        centerR.SetText(centerL.GetText());
        if(numbersR.GetText() != numbersL.GetText()) numbersR.Toggle();
        if(linesR.GetText() != linesL.GetText()) linesR.Toggle();
        break;
    case 16: // Save Changes for graphs
        UpdateGraphs();
        break;
    case 17: // Save Changes for equation
        UpdateEquation();
        break;
    case 19: // Linear Preset
        equation.SetText("A*z + B");
        UpdateEquation();
        break;
    case 20: // Quadratic Preset
        equation.SetText("z^2 + C");
        UpdateEquation();
        break;
    case 21: // Inverse Quadratic Preset
        equation.SetText("sqrt(z - C)");
        UpdateEquation();
        break;
    case 22: // Polar Preset
        equation.SetText("(R*cos(2*PI*T)+R*sin(2*PI*T)*i)*z+B");
        UpdateEquation();
        break;
    case 23: // Single point mode
        mode = drawMode::single;
        break;
    case 24: // Iterate mode
        mode = drawMode::iterative;
        break;
    case 25: // Clear
        Iterate(false);
        window->clear(sf::Color::White);
        grid.Draw();
        break;
    default:
        if(event.type == sf::Event::MouseButtonPressed)
            elements[activeBox]->OnClick(event.mouseButton.x, event.mouseButton.y);
        else
            elements[activeBox]->OnEnter();
    }
    drawGUI = true;
}

void Runner::Draw() {
    //window->clear(sf::Color::White);

    if(drawGUI) {
        drawGUI = false;
        sf::RectangleShape titleRect(sf::Vector2f(300, 25));
        titleRect.setPosition(lTitle.getPosition() - sf::Vector2f(5, 5));
        titleRect.setFillColor(sf::Color::White);
        window->draw(titleRect);

        window->draw(lTitle);
        window->draw(rTitle);

        for(int iii = 0; iii < elements.size(); iii++) {
            elements[iii]->DrawWhite();
        }
        for(int iii = 0; iii < elements.size(); iii++) {
            elements[iii]->Draw();
        }
    }
    grid.DrawTextless();
    window->draw(loc);

    window->display();
}
