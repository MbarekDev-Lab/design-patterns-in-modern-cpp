#pragma once
#include <string>
#include <iterator>

using namespace std;

// ============================================================================
// ITERATOR PATTERN EXERCISE: Linked List with Custom Iterator
// ============================================================================
// This implementation demonstrates the Iterator pattern using a simple
// linked list structure with a manually implemented forward iterator.
//
// Key iterator methods:
// - operator++ (prefix)  - Move to next element
// - operator== / !=      - Compare iterators
// - operator*            - Dereference to access element
// - operator->           - Arrow access to element members
// ============================================================================

using namespace std;

// ============================================================================
// NODE: Singly Linked List Node
// ============================================================================
struct Node
{
    string value;
    Node *next;

    explicit Node(const string &value)
        : value(value), next(nullptr)
    {
    }

    Node(const string &value, Node *const parent)
        : value(value), next(nullptr)
    {
        parent->next = this;
    }
};

// ============================================================================
// LIST ITERATOR: Custom Forward Iterator
// ============================================================================
// Implements the std::forward_iterator interface manually:
// - Supports operator++    (advance to next)
// - Supports operator==    (equality comparison)
// - Supports operator!=    (inequality comparison)
// - Supports operator*     (dereference)
// - Supports operator->    (member access)
// ============================================================================
struct ListIterator
{
    // ========================================================================
    // Iterator type definitions for STL compatibility
    // ========================================================================
    typedef forward_iterator_tag iterator_category;
    typedef Node value_type;
    typedef ptrdiff_t difference_type;
    typedef Node *pointer;
    typedef Node &reference;

    Node *current;

    ListIterator()
        : current(nullptr)
    {
    }

    explicit ListIterator(Node *const current)
        : current(current)
    {
    }

    // ========================================================================
    // Dereference operator: get reference to current node
    // ========================================================================
    Node &operator*() const
    {
        return *current;
    }

    // ========================================================================
    // Arrow operator: access members of current node
    // ========================================================================
    Node *operator->() const
    {
        return current;
    }

    // ========================================================================
    // Pre-increment operator: move to next node
    // ========================================================================
    ListIterator &operator++()
    {
        current = current->next;
        return *this;
    }

    // ========================================================================
    // Post-increment operator: move to next node, return old iterator
    // ========================================================================
    ListIterator operator++(int)
    {
        ListIterator temp = *this;
        current = current->next;
        return temp;
    }

    // ========================================================================
    // Equality operator: check if two iterators point to same node
    // ========================================================================
    bool operator==(const ListIterator &other) const
    {
        return current == other.current;
    }

    // ========================================================================
    // Inequality operator: check if two iterators point to different nodes
    // ========================================================================
    bool operator!=(const ListIterator &other) const
    {
        return current != other.current;
    }
};

// ============================================================================
// LINKED LIST: Simple wrapper for a linked list
// ============================================================================
struct LinkedList
{
    Node *head;

    LinkedList()
        : head(nullptr)
    {
    }

    explicit LinkedList(Node *head)
        : head(head)
    {
    }

    // ========================================================================
    // Insert a new node at the beginning of the list
    // ========================================================================
    void push_front(const string &value)
    {
        Node *new_node = new Node(value);
        new_node->next = head;
        head = new_node;
    }

    // ========================================================================
    // Insert a new node at the end of the list
    // ========================================================================
    void push_back(const string &value)
    {
        Node *new_node = new Node(value);

        if (!head)
        {
            head = new_node;
            return;
        }

        Node *current = head;
        while (current->next)
        {
            current = current->next;
        }

        current->next = new_node;
    }

    // ========================================================================
    // Get the size of the list
    // ========================================================================
    int size() const
    {
        int count = 0;
        Node *current = head;
        while (current)
        {
            count++;
            current = current->next;
        }
        return count;
    }

    // ========================================================================
    // Get begin iterator
    // ========================================================================
    ListIterator begin()
    {
        return ListIterator(head);
    }

    // ========================================================================
    // Get end iterator
    // ========================================================================
    ListIterator end()
    {
        return ListIterator(nullptr);
    }

    // ========================================================================
    // Get a value by index
    // ========================================================================
    string at(int index) const
    {
        Node *current = head;
        for (int i = 0; i < index && current; ++i)
        {
            current = current->next;
        }
        return current ? current->value : "";
    }

    // ========================================================================
    // Find a value in the list
    // ========================================================================
    bool contains(const string &value) const
    {
        Node *current = head;
        while (current)
        {
            if (current->value == value)
                return true;
            current = current->next;
        }
        return false;
    }

    // ========================================================================
    // Cleanup: delete all nodes
    // ========================================================================
    ~LinkedList()
    {
        Node *current = head;
        while (current)
        {
            Node *next = current->next;
            delete current;
            current = next;
        }
    }
};
