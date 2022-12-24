#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <math.h>
#include <string>

#include "LocationsContainer.h"

using namespace std;
// g++ -c pr.cpp;g++ pr.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system;./sfml-app

int main()
{
    LocationsContainer container;

    sf::RenderWindow window(sf::VideoMode(869, 613), "GIKI Navigator", sf::Style::Titlebar | sf::Style::Close);

    sf::Texture backGround;
    sf::Sprite sBg;
    if (!backGround.loadFromFile("Map.png"))
        cout << "File not found\n";

    sBg.setTexture(backGround);

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
                cout << "BUILDING " << container.getClosestBuilding(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) << endl;
                cout << "JUNCTION " << container.getClosestJunction(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) << endl;
                container.writeJunctions(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            }

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::U)
            {
                // container.updateJunctionList();
                // updateBuildingList(Buildings);
            }
        }

        window.draw(sBg);
        container.displayAll(window);
        window.display();
    }

    return 0;
}