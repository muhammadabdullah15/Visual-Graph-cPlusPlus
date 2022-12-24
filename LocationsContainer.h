#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <math.h>
#include <string>

#include "GraphAdjacencyList.h"
#include "Queue.h"

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
    UndirectedGraph *graph;
    string displayText;

    Queue<int> *travelQueue;
    Queue<int> *travelPath;

    int NUM_BUILDINGS;
    int NUM_JUNCTIONS;

public:
    LocationsContainer()
    {
        displayText = "Click on a node to add it to the travel queue!";
        NUM_BUILDINGS = NUM_JUNCTIONS = 0;
        updateBuildingList();
        updateJunctionList();
        graph = new UndirectedGraph(NUM_BUILDINGS + NUM_JUNCTIONS);
        travelQueue = new Queue<int>;
        travelPath = new Queue<int>;
        // cout << "B=" << NUM_BUILDINGS << "J=" << NUM_JUNCTIONS << endl;
        addJunctionsToGraph();
        // testPrintJunctions();
        // graph->dijkstra(0);
        // graph->displayGraph();
        // test();
    }

    void testD(int s, int d)
    {
        Queue<int> temp;
        graph->dijkstra(s, d, &temp);
        temp.printAll();
        return;
    }

    void addToPath(int cordX, int cordY)
    {
        if (getClosestBuilding(cordX, cordY) == -1)
            return;

        int building = getClosestBuilding(cordX, cordY);
        if (travelQueue->isEmpty())
            displayText = "Click on another node to add it to the travel queue OR \nPress 'Enter' to get shortest path OR\nPress 'R' to reset to beginning\n\nCurrent travel list:";
        displayText += ("\n" + Buildings[building].name);
        travelQueue->enqueue(building);
        travelQueue->printAll();
    }

    void evaluateQueue()
    {
        if (travelQueue->isEmpty())
        {
            displayText = "Add nodes to queue before evaluating!";
            return;
        }

        int source = travelQueue->dequeue();

        while (!travelQueue->isEmpty())
        {
            int destination = travelQueue->dequeue();
            graph->dijkstra(source, destination, travelPath);
            travelPath->dequeue();
            source = destination;
        }

        displayText = "The shortest path from (" + Buildings[travelQueue->getFront()].name + ") to (" + Buildings[travelQueue->getRear()].name + ") is highlighted below!\n\nPress 'R' to reset to beginning";
    }

    void resetTravelQueue()
    {
        while (!travelPath->isEmpty())
            travelPath->dequeue();

        displayText = "Click on a node to add it to the travel queue!";
        return;
    }

    void addJunctionsToGraph()
    {
        ifstream readfile;
        readfile.open("coords_junctionConnections.txt");

        int dataSets, source, destination, temp, distance, cordX, cordY;
        readfile >> dataSets;
        Location *tempLocations;

        // cout << "dataSets" << dataSets << endl;

        for (int i = 0; i < dataSets; i++)
        {
            distance = 0;
            readfile >> source >> destination >> temp;

            tempLocations = new Location[temp];

            for (int j = 0; j < temp; j++)
                readfile >> tempLocations[j].cordX >> tempLocations[j].cordY;

            for (int k = 0; k < temp - 1; k++)
            {
                // cout << "distance between " << i << " & " << i + 1 << ": " << distanceBetweenPoints(tempLocations[k], tempLocations[k + 1]) << endl;
                distance += distanceBetweenPoints(tempLocations[k], tempLocations[k + 1]);
            }

            // cout << "DATASET: " << i + 1 << "\tS: " << source << "\tD: " << destination << "\tDIST: " << distance << endl;
            if (distance == 0)
                graph->addConnection(source, destination);
            else
                graph->addConnection(source, destination, distance);

            delete[] tempLocations;

            // for (int k = 0; k < temp; k++)
            // cout << tempLocations[k];
        }

        readfile.close();
        // graph->displayGraph();
        return;
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

        int temp;
        for (int i = 0; !readfile.eof() && i < NUM_JUNCTIONS; i++)
        {
            readfile >> temp;
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

    void displayJunctions(sf::RenderWindow &window)
    {
        sf::CircleShape marker(2.f);
        marker.setFillColor(sf::Color::Green);

        for (int i = 0; i < NUM_JUNCTIONS; i++)
        {
            marker.setPosition(Junctions[i].cordX - 2, Junctions[i].cordY - 2);
            window.draw(marker);
        }
        return;
        return;
    }

    void displayTextInfo(sf::RenderWindow &window)
    {
        sf::Text text;
        sf::Font font;
        if (!font.loadFromFile("Media/Montserrat-Medium.ttf"))
        {
            cout << "ERROR LOADING FONT";
            return;
        }
        text.setFont(font);
        text.setString(displayText);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        window.draw(text);
    }

    void displayAll(sf::RenderWindow &window)
    {
        displayBuildings(window);
        // displayPaths(window);
        displayJunctions(window);
        displayTextInfo(window);
    }

    int getClosestBuilding(int cordX, int cordY)
    {
        Location temp;
        temp.cordX = cordX;
        temp.cordY = cordY;

        for (int i = 0; i < NUM_BUILDINGS; i++)
        {
            // cout << "id:" << i << ": " << Buildings[i].name << "-" << distanceBetweenPoints(Buildings[i], temp) << endl;
            if (distanceBetweenPoints(Buildings[i], temp) >= 0 && distanceBetweenPoints(Buildings[i], temp) <= 10)
            {
                cout << Buildings[i];
                return i;
            }
        }

        return -1;
    }

    int getClosestJunction(int cordX, int cordY)
    {
        Location temp;
        temp.cordX = cordX;
        temp.cordY = cordY;

        for (int i = 0; i < NUM_JUNCTIONS; i++)
        {
            // cout << "id:" << i << ": " << Buildings[i].name << "-" << distanceBetweenPoints(Buildings[i], temp) << endl;
            if (distanceBetweenPoints(Junctions[i], temp) >= 0 && distanceBetweenPoints(Junctions[i], temp) <= 20)
                return i + NUM_BUILDINGS;
        }

        return -1;
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
        writeFile.open("new.txt", ios::app);

        // writeFile << NUM_JUNCTIONS + 1 << endl;

        // for (int i = 0; i < NUM_JUNCTIONS; i++)
        // writeFile << Junctions[i].cordX << ' ' << Junctions[i].cordY << endl;

        // NUM_JUNCTIONS++;
        int temp = getClosestBuilding(cordX, cordY);
        if (temp != -1)
            writeFile << temp << "\t";
        else
        {
            temp = getClosestJunction(cordX, cordY);
            if (temp != -1)
                writeFile << temp << "\t";
        }
        writeFile << cordX << ' ' << cordY << endl;

        writeFile.close();

        // updateJunctionList();
    }
};
