#include <SFML/Graphics.hpp>
#include "gui/button.h"
#include "gui/text.h"
#include "gui/checkbox.h"
#include "graph/DoubleGrid.h"
#include <iostream>

int main() {
    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    /****** Main window ******/
    //{
    sf::RenderWindow window(sf::VideoMode(1265, 620), "Complex Paint Revamped");
    window.setPosition(sf::Vector2i(0, 100));

    InputBox equation = InputBox(&window, inFont, 1, 1, 200, 15);

    DoubleGrid grid = DoubleGrid(&window, inFont, 160);

    Button graphModify = Button(&window, inFont, window.getSize().x / 2 - 54, 140, 117, 15, "Modify Graphs");
    Button eqModify = Button(&window, inFont, 210, 2, 135, 15, "Modify Equation");

    sf::CircleShape loc = sf::CircleShape(2, 30);
    loc.setFillColor(sf::Color::Black);
    //}


    /****** Graph settings window ******/
    //{
    sf::RenderWindow graphOptions(sf::VideoMode(300, 200), "Graph Settings");
    graphOptions.close();

    // The OK button
    Button okGraph = Button(&graphOptions, inFont, graphOptions.getSize().x / 2 - 54, graphOptions.getSize().y - 20,
                                108, 15, "Save Changes");

    // Title and checkboxes for the input (left-hand) graph
    sf::Text inTitle = sf::Text("Input", inFont, 15);
    inTitle.setPosition(graphOptions.getSize().x / 4 - 25, 5);
    inTitle.setColor(sf::Color::Black);

    Checkbox numbersI = Checkbox(&graphOptions, inFont, 5, 130, "Numbers", true);
    Checkbox linesI = Checkbox(&graphOptions, inFont, 5, 150, "Lines", false);

    // Title and input for the output (right-hand) graph
    sf::Text outTitle = sf::Text("Output", inFont, 15);
    outTitle.setPosition(graphOptions.getSize().x * 3 / 4 - 30, 5);
    outTitle.setColor(sf::Color::Black);

    Checkbox numbersO = Checkbox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 130, "Numbers", true);
    Checkbox linesO = Checkbox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 150, "Lines", false);

    // Input boxes for both sides
    std::vector<InputBox> graphInputs = std::vector<InputBox>(0);
    //{
        // The number beside each input box corresponds to its number, for determining which box is "active"
        InputBox xRangeI = InputBox(&graphOptions, inFont, 5, 30, 55, 15, "x Range"); // 0
        InputBox yRangeI = InputBox(&graphOptions, inFont, 5, 50, 55, 15, "y Range"); // 1
        InputBox xScaleI = InputBox(&graphOptions, inFont, 5, 70, 55, 15, "x Scale"); // 2
        InputBox yScaleI = InputBox(&graphOptions, inFont, 5, 90, 55, 15, "y Scale"); // 3
        InputBox centerI = InputBox(&graphOptions, inFont, 5, 110, 55, 15, "Center"); // 4
        // Output-side boxes
        InputBox xRangeO = InputBox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 30, 55, 15, "x Range"); // 5
        InputBox yRangeO = InputBox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 50, 55, 15, "y Range"); // 6
        InputBox xScaleO = InputBox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 70, 55, 15, "x Scale"); // 7
        InputBox yScaleO = InputBox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 90, 55, 15, "y Scale"); // 8
        InputBox centerO = InputBox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 110, 55, 15, "Center"); // 9

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
    sf::RenderWindow eqOptions(sf::VideoMode(320, 240), "Equation and Parameters");
    eqOptions.close();

    InputBox eqEnter = InputBox(&eqOptions, inFont, 5, 5, 100, 15, "");

    //} // End equation setting window

    // Variable containing the number of the active input box
    int activeBox = 0;

    while(window.isOpen() || graphOptions.isOpen() || eqOptions.isOpen()) {
        sf::Event event;

        /** Event Handling for the main window **/
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                window.close();
            } else if(event.type == sf::Event::MouseMoved) {
                sf::Vector2f graphCoords = grid.lGrid.WindowToGraph(event.mouseMove.x, event.mouseMove.y);
                loc.setPosition(grid.rGrid.GraphToWindow(graphCoords));
                graphCoords = grid.rGrid.GraphToWindow(graphCoords);
                std::cout << "(" << graphCoords.x << "," << graphCoords.y << ")\n";
            } else if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) ||
                      (event.type == sf::Event::MouseButtonPressed &&
                       graphModify.IsPressed(event.mouseButton.x, event.mouseButton.y))) {
                    graphOptions.create(sf::VideoMode(300, 200), "Graph Settings");
                    window.close();
            } else if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) ||
                      (event.type == sf::Event::MouseButtonPressed &&
                       eqModify.IsPressed(event.mouseButton.x, event.mouseButton.y))) {
                    eqOptions.create(sf::VideoMode(320, 240), "Equation and Parameters");
                    window.close();
            }
        }

        /** Event Handling for the graph window **/
        while(graphOptions.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                window.create(sf::VideoMode(1265, 620), "Complex Paint Revamped");
                graphOptions.close();
            } else if(event.type == sf::Event::MouseMoved) {
                graphInputs[activeBox].SetBoxColor(sf::Color(150, 150, 150));
                int x = event.mouseMove.x;
                int y = event.mouseMove.y;
                if(y <= graphOptions.getSize().y * 3 / 20) y = graphOptions.getSize().y * 3 / 20 + 1;
                if(y >= graphOptions.getSize().y * 13 / 20) y = graphOptions.getSize().y * 12 / 20 + 1;
                std::cout << y << "\n";
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
                graphInputs[activeBox].EnterText(event.text.unicode);
            } else if(event.type == sf::Event::MouseButtonPressed &&
                      !okGraph.IsPressed(event.mouseButton.x, event.mouseButton.y)) {
                if(numbersI.IsPressed(event.mouseButton.x, event.mouseButton.y))
                    numbersI.Toggle();
                if(linesI.IsPressed(event.mouseButton.x, event.mouseButton.y))
                    linesI.Toggle();
                if(numbersO.IsPressed(event.mouseButton.x, event.mouseButton.y))
                    numbersO.Toggle();
                if(linesO.IsPressed(event.mouseButton.x, event.mouseButton.y))
                    linesO.Toggle();
                } else if((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
                          (event.type == sf::Event::MouseButtonPressed &&
                           okGraph.IsPressed(event.mouseButton.x, event.mouseButton.y))) {
                    grid.lGrid.SetRange(graphInputs[0].GetStringAsInt(),
                                        graphInputs[1].GetStringAsInt());
                    grid.lGrid.SetScale(graphInputs[2].GetStringAsInt(),
                                        graphInputs[3].GetStringAsInt());
                    if(graphInputs[4].GetStoredString() != "")
                        grid.lGrid.SetCenter(graphInputs[4].GetStringAsVector());
                    grid.lGrid.SetNumbers(numbersI.IsToggled());
                    grid.lGrid.SetLines(linesI.IsToggled());

                    grid.rGrid.SetRange(graphInputs[5].GetStringAsInt(),
                                        graphInputs[6].GetStringAsInt());
                    grid.rGrid.SetScale(graphInputs[7].GetStringAsInt(),
                                        graphInputs[8].GetStringAsInt());
                    if(graphInputs[9].GetStoredString() != "")
                        grid.rGrid.SetCenter(graphInputs[9].GetStringAsVector());
                    grid.rGrid.SetNumbers(numbersO.IsToggled());
                    grid.rGrid.SetLines(linesO.IsToggled());

                    window.create(sf::VideoMode(1265, 620), "Complex Paint Revamped");
                    graphOptions.close();
            }
        }

        /** Event Handling for the equation window **/
        while(eqOptions.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                window.create(sf::VideoMode(1265, 620), "Complex Paint Revamped");
                eqOptions.close();
            } else if(event.type == sf::Event::TextEntered) {
                eqEnter.EnterText(event.text.unicode);
            } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
                equation.SetString(eqEnter.GetStoredString());
                window.create(sf::VideoMode(1265, 620), "Complex Paint Revamped");
                eqOptions.close();
            }
        }

        /** Draw everything to the windows **/
        //{
        window.clear(sf::Color::White);
        graphOptions.clear(sf::Color::White);
        eqOptions.clear(sf::Color::White);

        // Draw everything on the main window
        equation.Draw();
        grid.Draw();

        graphModify.Draw();
        eqModify.Draw();

        window.draw(loc);

        // Draw everything on the graph options window
        if(graphOptions.isOpen()) {
            graphOptions.draw(inTitle);
            graphOptions.draw(outTitle);
            okGraph.Draw();
            numbersI.Draw();
            linesI.Draw();
            numbersO.Draw();
            linesO.Draw();
            for(int iii = 0; iii < graphInputs.size(); iii++) {
                graphInputs[iii].Draw();
            }
        }

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
