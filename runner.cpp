#include "runner.h"

Runner::Runner(sf::RenderWindow* w, sf::Font font) :
    window(w),
    inFont(font)
{
    Init();
}


void Runner::Init() {
    grid = DoubleGrid(window, inFont, 200);

    elements = std::vector<GUI*>(0);

    xRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 30, 55, 15, "x Range"); // 0
    elements.push_back(&xRangeL);

    yRangeL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 50, 55, 15, "y Range"); // 1
    elements.push_back(&yRangeL);

    xScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 70, 55, 15, "x Scale"); // 2
    elements.push_back(&xScaleL);

    yScaleL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 90, 55, 15, "y Scale"); // 3
    elements.push_back(&yScaleL);

    centerL = InputBox(window, inFont, window->getSize().x * 3 / 4 + 5, 110, 55, 15, "Center "); // 4
    elements.push_back(&centerL);

    numbersL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 130, "Numbers", true); // 5
    elements.push_back(&numbersL);

    linesL = Checkbox(window, inFont, window->getSize().x * 3 / 4 + 5, 150, "Lines", false);    // 6
    elements.push_back(&linesL);

    // Output-side boxes
    xRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 30, 55, 15, "x Range"); // 5
    elements.push_back(&xRangeR);

    yRangeR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 50, 55, 15, "y Range"); // 6
    elements.push_back(&yRangeR);

    xScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 70, 55, 15, "x Scale"); // 7
    elements.push_back(&xScaleR);

    yScaleR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 90, 55, 15, "y Scale"); // 8
    elements.push_back(&yScaleR);

    centerR = InputBox(window, inFont, window->getSize().x * 7 / 8 + 5, 110, 55, 15, "Center "); // 9
    elements.push_back(&centerR);

    numbersR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 130, "Numbers", true);
    elements.push_back(&numbersR);

    linesR = Checkbox(window, inFont, window->getSize().x * 7 / 8 + 5, 150, "Lines", false);
    elements.push_back(&linesR);

    okGraph = Button(window, inFont, window->getSize().x * 7 / 8 - 54, 175,
                                108, 15, "Save Changes");
    elements.push_back(&okGraph);

    equation = InputBox(window, inFont, 1, 1, 200, 15);
    elements.push_back(&equation);


    loc = sf::CircleShape(1, 30);
    loc.setFillColor(sf::Color::Black);
}

void Runner::HandleEvents() {
    sf::Event event;
    while(window->pollEvent(event)) {
        if(event.type == sf::Event::Closed ||
           event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            window->close();
    }
}

void Runner::Draw() {
    window->clear(sf::Color::White);

    for(int iii = 0; iii < elements.size(); iii++) {
        elements[iii]->Draw();
    }

    grid.Draw();
    window->draw(loc);

    window->display();
}
