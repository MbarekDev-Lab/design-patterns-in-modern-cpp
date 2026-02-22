#pragma once
#include <vector>

using namespace std;

// ============================================================================
// ITERATOR CODING EXERCISE: Binary Tree Preorder Traversal
// ============================================================================
// Implement preorder traversal that visits nodes in Root-Left-Right order
// and accumulates them into a result vector.
//
// Preorder Traversal:
// 1. Visit the current node (add to result)
// 2. Recursively traverse left subtree
// 3. Recursively traverse right subtree
// ============================================================================

template <typename T>
struct Node
{
    T value;
    Node *left;
    Node *right;
    Node *parent;

    explicit Node(T value)
        : value(value), left(nullptr), right(nullptr), parent(nullptr)
    {
    }

    Node(T value, Node<T> *left, Node<T> *right)
        : value(value), left(left), right(right), parent(nullptr)
    {
        if (left)
            left->parent = this;
        if (right)
            right->parent = this;
    }

    // ========================================================================
    // Preorder Traversal: Root -> Left -> Right
    // ========================================================================
    // Traverse the node and its children in preorder and put all
    // results into the `result` accumulator vector.
    //
    // Time Complexity: O(n) where n is number of nodes
    // Space Complexity: O(h) where h is height (recursion stack)
    // ========================================================================
    void preorder_traversal(vector<Node<T> *> &result)
    {
        // Step 1: Visit current node (add to result)
        result.push_back(this);

        // Step 2: Recursively traverse left subtree
        if (left)
            left->preorder_traversal(result);

        // Step 3: Recursively traverse right subtree
        if (right)
            right->preorder_traversal(result);
    }

    // ========================================================================
    // Helper: Get value from preorder traversal
    // ========================================================================
    void preorder_values(vector<T> &result)
    {
        result.push_back(value);
        if (left)
            left->preorder_values(result);
        if (right)
            right->preorder_values(result);
    }

    // ========================================================================
    // Destructor: Clean up the tree
    // ========================================================================
    ~Node()
    {
        if (left)
            delete left;
        if (right)
            delete right;
    }
};
