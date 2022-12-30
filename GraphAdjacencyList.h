/**
 * @file GraphAdjacencyList.h
 * @brief Header file for Graph implementation using adjacency list
 */
#pragma once
#include <iostream>
#include <bits/stdc++.h>
#include "Queue.h"
using namespace std;

/**
 * @brief Edges Class to store edge data
 */
class Edges
{
private:
    struct edge
    {
        int destination; // Destination vertex of the edge
        int weight;      // Weight of the edge
        edge *next;      // Address to the next edge
    };
    edge *head; // Address to the next edge node

public:
    /**
     * @brief Construct a new Edges object
     *
     */
    Edges() { head = NULL; }

    /**
     * @brief Adds a new edge node
     *
     * @param dest  Destination vertex
     * @param weight Weight of the edge
     */
    void addDestination(int dest, int weight)
    {
        // Creates a new edge and assigns data
        edge *newEdge = new edge;
        newEdge->destination = dest;
        newEdge->weight = weight;
        newEdge->next = NULL;

        // If the list is empty, the new node is the head
        if (!head)
        {
            head = newEdge;
            return;
        }

        // else traverse the list and add the new node to the end of the list
        edge *temp = head;
        while (temp->next)
            temp = temp->next;

        temp->next = newEdge;
        return;
    }

    /**
     * @brief Searches the list for a particular destination and returns the weight of the edge
     *
     * @param dest  Destination vertex to search
     * @return int  return weight
     */
    int search(int dest)
    {
        // Traverse the list and searches for the destination
        edge *temp = head;
        while (temp)
        {
            // when found, return the weight
            if (temp->destination == dest)
                return temp->weight;
            temp = temp->next;
        }

        // else return 0 if no connection
        return 0;
    }

    /**
     * @brief Prints out all the edges
     *
     */
    void print()
    {
        // Traverses the list and prints each node's data
        edge *temp = head;
        while (temp)
        {
            cout << temp->destination << "(" << temp->weight << ") ->";
            temp = temp->next;
        }

        return;
    }
};

/**
 * @brief Graph class
 *
 */
class UndirectedGraph
{
private:
    const int vertices; // Number of vertices in the graph
    Edges *sourceList;  // Edges array

    /**
     * @brief Utility function for dijkstra's - Generates Path queue recursively
     *
     * @param vertex    source vertex
     * @param parents   parents of each vertex
     * @param path      path queue
     */
    void getPath(int vertex, int *parents, Queue<int> *path)
    {
        // Base case: The passed vertex doesn't have a further parent
        if (vertex == -1)
            return;

        // Recursively get the parents of each passed vertex until there are no parents
        getPath(parents[vertex], parents, path);

        // Enqueue the vertex to the queue
        path->enqueue(vertex);
    }

public:
    /**
     * @brief Construct a new Undirected Graph object
     *
     * @param vert initialise the number of vertices
     */
    UndirectedGraph(int vert) : vertices(vert) { sourceList = new Edges[vert]; }

    /**
     * @brief adds a new connection in the graph by adding a destination to a source's edge list
     *
     * @param source            source vertex
     * @param destination       destination vertex
     * @param weight            weight
     */
    void addConnection(int source, int destination, int weight)
    {
        // If the passed parameters are out of range, print error message and return
        if (source >= vertices || destination >= vertices)
        {
            cout << "Source/Destination vertex out of range!\n";
            return;
        }

        // Else add an edge from source to destination and destination to source
        sourceList[source].addDestination(destination, weight);
        sourceList[destination].addDestination(source, weight);
    }

    /**
     * @brief displays the graph's adjacency's list
     *
     */
    void displayGraph()
    {
        cout << "ADJACENCY LIST:\n";
        // For each vertex in the graph, print it's edge list
        for (int i = 0; i < vertices; i++)
        {
            cout << i << ":\t";
            sourceList[i].print();
            cout << endl;
        }
        cout << endl;
    }

    /**
     * @brief Dijkstra's algorithm to calculate shortest path between source and destiantion
     *
     * @param source        Source vertex
     * @param destination   Destination vertex
     * @param path          Path Queue
     */
    void dijkstra(int source, int destination, Queue<int> *path)
    {
        // Make a new array to store shortest distances and parents of each vertex
        int *shortestDistances, *parents;
        // Make a new array to keep track of visited vertices
        bool *added;

        // Initialize all the arrays
        shortestDistances = new int[vertices];
        parents = new int[vertices];
        added = new bool[vertices];
        for (int i = 0; i < vertices; i++)
        {
            shortestDistances[i] = 10000;
            added[i] = false;
        }

        // Set the source's distance to 0
        shortestDistances[source] = 0;
        // Set the source's parents to empty
        parents[source] = -1;

        // For all vertices
        for (int i = 1; i < vertices; i++)
        {
            // Set nearest vertex to empty and shortest distance to maximum value
            int nearest = -1;
            int shortestDist = 100000;

            // For all vertices
            for (int j = 0; j < vertices; j++)
            {
                // If the vertex was not added and the shortest distance of the vertex is less than the overall shortest distance
                if (!added[j] && shortestDistances[j] < shortestDist)
                {
                    // Set the nearest vertex to current vertex
                    nearest = j;
                    // Set the current shortest distance to this vertex's shortest distance
                    shortestDist = shortestDistances[j];
                }
            }

            // Set the new nearest vertex to added
            added[nearest] = true;

            // For all vertices
            for (int j = 0; j < vertices; j++)
            {
                // Set the edge's distance to the nearest node's weight
                int edgeDist = sourceList[nearest].search(j);
                // If their is a connection between the edge and the sum of traversed path is less than the current stored shortest distances
                if (edgeDist > 0 && ((shortestDist + edgeDist) < shortestDistances[j]))
                {
                    // Replace the parents of the vertex with the nearest vertex
                    parents[j] = nearest;
                    // Replace the shortest distances with the traversal path
                    shortestDistances[j] = shortestDist + edgeDist;
                }
            }
        }

        // Call the utility function to enqueue the points to path queue
        getPath(destination, parents, path);
    }
};