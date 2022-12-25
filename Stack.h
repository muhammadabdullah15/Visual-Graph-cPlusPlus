#pragma once

#include <iostream>
using namespace std;

template <class t>
class Stack
{
private:
    struct node
    {
        t data;
        node *next;
    };

    node *head;

public:
    Stack()
    {
        head = NULL;
    }

    void push(t value)
    {
        // cout << "pushed " << value << endl;
        node *ptrNew = new node, *ptrTemp = head;
        ptrNew->data = value;
        ptrNew->next = NULL;

        if (head == NULL)
        {
            head = ptrNew;
            return;
        }

        ptrNew->next = head;
        head = ptrNew;
        // printAll();
        cout << size() << endl;
        return;
    }

    t pop()
    {

        if (isEmpty())
        {
            cout << "Stack Empty!" << endl;
            return {0};
        }
        t value = head->data;
        head = head->next;
        return value;
    }

    int size()
    {
        int size = 0;

        node *ptrTemp = head;

        while (ptrTemp != NULL)
        {
            size++;
            ptrTemp = ptrTemp->next;
        }

        return size;
    }

    bool isEmpty()
    {
        if (head == NULL)
            return true;
        return false;
    }

    t peek()
    {
        if (!isEmpty())
            return head->data;

        return {0};
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
        cout << endl
             << endl;
        return;
    }
};