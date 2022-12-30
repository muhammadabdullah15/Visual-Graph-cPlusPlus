/**
 * @file LocationsContainer.h
 * @brief Header file to store & manage all saved locations and related functions
 */
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <math.h>
#include <string>

#include "GraphAdjacencyList.h" //Graph used for storage of Locations and dijkstra's algorithm for shortest path
#include "Queue.h"              //Queue used to store points to visit
#include "Stack.h"              //Stack used to reverse data when stored in reverse

using namespace std;
/**
 * @brief Location struct to store all the locations with coordinates & names
 *
 */
struct Location
{
    string name; // Name of location
    int cordX;   // X-coordinate
    int cordY;   // Y-coordinate
};

/**
 * @brief Overloaded operator to display formatted Location data
 *
 * @param os Stream
 * @param l Location to display
 * @return ostream& stream
 */
ostream &operator<<(ostream &os, const Location &l)
{
    os << "Name:\t" << l.name << endl
       << "Coords:\t(" << l.cordX << "," << l.cordY << ")" << endl;
    return os;
}

/**
 * @brief Function to calculate distance between two points
 *
 * @param l1    Location 1
 * @param l2    Location 2
 * @return int  Distance
 */
int distanceBetweenPoints(Location l1, Location l2)
{
    return sqrt(pow(l1.cordX - l2.cordX, 2) + pow((l1.cordY - l2.cordY), 2));
}

/**
 * @brief Object-oriented approach to store all data and related functions
 *
 */
class LocationsContainer
{
private:
    Location *Buildings; // Stores all buildings
    Location *Junctions; // Stores all junctions
    Location *Paths;     // Stores paths to display

    UndirectedGraph *graph; // Combines junctions & buildings while also storing distances as weights

    Queue<int> *travelQueue;           // Stores the buildings added to queue by user through GUI
    Queue<int> *travelPath;            // Stores detailed shortest path after using Dijkstra's algorithm
    Queue<Location> *visualPathPoints; // Stores the converted coordinates from the travelPath

    string displayText; //   Text to display
    bool displayPaths;  // Whether to display the text or not

    int NUM_BUILDINGS; // Total number of buildings
    int NUM_JUNCTIONS; // Total number of junctions
    int NUM_PATHS;     // Total number of paths in final drawing array

public:
    LocationsContainer()
    {
        NUM_BUILDINGS = NUM_JUNCTIONS = 0;                          // Initialize buildings & junctions to 0
        displayPaths = false;                                       // Do not display text initially
        updateBuildingList();                                       // Updates building list
        updateJunctionList();                                       // Updates junction list
        graph = new UndirectedGraph(NUM_BUILDINGS + NUM_JUNCTIONS); // Create a graph for buildings and junctions combined
        travelQueue = new Queue<int>;                               // Make a queue for user to add buildings to
        reset();                                                    // Resets the text and sets to not display paths
        visualPathPoints = new Queue<Location>;                     // Make a queue for storing coordinates of path points
        addJunctionsToGraph();                                      // Add junctions with weights to graph
    }

