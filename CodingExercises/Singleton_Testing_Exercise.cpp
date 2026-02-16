#include <iostream>
#include <vector>
#include <string>
#include <gtest/gtest.h>
#include "../include/singleton_testing.h"

using namespace std;

// ============================================================================
// SINGLETON PATTERN - GTEST CODING EXERCISE
// ============================================================================
// This exercise demonstrates testing patterns for Singleton design.
// Two approaches are shown:
// 1. SingletonRecordFinder - Tightly coupled to singleton (HARD to test)
// 2. ConfigurableRecordFinder - Dependency injection (EASY to test)
// ============================================================================

namespace SingletonExerciseTests
{
    // ========================================================================
    // TEST SUITE 1: Singleton Instance Tests
    // ========================================================================

    class DatabaseTests : public testing::Test
    {
    protected:
        void SetUp() override
        {
            // Reset instance count before each test
            SingletonDatabase::get_instance_count();
        }
    };

    // Test verifying singleton behavior
    TEST_F(DatabaseTests, IsSingletonTest)
    {
        auto &db = SingletonDatabase::get();
        auto &db2 = SingletonDatabase::get();

        // Both references should point to the same object
        ASSERT_EQ(&db, &db2);

        // Instance count should be 1 (created only once)
        ASSERT_EQ(1, SingletonDatabase::get_instance_count());
    }

    // Test data exists in singleton
    TEST_F(DatabaseTests, ContainsExpectedData)
    {
        auto &db = SingletonDatabase::get();

        EXPECT_EQ(37400068, db.get_population("Tokyo"));
        EXPECT_EQ(17500000, db.get_population("Seoul"));
        EXPECT_EQ(17400000, db.get_population("Mexico City"));
    }

    // ========================================================================
    // TEST SUITE 2: RecordFinder Tests - Problem Approach
    // ========================================================================

    class RecordFinderTests : public testing::Test
    {
    };

    // This test demonstrates the PROBLEM with Singleton approach
    // It works but is tightly coupled to the real database
    TEST_F(RecordFinderTests, SingletonTotalPopulationTest)
    {
        SingletonRecordFinder rf;
        vector<string> names{"Seoul", "Mexico City"};
        int tp = rf.total_population(names);

        // Test passes if database is initialized correctly
        EXPECT_EQ(17500000 + 17400000, tp);
    }

    // ========================================================================
    // TEST SUITE 3: RecordFinder Tests - Solution Approach
    // ========================================================================

    class DependencyInjectionTests : public testing::Test
    {
    };

    // This test demonstrates the SOLUTION using dependency injection
    // It uses mock data and is not dependent on the real database
    TEST_F(DependencyInjectionTests, DependencyInjectionTotalPopulationTest)
    {
        DummyDatabase db{};
        ConfigurableRecordFinder rf{db};

        vector<string> names{"alpha", "gamma"};
        EXPECT_EQ(4, rf.total_population(names)); // 1 + 3
    }

    // Test with different data in a different test (isolation)
    TEST_F(DependencyInjectionTests, DependencyInjectionMultipleCitiesTest)
    {
        DummyDatabase db{};
        ConfigurableRecordFinder rf{db};

        vector<string> names{"alpha", "beta", "gamma"};
        EXPECT_EQ(6, rf.total_population(names)); // 1 + 2 + 3
    }

    // Test with empty list
    TEST_F(DependencyInjectionTests, EmptyListTest)
    {
        DummyDatabase db{};
        ConfigurableRecordFinder rf{db};

        vector<string> names{};
        EXPECT_EQ(0, rf.total_population(names));
    }

    // Test with single city
    TEST_F(DependencyInjectionTests, SingleCityTest)
    {
        DummyDatabase db{};
        ConfigurableRecordFinder rf{db};

        vector<string> names{"beta"};
        EXPECT_EQ(2, rf.total_population(names));
    }

    // ========================================================================
    // TEST SUITE 4: Integration Tests
    // ========================================================================

    class IntegrationTests : public testing::Test
    {
    };

    // Test that same finder code works with different databases
    TEST_F(IntegrationTests, ConfigurablWithDummyDatabase)
    {
        DummyDatabase dummy{};
        ConfigurableRecordFinder finder(dummy);

        EXPECT_EQ(3, finder.total_population(vector<string>{"alpha"}));
    }

    // Test with real data
    TEST_F(IntegrationTests, ConfigurableWithSingletonDatabase)
    {
        SingletonDatabase &real_db = SingletonDatabase::get();
        ConfigurableRecordFinder finder(real_db);

        vector<string> cities{"Tokyo", "Seoul"};
        int expected = 37400068 + 17500000;
        EXPECT_EQ(expected, finder.total_population(cities));
    }

