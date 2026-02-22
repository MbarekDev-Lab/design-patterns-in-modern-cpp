#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <queue>

using namespace std;

// ============================================================================
// ITERATOR PATTERN: Binary Tree Traversal
// ============================================================================
// The Iterator pattern provides a way to access elements of a collection
// sequentially without exposing its underlying representation.
//
// This implementation demonstrates multiple traversal strategies:
// - Pre-order (Root, Left, Right)
// - In-order (Left, Root, Right)
// - Post-order (Left, Right, Root)
// - Level-order (Breadth-first)
// ============================================================================

template <typename T>
struct BinaryTree;

// ============================================================================
// NODE: Binary Tree Node
// ============================================================================
template <typename T>
struct Node
{
    T value;
    Node<T> *left;
    Node<T> *right;
    Node<T> *parent;
    BinaryTree<T> *tree;

    explicit Node(const T &value)
        : value(value), left(nullptr), right(nullptr), parent(nullptr), tree(nullptr)
    {
    }

    Node(const T &value, Node<T> *left, Node<T> *right)
        : value(value), left(left), right(right), parent(nullptr), tree(nullptr)
    {
        if (this->left)
            this->left->parent = this;
        if (this->right)
            this->right->parent = this;
    }

    void set_tree(BinaryTree<T> *t)
    {
        tree = t;
        if (left)
            left->set_tree(t);
        if (right)
            right->set_tree(t);
    }

    ~Node()
    {
        if (left)
            delete left;
        if (right)
            delete right;
    }
};

// ============================================================================
// ITERATORS: Different Traversal Strategies
// ============================================================================

// Pre-order Iterator (Root -> Left -> Right)
// Uses a stack-based approach for correct traversal
template <typename T>
struct PreOrderIterator
{
    vector<Node<T> *> stack;

    explicit PreOrderIterator(Node<T> *root)
    {
        if (root)
            stack.push_back(root);
    }

    bool operator!=(const PreOrderIterator<T> &other) const
    {
        return !stack.empty() || !other.stack.empty();
    }

    PreOrderIterator<T> &operator++()
    {
        if (!stack.empty())
        {
            auto node = stack.back();
            stack.pop_back();

            // Push right first (so left is processed first due to LIFO)
            if (node->right)
                stack.push_back(node->right);
            if (node->left)
                stack.push_back(node->left);
        }
        return *this;
    }

    Node<T> *current() const { return stack.empty() ? nullptr : stack.back(); }
    Node<T> &operator*() { return *current(); }
    Node<T> *operator->() { return current(); }
};

// In-order Iterator (Left -> Root -> Right)
template <typename T>
struct InOrderIterator
{
    Node<T> *current;

    explicit InOrderIterator(Node<T> *node)
        : current(node)
    {
        if (node)
            while (current->left)
                current = current->left;
    }

    bool operator!=(const InOrderIterator<T> &other) const
    {
        return current != other.current;
    }

    InOrderIterator<T> &operator++()
    {
        if (current->right)
        {
            current = current->right;
            while (current->left)
                current = current->left;
        }
        else
        {
            Node<T> *p = current->parent;
            while (p && current == p->right)
            {
                current = p;
                p = p->parent;
            }
            current = p;
        }
        return *this;
    }

    Node<T> &operator*() { return *current; }
    Node<T> *operator->() { return current; }
};

// ============================================================================
// BINARY TREE with Multiple Traversal Strategies
// ============================================================================
template <typename T>
struct BinaryTree
{
    Node<T> *root;

    explicit BinaryTree(Node<T> *root = nullptr)
        : root(root)
    {
        if (root)
            root->set_tree(this);
    }

    ~BinaryTree()
    {
        if (root)
            delete root;
    }

    // ========================================================================
    // Pre-order Traversal (Root, Left, Right)
    // ========================================================================
    struct PreOrderTraversal
    {
        BinaryTree<T> &tree;

        PreOrderTraversal(BinaryTree<T> &tree)
            : tree(tree)
        {
        }

        PreOrderIterator<T> begin()
        {
            return PreOrderIterator<T>(tree.root);
        }

        PreOrderIterator<T> end()
        {
            return PreOrderIterator<T>(nullptr);
        }
    };

    PreOrderTraversal pre_order()
    {
        return PreOrderTraversal(*this);
    }

    // ========================================================================
    // In-order Traversal (Left, Root, Right)
    // ========================================================================
    struct InOrderTraversal
    {
        BinaryTree<T> &tree;

        InOrderTraversal(BinaryTree<T> &tree)
            : tree(tree)
        {
        }

        InOrderIterator<T> begin()
        {
            return InOrderIterator<T>(tree.root);
        }

        InOrderIterator<T> end()
        {
            return InOrderIterator<T>(nullptr);
        }
    };

    InOrderTraversal in_order()
    {
        return InOrderTraversal(*this);
    }

    // ========================================================================
    // Post-order Traversal (Left, Right, Root) - Vector-based
    // ========================================================================
    vector<Node<T> *> post_order()
    {
        vector<Node<T> *> result;
        post_order_impl(root, result);
        return result;
    }

private:
    void post_order_impl(Node<T> *node, vector<Node<T> *> &result)
    {
        if (node)
        {
            post_order_impl(node->left, result);
            post_order_impl(node->right, result);
            result.push_back(node);
        }
    }

public:
    // ========================================================================
    // Level-order Traversal (Breadth-first)
    // ========================================================================
    vector<Node<T> *> level_order()
    {
        vector<Node<T> *> result;
        if (!root)
            return result;

        queue<Node<T> *> q;
        q.push(root);

        while (!q.empty())
        {
            Node<T> *node = q.front();
            q.pop();
            result.push_back(node);

            if (node->left)
                q.push(node->left);
            if (node->right)
                q.push(node->right);
        }

        return result;
    }

    // ========================================================================
    // Helper: Get size of tree
    // ========================================================================
    int size() const
    {
        return size_impl(root);
    }

private:
    int size_impl(Node<T> *node) const
    {
        if (!node)
            return 0;
        return 1 + size_impl(node->left) + size_impl(node->right);
    }

public:
    // ========================================================================
    // Helper: Get height of tree
    // ========================================================================
    int height() const
    {
        return height_impl(root);
    }

private:
    int height_impl(Node<T> *node) const
    {
        if (!node)
            return 0;
        return 1 + max(height_impl(node->left), height_impl(node->right));
    }
};
