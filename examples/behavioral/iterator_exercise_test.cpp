#include <iostream>
#include <cassert>
#include <string>
#include <algorithm>
#include <vector>
#include "../../include/behavioral/iterator_exercise.h"

using namespace std;

// ============================================================================
// ITERATOR EXERCISE TEST SUITE: Linked List with Boost Iterator Facade
// ============================================================================

void test_basic_node_creation()
{
    cout << "Test: Basic node creation... ";

    Node node1{"first"};
    assert(node1.value == "first");
    assert(node1.next == nullptr);

    cout << "PASS\n";
}

void test_node_chaining()
{
    cout << "Test: Node chaining... ";

    Node first{"first"};
    Node second{"second", &first};

    assert(first.value == "first");
    assert(first.next == &second);
    assert(second.value == "second");

    cout << "PASS\n";
}

void test_list_iterator_dereference()
{
    cout << "Test: List iterator dereference... ";

    Node alpha{"alpha"};
    Node beta{"beta", &alpha};

    ListIterator it{&alpha};
    assert(it->value == "alpha");

    ++it;
    assert(it->value == "beta");

    cout << "PASS\n";
}

void test_list_iterator_increment()
{
    cout << "Test: List iterator increment... ";

    Node first{"first"};
    Node second{"second", &first};
    Node third{"third", &second};

    ListIterator it{&first};
    assert(it->value == "first");

    ++it;
    assert(it->value == "second");

    ++it;
    assert(it->value == "third");

    ++it;
    assert(it == ListIterator{});

    cout << "PASS\n";
}

void test_list_iterator_equality()
{
    cout << "Test: List iterator equality... ";

    Node first{"first"};
    Node second{"second", &first};

    ListIterator it1{&first};
    ListIterator it2{&first};
    ListIterator it3{&second};

    assert(it1 == it2);
    assert(it1 != it3);

    cout << "PASS\n";
}

void test_list_iterator_inequality()
{
    cout << "Test: List iterator inequality... ";

    Node first{"first"};
    Node second{"second", &first};

    ListIterator it1{&first};
    ListIterator it2{&second};
    ListIterator end{};

    assert(it1 != it2);
    assert(it1 != end);
    assert(!(it1 == it2));

    cout << "PASS\n";
}

void test_linkedlist_push_back()
{
    cout << "Test: LinkedList push_back... ";

    LinkedList list;
    list.push_back("first");
    list.push_back("second");
    list.push_back("third");

    assert(list.size() == 3);
    assert(list.at(0) == "first");
    assert(list.at(1) == "second");
    assert(list.at(2) == "third");

    cout << "PASS\n";
}

void test_linkedlist_push_front()
{
    cout << "Test: LinkedList push_front... ";

    LinkedList list;
    list.push_back("second");
    list.push_back("third");
    list.push_front("first");

    assert(list.size() == 3);
    assert(list.at(0) == "first");
    assert(list.at(1) == "second");
    assert(list.at(2) == "third");

    cout << "PASS\n";
}

void test_linkedlist_size()
{
    cout << "Test: LinkedList size... ";

    LinkedList list;
    assert(list.size() == 0);

    list.push_back("a");
    assert(list.size() == 1);

    list.push_back("b");
    assert(list.size() == 2);

    list.push_back("c");
    assert(list.size() == 3);

    cout << "PASS\n";
}

void test_linkedlist_at()
{
    cout << "Test: LinkedList at... ";

    LinkedList list;
    list.push_back("alpha");
    list.push_back("beta");
    list.push_back("gamma");

    assert(list.at(0) == "alpha");
    assert(list.at(1) == "beta");
    assert(list.at(2) == "gamma");
    assert(list.at(3) == ""); // Out of bounds

    cout << "PASS\n";
}

void test_linkedlist_contains()
{
    cout << "Test: LinkedList contains... ";

    LinkedList list;
    list.push_back("apple");
    list.push_back("banana");
    list.push_back("cherry");

    assert(list.contains("apple") == true);
    assert(list.contains("banana") == true);
    assert(list.contains("cherry") == true);
    assert(list.contains("date") == false);

    cout << "PASS\n";
}

void test_for_each_algorithm()
{
    cout << "Test: for_each algorithm... ";

    LinkedList list;
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");

    vector<string> collected;
    for_each(list.begin(), list.end(),
             [&collected](const Node &n)
             {
                 collected.push_back(n.value);
             });

    assert(collected.size() == 3);
    assert(collected[0] == "a");
    assert(collected[1] == "b");
    assert(collected[2] == "c");

    cout << "PASS\n";
}

void test_range_based_for_loop()
{
    cout << "Test: Range-based for loop... ";

    LinkedList list;
    list.push_back("x");
    list.push_back("y");
    list.push_back("z");

    vector<string> values;
    for (auto &node : list)
    {
        values.push_back(node.value);
    }

    assert(values.size() == 3);
    assert(values[0] == "x");
    assert(values[1] == "y");
    assert(values[2] == "z");

    cout << "PASS\n";
}

