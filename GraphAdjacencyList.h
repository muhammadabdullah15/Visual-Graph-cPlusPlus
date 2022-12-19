#pragma once
#include <iostream>
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

        if (sourceList[source].search(destination))
            return;

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

        if (sourceList[source].search(destination) != 0)
            return;

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

    void BFS(int start)
    {
        if (start >= vertices)
        {
            cout << "Start vertex out of range!\n";
            return;
        }

        Queue q;

        int visited[vertices] = {0}, levels[vertices] = {0};
        visited[start] = 1;
        levels[start] = 0;

        q.enqueue(start);
        cout << start << ", ";
        while (!q.isEmpty())
        {
            int temp = q.dequeue();
            for (int i = 0; i < vertices; i++)
            {
                if (sourceList[temp].search(i) >= 1 && !visited[i])
                {
                    cout << i << ", ";
                    levels[i] = levels[temp] + 1;
                    visited[i] = 1;
                    q.enqueue(i);
                }
            }
        }

        cout << endl;
        for (int level = 0; level < vertices - 1; level++)
        {
            cout << "Level " << level << " nodes: ";
            for (int i = 0; i < vertices; i++)
                if (levels[i] == level)
                    cout << i << ",";
            cout << endl;
            // cout << "\nLevel of " << i << ": " << levels[i];
        }

        return;
    }

    int countNodesAtLevel(int start, int level)
    {
        if (start >= vertices)
        {
            cout << "Start vertex out of range!\n";
            return 0;
        }

        if (level == 0)
            return 1;

        Queue q;

        int visited[vertices] = {0}, levels[vertices] = {0};
        visited[start] = 1;
        levels[start] = 0;

        q.enqueue(start);

        while (!q.isEmpty())
        {
            int temp = q.dequeue();
            for (int i = 0; i < vertices; i++)
            {
                if (sourceList[temp].search(i) >= 1 && !visited[i])
                {
                    levels[i] = levels[temp] + 1;
                    visited[i] = 1;
                    q.enqueue(i);
                }
            }
        }

        int count = 0;
        for (int i = 0; i < vertices; i++)
            if (levels[i] == level)
                count++;

        return count;
    }
};