    // Test with configurable database
    TEST_F(IntegrationTests, ConfigurableWithConfigurableDatabase)
    {
        ConfigurableDatabase config_db{};
        ConfigurableRecordFinder finder(config_db);

        vector<string> cities{"London", "Paris"};
        int expected = 9002488 + 2161000;
        EXPECT_EQ(expected, finder.total_population(cities));
    }

    // ========================================================================
    // TEST SUITE 5: Comparison Tests
    // ========================================================================

    class ComparisonTests : public testing::Test
    {
    };

    // Demonstrate that injection is better than singleton
    TEST_F(ComparisonTests, SameCodeWorksWithMultipleDatabases)
    {
        // Create two different finders with different databases
        DummyDatabase dummy{};
        SingletonDatabase &real = SingletonDatabase::get();

        ConfigurableRecordFinder finder1(dummy);
        ConfigurableRecordFinder finder2(real);

        // Same API, different results based on data
        vector<string> test_cities{"alpha"};
        vector<string> real_cities{"Tokyo"};

        int test_result = finder1.total_population(test_cities);
        int real_result = finder2.total_population(real_cities);

        EXPECT_EQ(1, test_result);
        EXPECT_EQ(37400068, real_result);

        // Both work correctly without changing finder code!
    }

    // ========================================================================
    // TEST SUITE 6: Maintainability Tests
    // ========================================================================

    class MaintainabilityTests : public testing::Test
    {
    };

    // Easy to test different scenarios independently
    TEST_F(MaintainabilityTests, ScenarioOne)
    {
        DummyDatabase db{};
        ConfigurableRecordFinder rf{db};
        EXPECT_EQ(3, rf.total_population(vector<string>{"gamma"}));
    }

    // Can run scenario tests without affecting each other
    TEST_F(MaintainabilityTests, ScenarioTwo)
    {
        DummyDatabase db{};
        ConfigurableRecordFinder rf{db};
        EXPECT_EQ(6, rf.total_population(vector<string>{"alpha", "beta", "gamma"}));
    }

    // Another independent test
    TEST_F(MaintainabilityTests, ScenarioThree)
    {
        ConfigurableDatabase db{};
        ConfigurableRecordFinder rf{db};
        EXPECT_EQ(11163488, rf.total_population(vector<string>{"London", "Paris"}));
    }

} // namespace SingletonExerciseTests

// ============================================================================
// LEARNING GUIDE
// ============================================================================
/*

KEY CONCEPTS TESTED:

1. SINGLETON PATTERN PROBLEM:
   - SingletonRecordFinder is tightly coupled
   - Cannot test with mock data
   - Difficult to isolate for unit testing
   - Requires real database to exist

2. DEPENDENCY INJECTION SOLUTION:
   - ConfigurableRecordFinder accepts Database interface
   - Works with any Database implementation
   - Easy to test with mock data (DummyDatabase)
   - Tests are independent and repeatable

3. CORE PRINCIPLE:
   - Program to interfaces, not implementations
   - Inject dependencies through constructor
   - Enables testability and flexibility

4. TEST STRUCTURE:
   - Test Suite 1: Verify singleton works
   - Test Suite 2: Current (bad) approach
   - Test Suite 3: Better approach with DI
   - Test Suite 4: Integration tests
   - Test Suite 5: Comparison tests
   - Test Suite 6: Maintainability tests

5. RUNNING TESTS:
   g++ -std=c++17 -I. -Ipath/to/gtest/include \
       CodingExercises/Singleton_Testing_Exercise.cpp \
       -Lpath/to/gtest/lib -lgtest -lgtest_main -o test
   ./test

   OR with gtest pre-installed:
   g++ -std=c++17 -I. CodingExercises/Singleton_Testing_Exercise.cpp \
       -lgtest -lgtest_main -o test
   ./test

EXERCISE QUESTIONS:

1. Why is SingletonRecordFinder hard to test?
   Answer: Because it's tightly coupled to SingletonDatabase and
           cannot use mock implementations.

2. Why is ConfigurableRecordFinder easy to test?
   Answer: Because it depends on the Database interface, not a
           concrete implementation. Any Database can be injected.

3. What's the difference between Singleton and Service Locator?
   Answer: Service Locator is still a form of global state but
           provides better flexibility through registration.

4. What principle does DI follow?
   Answer: Dependency Inversion Principle (DIP) - depend on
           abstractions, not concrete implementations.

5. Why is DummyDatabase useful?
   Answer: It provides controlled test data without needing
           external files or real databases.

*/
