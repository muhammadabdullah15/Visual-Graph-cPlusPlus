#pragma once

#include <iostream>
using namespace std;

class Queue
{
private:
    struct node
    {
        int data;
        node *next;
    };

    node *head;
    node *rear;

public:
    Queue()
    {
        head = NULL;
        rear = NULL;
    }

    void enqueue(int value)
    {
        node *ptrNew = new node, *ptrTemp = head;
        ptrNew->data = value;
        ptrNew->next = NULL;
        if (head == NULL)
        {
            head = ptrNew;
            rear = ptrNew;
            return;
        }
        while (ptrTemp->next != NULL)
            ptrTemp = ptrTemp->next;
        ptrTemp->next = ptrNew;
        rear = ptrTemp->next;

        return;
    }

    int dequeue()
    {
        if (isEmpty())
        {
            cout << "List empty!" << endl;
            return -1;
        }

        int headval = head->data;
        head = head->next;
        return headval;
    }

    bool isEmpty()
    {
        if (head == NULL)
            return true;
        else
            return false;
    }

    void getfirst()
    {
        cout << "First value: " << head->data;
    }

    void getlast()
    {
        cout << "Last value: " << rear->data;
    }

    void printAll()
    {
        node *ptrTemp = head;
        cout << endl;
        while (ptrTemp != NULL)
        {
            cout << ptrTemp->data << (ptrTemp->next == NULL ? "" : ",");
            ptrTemp = ptrTemp->next;
        }

        return;
    }
};
