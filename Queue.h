/**
 * @file Queue.h
 * @brief Header file for a queue implemented through linked list
 */
#pragma once

#include <iostream>
using namespace std;

/**
 * @brief Queue class
 *
 * @tparam T datatype of queue
 */
template <class T>
class Queue
{
private:
    struct node
    {
        T data;     // Data stored in node
        node *next; // Address to next node
    };

    node *head; // Address of first node
    node *rear; // Address of last node

public:
    /**
     * @brief Construct a new Queue object
     *
     */
    Queue()
    {
        // Initialize head and rear to NULL
        head = NULL;
        rear = NULL;
    }

    /**
     * @brief Adds an element to queue
     *
     * @param value Value to add
     */
    void enqueue(T value)
    {
        // Creates a new node and assigns data
        node *ptrNew = new node, *ptrTemp = head;
        ptrNew->data = value;
        ptrNew->next = NULL;

        // If the head is NULL, the new element is the new head and tail
        if (!head)
        {
            head = ptrNew;
            rear = ptrNew;
            return;
        }

        // else traverse the queue until finished and add the new node at the end of queue
        while (ptrTemp->next != NULL)
            ptrTemp = ptrTemp->next;
        ptrTemp->next = ptrNew;
        rear = ptrTemp->next;

        return;
    }

    /**
     * @brief Dequeues the first element from the queue and returns it
     *
     * @return T Value of dequeued element
     */
    T dequeue()
    {
        // If empty return 0
        if (isEmpty())
        {
            cout << "List empty!" << endl;
            return {0};
        }

        // else return the value of head and dequeue head
        T headval = head->data;
        head = head->next;
        return headval;
    }

    /**
     * @brief Get the first node's value
     *
     * @return T the value of first node
     */
    T getFront() { return head->data; }

    /**
     * @brief Get the last node's value
     *
     * @return T the value of last node
     */
    T getRear() { return rear->data; }

    /**
     * @brief Deletes the last node's value (Function specific to current use-case (not part of normal queue))
     *
     */
    void deleteRear()
    {
        // If empty then return
        if (isEmpty())
            return;

        // else traverse queue and delete last
        node *ptrTemp = head;
        while (ptrTemp->next != rear)
            ptrTemp = ptrTemp->next;
        rear = ptrTemp;
        ptrTemp->next = NULL;
        return;
    }

    /**
     * @brief Get the Size of the queue
     *
     * @return int size
     */
    int getSize()
    {
        // Traverses the queue while keeping count
        node *temp = head;
        int count = 0;
        while (temp)
        {
            count++;
            temp = temp->next;
        }
        // return the count
        return count;
    }

    /**
     * @brief checks if the queue is empty
     *
     * @return true if the queue is empty
     * @return false if the queue is not empty
     */
    bool isEmpty()
    {
        if (head == NULL)
            return true;

        return false;
    }

    /**
     * @brief Prints all the elements of the queue
     *
     */
    void printAll()
    {
        if (isEmpty())
            return;
        // Traverses the queue and prints each element
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
