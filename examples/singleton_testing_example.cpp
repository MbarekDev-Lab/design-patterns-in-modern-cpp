#include <iostream>
#include <vector>
#include <cassert>
#include "../include/singleton_testing.h"

using namespace std;

// ============================================================================
// EXAMPLE 1: VIOLATION - Tight Coupling with Singleton
// ============================================================================

void example_1_singleton_problem()
{
    cout << "\n========== EXAMPLE 1: Problem - Tight Coupling ==========\n";

    cout << "RealDatabase:\n";
    SingletonDatabase::get().show_all();

    cout << "\nUsing SingletonRecordFinder (tightly coupled):\n";
    SingletonRecordFinder finder;

    vector<string> cities = {"Tokyo", "Delhi"};
    int population = finder.total_population(cities);

    cout << "Total population of " << cities[0] << " and " << cities[1]
         << ": " << population << "\n";

    cout << "\nProblems:\n";
    cout << "  ✗ Cannot test without real database file\n";
    cout << "  ✗ Cannot use mock data\n";
    cout << "  ✗ Hard to test in isolation\n";
    cout << "  ✗ Cannot swap implementations\n";
}

// ============================================================================
// EXAMPLE 2: SOLUTION - Dependency Injection with Interface
// ============================================================================

void example_2_dependency_injection()
{
    cout << "\n========== EXAMPLE 2: Solution - Dependency Injection ==========\n";

    cout << "Using DummyDatabase (test data):\n";
    DummyDatabase dummy;
    dummy.show_all();

    cout << "\nUsing ConfigurableRecordFinder (loosely coupled):\n";
    ConfigurableRecordFinder finder(dummy);

    vector<string> cities = {"alpha", "beta"};
    int population = finder.total_population(cities);

    cout << "Total population: " << population << "\n";

    cout << "\nBenefits:\n";
    cout << "  ✓ Works with any Database implementation\n";
    cout << "  ✓ Can test with mock data\n";
    cout << "  ✓ Easy to test in isolation\n";
    cout << "  ✓ Can swap implementations at runtime\n";
}

// ============================================================================
// EXAMPLE 3: Testing with Different Implementations
// ============================================================================

void example_3_testable_code()
{
    cout << "\n========== EXAMPLE 3: Testing Different Implementations ==========\n";

    // Test with dummy data
    cout << "Test 1: Using DummyDatabase\n";
    {
        DummyDatabase db;
        ConfigurableRecordFinder finder(db);
        vector<string> cities = {"alpha", "gamma"};
        int result = finder.total_population(cities);
        cout << "  Result: " << result << "\n";
        assert(result == 4); // 1 + 3
        cout << "  ✓ Test passed\n";
    }

    // Test with real data
    cout << "\nTest 2: Using ConfigurableDatabase\n";
    {
        ConfigurableDatabase db;
        ConfigurableRecordFinder finder(db);
        vector<string> cities = {"London", "Paris"};
        int result = finder.total_population(cities);
        cout << "  Result: " << result << "\n";
        assert(result == 9002488 + 2161000);
        cout << "  ✓ Test passed\n";
    }

    cout << "\nProblems with SingletonRecordFinder:\n";
    cout << "  ✗ Cannot run both tests reliably\n";
    cout << "  ✗ Requires specific environment setup\n";
    cout << "  ✗ Cannot switch database between tests\n";
}

// ============================================================================
// EXAMPLE 4: Service Locator Pattern (Alternative)
// ============================================================================

void example_4_service_locator()
{
    cout << "\n========== EXAMPLE 4: Service Locator Pattern ==========\n";

    cout << "Setup: Registering DummyDatabase\n";
    ServiceLocator::set_database(make_unique<DummyDatabase>());

    cout << "Using service locator to get database\n";
    // In real code, consumer would just use ServiceLocator::get_database()
    // Example: auto& db = ServiceLocator::get_database();

    cout << "Switching to different implementation\n";
    ServiceLocator::set_database(make_unique<ConfigurableDatabase>());

    cout << "\nBenefits:\n";
    cout << "  ✓ Single point of database registration\n";
    cout << "  ✓ Can change implementation globally\n";
    cout << "  ✓ Useful for plugin architectures\n";

    cout << "\nDrawbacks:\n";
    cout << "  ✗ Still a form of global state\n";
    cout << "  ✗ Can lead to hidden dependencies\n";
    cout << "  ✗ Prefer constructor injection when possible\n";

    ServiceLocator::reset();
}

// ============================================================================
// EXAMPLE 5: Comparison - Direct Dependency Injection
// ============================================================================

