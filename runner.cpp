#include "runner.h"

Runner::Runner(sf::RenderWindow* w, sf::Font* font) :
    window(w),
    inFont(font)
{
    Init();
}


void Runner::Init() {
    isIterating = false;
    grid = DoubleGrid(window, *inFont, 200);

    locPos = std::complex<double>(0, 0);
    graphCoords = sf::Vector2f(0, 0);

    lTitle = sf::Text("Left Graph", *inFont, 15);
    lTitle.setPosition(window->getSize().x * 13 / 16 - 50, 5);
    lTitle.setColor(sf::Color::Black);

    rTitle = sf::Text("Right Graph", *inFont, 15);
    rTitle.setPosition(window->getSize().x * 15 / 16 - 55, 5);
    rTitle.setColor(sf::Color::Black);

    elements = std::vector<GUI*>(0);

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

    // Output-side boxes
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

    okGraph = Button(window, inFont, window->getSize().x * 7 / 8 - 54, 175, // 14
                                108, 15, "Save Changes");
    elements.push_back(&okGraph);

    equation = InputBox(window, inFont, 2, 2, 200, 15); // 15
    elements.push_back(&equation);

    okEquation = Button(window, inFont, equation.GetPosition().x + equation.GetSize().x + 7, equation.GetPosition().y,
                        108, 15, "Save Changes"); // 16
    elements.push_back(&okEquation);

    activeBox = 15;
    for(int iii = 0; iii < elements.size(); iii++) {
        elements[iii]->SetActive(false);
    }
    elements[activeBox]->SetActive(true);

    points = std::vector<sf::CircleShape>(0);

    loc = sf::CircleShape(1, 30);
    loc.setFillColor(sf::Color::Black);
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
        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
            if(activeBox == 14)
                UpdateGraphs();
            else if(activeBox == 16)
                UpdateEquation();
            else
                elements[activeBox]->OnEnter();
        } else if(event.type == sf::Event::MouseMoved) {
            if(event.mouseMove.y < 200) // Upper part of the screen
                SetActiveElement(event.mouseMove.x, event.mouseMove.y);
            else // In one of the graphs
                loc.setPosition(grid.rGrid.GraphToWindow(grid.lGrid.WindowToGraph(event.mouseMove.x, event.mouseMove.y)));
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
    if(!isIterating && keepIterating) { // Starting a new iteration
        points.clear();
    }
    isIterating = keepIterating;
    if(keepIterating) {
        for(int iii = 0; iii < 30; iii++) { // Iterate 20 points at once
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
            leftToRight = !leftToRight; // Flip which grid is the inp
            newLoc.setFillColor(sf::Color::Black);
            points.push_back(newLoc);
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
    std::cout << activeBox << "\n";
    elements[activeBox]->SetActive(true);
}

void Runner::UpdateGraphs() {
    points.clear();
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
}

void Runner::UpdateEquation() {
    return; // Will pass equation and variables to parser
}

void Runner::Draw() {
    window->clear(sf::Color::White);

    window->draw(lTitle);
    window->draw(rTitle);
    for(int iii = 0; iii < elements.size(); iii++) {
        elements[iii]->Draw();
    }

    grid.Draw();
    for(int iii = 0; iii < points.size(); iii++) {
        window->draw(points[iii]);
    }
    window->draw(loc);

    window->display();
}