void test_find_algorithm()
{
    cout << "Test: find algorithm... ";

    LinkedList list;
    list.push_back("red");
    list.push_back("green");
    list.push_back("blue");

    auto it = find_if(list.begin(), list.end(),
                      [](const Node &n)
                      {
                          return n.value == "green";
                      });

    assert(it != list.end());
    assert(it->value == "green");

    cout << "PASS\n";
}

void test_find_not_found()
{
    cout << "Test: find not found... ";

    LinkedList list;
    list.push_back("one");
    list.push_back("two");
    list.push_back("three");

    auto it = find_if(list.begin(), list.end(),
                      [](const Node &n)
                      {
                          return n.value == "four";
                      });

    assert(it == list.end());

    cout << "PASS\n";
}

void test_count_if_algorithm()
{
    cout << "Test: count_if algorithm... ";

    LinkedList list;
    list.push_back("cat");  // 3 chars
    list.push_back("dog");  // 3 chars
    list.push_back("bird"); // 4 chars
    list.push_back("fish"); // 4 chars
    list.push_back("ant");  // 3 chars

    int count = count_if(list.begin(), list.end(),
                         [](const Node &n)
                         {
                             return n.value.length() == 3;
                         });

    assert(count == 3);

    cout << "PASS\n";
}

void test_vector_construction_from_iterator()
{
    cout << "Test: Vector construction from iterators... ";

    LinkedList list;
    list.push_back("first");
    list.push_back("second");
    list.push_back("third");

    vector<Node> vec(list.begin(), list.end());

    assert(vec.size() == 3);
    assert(vec[0].value == "first");
    assert(vec[1].value == "second");
    assert(vec[2].value == "third");

    cout << "PASS\n";
}

void test_empty_list_iteration()
{
    cout << "Test: Empty list iteration... ";

    LinkedList list;

    int count = 0;
    for (auto &node : list)
    {
        count++;
    }

    assert(count == 0);
    assert(list.size() == 0);

    cout << "PASS\n";
}

void test_single_element_list()
{
    cout << "Test: Single element list... ";

    LinkedList list;
    list.push_back("only");

    assert(list.size() == 1);
    assert(list.at(0) == "only");
    assert(list.contains("only") == true);

    auto it = list.begin();
    assert(it != list.end());
    assert(it->value == "only");

    ++it;
    assert(it == list.end());

    cout << "PASS\n";
}

void test_large_list()
{
    cout << "Test: Large list (100 elements)... ";

    LinkedList list;
    for (int i = 0; i < 100; ++i)
    {
        list.push_back("item_" + to_string(i));
    }

    assert(list.size() == 100);
    assert(list.at(0) == "item_0");
    assert(list.at(50) == "item_50");
    assert(list.at(99) == "item_99");

    cout << "PASS\n";
}

void test_iterator_loop_with_manual_increment()
{
    cout << "Test: Manual iterator loop... ";

    LinkedList list;
    list.push_back("a");
    list.push_back("b");
    list.push_back("c");
    list.push_back("d");
    list.push_back("e");

    vector<string> collected;
    for (ListIterator it = list.begin(); it != list.end(); ++it)
    {
        collected.push_back(it->value);
    }

    assert(collected.size() == 5);
    assert(collected[0] == "a");
    assert(collected[4] == "e");

    cout << "PASS\n";
}

void test_iterator_copy()
{
    cout << "Test: Iterator copy... ";

    Node first{"first"};
    Node second{"second", &first};

    ListIterator it1{&first};
    ListIterator it2 = it1;

    assert(it1 == it2);
    assert(it1->value == it2->value);

    ++it2;
    assert(it1 != it2);

    cout << "PASS\n";
}

void test_find_first_matching()
{
    cout << "Test: Find first matching element... ";

    LinkedList list;
    list.push_back("apple");
    list.push_back("apricot");
    list.push_back("banana");
    list.push_back("avocado");

    auto it = find_if(list.begin(), list.end(),
                      [](const Node &n)
                      {
                          return n.value[0] == 'a';
                      });

    assert(it != list.end());
    assert(it->value == "apple"); // First match

    cout << "PASS\n";
}

int main()
{
    cout << "========================================\n";
    cout << "  ITERATOR EXERCISE TEST SUITE\n";
    cout << "========================================\n\n";

    test_basic_node_creation();
    test_node_chaining();
    test_list_iterator_dereference();
    test_list_iterator_increment();
    test_list_iterator_equality();
    test_list_iterator_inequality();
    test_linkedlist_push_back();
    test_linkedlist_push_front();
    test_linkedlist_size();
    test_linkedlist_at();
    test_linkedlist_contains();
    test_for_each_algorithm();
    test_range_based_for_loop();
    test_find_algorithm();
    test_find_not_found();
    test_count_if_algorithm();
    test_vector_construction_from_iterator();
    test_empty_list_iteration();
    test_single_element_list();
    test_large_list();
    test_iterator_loop_with_manual_increment();
    test_iterator_copy();
    test_find_first_matching();

    cout << "\n========================================\n";
    cout << "  ALL TESTS PASSED! (23 tests)\n";
    cout << "========================================\n";

    return 0;
}
