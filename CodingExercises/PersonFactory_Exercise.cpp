/*
=============================================================================
FACTORY PATTERN EXERCISE - PersonFactory
From: Udemy Design Patterns in Modern C++ Course
Topic: Simple Factory Pattern with Auto-incrementing IDs
=============================================================================

EXERCISE DESCRIPTION:
Create a PersonFactory that generates Person objects with auto-incrementing IDs.
Each time create_person() is called, it should:
1. Return a new Person object
2. Auto-increment the ID counter
3. Assign the current ID to the Person

LEARNING OBJECTIVES:
✓ Understand factory methods
✓ Manage internal state (counter)
✓ Create objects with consistent IDs
✓ Write unit tests with Google Test (gtest)

=============================================================================
*/

#include <vector>
#include <string>
#include <iostream>
using namespace std;

namespace FactoryExercise
{

    // =========================================================================
    // SOLUTION: Person and PersonFactory
    // =========================================================================

    struct Person
    {
        int id;
        string name;

        // Helper for debugging
        void print() const
        {
            cout << "Person(id=" << id << ", name=" << name << ")\n";
        }
    };

    class PersonFactory
    {
    private:
        int id{0}; // Auto-incrementing ID counter

    public:
        /**
         * Create a new Person with auto-incrementing ID
         * @param name The name of the person
         * @return Person object with next available ID
         */
        Person create_person(const string &name)
        {
            return {id++, name}; // Return current ID, then increment
        }

        /**
         * Get the next ID that will be assigned (without incrementing)
         * @return The next ID value
         */
        int get_next_id() const
        {
            return id;
        }

        /**
         * Reset the ID counter back to 0
         * Useful for testing or starting fresh
         */
        void reset()
        {
            id = 0;
        }
    };

    // =========================================================================
    // MANUAL TESTS (Run without gtest)
    // =========================================================================

    void manual_tests()
    {
        cout << "=== PERSON FACTORY EXERCISE ===" << endl
             << endl;

        cout << "--- Test 1: Basic Creation ---\n";
        PersonFactory pf;

        auto p1 = pf.create_person("Alice");
        p1.print(); // Person(id=0, name=Alice)

        auto p2 = pf.create_person("Bob");
        p2.print(); // Person(id=1, name=Bob)

        auto p3 = pf.create_person("Charlie");
        p3.print(); // Person(id=2, name=Charlie)

        cout << "\n--- Test 2: ID Auto-Increment ---\n";
        cout << "p1.id = " << p1.id << " (expected: 0)\n";
        cout << "p2.id = " << p2.id << " (expected: 1)\n";
        cout << "p3.id = " << p3.id << " (expected: 2)\n";

        cout << "\n--- Test 3: Next ID Preview ---\n";
        cout << "Next ID will be: " << pf.get_next_id() << "\n";

        auto p4 = pf.create_person("Diana");
        cout << "Created person with ID: " << p4.id << "\n";
        cout << "Next ID now: " << pf.get_next_id() << "\n";

        cout << "\n--- Test 4: Multiple Factories ---\n";
        PersonFactory pf1, pf2;

        auto person1 = pf1.create_person("Frank");
        auto person2 = pf2.create_person("Grace");

        cout << "Factory 1 created: ";
        person1.print(); // Person(id=0, name=Frank)

        cout << "Factory 2 created: ";
        person2.print(); // Person(id=0, name=Grace)
        cout << "(Note: Each factory has its own counter)\n";

        cout << "\n--- Test 5: Reset Counter ---\n";
        cout << "Current next ID: " << pf.get_next_id() << "\n";
        pf.reset();
        cout << "After reset, next ID: " << pf.get_next_id() << "\n";

        auto p5 = pf.create_person("Henry");
        cout << "Created person: ";
        p5.print(); // Person(id=0, name=Henry)

        cout << "\n--- Test 6: Bulk Creation ---\n";
        PersonFactory bulk_factory;
        vector<Person> people;

        vector<string> names = {"John", "Jane", "Jack", "Jill", "Joe"};
        for (const auto &name : names)
        {
            people.push_back(bulk_factory.create_person(name));
        }

        cout << "Created " << people.size() << " people:\n";
        for (const auto &person : people)
        {
            cout << "  ";
            person.print();
        }
    }

} // namespace FactoryExercise

// =========================================================================
// GOOGLE TEST TESTS (Uncomment #include <gtest/gtest.h> to use)
// =========================================================================

#ifdef GTEST_INCLUDED

using namespace FactoryExercise;

namespace FactoryExerciseTests
{

    class PersonFactoryTest : public testing::Test
    {
    protected:
        PersonFactory factory;
    };

