#include <SFML/Graphics.hpp>
#include <complex>
#include <random>
#include "gui/button.h"
#include "gui/text.h"
#include "gui/checkbox.h"
#include "graph/DoubleGrid.h"
#include "runner.h"
#include <iostream>

int main1();
int main2();

int main() {
    //main1();
    main2();
    return 0;
}

int main2() {
    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    sf::RenderWindow window(sf::VideoMode(1265, 725), "Complex Paint Revamped");
    window.setPosition(sf::Vector2i(0, 0));

    Runner run = Runner(&window, &inFont);

    while(window.isOpen()) {
        run.HandleEvents();
        run.Draw();
    }
    return 0;
}

int main1() {/*
    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    /****** Main window ******/
    /*//{
    sf::RenderWindow window(sf::VideoMode(1265, 725), "Complex Paint Revamped");
    window.setPosition(sf::Vector2i(0, 0));

    InputBox equation = InputBox(&window, inFont, 1, 1, 200, 15);

    DoubleGrid grid = DoubleGrid(&window, inFont, 200);

    Button graphModify = Button(&window, inFont, window.getSize().x / 2 - 54, 140, 117, 15, "Modify Graphs");
    Button eqModify = Button(&window, inFont, 210, 2, 135, 15, "Modify Equation");

    sf::CircleShape loc = sf::CircleShape(1, 30);
    loc.setFillColor(sf::Color::Black);
    //}


    /****** Graph settings window ******/
    //{
    /*sf::RenderWindow graphOptions(sf::VideoMode(300, 200), "Graph Settings");
    graphOptions.close();

    // The OK button
    Button okGraph = Button(&window, inFont, window.getSize().x * 7 / 8 - 54, 175,
                                108, 15, "Save Changes");

    // Title and checkboxes for the input (left-hand) graph
    sf::Text inTitle = sf::Text("Left Graph", inFont, 15);
    inTitle.setPosition(window.getSize().x * 13 / 16 - 50, 5);
    inTitle.setColor(sf::Color::Black);

    Checkbox numbersI = Checkbox(&window, inFont, window.getSize().x * 3 / 4 + 5, 130, "Numbers", true);
    Checkbox linesI = Checkbox(&window, inFont, window.getSize().x * 3 / 4 + 5, 150, "Lines", false);

    // Title and input for the output (right-hand) graph
    sf::Text outTitle = sf::Text("Right Graph", inFont, 15);
    outTitle.setPosition(window.getSize().x * 15 / 16 - 55, 5);
    outTitle.setColor(sf::Color::Black);

    Checkbox numbersO = Checkbox(&window, inFont, window.getSize().x * 7 / 8 + 5, 130, "Numbers", true);
    Checkbox linesO = Checkbox(&window, inFont, window.getSize().x * 7 / 8 + 5, 150, "Lines", false);

    // Input boxes for both sides
    std::vector<InputBox> graphInputs = std::vector<InputBox>(0);
    //{
        // The number beside each input box corresponds to its number, for determining which box is "active"
        InputBox xRangeI = InputBox(&window, inFont, window.getSize().x * 3 / 4 + 5, 30, 55, 15, "x Range"); // 0
        InputBox yRangeI = InputBox(&window, inFont, window.getSize().x * 3 / 4 + 5, 50, 55, 15, "y Range"); // 1
        InputBox xScaleI = InputBox(&window, inFont, window.getSize().x * 3 / 4 + 5, 70, 55, 15, "x Scale"); // 2
        InputBox yScaleI = InputBox(&window, inFont, window.getSize().x * 3 / 4 + 5, 90, 55, 15, "y Scale"); // 3
        InputBox centerI = InputBox(&window, inFont, window.getSize().x * 3 / 4 + 5, 110, 55, 15, "Center "); // 4
        // Output-side boxes
        InputBox xRangeO = InputBox(&window, inFont, window.getSize().x * 7 / 8 + 5, 30, 55, 15, "x Range"); // 5
        InputBox yRangeO = InputBox(&window, inFont, window.getSize().x * 7 / 8 + 5, 50, 55, 15, "y Range"); // 6
        InputBox xScaleO = InputBox(&window, inFont, window.getSize().x * 7 / 8 + 5, 70, 55, 15, "x Scale"); // 7
        InputBox yScaleO = InputBox(&window, inFont, window.getSize().x * 7 / 8 + 5, 90, 55, 15, "y Scale"); // 8
        InputBox centerO = InputBox(&window, inFont, window.getSize().x * 7 / 8 + 5, 110, 55, 15, "Center "); // 9

        graphInputs.push_back(xRangeI); // We have to create the input boxes then add them to the vector in two steps
        graphInputs.push_back(yRangeI); // in order to deal with a bug where the caption isn't displayed if we do it
        graphInputs.push_back(xScaleI); // in one step
        graphInputs.push_back(yScaleI);
        graphInputs.push_back(centerI);
        graphInputs.push_back(xRangeO);
        graphInputs.push_back(yRangeO);
        graphInputs.push_back(xScaleO);
        graphInputs.push_back(yScaleO);
        graphInputs.push_back(centerO);

        for(int iii = 0; iii < graphInputs.size(); iii++) {
            graphInputs[iii].SetBoxColor(sf::Color(150, 150, 150));
        }
    //}
    //} // End graph setting window

    /****** Equation setting window ******/
    //{
    /*sf::RenderWindow eqOptions(sf::VideoMode(320, 240), "Equation and Parameters");
    eqOptions.close();

    InputBox eqEnter = InputBox(&eqOptions, inFont, 5, 5, 310, 15, "");

    //} // End equation setting window

    // Variable containing the number of the active input box
    int activeBox = 0;
    auto locPos = std::complex<double>(0.0, 0.0);
    auto factor = std::complex<double>(.37, .5);
    std::vector<sf::CircleShape> figs = std::vector<sf::CircleShape>(0);

    while(window.isOpen() || graphOptions.isOpen() || eqOptions.isOpen()) {
        sf::Event event;

        /** Event Handling for the main window **/
     /*   while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                window.close();
            } else if(event.type == sf::Event::MouseMoved) {
                sf::Vector2f graphCoords = grid.lGrid.WindowToGraph(event.mouseMove.x, event.mouseMove.y);
                loc.setPosition(grid.rGrid.GraphToWindow(graphCoords));
            } else if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) ||
                      (event.type == sf::Event::MouseButtonPressed &&
                       graphModify.OnClick(event.mouseButton.x, event.mouseButton.y))) {
                graphOptions.create(sf::VideoMode(300, 200), "Graph Settings");
                window.close();
            } else if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) ||
                      (event.type == sf::Event::MouseButtonPressed &&
                       eqModify.OnClick(event.mouseButton.x, event.mouseButton.y))) {
                eqOptions.create(sf::VideoMode(320, 240), "Equation and Parameters");
                window.close();
            } else if(event.type == sf::Event::MouseButtonPressed) {
                int sign = 1;
                sf::Vector2f graphCoords;
                if(event.mouseButton.x < window.getSize().x / 2 &&
                   event.mouseButton.y > 160) { // Only change these if we're inside the left window
                    graphCoords = grid.lGrid.WindowToGraph(event.mouseButton.x, event.mouseButton.y);
                    locPos = {graphCoords.x, graphCoords.y};
                    figs.clear();
                }
                for(int iii = 0; iii < 1000; iii++) {
                    //locPos = locPos * factor + std::complex<double>(1,5);
                    if(rand() < RAND_MAX / 2) sign = 1;
                    else sign = -1;
                    locPos = std::complex<double>(sign, 0) * sqrt(locPos + std::complex<double>(1, 0));
                    graphCoords = sf::Vector2f(locPos.real(), locPos.imag());
                    sf::CircleShape newLoc = sf::CircleShape(1, 30);
                    newLoc.setPosition(grid.rGrid.GraphToWindow(graphCoords) - sf::Vector2f(1,1));
                    newLoc.setFillColor(sf::Color::Black);
                    figs.push_back(newLoc);
                    //loc.setPosition(grid.rGrid.GraphToWindow(graphCoords));
                }
            }
        }

        /** Event Handling for the graph window **/
       /* while(graphOptions.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                window.create(sf::VideoMode(1265, 620), "Complex Paint Revamped");
                graphOptions.close();
            } else if(event.type == sf::Event::MouseMoved) {
                graphInputs[activeBox].SetBoxColor(sf::Color(150, 150, 150));
                int x = event.mouseMove.x;
                int y = event.mouseMove.y;
                if(y <= graphOptions.getSize().y * 3 / 20) y = graphOptions.getSize().y * 3 / 20 + 1;
                if(y >= graphOptions.getSize().y * 13 / 20 - 5) y = graphOptions.getSize().y * 12 / 20 + 1;
                activeBox = (y - graphOptions.getSize().y * 3 / 20) / (graphOptions.getSize().y / 10);
                if(x > graphOptions.getSize().x / 2) activeBox += 5;
                if(activeBox > 9) activeBox %= 10;
                graphInputs[activeBox].SetBoxColor(sf::Color::White);
            }  else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                graphInputs[activeBox].SetBoxColor(sf::Color(150, 150, 150));
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    activeBox = (activeBox + 9) % 10; // Decrement by 1, avoiding negatives
                else
                    activeBox = ++activeBox % 10;
                graphInputs[activeBox].SetBoxColor(sf::Color::White);
            } else if(event.type == sf::Event::TextEntered) {
                graphInputs[activeBox].OnTextEntered(event.text.unicode);
            } else if(event.type == sf::Event::MouseButtonPressed &&
                      !okGraph.OnClick(event.mouseButton.x, event.mouseButton.y)) {
                if(numbersI.OnClick(event.mouseButton.x, event.mouseButton.y))
                    numbersI.Toggle();
                if(linesI.OnClick(event.mouseButton.x, event.mouseButton.y))
                    linesI.Toggle();
                if(numbersO.OnClick(event.mouseButton.x, event.mouseButton.y))
                    numbersO.Toggle();
                if(linesO.OnClick(event.mouseButton.x, event.mouseButton.y))
                    linesO.Toggle();
                } else if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
                          (event.type == sf::Event::MouseButtonPressed &&
                           okGraph.OnClick(event.mouseButton.x, event.mouseButton.y))) {
                    grid.lGrid.SetRange(graphInputs[0].GetStringAsDouble(),
                                        graphInputs[1].GetStringAsDouble());
                    grid.lGrid.SetScale(graphInputs[2].GetStringAsDouble(),
                                        graphInputs[3].GetStringAsDouble());
                    if(graphInputs[4].GetText() != "")
                        grid.lGrid.SetCenter(graphInputs[4].GetStringAsVector());
                    grid.lGrid.SetNumbers(numbersI.GetText() == "x");
                    grid.lGrid.SetLines(linesI.GetText() == "x");

                    grid.rGrid.SetRange(graphInputs[5].GetStringAsDouble(),
                                        graphInputs[6].GetStringAsDouble());
                    grid.rGrid.SetScale(graphInputs[7].GetStringAsDouble(),
                                        graphInputs[8].GetStringAsDouble());
                    if(graphInputs[9].GetText() != "")
                        grid.rGrid.SetCenter(graphInputs[9].GetStringAsVector());
                    grid.rGrid.SetNumbers(numbersO.GetText() == "x");
                    grid.rGrid.SetLines(linesO.GetText() == "x");

                    window.create(sf::VideoMode(1265, 620), "Complex Paint Revamped");
                    graphOptions.close();
            }
        }

        /** Event Handling for the equation window **/
        /*while(eqOptions.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                window.create(sf::VideoMode(1265, 620), "Complex Paint Revamped");
                eqOptions.close();
            } else if(event.type == sf::Event::TextEntered) {
                eqEnter.OnTextEntered(event.text.unicode);
            } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
                equation.SetText(eqEnter.GetText());
                window.create(sf::VideoMode(1265, 620), "Complex Paint Revamped");
                eqOptions.close();
            }
        }

        /** Draw everything to the windows **/
        //{
       /* window.clear(sf::Color::White);
        graphOptions.clear(sf::Color::White);
        eqOptions.clear(sf::Color::White);

        // Draw everything on the main window
        equation.Draw();
        grid.Draw();

        graphModify.Draw();
        eqModify.Draw();
        for(int iii = 0; iii < figs.size(); iii++) {
            window.draw(figs[iii]);
        }

        window.draw(loc);

        // Draw everything on the graph options window
        if(true){//graphOptions.isOpen()) {
            window.draw(inTitle);
            window.draw(outTitle);
            okGraph.Draw();
            numbersI.Draw();
            linesI.Draw();
            numbersO.Draw();
            linesO.Draw();
            for(int iii = 0; iii < graphInputs.size(); iii++) {
                graphInputs[iii].Draw();
            }
        }
        xRangeI.Draw();

        // Draw everything on the equation options window
        if(eqOptions.isOpen()) {
            eqEnter.Draw();
        }
        // Display everything we've drawn
        window.display();
        graphOptions.display();
        eqOptions.display();
    //} // End drawing segment
    }
    return 0;
}
*/
