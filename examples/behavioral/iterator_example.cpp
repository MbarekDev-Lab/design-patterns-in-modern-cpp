#include <iostream>
#include <string>
#include "../../include/behavioral/iterator.h"

using namespace std;

// ============================================================================
// ITERATOR PATTERN EXAMPLES: Binary Tree Traversal
// ============================================================================

// Example 1: Pre-order Traversal
void example_preorder()
{
    cout << "\n=== Example 1: Pre-order Traversal (Root, Left, Right) ===\n";

    // Build a simple binary tree:
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5)),
                      new Node<int>(3)));

    cout << "Pre-order traversal: ";
    for (auto &node : tree.pre_order())
    {
        cout << node.value << " ";
    }
    cout << "\n"; // Output: 1 2 4 5 3
}

// Example 2: In-order Traversal
void example_inorder()
{
    cout << "\n=== Example 2: In-order Traversal (Left, Root, Right) ===\n";

    // Build a Binary Search Tree:
    //       5
    //      / \
    //     3   7
    //    / \ / \
    //   2  4 6  8

    BinaryTree<int> tree(
        new Node<int>(5,
                      new Node<int>(3,
                                    new Node<int>(2),
                                    new Node<int>(4)),
                      new Node<int>(7,
                                    new Node<int>(6),
                                    new Node<int>(8))));

    cout << "In-order traversal: ";
    for (auto &node : tree.in_order())
    {
        cout << node.value << " ";
    }
    cout << "\n"; // Output: 2 3 4 5 6 7 8 (sorted!)
}

// Example 3: Post-order Traversal
void example_postorder()
{
    cout << "\n=== Example 3: Post-order Traversal (Left, Right, Root) ===\n";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5)),
                      new Node<int>(3)));

    cout << "Post-order traversal: ";
    auto postorder_nodes = tree.post_order();
    for (auto node : postorder_nodes)
    {
        cout << node->value << " ";
    }
    cout << "\n"; // Output: 4 5 2 3 1
}

// Example 4: Level-order Traversal
void example_levelorder()
{
    cout << "\n=== Example 4: Level-order Traversal (Breadth-first) ===\n";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5)),
                      new Node<int>(3,
                                    nullptr,
                                    new Node<int>(6))));

    cout << "Level-order traversal: ";
    auto levelorder_nodes = tree.level_order();
    for (auto node : levelorder_nodes)
    {
        cout << node->value << " ";
    }
    cout << "\n"; // Output: 1 2 3 4 5 6
}

// Example 5: String Tree
void example_string_tree()
{
    cout << "\n=== Example 5: String Tree (Family Tree) ===\n";

    BinaryTree<string> family(
        new Node<string>("grandfather",
                         new Node<string>("father",
                                          new Node<string>("son"),
                                          new Node<string>("daughter")),
                         new Node<string>("uncle")));

    cout << "Family tree (in-order): ";
    for (auto &node : family.in_order())
    {
        cout << node.value << " -> ";
    }
    cout << "end\n";
}

// Example 6: Tree Statistics
void example_tree_statistics()
{
    cout << "\n=== Example 6: Tree Statistics ===\n";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5,
                                                  new Node<int>(8),
                                                  nullptr)),
                      new Node<int>(3,
                                    nullptr,
                                    new Node<int>(6))));

    cout << "Tree size: " << tree.size() << "\n";
    cout << "Tree height: " << tree.height() << "\n";

    cout << "Pre-order count: ";
    int count = 0;
    for (auto &node : tree.pre_order())
    {
        count++;
    }
    cout << count << "\n";
}

// Example 7: Comparing Different Traversal Orders
void example_comparison()
{
    cout << "\n=== Example 7: Comparing Traversal Orders ===\n";

    // Simple tree for clear output
    //       A
    //      / \
    //     B   C

    BinaryTree<char> tree(
        new Node<char>('A',
                       new Node<char>('B'),
                       new Node<char>('C')));

    cout << "Pre-order:   ";
    for (auto &node : tree.pre_order())
        cout << node.value << " ";
    cout << "\n";

    cout << "In-order:    ";
    for (auto &node : tree.in_order())
        cout << node.value << " ";
    cout << "\n";

    cout << "Post-order:  ";
    for (auto node : tree.post_order())
        cout << node->value << " ";
    cout << "\n";

    cout << "Level-order: ";
    for (auto node : tree.level_order())
        cout << node->value << " ";
    cout << "\n";
}

// Example 8: Single Node and Empty Operations
void example_edge_cases()
{
    cout << "\n=== Example 8: Edge Cases ===\n";

    // Single node
    BinaryTree<int> single(new Node<int>(42));
    cout << "Single node tree size: " << single.size() << "\n";
    cout << "Single node tree height: " << single.height() << "\n";

    cout << "Single node pre-order: ";
    for (auto &node : single.pre_order())
        cout << node.value << " ";
    cout << "\n";
}

int main()
{
    example_preorder();
    example_inorder();
    example_postorder();
    example_levelorder();
    example_string_tree();
    example_tree_statistics();
    example_comparison();
    example_edge_cases();

    return 0;
}
