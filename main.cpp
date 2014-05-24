#include <SFML/Graphics.hpp>
#include "gui/button.h"
#include "gui/text.h"
#include "gui/checkbox.h"
#include "graph/DoubleGrid.h"
#include <iostream>

int main() {
    sf::Font inFont;
    if(!inFont.loadFromFile("VeraMono.ttf")){/*error handling*/}

    // Main Window
    sf::RenderWindow window(sf::VideoMode(1265, 620), "Complex Paint Revamped");
    window.setPosition(sf::Vector2i(0, 100));

    InputBox equation = InputBox(&window, inFont, 1, 1, 200, 15);

    DoubleGrid grid = DoubleGrid(&window, inFont, 160);

    Button graphModify = Button(&window, inFont, window.getSize().x / 2 - 54, 140, 117, 15, "Modify Graphs");
    Button eqModify = Button(&window, inFont, 210, 2, 135, 15, "Modify Equation");

    sf::CircleShape loc = sf::CircleShape(2, 30);
    loc.setFillColor(sf::Color::Black);

    // Graph settings window
    sf::RenderWindow graphOptions(sf::VideoMode(300, 200), "Graph Settings");
    graphOptions.setVisible(true);

    // The OK button
    Button okGraph = Button(&graphOptions, inFont, graphOptions.getSize().x / 2 - 54, graphOptions.getSize().y - 20,
                                108, 15, "Save Changes");

    // Title and input for the input (left-hand) graph
    sf::Text inTitle = sf::Text("Input", inFont, 15);
    inTitle.setPosition(graphOptions.getSize().x / 4 - 25, 5);
    inTitle.setColor(sf::Color::Black);

    InputBox xRangeI = InputBox(&graphOptions, inFont, 5, 30, 50, 15, "x Range");
    InputBox yRangeI = InputBox(&graphOptions, inFont, 5, 50, 50, 15, "y Range");
    InputBox xScaleI = InputBox(&graphOptions, inFont, 5, 70, 50, 15, "x Scale");
    InputBox yScaleI = InputBox(&graphOptions, inFont, 5, 90, 50, 15, "y Scale");
    InputBox centerI = InputBox(&graphOptions, inFont, 5, 110, 50, 15, "Center");
    Checkbox numbersI = Checkbox(&graphOptions, inFont, 5, 130, "Numbers", true);
    Checkbox linesI = Checkbox(&graphOptions, inFont, 5, 150, "Lines", false);

    // Title and input for the output (right-hand) graph
    sf::Text outTitle = sf::Text("Output", inFont, 15);
    outTitle.setPosition(graphOptions.getSize().x * 3 / 4 - 30, 5);
    outTitle.setColor(sf::Color::Black);

    InputBox xRangeO = InputBox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 30, 50, 15, "x Range");
    InputBox yRangeO = InputBox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 50, 50, 15, "y Range");
    InputBox xScaleO = InputBox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 70, 50, 15, "x Scale");
    InputBox yScaleO = InputBox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 90, 50, 15, "y Scale");
    InputBox centerO = InputBox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 110, 50, 15, "Center");
    Checkbox numbersO = Checkbox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 130, "Numbers", true);
    Checkbox linesO = Checkbox(&graphOptions, inFont, graphOptions.getSize().x / 2 + 5, 150, "Lines", false);


    // Equation setting window
    sf::RenderWindow eqOptions(sf::VideoMode(320, 240), "Equation and Parameters");
    eqOptions.setVisible(false);

    // Coordinates of the last known mouse position; for tracking which inputBox is "active"
    int mouseX = 0;
    int mouseY = 0;

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) { // Main window Loop
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                window.close();
            } else if(event.type == sf::Event::MouseMoved) {
                loc.setPosition(window.getSize().x / 2 + event.mouseMove.x - 1, event.mouseMove.y - 1);
            } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                grid.lGrid.ToggleLines();
                grid.rGrid.ToggleLines();
            } else if(event.type == sf::Event::MouseButtonPressed) {
                if(graphModify.IsPressed(event.mouseButton.x, event.mouseButton.y)) {
                    graphOptions.setVisible(true);
                }
                else if(eqModify.IsPressed(event.mouseButton.x, event.mouseButton.y)) {
                    eqOptions.setVisible(true);
                }
            }
        }

        while(graphOptions.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                graphOptions.setVisible(false);
            } else if(event.type == sf::Event::MouseMoved) {
                mouseX = event.mouseButton.x;
                mouseY = event.mouseButton.y;
            } else if(event.type == sf::Event::TextEntered) {
                /*if(mouseX <= graphOptions.getSize().y / 2 ) { // On the left side: input
                    if(mouseY < 45)
                        xRangeI.EnterText(event.text.unicode);
                    else if (mouseY < 65)
                        yRangeI.EnterText(event.text.unicode);
                }*/
            }
        }

        while(eqOptions.pollEvent(event)) {
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed &&
                                                   event.key.code == sf::Keyboard::Escape)) {
                eqOptions.setVisible(false);
            }
        }

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
        graphOptions.draw(inTitle);
        graphOptions.draw(outTitle);
        okGraph.Draw();

        xRangeI.Draw();
        yRangeI.Draw();
        xScaleI.Draw();
        yScaleI.Draw();
        centerI.Draw();
        numbersI.Draw();
        linesI.Draw();

        xRangeO.Draw();
        yRangeO.Draw();
        xScaleO.Draw();
        yScaleO.Draw();
        centerO.Draw();
        numbersO.Draw();
        linesO.Draw();


        // Display everything we've drawn
        window.display();
        graphOptions.display();
        eqOptions.display();
    }
    return 0;
}
