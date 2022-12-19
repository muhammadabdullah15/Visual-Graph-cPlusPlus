#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <math.h>
#include <string>

#include "CompletedFunctions.h"

using namespace std;
// g++ -c pr.cpp;g++ pr.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system;./sfml-app

void test(Location Junctions[NUM_JUNCTIONS])
{
    for (int i = 0; i < NUM_JUNCTIONS && Junctions[i].cordX > 30; i++)
        cout << Junctions[i];
}

int main()
{
    Location Buildings[NUM_LOCATIONS];
    Location Junctions[NUM_JUNCTIONS];
    updateBuildingList(Buildings);
    updateJunctionList(Junctions);
    test(Junctions);
    // test(Buildings);

    sf::RenderWindow window(sf::VideoMode(869, 613), "GIKI Navigator");

    sf::Texture backGround;
    sf::Sprite sBg;
    if (!backGround.loadFromFile("Map2.png"))
        std::cout << "File not found\n";

    sBg.setTexture(backGround);

    // sf::VertexArray lines(sf::LinesStrip, 2);
    // lines[0].color = lines[1].color = sf::Color::Red;
    // lines[0].position = sf::Vector2f(0, 0);
    // lines[1].position = sf::Vector2f(100, 100);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonReleased)
            {
                cout << "Click Location: " << sf::Mouse::getPosition(window).x << "," << sf::Mouse::getPosition(window).y << std::endl;
                // click(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, Buildings);
                writeJunctions(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            }

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::U)
            {
                updateJunctionList(Junctions);
                updateBuildingList(Buildings);
            }
        }

        window.draw(sBg);
        displayMarkers(window, Buildings);
        displayPaths(window, Junctions);
        window.display();
    }

    return 0;
}