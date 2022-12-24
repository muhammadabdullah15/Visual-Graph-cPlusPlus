#pragma once
#include <iostream>
#include <bits/stdc++.h>
#include "Queue.h"

using namespace std;

class Edges
{
private:
    struct edge
    {
        int destination, weight;
        edge *next;
    };
    edge *head;

public:
    Edges()
    {
        head = NULL;
    }

    void addDestination(int dest, int weight)
    {
        edge *newEdge = new edge;
        newEdge->destination = dest;
        newEdge->weight = weight;
        newEdge->next = NULL;

        if (!head)
        {
            head = newEdge;
            return;
        }

        edge *temp = head;
        while (temp->next)
            temp = temp->next;

        temp->next = newEdge;
        return;
    }

    int search(int dest)
    {
        edge *temp = head;
        while (temp)
        {
            if (temp->destination == dest)
                return temp->weight;
            temp = temp->next;
        }

        return 0;
    }

    void print()
    {
        edge *temp = head;
        while (temp)
        {
            cout << temp->destination << "(" << temp->weight << ") ->";
            temp = temp->next;
        }

        return;
    }
};

class UndirectedGraph
{
private:
    const int vertices;
    Edges *sourceList;

public:
    UndirectedGraph(int vert) : vertices(vert) { sourceList = new Edges[vert]; }

    void addConnection(int source, int destination, int weight)
    {
        if (source >= vertices || destination >= vertices)
        {
            cout << "Source/Destination vertex out of range!\n";
            return;
        }

        // if (sourceList[source].search(destination))
        //     return;

        sourceList[source].addDestination(destination, weight);
        sourceList[destination].addDestination(source, weight);
    }

    void addConnection(int source, int destination)
    {
        if (source >= vertices || destination >= vertices)
        {
            cout << "Source/Destination vertex out of range!\n";
            return;
        }

        // if (sourceList[source].search(destination) != 0)
        //     return;

        sourceList[source].addDestination(destination, 1);
        sourceList[destination].addDestination(source, 1);
    }

    void displayGraph()
    {
        cout << "ADJACENCY LIST:\n";
        for (int i = 0; i < vertices; i++)
        {
            cout << i << ":\t";
            sourceList[i].print();
            cout << endl;
        }
        cout << endl;
    }

    void dijkstra(int source)
    {
        int *shortestDistances, *parents;
        bool *added;
        shortestDistances = new int[vertices];
        parents = new int[vertices];
        added = new bool[vertices];

        for (int i = 0; i < vertices; i++)
        {
            shortestDistances[i] = 10000;
            added[i] = false;
        }

        shortestDistances[source] = 0;
        parents[source] = -1;

        for (int i = 1; i < vertices; i++)
        {
            int nearest = -1;
            int shortestDist = 100000;
            for (int j = 0; j < vertices; j++)
            {
                if (!added[j] && shortestDistances[j] < shortestDist)
                {
                    nearest = j;
                    shortestDist = shortestDistances[j];
                }
            }

            added[nearest] = true;
            cout << nearest << endl;
            for (int j = 0; j < vertices; j++)
            {
                cout << nearest << "," << j << endl;
                int edgeDist = sourceList[nearest].search(j);
                if (edgeDist > 0 && ((shortestDist + edgeDist) < shortestDistances[j]))
                {
                    parents[j] = nearest;
                    shortestDistances[j] = shortestDist + edgeDist;
                }
            }
        }
        solution(source, shortestDistances, parents);
    }

    void printPath(int vertex, int *parents)
    {
        // cout << "V Received:\t" << vertex << "\n";
        if (vertex == -1)
            return;
        printPath(parents[vertex], parents);
        cout << vertex << " ";
    }

    void solution(int start, int *distances, int *parents)
    {
        // cout << "Parents:\n";
        // for (int i = 0; i < vertices; i++)
        //     cout << "Parents of " << i << "\t" << parents[i] << endl;

        cout << "Vertex\t Distance\tPath";
        for (int i = 0; i < vertices; i++)
        {
            if (i != start)
            {
                cout << endl
                     << start << " -> " << i << "\t\t" << distances[i] << "\t\t";
                // cout << "V Sent:\t" << i << "\n";
                printPath(i, parents);
            }
        }
        cout << endl
             << endl;
    }
};