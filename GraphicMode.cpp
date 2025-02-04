#include <vector>
#include <utility>
#include <iostream>
#include <SFML\Graphics.hpp>
#include "GraphicMode.h"

sf::FloatRect GraphicMode::get_screen_size(const std::vector<std::pair<float,float>> &points) {
    float x_min = 9000.0f;
    float y_min = 9000.0f;
    float x_max = 0.0f;
    float y_max = 0.0f;
    for (const auto& p : points) {
        if (p.first > x_max)
            x_max = p.first;
        if (p.first < x_min)
            x_min = p.first;
        if (p.second > y_max)
            y_max = p.second;
        if (p.second < y_min)
            y_min = p.second;
    }
    // Add Constant to Screen
    float const_x = (x_max - x_min)*20/100;
    float const_y = (y_max - y_min)*20/100;
    if (const_x < 10) const_x = 10;
    if (const_y < 10) const_y = 10;

    // Get correct Relation between height and width
    float rectWidth = x_max+2*const_x;
    float rectHeight = y_max+2*const_y;
    if (rectWidth/rectHeight > 800.0f/600.0f) {
        // Width is bigger
        rectHeight = 600.0f*rectWidth/800.0f;
    } else {
        rectWidth = 800.0f*rectHeight/600.0f;
    }

    return {x_min-const_x, y_min-const_y , rectWidth, rectHeight};
}

void GraphicMode::start(){
    // Algorithmn
    //auto alg = Algorithm_RandomLines();
    algorithm_.setGraphicMode(true);
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Convex Hull - "+algorithm_.algorithmName);

    std::vector<std::pair<float, float>> points = algorithm_.getPoints();
    sf::View view(get_screen_size(points));
    window.setView(view);

    // clear the window with black color
    window.clear(sf::Color::White);

    std::vector<sf::CircleShape> graph_points;
    // Points
    for (const auto& point: points) {
        sf::CircleShape graph_point(2); // Radius des Punktes
        graph_point.setFillColor(sf::Color::Black);
        graph_point.setPosition(point.first - graph_point.getRadius(), point.second - graph_point.getRadius());
        graph_points.push_back(graph_point);
    }

    // Button
    sf::RectangleShape button(sf::Vector2f(150, 50));
    button.setFillColor(sf::Color(211,211,211));
    sf::Font font;
    //std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    if (!font.loadFromFile("../../fonts/arial.ttf")) {
        std::cerr << "Font konnte nicht geladen werden!" << std::endl;
        return;
    }
    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString("Start");
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::Black);

    // draw Button and Text
    sf::View uiView = window.getDefaultView();
    sf::Vector2u windowSize = window.getSize();
    button.setPosition(windowSize.x - button.getSize().x - 10, windowSize.y - button.getSize().y - 10);
    buttonText.setPosition(button.getPosition().x + 15, button.getPosition().y + 15);


    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            // Next Button clicked?
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                if (button.getGlobalBounds().contains(worldPos)) {
                    if (buttonText.getString() == "Start" || buttonText.getString() == "Restart") {
                        // Initialise Algorithm
                        algorithm_.initialize();
                        // ALgorithm Initialisieren

                        buttonText.setString("Step");
                    }
                    else {
                        // Algorithm is running - next step
                        // Algorithm Step -- Return true or false
                        bool finished = algorithm_.step();   // Result Algorithm
                        if (true == finished) {
                            buttonText.setString("Restart");
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        window.setView(view);
        // Points
        for (const auto& graph_point : graph_points) {
            window.draw(graph_point);
        }

        // Lines
        for (const auto& line: algorithm_.getLines()) {
            sf::VertexArray graph_line(sf::LinesStrip, 2);

            //graph_line.setLineColor(sf::Color::Black);
            graph_line[0].position = sf::Vector2f(line.first->first, line.first->second);
            graph_line[0].color  = sf::Color::Red;
            graph_line[1].position = sf::Vector2f(line.second->first, line.second->second);
            graph_line[1].color  = sf::Color::Red;

            window.draw(graph_line);
        }

        // Convex Hull
        for (const auto& line: algorithm_.getConvexHull()) {
            sf::VertexArray graph_line(sf::LinesStrip, 2);

            //graph_line.setLineColor(sf::Color::Black);
            graph_line[0].position = sf::Vector2f(line.first->first, line.first->second);
            graph_line[0].color  = sf::Color::Blue;
            graph_line[1].position = sf::Vector2f(line.second->first, line.second->second);
            graph_line[1].color  = sf::Color::Blue;

            window.draw(graph_line);
        }

        // Button - they are in different coordinatesystem than points and lines
        window.setView(uiView);
        window.draw(button);
        window.draw(buttonText);

        // end the current frame
        window.display();
    }
}
