#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

// #define NUM_BUILDINGS 50
// #define NUM_JUNCTIONS 100

struct Location
{
    string name;
    int cordX, cordY;
};

ostream &operator<<(ostream &os, const Location &l)
{
    os << "Name:\t" << l.name << endl
       << "Coords:\t(" << l.cordX << "," << l.cordY << ")" << endl;
    return os;
}

int distanceBetweenPoints(Location l1, Location l2)
{
    return sqrt(pow(l1.cordX - l2.cordX, 2) + pow((l1.cordY - l2.cordY), 2));
}

class LocationsContainer
{
private:
    Location *Buildings;
    Location *Junctions;

    int NUM_BUILDINGS;
    int NUM_JUNCTIONS;

public:
    LocationsContainer()
    {
        NUM_BUILDINGS = NUM_JUNCTIONS = 0;
    }

    void updateBuildingList()
    {
        cout << "Updating Buildings List\n";

        ifstream readfile;
        readfile.open("coords_locations.txt");

        if (NUM_BUILDINGS)
            delete[] Buildings;

        readfile >> NUM_BUILDINGS;
        Buildings = new Location[NUM_BUILDINGS];

        for (int i = 0; !readfile.eof() && i < NUM_BUILDINGS; i++)
        {
            readfile >> Buildings[i].cordX;
            readfile >> Buildings[i].cordY;
            readfile >> Buildings[i].name;
            replace(Buildings[i].name.begin(), Buildings[i].name.end(), '-', ' ');
            // cout << "name len" << i << " : " << (Buildings[i].name).length() << endl;
        }

        readfile.close();
    }

    void updateJunctionList()
    {
        cout << "Updating Junctions List\n";
        ifstream readfile;
        readfile.open("coords_junctions.txt");

        if (NUM_JUNCTIONS)
            delete[] Junctions;

        readfile >> NUM_JUNCTIONS;
        Junctions = new Location[NUM_JUNCTIONS];

        for (int i = 0; !readfile.eof() && i < NUM_JUNCTIONS; i++)
        {
            readfile >> Junctions[i].cordX;
            readfile >> Junctions[i].cordY;
        }

        readfile.close();
    }

    void displayBuildings(sf::RenderWindow &window)
    {
        sf::CircleShape marker(10.f);
        sf::Color clr(0xb33f0dff);
        marker.setFillColor(clr);

        for (int i = 0; i < NUM_BUILDINGS; i++)
        {
            marker.setPosition(Buildings[i].cordX - 10, Buildings[i].cordY - 10);
            window.draw(marker);
        }
        return;
    }

    void displayPaths(sf::RenderWindow &window)
    {

        sf::VertexArray paths;
        paths.clear();
        paths.setPrimitiveType(sf::Lines);
        paths.resize(1);
        paths[0] = {sf::Vector2f(Junctions[0].cordX, Junctions[0].cordY), sf::Color::White};

        for (int i = 1; i < NUM_JUNCTIONS; i++)
        {
            paths.append(sf::Vector2f(Junctions[i].cordX, Junctions[i].cordY));
            paths.append(sf::Vector2f(Junctions[i].cordX, Junctions[i].cordY));
        }
        window.draw(paths);
        return;
    }

    void displayAll(sf::RenderWindow &window)
    {
        displayBuildings(window);
        displayPaths(window);
    }

    void click(int cordX, int cordY)
    {
        Location temp;
        temp.cordX = cordX;
        temp.cordY = cordY;

        for (int i = 0; i < NUM_BUILDINGS; i++)
        {
            // cout << "id:" << i << ": " << Buildings[i].name << "-" << distanceBetweenPoints(Buildings[i], temp) << endl;
            if (distanceBetweenPoints(Buildings[i], temp) >= 0 && distanceBetweenPoints(Buildings[i], temp) <= 10)
            {
                // addPoint(i, Buildings);
                return;
            }
        }

        return;
    }

    void testPrintBuildings()
    {
        cout << "BUILDING LIST:" << endl
             << "Number of buildings=" << NUM_BUILDINGS << endl;

        for (int i = 0; i < NUM_BUILDINGS; i++)
            cout << Buildings[i] << endl;
    }

    void testPrintJunctions()
    {
        cout << "JUNCTIONS LIST:" << endl
             << "Number of Junctions=" << NUM_JUNCTIONS << endl;

        for (int i = 0; i < NUM_JUNCTIONS; i++)
            cout << Junctions[i] << endl;
    }

    void writeJunctions(int cordX, int cordY)
    {

        ofstream writeFile;
        writeFile.open("coords_junctions.txt", ios::out);

        writeFile << NUM_JUNCTIONS + 1 << endl;

        for (int i = 0; i < NUM_JUNCTIONS; i++)
            writeFile << Junctions[i].cordX << ' ' << Junctions[i].cordY << endl;

        NUM_JUNCTIONS++;
        writeFile << cordX << ' ' << cordY << endl;

        writeFile.close();

        updateJunctionList();
    }
};
