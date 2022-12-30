/**
 * @file FinalProject.cpp
 * @brief Driver code
 *  */
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <math.h>
#include <string>

#include "LocationsContainer.h"

using namespace std;

/**
 * @brief Main driver function
 *
 * @return int error code (if any)
 */
int main()
{
    // Make a new instance of the main container
    LocationsContainer container;

    // Make a new render window and assign it a size and title
    sf::RenderWindow window(sf::VideoMode(869, 613), "GIKI Navigator", sf::Style::Titlebar | sf::Style::Close);

    // Place the background map onto the image
    sf::Texture backGround;
    sf::Sprite sBg;
    if (!backGround.loadFromFile("Media/Map.png"))
        cout << "File not found\n";
    sBg.setTexture(backGround);

    // Main loop of GUI
    while (window.isOpen())
    {
        // Make a new event handler
        sf::Event event;
        // Continuously check for events
        while (window.pollEvent(event))
        {
            // Classify the event
            switch (event.type)
            {
            case sf::Event::Closed: // If the close button was clicked, close the window
                window.close();
                break;
            case sf::Event::MouseButtonReleased: // If the mouse button was clicked
                // Print the click location
                cout << "Click Location: " << sf::Mouse::getPosition(window).x << "," << sf::Mouse::getPosition(window).y << std::endl;
                // Print the closest building
                cout << "BUILDING " << container.getClosestBuilding(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) << endl;
                // Adds the building to travel queue
                container.addToQueue(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            case sf::Event::KeyReleased: // If the keyboard key was presses
                if (event.key.code == sf::Keyboard::Enter)
                    // If the enter key was pressed, evaluate the queue
                    container.evaluateQueue();
                else if (event.key.code == sf::Keyboard::R)
                    // If the R key was pressed, call the reset function
                    container.reset();
            }
        }

        // Draw the background
        window.draw(sBg);
        // Call the container's display function to display all texts, buildings, paths, and junctions
        container.displayAll(window);
        // Draw the window
        window.display();
    }

    return 0;
}
