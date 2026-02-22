#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "../../include/behavioral/iterator_coding_exercise.h"

using namespace std;

// ============================================================================
// ITERATOR CODING EXERCISE TEST SUITE: Preorder Traversal
// ============================================================================

void test_single_node()
{
    cout << "Test: Single node tree... ";

    Node<int> *root = new Node<int>(42);
    vector<Node<int> *> result;

    root->preorder_traversal(result);

    assert(result.size() == 1);
    assert(result[0]->value == 42);

    delete root;
    cout << "PASS\n";
}

void test_simple_three_node_tree()
{
    cout << "Test: Simple three-node tree... ";

    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2),
                                    new Node<int>(3));

    vector<Node<int> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 3);
    assert(result[0]->value == 1);
    assert(result[1]->value == 2);
    assert(result[2]->value == 3);

    delete root;
    cout << "PASS\n";
}

void test_five_node_tree()
{
    cout << "Test: Five-node tree... ";

    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2,
                                                  new Node<int>(4),
                                                  new Node<int>(5)),
                                    new Node<int>(3));

    vector<Node<int> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 5);
    assert(result[0]->value == 1);
    assert(result[1]->value == 2);
    assert(result[2]->value == 4);
    assert(result[3]->value == 5);
    assert(result[4]->value == 3);

    delete root;
    cout << "PASS\n";
}

void test_left_skewed_tree()
{
    cout << "Test: Left-skewed tree (linked list)... ";

    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2,
                                                  new Node<int>(3),
                                                  nullptr),
                                    nullptr);

    vector<Node<int> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 3);
    assert(result[0]->value == 1);
    assert(result[1]->value == 2);
    assert(result[2]->value == 3);

    delete root;
    cout << "PASS\n";
}

void test_right_skewed_tree()
{
    cout << "Test: Right-skewed tree... ";

    Node<int> *root = new Node<int>(1,
                                    nullptr,
                                    new Node<int>(2,
                                                  nullptr,
                                                  new Node<int>(3)));

    vector<Node<int> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 3);
    assert(result[0]->value == 1);
    assert(result[1]->value == 2);
    assert(result[2]->value == 3);

    delete root;
    cout << "PASS\n";
}

void test_balanced_tree()
{
    cout << "Test: Balanced tree (7 nodes)... ";

    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2,
                                                  new Node<int>(4),
                                                  new Node<int>(5)),
                                    new Node<int>(3,
                                                  new Node<int>(6),
                                                  new Node<int>(7)));

    vector<Node<int> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 7);
    assert(result[0]->value == 1);
    assert(result[1]->value == 2);
    assert(result[2]->value == 4);
    assert(result[3]->value == 5);
    assert(result[4]->value == 3);
    assert(result[5]->value == 6);
    assert(result[6]->value == 7);

    delete root;
    cout << "PASS\n";
}

void test_string_tree()
{
    cout << "Test: String tree... ";

    Node<string> *root = new Node<string>("root",
                                          new Node<string>("left"),
                                          new Node<string>("right"));

    vector<Node<string> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 3);
    assert(result[0]->value == "root");
    assert(result[1]->value == "left");
    assert(result[2]->value == "right");

    delete root;
    cout << "PASS\n";
}

void test_char_tree()
{
    cout << "Test: Character tree... ";

    Node<char> *root = new Node<char>('A',
                                      new Node<char>('B'),
                                      new Node<char>('C'));

    vector<Node<char> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 3);
    assert(result[0]->value == 'A');
    assert(result[1]->value == 'B');
    assert(result[2]->value == 'C');

    delete root;
    cout << "PASS\n";
}

void test_double_tree()
{
    cout << "Test: Double value tree... ";

    Node<double> *root = new Node<double>(1.5,
                                          new Node<double>(2.5),
                                          new Node<double>(3.5));

    vector<Node<double> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 3);
    assert(result[0]->value == 1.5);
    assert(result[1]->value == 2.5);
    assert(result[2]->value == 3.5);

    delete root;
    cout << "PASS\n";
}

void test_parent_pointers()
{
    cout << "Test: Parent pointers set correctly... ";

    Node<int> *left = new Node<int>(2);
    Node<int> *right = new Node<int>(3);
    Node<int> *root = new Node<int>(1, left, right);

    assert(root->parent == nullptr);
    assert(left->parent == root);
    assert(right->parent == root);

    delete root;
    cout << "PASS\n";
}

void test_left_only_tree()
{
    cout << "Test: Left-only tree (one right child)... ";

    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2),
                                    nullptr);

    vector<Node<int> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 2);
    assert(result[0]->value == 1);
    assert(result[1]->value == 2);

    delete root;
    cout << "PASS\n";
}

void test_right_only_tree()
{
    cout << "Test: Right-only tree (one left child)... ";

    Node<int> *root = new Node<int>(1,
                                    nullptr,
                                    new Node<int>(3));

    vector<Node<int> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 2);
    assert(result[0]->value == 1);
    assert(result[1]->value == 3);

    delete root;
    cout << "PASS\n";
}