void example_5_comparison()
{
    cout << "\n========== EXAMPLE 5: Comparison of Approaches ==========\n";

    cout << "Approach 1: Singleton (Bad)\n";
    cout << "  Code: SingletonRecordFinder finder;\n";
    cout << "  Problems:\n";
    cout << "    - Tight coupling\n";
    cout << "    - Hard to test\n";
    cout << "    - Cannot mock\n\n";

    cout << "Approach 2: Constructor Injection (Good)\n";
    cout << "  Code: ConfigurableRecordFinder finder(database);\n";
    cout << "  Benefits:\n";
    cout << "    - Loose coupling\n";
    cout << "    - Easy to test\n";
    cout << "    - Can mock implementations\n";
    cout << "    - Dependencies visible in constructor\n\n";

    cout << "Approach 3: Service Locator (OK)\n";
    cout << "  Code: auto& db = ServiceLocator::get_database();\n";
    cout << "  Trade-offs:\n";
    cout << "    - Better than singleton\n";
    cout << "    - Worse than constructor injection\n";
    cout << "    - Hidden dependencies\n";
}

// ============================================================================
// EXAMPLE 6: Unit Test Demonstration
// ============================================================================

void example_6_unit_testing()
{
    cout << "\n========== EXAMPLE 6: Unit Testing Example ==========\n";

    cout << "Test Suite: RecordFinder Tests\n\n";

    // Test 1: Empty cities list
    cout << "Test 1: Empty cities list\n";
    {
        DummyDatabase db;
        ConfigurableRecordFinder finder(db);
        vector<string> cities;
        assert(finder.total_population(cities) == 0);
        cout << "  ✓ Passed\n";
    }

    // Test 2: Single city
    cout << "\nTest 2: Single city\n";
    {
        DummyDatabase db;
        ConfigurableRecordFinder finder(db);
        vector<string> cities = {"alpha"};
        assert(finder.total_population(cities) == 1);
        cout << "  ✓ Passed\n";
    }

    // Test 3: Multiple cities
    cout << "\nTest 3: Multiple cities\n";
    {
        DummyDatabase db;
        ConfigurableRecordFinder finder(db);
        vector<string> cities = {"alpha", "beta", "gamma"};
        assert(finder.total_population(cities) == 6); // 1+2+3
        cout << "  ✓ Passed\n";
    }

    // Test 4: Works with different database
    cout << "\nTest 4: Works with different database\n";
    {
        ConfigurableDatabase db;
        ConfigurableRecordFinder finder(db);
        vector<string> cities = {"London", "Berlin"};
        int result = finder.total_population(cities);
        assert(result == 9002488 + 3645000);
        cout << "  ✓ Passed\n";
    }

    cout << "\nAll tests passed! ✓\n";
}

// ============================================================================
// EXAMPLE 7: Real-world Scenario
// ============================================================================

void example_7_real_world()
{
    cout << "\n========== EXAMPLE 7: Real-world Application ==========\n";

    cout << "Scenario: Building a Population Analysis Service\n\n";

    cout << "Development Environment (using test data):\n";
    {
        DummyDatabase testDb;
        ConfigurableRecordFinder finder(testDb);

        vector<string> test_capitals = {"alpha", "beta", "gamma"};
        int population = finder.total_population(test_capitals);
        cout << "  Test result: " << population << "\n";
    }

    cout << "\nProduction Environment (using real data):\n";
    {
        ConfigurableDatabase prodDb;
        ConfigurableRecordFinder finder(prodDb);

        vector<string> capitals = {"London", "Paris", "Berlin"};
        int population = finder.total_population(capitals);
        cout << "  Production result: " << population << "\n";
    }

    cout << "\nIntegration Testing:\n";
    {
        SingletonDatabase &realDb = SingletonDatabase::get();
        ConfigurableRecordFinder finder(realDb);

        vector<string> cities = {"Tokyo", "Shanghai"};
        int population = finder.total_population(cities);
        cout << "  Integration result: " << population << "\n";
    }

    cout << "\nKey Insight: Same code works with all implementations!\n";
}

// ============================================================================
// Main Entry Point
// ============================================================================

int main()
{
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║     SINGLETON PATTERN - Testing & Dependency Injection    ║\n";
    cout << "║  Problem: Singletons are hard to test and tightly coupled ║\n";
    cout << "║  Solution: Use interfaces and dependency injection        ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";

    example_1_singleton_problem();
    example_2_dependency_injection();
    example_3_testable_code();
    example_4_service_locator();
    example_5_comparison();
    example_6_unit_testing();
    example_7_real_world();

    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                    KEY TAKEAWAYS                           ║\n";
    cout << "├────────────────────────────────────────────────────────────┤\n";
    cout << "│ 1. Singletons make code hard to test and couple modules   │\n";
    cout << "│ 2. Use interfaces and dependency injection instead        │\n";
    cout << "│ 3. Pass dependencies through constructor (explicit)       │\n";
    cout << "│ 4. Mock objects enable unit testing                       │\n";
    cout << "│ 5. Loose coupling = easier testing and maintenance        │\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    return 0;
}
