#pragma once

#include <iostream>
using namespace std;

template <class T>
class Queue
{
private:
    struct node
    {
        T data;
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

    void enqueue(T value)
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

    T dequeue()
    {
        if (isEmpty())
        {
            cout << "List empty!" << endl;
            return {0};
        }

        T headval = head->data;
        head = head->next;
        return headval;
    }

    void deleteRear()
    {
        if (isEmpty())
            return;
        node *ptrTemp = head;
        while (ptrTemp->next != rear)
            ptrTemp = ptrTemp->next;
        rear = ptrTemp;
        ptrTemp->next = NULL;
        return;
    }

    T getFront() { return head->data; }

    T getRear() { return rear->data; }

    bool isEmpty()
    {
        if (head == NULL)
            return true;

        return false;
    }

    void printAll()
    {
        if (isEmpty())
            return;
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
