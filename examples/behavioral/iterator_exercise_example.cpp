#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "../../include/behavioral/iterator_exercise.h"

using namespace std;

// ============================================================================
// ITERATOR PATTERN EXERCISE EXAMPLES: Linked List with Boost Iterator Facade
// ============================================================================

// Example 1: Basic iteration with for_each
void example_basic_iteration()
{
    cout << "\n=== Example 1: Basic Iteration with for_each ===\n";

    Node alpha{"alpha"};
    Node beta{"beta", &alpha};
    Node gamma{"gamma", &beta};

    cout << "Linked list: ";
    for_each(ListIterator{&alpha}, ListIterator{},
             [](const Node &n)
             {
                 cout << n.value << " ";
             });
    cout << "\n";
}

// Example 2: Manual iteration with while loop
void example_manual_iteration()
{
    cout << "\n=== Example 2: Manual Iteration ===\n";

    Node first{"first"};
    Node second{"second", &first};
    Node third{"third", &second};
    Node fourth{"fourth", &third};

    cout << "Iterating manually: ";
    for (ListIterator it{&first}; it != ListIterator{}; ++it)
    {
        cout << it->value << " ";
    }
    cout << "\n";
}

// Example 3: LinkedList wrapper with push_back
void example_linkedlist_wrapper()
{
    cout << "\n=== Example 3: LinkedList Wrapper ===\n";

    LinkedList fruits;
    fruits.push_back("apple");
    fruits.push_back("banana");
    fruits.push_back("cherry");
    fruits.push_back("date");

    cout << "Fruits list: ";
    for (auto &node : fruits)
    {
        cout << node.value << " ";
    }
    cout << "\n";

    cout << "List size: " << fruits.size() << "\n";
}

// Example 4: LinkedList with push_front
void example_push_front()
{
    cout << "\n=== Example 4: Push Front ===\n";

    LinkedList colors;
    colors.push_back("blue");
    colors.push_back("green");

    cout << "Before push_front: ";
    for (auto &node : colors)
    {
        cout << node.value << " ";
    }
    cout << "\n";

    colors.push_front("red");
    cout << "After push_front(red): ";
    for (auto &node : colors)
    {
        cout << node.value << " ";
    }
    cout << "\n";
}

// Example 5: Using std::vector with range construction
void example_range_construction()
{
    cout << "\n=== Example 5: Range Construction to Vector ===\n";

    LinkedList animals;
    animals.push_back("cat");
    animals.push_back("dog");
    animals.push_back("bird");
    animals.push_back("fish");

    // Use the iterator to construct a vector
    vector<Node> animal_vector(animals.begin(), animals.end());

    cout << "Animal count: " << animal_vector.size() << "\n";
    cout << "Vector contents: ";
    for (const auto &node : animal_vector)
    {
        cout << node.value << " ";
    }
    cout << "\n";
}

// Example 6: Using std::find algorithm
void example_find_algorithm()
{
    cout << "\n=== Example 6: Using std::find ===\n";

    LinkedList cities;
    cities.push_back("New York");
    cities.push_back("Los Angeles");
    cities.push_back("Chicago");
    cities.push_back("Houston");

    // Find a city
    auto it = find_if(cities.begin(), cities.end(),
                      [](const Node &n)
                      {
                          return n.value == "Chicago";
                      });

    if (it != cities.end())
    {
        cout << "Found: " << it->value << "\n";
    }
    else
    {
        cout << "Not found\n";
    }
}

// Example 7: Count elements with specific property
void example_count_if()
{
    cout << "\n=== Example 7: Count If ===\n";

    LinkedList words;
    words.push_back("apple");    // 5 chars
    words.push_back("cat");      // 3 chars
    words.push_back("dog");      // 3 chars
    words.push_back("elephant"); // 8 chars
    words.push_back("bee");      // 3 chars

    int three_letter_words = count_if(words.begin(), words.end(),
                                      [](const Node &n)
                                      {
                                          return n.value.length() == 3;
                                      });

    cout << "Words with 3 letters: " << three_letter_words << "\n";
}

// Example 8: Transform with copy
void example_transform()
{
    cout << "\n=== Example 8: Transform (Uppercase) ===\n";

    LinkedList original;
    original.push_back("hello");
    original.push_back("world");

    // Extract values into a vector
    vector<string> values;
    for (auto &node : original)
    {
        string upper = node.value;
        for (char &c : upper)
            c = toupper(c);
        values.push_back(upper);
    }

    cout << "Original: ";
    for (auto &node : original)
        cout << node.value << " ";
    cout << "\n";

    cout << "Uppercase: ";
    for (const auto &val : values)
        cout << val << " ";
    cout << "\n";
}

int main()
{
    cout << "========================================\n";
    cout << "  ITERATOR EXERCISE: Linked List\n";
    cout << "========================================\n";

    example_basic_iteration();
    example_manual_iteration();
    example_linkedlist_wrapper();
    example_push_front();
    example_range_construction();
    example_find_algorithm();
    example_count_if();
    example_transform();

    cout << "\n========================================\n";

    return 0;
}
