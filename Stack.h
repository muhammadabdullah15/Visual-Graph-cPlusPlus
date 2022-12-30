/**
 * @file Stack.h
 * @brief Header file for a stack implemented using linked list
 */

#pragma once

#include <iostream>
using namespace std;

/**
 * @brief Stack class
 *
 * @tparam t datatype of stack
 */
template <class t>
class Stack
{
private:
    struct node
    {
        t data;     // Data stored in node
        node *next; // Address to next node
    };

    node *head; // Address of start node

public:
    /**
     * @brief Construct a new Stack object
     *
     */
    Stack() { head = NULL; } // Initialize head to NULL

    /**
     * @brief Pushes value to stack
     *
     * @param value Value to push
     */
    void push(t value)
    {
        // Create a new node and insert data in it
        node *ptrNew = new node, *ptrTemp = head;
        ptrNew->data = value;
        ptrNew->next = NULL;

        // If stack is empty, new node is the head
        if (!head)
        {
            head = ptrNew;
            return;
        }

        // else push all values downwards and make new node the head
        ptrNew->next = head;
        head = ptrNew;
        return;
    }

    /**
     * @brief Pops a value from stack
     *
     * @return t the popped value
     */
    t pop()
    {
        // If stack is empty return 0
        if (isEmpty())
        {
            cout << "Stack Empty!" << endl;
            return {0};
        }

        // else pop the value and return it
        t value = head->data;
        head = head->next;
        return value;
    }

    /**
     * @brief Calculates the size of the stack (number of elements)
     *
     * @return int size
     */
    int size()
    {

        // Traverses the stack until finished and increments a counter
        int size = 0;
        node *ptrTemp = head;

        while (ptrTemp)
        {
            size++;
            ptrTemp = ptrTemp->next;
        }

        return size;
    }

    /**
     * @brief Checks if the stack is empty
     *
     * @return true if the stack is empty
     * @return false if the stack is not empty
     */
    bool isEmpty()
    {
        if (!head)
            return true;
        return false;
    }

    /**
     * @brief gets the top value of the stack
     *
     * @return t Value of top
     */
    t peek()
    {
        if (!isEmpty())
            return head->data;

        return {0};
    }

    /**
     * @brief prints all the values in the stack
     *
     */
    void printAll()
    {
        // Traverses stack until finished and prints out each value
        node *ptrTemp = head;
        cout << endl;
        while (ptrTemp != NULL)
        {
            cout << ptrTemp->data << (ptrTemp->next == NULL ? "" : ",");
            ptrTemp = ptrTemp->next;
        }
        cout << endl
             << endl;
        return;
    }
};