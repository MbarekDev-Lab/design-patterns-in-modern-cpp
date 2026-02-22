#include <iostream>
#include <vector>
#include <string>
#include "../../include/behavioral/iterator_coding_exercise.h"

using namespace std;

// ============================================================================
// ITERATOR CODING EXERCISE EXAMPLES: Preorder Traversal
// ============================================================================

// Example 1: Simple integer tree
void example_simple_tree()
{
    cout << "\n=== Example 1: Simple Integer Tree ===\n";

    // Build tree:
    //       1
    //      / \
    //     2   3

    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2),
                                    new Node<int>(3));

    vector<Node<int> *> nodes;
    root->preorder_traversal(nodes);

    cout << "Preorder traversal (nodes): ";
    for (auto node : nodes)
    {
        cout << node->value << " ";
    }
    cout << "\n";

    delete root;
}

// Example 2: Larger tree
void example_larger_tree()
{
    cout << "\n=== Example 2: Larger Tree ===\n";

    // Build tree:
    //         1
    //        / \
    //       2   3
    //      / \
    //     4   5

    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2,
                                                  new Node<int>(4),
                                                  new Node<int>(5)),
                                    new Node<int>(3));

    vector<Node<int> *> nodes;
    root->preorder_traversal(nodes);

    cout << "Preorder traversal: ";
    for (auto node : nodes)
    {
        cout << node->value << " ";
    }
    cout << "\n"; // Expected: 1 2 4 5 3

    delete root;
}

// Example 3: String tree (names)
void example_string_tree()
{
    cout << "\n=== Example 3: String Tree ===\n";

    Node<string> *root = new Node<string>("root",
                                          new Node<string>("left",
                                                           new Node<string>("left-left"),
                                                           new Node<string>("left-right")),
                                          new Node<string>("right"));

    vector<Node<string> *> nodes;
    root->preorder_traversal(nodes);

    cout << "Preorder (string values): ";
    for (auto node : nodes)
    {
        cout << node->value << " -> ";
    }
    cout << "end\n";

    delete root;
}

// Example 4: Extract values instead of pointers
void example_extract_values()
{
    cout << "\n=== Example 4: Extract Values ===\n";

    Node<int> *root = new Node<int>(10,
                                    new Node<int>(20,
                                                  new Node<int>(40),
                                                  new Node<int>(50)),
                                    new Node<int>(30));

    vector<int> values;
    root->preorder_values(values);

    cout << "Values in preorder: ";
    for (auto val : values)
    {
        cout << val << " ";
    }
    cout << "\n"; // Expected: 10 20 40 50 30

    delete root;
}

// Example 5: Single node tree
void example_single_node()
{
    cout << "\n=== Example 5: Single Node Tree ===\n";

    Node<int> *root = new Node<int>(42);

    vector<Node<int> *> nodes;
    root->preorder_traversal(nodes);

    cout << "Single node preorder: " << nodes[0]->value << "\n";

    delete root;
}

// Example 6: Left-skewed tree (linked list style)
void example_left_skewed()
{
    cout << "\n=== Example 6: Left-Skewed Tree ===\n";

    Node<char> *root = new Node<char>('A',
                                      new Node<char>('B',
                                                     new Node<char>('C'),
                                                     nullptr),
                                      nullptr);

    vector<Node<char> *> nodes;
    root->preorder_traversal(nodes);

    cout << "Left-skewed preorder: ";
    for (auto node : nodes)
    {
        cout << node->value << " ";
    }
    cout << "\n"; // Expected: A B C

    delete root;
}

// Example 7: Right-skewed tree
void example_right_skewed()
{
    cout << "\n=== Example 7: Right-Skewed Tree ===\n";

    Node<char> *root = new Node<char>('X',
                                      nullptr,
                                      new Node<char>('Y',
                                                     nullptr,
                                                     new Node<char>('Z')));

    vector<Node<char> *> nodes;
    root->preorder_traversal(nodes);

    cout << "Right-skewed preorder: ";
    for (auto node : nodes)
    {
        cout << node->value << " ";
    }
    cout << "\n"; // Expected: X Y Z

    delete root;
}

// Example 8: Deep tree
void example_deep_tree()
{
    cout << "\n=== Example 8: Deep Tree (Height = 4) ===\n";

    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2,
                                                  new Node<int>(4,
                                                                new Node<int>(8),
                                                                nullptr),
                                                  nullptr),
                                    new Node<int>(3));

    vector<Node<int> *> nodes;
    root->preorder_traversal(nodes);

    cout << "Deep tree preorder: ";
    for (auto node : nodes)
    {
        cout << node->value << " ";
    }
    cout << "\n"; // Expected: 1 2 4 8 3

    delete root;
}

// Example 9: Double (floating point) values
void example_double_tree()
{
    cout << "\n=== Example 9: Double Value Tree ===\n";

    Node<double> *root = new Node<double>(3.14,
                                          new Node<double>(2.71),
                                          new Node<double>(1.41));

    vector<Node<double> *> nodes;
    root->preorder_traversal(nodes);

    cout << "Double tree preorder: ";
    for (auto node : nodes)
    {
        cout << node->value << " ";
    }
    cout << "\n";

    delete root;
}

int main()
{
    cout << "========================================\n";
    cout << "  ITERATOR CODING EXERCISE\n";
    cout << "  Preorder Tree Traversal\n";
    cout << "========================================\n";

    example_simple_tree();
    example_larger_tree();
    example_string_tree();
    example_extract_values();
    example_single_node();
    example_left_skewed();
    example_right_skewed();
    example_deep_tree();
    example_double_tree();

    cout << "\n========================================\n";

    return 0;
}
