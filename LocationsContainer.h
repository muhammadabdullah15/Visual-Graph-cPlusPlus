#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

#define NUM_LOCATIONS 50
#define NUM_JUNCTIONS 100

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

void updateBuildingList(Location Buildings[NUM_LOCATIONS])
{
    cout << "Updating Buildings List\n";

    ifstream readfile;
    readfile.open("coords_locations.txt");
    for (int i = 0; !readfile.eof() && i < NUM_LOCATIONS; i++)
    {
        readfile >> Buildings[i].cordX;
        readfile >> Buildings[i].cordY;
        readfile >> Buildings[i].name;
        replace(Buildings[i].name.begin(), Buildings[i].name.end(), '-', ' ');
        // cout << "name len" << i << " : " << (Buildings[i].name).length() << endl;
        if (Buildings[i].name == "")
            break;
    }

    // for (int i = 0; i < NUM_LOCATIONS && Buildings[i].name != ""; i++)
    // {
    //     cout << "ID: " << i << endl
    //          << Buildings[i];
    // }
    readfile.close();
}

void displayMarkers(sf::RenderWindow &window, Location Buildings[NUM_LOCATIONS])
{
    sf::CircleShape marker(10.f);
    sf::Color clr(0xb33f0dff);
    marker.setFillColor(clr);

    // marker.setPosition(-10, -10);
    // window.draw(marker);

    for (int i = 0; i < NUM_LOCATIONS && Buildings[i].name != ""; i++)
    {
        // if (Buildings[i].name == "")
        //     break;
        // // cout << "Drawing" << temp << endl;
        marker.setPosition(Buildings[i].cordX - 10, Buildings[i].cordY - 10);
        window.draw(marker);
    }
    return;
}

void addPoint(int i, Location Buildings[NUM_LOCATIONS])
{
    cout << Buildings[i] << endl;
}

void click(int cordX, int cordY, Location Buildings[NUM_LOCATIONS])
{
    Location temp;
    temp.cordX = cordX;
    temp.cordY = cordY;

    for (int i = 0; i < NUM_LOCATIONS && Buildings[i].name != ""; i++)
    {
        // cout << "id:" << i << ": " << Buildings[i].name << "-" << distanceBetweenPoints(Buildings[i], temp) << endl;
        if (distanceBetweenPoints(Buildings[i], temp) >= 0 && distanceBetweenPoints(Buildings[i], temp) <= 10)
        {
            addPoint(i, Buildings);
            return;
        }
    }

    return;
}

void writeJunctions(int cordX, int cordY)
{
    ofstream writeFile;
    writeFile.open("coords_junctions.txt", ios::app);
    writeFile << cordX << ' ' << cordY << endl;
    writeFile.close();
}

void updateJunctionList(Location Junctions[NUM_JUNCTIONS])
{
    cout << "Updating Junctions List\n";
    ifstream readfile;
    readfile.open("coords_junctions.txt");
    for (int i = 0; !readfile.eof() && i < NUM_JUNCTIONS; i++)
    {
        readfile >> Junctions[i].cordX;
        readfile >> Junctions[i].cordY;
        if (Junctions[i].cordX < 30)
            break;
    }

    readfile.close();
}

void displayPaths(sf::RenderWindow &window, Location Junctions[NUM_JUNCTIONS])
{

    // sf::VertexArray polygonImage2;
    // polygonImage2.clear();
    // polygonImage2.setPrimitiveType(sf::Lines);
    // polygonImage2.resize(10);

    // float c = 1.0f;

    // polygonImage2[0] = {sf::Vector2f{100, 100}, sf::Color::Black};
    // polygonImage2[1] = {sf::Vector2f{100, 200}, sf::Color::Black};

    // polygonImage2[2] = {sf::Vector2f{100, 200}, sf::Color::Black};
    // polygonImage2[3] = {sf::Vector2f{200, 200}, sf::Color::Black};

    // polygonImage2[4] = {sf::Vector2f{200, 200}, sf::Color::Black};
    // polygonImage2[5] = {sf::Vector2f{200, 100}, sf::Color::Black};

    // polygonImage2[6] = {sf::Vector2f{100, 100}, sf::Color::Black};
    // polygonImage2[7] = {sf::Vector2f{200, 100}, sf::Color::Black};
    // // polygonImage2[4] = {sf::Vector2f{100, 100}, sf::Color::Black};

    // window.draw(polygonImage2);

    sf::VertexArray paths;
    paths.clear();
    paths.setPrimitiveType(sf::Lines);
    paths.resize(NUM_JUNCTIONS);

    // sf::VertexArray paths(sf::Lines, NUM_JUNCTIONS);
    // // sf::Color clr(0xb33f0dff);
    // paths.clear();
    paths[0] = {sf::Vector2f(Junctions[0].cordX, Junctions[0].cordY), sf::Color::White};
    // paths[1] = {sf::Vector2f(Junctions[1].cordX, Junctions[1].cordY), sf::Color::Black};

    for (int i = 1, j = 1; i < NUM_JUNCTIONS && Junctions[i].cordX > 30; i++, j += 2)
    {
        paths[j] = {sf::Vector2f(Junctions[i].cordX, Junctions[i].cordY), sf::Color::White};
        paths[j + 1] = {sf::Vector2f(Junctions[i].cordX, Junctions[i].cordY), sf::Color::White};

        // paths.append(sf::Vertex(sf::Vector2f(Junctions[i].cordX, Junctions[i].cordY), sf::Color::Red));
        // paths.append(sf::Vertex(sf::Vector2f(Junctions[i].cordX, Junctions[i].cordY), sf::Color::Red));
        // paths[i].color = sf::Color::Red;
        // paths[i].position = sf::Vector2f(Junctions[i].cordX, Junctions[i].cordY);
    }
    window.draw(paths);
    return;
}