void test_preorder_values_single_node()
{
    cout << "Test: Preorder values - single node... ";

    Node<int> *root = new Node<int>(42);
    vector<int> values;

    root->preorder_values(values);

    assert(values.size() == 1);
    assert(values[0] == 42);

    delete root;
    cout << "PASS\n";
}

void test_preorder_values_multiple_nodes()
{
    cout << "Test: Preorder values - multiple nodes... ";

    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2,
                                                  new Node<int>(4),
                                                  new Node<int>(5)),
                                    new Node<int>(3));

    vector<int> values;
    root->preorder_values(values);

    assert(values.size() == 5);
    assert(values[0] == 1);
    assert(values[1] == 2);
    assert(values[2] == 4);
    assert(values[3] == 5);
    assert(values[4] == 3);

    delete root;
    cout << "PASS\n";
}

void test_deep_tree_height_5()
{
    cout << "Test: Deep tree (height = 5)... ";

    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2,
                                                  new Node<int>(3,
                                                                new Node<int>(4,
                                                                              new Node<int>(5),
                                                                              nullptr),
                                                                nullptr),
                                                  nullptr),
                                    nullptr);

    vector<Node<int> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 5);
    assert(result[0]->value == 1);
    assert(result[1]->value == 2);
    assert(result[2]->value == 3);
    assert(result[3]->value == 4);
    assert(result[4]->value == 5);

    delete root;
    cout << "PASS\n";
}

void test_wide_tree()
{
    cout << "Test: Wide tree (many nodes at level 2)... ";

    // Tree with root and children at level 1
    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2,
                                                  new Node<int>(6),
                                                  new Node<int>(7)),
                                    new Node<int>(3,
                                                  new Node<int>(8),
                                                  new Node<int>(9)));

    vector<Node<int> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 7);
    // Preorder: 1, 2, 6, 7, 3, 8, 9
    assert(result[0]->value == 1);
    assert(result[1]->value == 2);
    assert(result[2]->value == 6);
    assert(result[3]->value == 7);
    assert(result[4]->value == 3);
    assert(result[5]->value == 8);
    assert(result[6]->value == 9);

    delete root;
    cout << "PASS\n";
}

void test_complex_tree()
{
    cout << "Test: Complex asymmetric tree... ";

    Node<int> *root = new Node<int>(1,
                                    new Node<int>(2,
                                                  new Node<int>(4,
                                                                new Node<int>(8),
                                                                nullptr),
                                                  new Node<int>(5)),
                                    new Node<int>(3,
                                                  nullptr,
                                                  new Node<int>(6,
                                                                new Node<int>(9),
                                                                new Node<int>(10))));

    vector<Node<int> *> result;
    root->preorder_traversal(result);

    assert(result.size() == 9);
    assert(result[0]->value == 1);
    assert(result[1]->value == 2);
    assert(result[2]->value == 4);
    assert(result[3]->value == 8);
    assert(result[4]->value == 5);
    assert(result[5]->value == 3);
    assert(result[6]->value == 6);
    assert(result[7]->value == 9);
    assert(result[8]->value == 10);

    delete root;
    cout << "PASS\n";
}

void test_result_vector_ordering()
{
    cout << "Test: Result vector maintains order... ";

    // This test ensures that the result vector is filled in the
    // exact order of preorder traversal
    Node<int> *root = new Node<int>(10,
                                    new Node<int>(20,
                                                  new Node<int>(40),
                                                  new Node<int>(50)),
                                    new Node<int>(30));

    vector<Node<int> *> result;
    root->preorder_traversal(result);

    // Verify the sequence: should be 10, 20, 40, 50, 30
    vector<int> expected = {10, 20, 40, 50, 30};
    for (size_t i = 0; i < expected.size(); ++i)
    {
        assert(result[i]->value == expected[i]);
    }

    delete root;
    cout << "PASS\n";
}

int main()
{
    cout << "========================================\n";
    cout << "  ITERATOR CODING EXERCISE TEST SUITE\n";
    cout << "  Preorder Traversal\n";
    cout << "========================================\n\n";

    test_single_node();
    test_simple_three_node_tree();
    test_five_node_tree();
    test_left_skewed_tree();
    test_right_skewed_tree();
    test_balanced_tree();
    test_string_tree();
    test_char_tree();
    test_double_tree();
    test_parent_pointers();
    test_left_only_tree();
    test_right_only_tree();
    test_preorder_values_single_node();
    test_preorder_values_multiple_nodes();
    test_deep_tree_height_5();
    test_wide_tree();
    test_complex_tree();
    test_result_vector_ordering();

    cout << "\n========================================\n";
    cout << "  ALL TESTS PASSED! (18 tests)\n";
    cout << "========================================\n";

    return 0;
}