    // Test 1: Basic person creation with correct ID
    TEST_F(PersonFactoryTest, CreatePersonWithCorrectID)
    {
        auto person = factory.create_person("Alice");
        ASSERT_EQ(0, person.id);
        ASSERT_EQ("Alice", person.name);
    }

    // Test 2: Multiple creations have incrementing IDs
    TEST_F(PersonFactoryTest, IDsAutoIncrement)
    {
        auto p1 = factory.create_person("Alice");
        auto p2 = factory.create_person("Bob");
        auto p3 = factory.create_person("Charlie");

        ASSERT_EQ(0, p1.id);
        ASSERT_EQ(1, p2.id);
        ASSERT_EQ(2, p3.id);
    }

    // Test 3: Simple person test (from course)
    TEST_F(PersonFactoryTest, SimplePersonTest)
    {
        auto p1 = factory.create_person("Chris");
        ASSERT_EQ("Chris", p1.name);

        auto p2 = factory.create_person("Sarah");
        ASSERT_EQ(1, p2.id) << "Expected the second created person's id to be = 1";
    }

    // Test 4: Get next ID preview
    TEST_F(PersonFactoryTest, GetNextID)
    {
        ASSERT_EQ(0, factory.get_next_id());

        factory.create_person("Alice");
        ASSERT_EQ(1, factory.get_next_id());

        factory.create_person("Bob");
        ASSERT_EQ(2, factory.get_next_id());
    }

    // Test 5: Reset functionality
    TEST_F(PersonFactoryTest, ResetCounter)
    {
        factory.create_person("Alice");
        factory.create_person("Bob");
        ASSERT_EQ(2, factory.get_next_id());

        factory.reset();
        ASSERT_EQ(0, factory.get_next_id());

        auto person = factory.create_person("Charlie");
        ASSERT_EQ(0, person.id);
    }

    // Test 6: Multiple factories have independent counters
    TEST_F(PersonFactoryTest, IndependentFactories)
    {
        PersonFactory factory2;

        auto p1 = factory.create_person("Alice");
        auto p2 = factory2.create_person("Bob");

        ASSERT_EQ(0, p1.id);
        ASSERT_EQ(0, p2.id); // factory2 starts at 0
    }

    // Test 7: Bulk creation
    TEST_F(PersonFactoryTest, BulkCreation)
    {
        vector<Person> people;
        vector<string> names = {"Alice", "Bob", "Charlie", "Diana", "Eve"};

        for (const auto &name : names)
        {
            people.push_back(factory.create_person(name));
        }

        ASSERT_EQ(5, people.size());
        for (int i = 0; i < people.size(); ++i)
        {
            ASSERT_EQ(i, people[i].id);
            ASSERT_EQ(names[i], people[i].name);
        }
    }

} // namespace FactoryExerciseTests

#endif // GTEST_INCLUDED

// =========================================================================
// MAIN FUNCTION - Run manual tests
// =========================================================================

int main()
{
    using namespace FactoryExercise;

    cout << "\n";
    cout << "====================================================\n";
    cout << "FACTORY PATTERN EXERCISE - PersonFactory\n";
    cout << "====================================================\n\n";

    manual_tests();

    cout << "\n====================================================\n";
    cout << "All manual tests completed!\n";
    cout << "====================================================\n\n";

    return 0;
}

/*
=============================================================================
KEY TAKEAWAYS:
=============================================================================

1. FACTORY PATTERN:
   - Encapsulates object creation logic
   - Provides controlled creation through a factory method
   - Maintains internal state (ID counter)

2. PERSON FACTORY USAGE:
   PersonFactory pf;
   auto person = pf.create_person("John");  // Creates with ID=0
   auto person2 = pf.create_person("Jane");  // Creates with ID=1

3. AUTO-INCREMENT PATTERN:
   - Uses internal counter (id variable)
   - Increments after each creation (id++)
   - Ensures unique, sequential IDs

4. BEST PRACTICES:
   - Encapsulate the counter (private member)
   - Provide factory method as public interface
   - Optional: provide getter/reset methods for testing

5. TESTING WITH GTEST:
   - Use testing::Test base class
   - Create TEST_F macros for test cases
   - Use ASSERT_EQ for equality checks
   - Use << for custom error messages

=============================================================================
EXTENSIONS/EXERCISES:
=============================================================================

1. Add person removal (remove by ID)
2. Add person lookup (find person by ID)
3. Use a map to store all created persons
4. Add validation (name cannot be empty)
5. Add person age field with factory defaults
6. Add multiple factory types (PersonFactory, CompanyFactory, etc.)

=============================================================================
*/
