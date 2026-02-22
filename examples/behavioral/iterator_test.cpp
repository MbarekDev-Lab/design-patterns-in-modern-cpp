#include <iostream>
#include <cassert>
#include <string>
#include "../../include/behavioral/iterator.h"

using namespace std;

// ============================================================================
// ITERATOR PATTERN TESTS
// ============================================================================

void test_preorder_traversal()
{
    cout << "Test: Pre-order traversal... ";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5)),
                      new Node<int>(3)));

    vector<int> values;
    for (auto &node : tree.pre_order())
    {
        values.push_back(node.value);
    }

    assert(values.size() == 5);
    assert(values[0] == 1);
    assert(values[1] == 2);
    assert(values[2] == 4);
    assert(values[3] == 5);
    assert(values[4] == 3);

    cout << "PASS\n";
}

void test_inorder_traversal()
{
    cout << "Test: In-order traversal... ";

    BinaryTree<int> tree(
        new Node<int>(5,
                      new Node<int>(3,
                                    new Node<int>(2),
                                    new Node<int>(4)),
                      new Node<int>(7,
                                    new Node<int>(6),
                                    new Node<int>(8))));

    vector<int> values;
    for (auto &node : tree.in_order())
    {
        values.push_back(node.value);
    }

    assert(values.size() == 7);
    // In-order should give sorted BST: 2 3 4 5 6 7 8
    assert(values[0] == 2);
    assert(values[1] == 3);
    assert(values[2] == 4);
    assert(values[3] == 5);
    assert(values[4] == 6);
    assert(values[5] == 7);
    assert(values[6] == 8);

    cout << "PASS\n";
}

void test_postorder_traversal()
{
    cout << "Test: Post-order traversal... ";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5)),
                      new Node<int>(3)));

    auto nodes = tree.post_order();
    assert(nodes.size() == 5);
    assert(nodes[0]->value == 4);
    assert(nodes[1]->value == 5);
    assert(nodes[2]->value == 2);
    assert(nodes[3]->value == 3);
    assert(nodes[4]->value == 1);

    cout << "PASS\n";
}

void test_levelorder_traversal()
{
    cout << "Test: Level-order traversal... ";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5)),
                      new Node<int>(3,
                                    nullptr,
                                    new Node<int>(6))));

    auto nodes = tree.level_order();
    assert(nodes.size() == 6);
    assert(nodes[0]->value == 1); // root
    assert(nodes[1]->value == 2); // left of root
    assert(nodes[2]->value == 3); // right of root
    assert(nodes[3]->value == 4); // left of 2
    assert(nodes[4]->value == 5); // right of 2
    assert(nodes[5]->value == 6); // right of 3

    cout << "PASS\n";
}

void test_single_node_tree()
{
    cout << "Test: Single node tree... ";

    BinaryTree<int> tree(new Node<int>(42));

    vector<int> preorder_values;
    for (auto &node : tree.pre_order())
    {
        preorder_values.push_back(node.value);
    }

    assert(preorder_values.size() == 1);
    assert(preorder_values[0] == 42);
    assert(tree.size() == 1);
    assert(tree.height() == 1);

    cout << "PASS\n";
}

void test_tree_size()
{
    cout << "Test: Tree size calculation... ";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5)),
                      new Node<int>(3)));

    assert(tree.size() == 5);

    cout << "PASS\n";
}

void test_tree_height()
{
    cout << "Test: Tree height calculation... ";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    nullptr),
                      new Node<int>(3)));

    assert(tree.height() == 3);

    cout << "PASS\n";
}

void test_tree_height_single_node()
{
    cout << "Test: Single node height... ";

    BinaryTree<int> tree(new Node<int>(1));

    assert(tree.height() == 1);

    cout << "PASS\n";
}

void test_string_tree_preorder()
{
    cout << "Test: String tree pre-order... ";

    BinaryTree<string> tree(
        new Node<string>("root",
                         new Node<string>("left"),
                         new Node<string>("right")));

    vector<string> values;
    for (auto &node : tree.pre_order())
    {
        values.push_back(node.value);
    }

    assert(values.size() == 3);
    assert(values[0] == "root");
    assert(values[1] == "left");
    assert(values[2] == "right");

    cout << "PASS\n";
}

void test_string_tree_inorder()
{
    cout << "Test: String tree in-order... ";

    BinaryTree<string> tree(
        new Node<string>("root",
                         new Node<string>("left"),
                         new Node<string>("right")));

    vector<string> values;
    for (auto &node : tree.in_order())
    {
        values.push_back(node.value);
    }

    assert(values.size() == 3);
    assert(values[0] == "left");
    assert(values[1] == "root");
    assert(values[2] == "right");

    cout << "PASS\n";
}