    /**
     * @brief Reads locations's file and stores in Buildings array
     *
     */
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
        }

        readfile.close();
    }

    /**
     * @brief Reads junction's file and stores in junction array
     *
     */
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

    /**
     * @brief Reads junction connections file and stores in graph
     *
     */
    void addJunctionsToGraph()
    {
        ifstream readfile;
        readfile.open("coords_junctionConnections.txt");

        int dataSets, source, destination, temp, distance, cordX, cordY;

        // Read the total number of datasets
        readfile >> dataSets;

        for (int i = 0; i <= dataSets; i++)
        {
            // Initialize current dataset's distance to 0
            distance = 0;
            // Read the source, destination, and the number of steps to get there
            readfile >> source >> destination >> temp;

            // Make a temp array to store the read coordinates for later calculation and populate it
            Location *tempLocations = new Location[temp];
            for (int j = 0; j < temp; j++)
                readfile >> tempLocations[j].cordX >> tempLocations[j].cordY;

            // Add distance between the first and second point to the distance variable
            if (source >= NUM_BUILDINGS) // If the path travels through a junction, get its coordinates from junctions array
                distance += distanceBetweenPoints(tempLocations[0], Junctions[source - NUM_BUILDINGS]);
            else // else get its coordinates from buildings array
                distance += distanceBetweenPoints(tempLocations[0], Buildings[source]);

            // Calculate and add all distances between locations to distance variable
            for (int k = 0; k < temp - 1; k++)
                distance += distanceBetweenPoints(tempLocations[k], tempLocations[k + 1]);

            // Output the read data
            cout << "DATASET: " << i + 1 << "\tS: " << source << "\tD: " << destination << "\tDISTANCE: " << distance << endl;

            // Add the data to the graph
            graph->addConnection(source, destination, distance);

            // Delete the temp array
            delete[] tempLocations;
        }

        readfile.close();
        return;
    }

    /**
     * @brief Adds user clicked location to travel queue
     *
     * @param cordX x-coordinates
     * @param cordY y-coordinates
     */
    void addToQueue(int cordX, int cordY)
    {
        // If the clicked location isn't near any building, return
        if (getClosestBuilding(cordX, cordY) == -1)
            return;

        // Get the id of closest building
        int building = getClosestBuilding(cordX, cordY);

        // Change the display text
        if (travelQueue->isEmpty())
            displayText = "Click on another node to add it to the travel queue OR \nPress 'Enter' to get shortest path OR\nPress 'R' to reset to beginning\n\nCurrent travel list:";
        displayText += ("\n" + Buildings[building].name);

        // Add the building to travel queue
        travelQueue->enqueue(building);
    }

    /**
     * @brief Evaluates the travel queue by applying dijkstra's algorithm to the points and generating a detailed path through all junctions
     *
     */
    void evaluateQueue()
    {
        // If the queue was empty when user pressed evaluate button, display error message
        if (travelQueue->isEmpty())
        {
            displayText = "Add nodes to queue before evaluating!\nPress 'R' to reset";
            return;
        }

        // Initialize a source variable with the first element of queue
        int source = travelQueue->dequeue();

        // Make a new queue for storing detailed travel path passing through all junctions
        travelPath = new Queue<int>;

        // Dequeue one element at a time from travel queue and
        // Calculate shortest path between source and destination using dijkstra's algorithm
        // Dijkstra's algorithm adds the resulting path to the passed queue i.e, travelPath
        while (!travelQueue->isEmpty())
        {
            int destination = travelQueue->dequeue();
            graph->dijkstra(source, destination, travelPath);
            if (!travelQueue->isEmpty())
                travelPath->deleteRear();
            source = destination;
        }

        // Change the display text to show output and instructions
        displayText = "The shortest path from (" + Buildings[travelPath->getFront()].name + ") to (" + Buildings[travelPath->getRear()].name + ") is highlighted below!\n\nPress 'R' to reset to beginning";

        // Calls the function to convert id's of locations/junctions to coordinates for gui to display
        makeVisualPath();

        // Adds the last point (destination) to the queue
        visualPathPoints->enqueue(Buildings[travelPath->getRear()]);

        // Converts the queue to normal array for function to displat
        convertToArray();

        return;
    }

    /**
     * @brief Converts the queue from ID type to Coordinates
     *
     */
    void makeVisualPath()
    {
        // Make a temp array to store all coordinates
        Location *all = new Location[NUM_BUILDINGS + NUM_JUNCTIONS];
        for (int i = 0; i < NUM_BUILDINGS; i++)
            all[i] = Buildings[i];
        for (int i = 0; i < NUM_JUNCTIONS; i++)
            all[i + NUM_JUNCTIONS] = Junctions[i];

        ifstream readfile;

        // Add the first point to variable
        int tempSource = travelPath->dequeue();
        int tempDest;

        // For each point in the travelPath, dequeue it and add its coordinates from source to destinations
        while (!travelPath->isEmpty())
        {
            // Get the destination which is the next node in the queue
            tempDest = travelPath->dequeue();

            // Read file to search the pixel values for each traversal
            readfile.open("coords_junctionConnections.txt");

            // Initialize the variables used later
            int dataSets, source, destination, temp, distance, cordX, cordY;
            Location tempL;

            // Read total number of datasets
            readfile >> dataSets;

            // Traverse through all datasets to search
            for (int i = 0; i <= dataSets; i++)
            {
                // Read the source and destination and the number of pixel sets it takes to get there
                readfile >> source >> destination >> temp;

                // If the source and destination match the searched values
                if (source == tempSource && destination == tempDest)
                {
                    // enqueue the start coordinate
                    visualPathPoints->enqueue(all[tempSource]);
                    // and all following coordinates to the queue
                    for (int j = 0; j <= temp; j++)
                    {
                        readfile >> tempL.cordX >> tempL.cordY;
                        visualPathPoints->enqueue(tempL);
                    }
                    // Delete the last coordinate set to avoid duplication
                    visualPathPoints->deleteRear();

                    // break from loop to search the next set of locations
                    break;
                }
                // else if the source and destinations are flipped from the searched values
                else if (source == tempDest && destination == tempSource)
                {
                    // First, put all the following coordinate sets into a temporary stack
                    Stack<Location> *tempStack = new Stack<Location>;
                    for (int j = 0; j < temp; j++)
                    {
                        readfile >> tempL.cordX >> tempL.cordY;
                        tempStack->push(tempL);
                    }
                    // then, pop from stack and into the queue (this reverses the data so the source and destination match the searched values)
                    visualPathPoints->enqueue(all[tempSource]);
                    while (!tempStack->isEmpty())
                        visualPathPoints->enqueue(tempStack->pop());

                    // delete the temporary stack
                    delete[] tempStack;

                    // break from loop to search the next set of locations
                    break;
                }
                // Else if nothing matches, skip the data to search the next set of coordinates
                else
                    for (int j = 0; j < temp; j++)
                        readfile >> tempL.cordX >> tempL.cordY;
            }
            readfile.close();
            // Set the next source
            tempSource = tempDest;
        }

        // Delete the temporary array
        delete[] all;
        return;
    }

    /**
     * @brief Converts the visualPathPoints Queue to an array for display purposes
     *
     */
    void convertToArray()
    {
        // Get the total coordinate sets in the queue
        NUM_PATHS = visualPathPoints->getSize();

        // Initialize the path array with the size
        Paths = new Location[NUM_PATHS];

        // Finally, add all the visualPathPoints to the array one at a time
        for (int i = 0; !visualPathPoints->isEmpty(); i++)
            Paths[i] = visualPathPoints->dequeue();

        // Set variable to true to draw the paths stored in array
        displayPaths = true;
    }

    /**
     * @brief Reset the display text and set the display path variable to false to hide path
     *
     */
    void reset()
    {
        displayPaths = false;
        displayText = "Click on a node to add it to the travel queue!";
        return;
    }

    /**
     * @brief Draws the buildings on the map as points
     *
     * @param window window to draw on
     */
    void displayBuildings(sf::RenderWindow &window)
    {
        // Make a new circle shape with a radius, and assign it a color
        sf::CircleShape marker(10.f);
        sf::Color clr(0xb33f0dff);
        marker.setFillColor(clr);

        // For all buildings, move the marker to their coordinates and draw them
        for (int i = 0; i < NUM_BUILDINGS; i++)
        {
            marker.setPosition(Buildings[i].cordX - 10, Buildings[i].cordY - 10);
            window.draw(marker);
        }
        return;
    }

    /**
     * @brief Draws the junction points on the map as points
     *
     * @param window window to draw on
     */
    void displayJunctions(sf::RenderWindow &window)
    {
        // Make a new circle shape with a radius, and assign it a color
        sf::CircleShape marker(2.f);
        marker.setFillColor(sf::Color::Green);

        // For all junctions, move the marker to their coordinates and draw them
        for (int i = 0; i < NUM_JUNCTIONS; i++)
        {
            marker.setPosition(Junctions[i].cordX - 2, Junctions[i].cordY - 2);
            window.draw(marker);
        }

        return;
    }

    /**
     * @brief Draws the instructional text on the map
     *
     * @param window window to draw on
     */
    void displayTextInfo(sf::RenderWindow &window)
    {
        // Make a new text and load a font into it
        sf::Text text;
        sf::Font font;
        if (!font.loadFromFile("Media/Montserrat-Medium.ttf"))
        {
            cout << "ERROR LOADING FONT";
            return;
        }
        text.setFont(font);

        // Set the text to display the current display text
        text.setString(displayText);

        // Draw the text with a specified size and color
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        window.draw(text);
    }

    /**
     * @brief Draws the calculated path on the map
     *
     * @param window window to draw on
     */
    void displayPath(sf::RenderWindow &window)
    {
        // Make a new path array
        sf::VertexArray paths;

        // Clear the paths and redefine the type
        paths.clear();
        paths.setPrimitiveType(sf::Lines);
        paths.resize(1);

        // Add the first point to the path array
        paths[0] = {sf::Vector2f(Paths[0].cordX, Paths[0].cordY), sf::Color::White};

        // Append the rest of the points
        for (int i = 1; i < NUM_PATHS; i++)
        {
            paths.append(sf::Vector2f(Paths[i].cordX, Paths[i].cordY));
            paths.append(sf::Vector2f(Paths[i].cordX, Paths[i].cordY));
        }

        // Draw the paths
        window.draw(paths);
        return;
    }

    /**
     * @brief Function which calls all the othe functions to display
     *
     * @param window window to display on
     */
    void displayAll(sf::RenderWindow &window)
    {
        displayBuildings(window);
        displayJunctions(window);
        displayTextInfo(window);
        if (displayPaths)
            displayPath(window);
    }

    /**
     * @brief Get the Closest Building's ID
     *
     * @param cordX x-coordinate of clicked position
     * @param cordY y-coordinate of clicked position
     * @return int id of building
     */
    int getClosestBuilding(int cordX, int cordY)
    {
        // Create a temp location and save the clicked coordinates into it
        Location temp;
        temp.cordX = cordX;
        temp.cordY = cordY;

        // Travel the buildings array and search for the coordinates
        for (int i = 0; i < NUM_BUILDINGS; i++)
        {
            // If the clicked location was within 10 units of a building, return its ID
            if (distanceBetweenPoints(Buildings[i], temp) >= 0 && distanceBetweenPoints(Buildings[i], temp) <= 10)
            {
                cout << Buildings[i];
                return i;
            }
        }

        // If not found, return -1
        return -1;
    }

    /**
     * @brief Get the Closest Junction's ID
     *
     * @param cordX x-coordinate of clicked position
     * @param cordY y-coordinate of clicked position
     * @return int id of junctions
     */
    int getClosestJunction(int cordX, int cordY)
    {
        // Create a temp location and save the clicked coordinates into it
        Location temp;
        temp.cordX = cordX;
        temp.cordY = cordY;

        // Travel the Junctions array and search for the coordinates
        for (int i = 0; i < NUM_JUNCTIONS; i++)
            // If the clicked location was within 10 units of a junctions, return its ID
            if (distanceBetweenPoints(Junctions[i], temp) >= 0 && distanceBetweenPoints(Junctions[i], temp) <= 20)
                return i + NUM_BUILDINGS;

        // If not found, return -1
        return -1;
    }

    /**
     * @brief Print all the Building details for testing
     *
     */
    void testPrintBuildings()
    {
        cout << "BUILDING LIST:" << endl
             << "Number of buildings=" << NUM_BUILDINGS << endl;

        for (int i = 0; i < NUM_BUILDINGS; i++)
            cout << Buildings[i] << endl;
    }

    /**
     * @brief Print all the Junction details for testing
     *
     */
    void testPrintJunctions()
    {
        cout << "JUNCTIONS LIST:" << endl
             << "Number of Junctions=" << NUM_JUNCTIONS << endl;

        for (int i = 0; i < NUM_JUNCTIONS; i++)
            cout << Junctions[i] << endl;
    }
};