void test_deep_tree()
{
    cout << "Test: Deep tree (linked list structure)... ";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(3,
                                                  new Node<int>(4,
                                                                new Node<int>(5),
                                                                nullptr),
                                                  nullptr),
                                    nullptr),
                      nullptr));

    assert(tree.size() == 5);
    assert(tree.height() == 5);

    cout << "PASS\n";
}

void test_balanced_tree()
{
    cout << "Test: Balanced tree height... ";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5)),
                      new Node<int>(3,
                                    new Node<int>(6),
                                    new Node<int>(7))));

    assert(tree.size() == 7);
    assert(tree.height() == 3);

    cout << "PASS\n";
}

void test_right_skewed_tree()
{
    cout << "Test: Right-skewed tree... ";

    BinaryTree<int> tree(
        new Node<int>(1,
                      nullptr,
                      new Node<int>(2,
                                    nullptr,
                                    new Node<int>(3,
                                                  nullptr,
                                                  new Node<int>(4)))));

    assert(tree.size() == 4);
    assert(tree.height() == 4);

    auto levelorder = tree.level_order();
    assert(levelorder[0]->value == 1);
    assert(levelorder[1]->value == 2);
    assert(levelorder[2]->value == 3);
    assert(levelorder[3]->value == 4);

    cout << "PASS\n";
}

void test_preorder_count_matches_size()
{
    cout << "Test: Pre-order count matches size... ";

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

    int count = 0;
    for (auto &node : tree.pre_order())
    {
        count++;
    }

    assert(count == tree.size());

    cout << "PASS\n";
}

void test_inorder_count_matches_size()
{
    cout << "Test: In-order count matches size... ";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5)),
                      new Node<int>(3)));

    int count = 0;
    for (auto &node : tree.in_order())
    {
        count++;
    }

    assert(count == tree.size());

    cout << "PASS\n";
}

void test_postorder_count_matches_size()
{
    cout << "Test: Post-order count matches size... ";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5)),
                      new Node<int>(3)));

    auto nodes = tree.post_order();
    assert(nodes.size() == tree.size());

    cout << "PASS\n";
}

void test_levelorder_count_matches_size()
{
    cout << "Test: Level-order count matches size... ";

    BinaryTree<int> tree(
        new Node<int>(1,
                      new Node<int>(2,
                                    new Node<int>(4),
                                    new Node<int>(5)),
                      new Node<int>(3)));

    auto nodes = tree.level_order();
    assert(nodes.size() == tree.size());

    cout << "PASS\n";
}

void test_char_tree()
{
    cout << "Test: Character tree... ";

    BinaryTree<char> tree(
        new Node<char>('A',
                       new Node<char>('B'),
                       new Node<char>('C')));

    vector<char> preorder;
    for (auto &node : tree.pre_order())
    {
        preorder.push_back(node.value);
    }

    assert(preorder.size() == 3);
    assert(preorder[0] == 'A');
    assert(preorder[1] == 'B');
    assert(preorder[2] == 'C');

    cout << "PASS\n";
}

void test_double_tree()
{
    cout << "Test: Double value tree... ";

    BinaryTree<double> tree(
        new Node<double>(3.14,
                         new Node<double>(2.71),
                         new Node<double>(1.41)));

    vector<double> preorder;
    for (auto &node : tree.pre_order())
    {
        preorder.push_back(node.value);
    }

    assert(preorder.size() == 3);
    assert(preorder[0] == 3.14);
    assert(preorder[1] == 2.71);
    assert(preorder[2] == 1.41);

    cout << "PASS\n";
}

int main()
{
    cout << "========================================\n";
    cout << "  ITERATOR PATTERN TEST SUITE\n";
    cout << "========================================\n\n";

    test_preorder_traversal();
    test_inorder_traversal();
    test_postorder_traversal();
    test_levelorder_traversal();
    test_single_node_tree();
    test_tree_size();
    test_tree_height();
    test_tree_height_single_node();
    test_string_tree_preorder();
    test_string_tree_inorder();
    test_deep_tree();
    test_balanced_tree();
    test_right_skewed_tree();
    test_preorder_count_matches_size();
    test_inorder_count_matches_size();
    test_postorder_count_matches_size();
    test_levelorder_count_matches_size();
    test_char_tree();
    test_double_tree();

    cout << "\n========================================\n";
    cout << "  ALL TESTS PASSED!\n";
    cout << "========================================\n";

    return 0;
}
