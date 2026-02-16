#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "../include/singleton_testing.h"

using namespace std;

// ============================================================================
// SINGLETON PATTERN - TESTING & DEPENDENCY INJECTION EXERCISE
// ============================================================================
// Manual test implementation that mirrors the gtest version
// ============================================================================

int test_count = 0;
int passed_count = 0;

#define TEST_CASE(name)                 \
    cout << "\nTest: " << name << "\n"; \
    test_count++;                       \
    try

#define EXPECT_EQ(expected, actual)                                                \
    if ((expected) != (actual))                                                    \
    {                                                                              \
        cout << "  ✗ FAILED: Expected " << expected << ", got " << actual << "\n"; \
    }                                                                              \
    else                                                                           \
    {                                                                              \
        cout << "  ✓ PASSED\n";                                                    \
        passed_count++;                                                            \
    }

#define ASSERT_EQ(expected, actual)                                                \
    if ((expected) != (actual))                                                    \
    {                                                                              \
        cout << "  ✗ FAILED: Expected " << expected << ", got " << actual << "\n"; \
        throw runtime_error("Assertion failed");                                   \
    }                                                                              \
    else                                                                           \
    {                                                                              \
        cout << "  ✓ PASSED\n";                                                    \
        passed_count++;                                                            \
    }

// ============================================================================
// TEST SUITE 1: Database Tests
// ============================================================================

void test_singleton_instance()
{
    TEST_CASE("Is Singleton Test")
    {
        auto &db = SingletonDatabase::get();
        auto &db2 = SingletonDatabase::get();

        ASSERT_EQ(1, SingletonDatabase::get_instance_count());
    }
    catch (const exception &e)
    {
        cout << "  Exception: " << e.what() << "\n";
    }
}

void test_contains_expected_data()
{
    TEST_CASE("Contains Expected Data")
    {
        auto &db = SingletonDatabase::get();

        EXPECT_EQ(37400068, db.get_population("Tokyo"));
        EXPECT_EQ(17500000, db.get_population("Seoul"));
        EXPECT_EQ(17400000, db.get_population("Mexico City"));
    }
    catch (const exception &e)
    {
        cout << "  Exception: " << e.what() << "\n";
    }
}

// ============================================================================
// TEST SUITE 2: Singleton RecordFinder (Tightly Coupled)
// ============================================================================

void test_singleton_total_population()
{
    TEST_CASE("Singleton Total Population Test")
    {
        SingletonRecordFinder rf;
        vector<string> names{"Seoul", "Mexico City"};
        int tp = rf.total_population(names);

        EXPECT_EQ(17500000 + 17400000, tp);
    }
    catch (const exception &e)
    {
        cout << "  Exception: " << e.what() << "\n";
    }
}

// ============================================================================
// TEST SUITE 3: Dependency Injection RecordFinder (Loosely Coupled)
// ============================================================================

void test_dependency_injection_total_population()
{
    TEST_CASE("Dependency Injection Total Population Test")
    {
        DummyDatabase db{};
        ConfigurableRecordFinder rf{db};

        vector<string> names{"alpha", "gamma"};
        EXPECT_EQ(4, rf.total_population(names)); // 1 + 3
    }
    catch (const exception &e)
    {
        cout << "  Exception: " << e.what() << "\n";
    }
}

void test_dependency_injection_multiple_cities()
{
    TEST_CASE("Dependency Injection Multiple Cities Test")
    {
        DummyDatabase db{};
        ConfigurableRecordFinder rf{db};

        vector<string> names{"alpha", "beta", "gamma"};
        EXPECT_EQ(6, rf.total_population(names)); // 1 + 2 + 3
    }
    catch (const exception &e)
    {
        cout << "  Exception: " << e.what() << "\n";
    }
}

void test_empty_list()
{
    TEST_CASE("Empty List Test")
    {
        DummyDatabase db{};
        ConfigurableRecordFinder rf{db};

        vector<string> names{};
        EXPECT_EQ(0, rf.total_population(names));
    }
    catch (const exception &e)
    {
        cout << "  Exception: " << e.what() << "\n";
    }
}

void test_single_city()
{
    TEST_CASE("Single City Test")
    {
        DummyDatabase db{};
        ConfigurableRecordFinder rf{db};

        vector<string> names{"beta"};
        EXPECT_EQ(2, rf.total_population(names));
    }
    catch (const exception &e)
    {
        cout << "  Exception: " << e.what() << "\n";
    }
}

// ============================================================================
// TEST SUITE 4: Integration Tests
// ============================================================================

void test_configurable_with_dummy()
{
    TEST_CASE("Configurable With Dummy Database")
    {
        DummyDatabase dummy{};
        ConfigurableRecordFinder finder(dummy);

        EXPECT_EQ(1, finder.total_population(vector<string>{"alpha"}));
    }
    catch (const exception &e)
    {
        cout << "  Exception: " << e.what() << "\n";
    }
}

void test_configurable_with_singleton()
{
    TEST_CASE("Configurable With Singleton Database")
    {
        SingletonDatabase &real_db = SingletonDatabase::get();
        ConfigurableRecordFinder finder(real_db);

        vector<string> cities{"Tokyo", "Seoul"};
        int expected = 37400068 + 17500000;
        EXPECT_EQ(expected, finder.total_population(cities));
    }
    catch (const exception &e)
    {
        cout << "  Exception: " << e.what() << "\n";
    }
}

void test_configurable_with_configurable()
{
    TEST_CASE("Configurable With Configurable Database")
    {
        ConfigurableDatabase config_db{};
        ConfigurableRecordFinder finder(config_db);

        vector<string> cities{"London", "Paris"};
        int expected = 9002488 + 2161000;
        EXPECT_EQ(expected, finder.total_population(cities));
    }
    catch (const exception &e)
    {
        cout << "  Exception: " << e.what() << "\n";
    }
}

// ============================================================================
// TEST SUITE 5: Comparison Tests
// ============================================================================

void test_same_code_multiple_databases()
{
    TEST_CASE("Same Code Works With Multiple Databases")
    {
        DummyDatabase dummy{};
        SingletonDatabase &real = SingletonDatabase::get();

        ConfigurableRecordFinder finder1(dummy);
        ConfigurableRecordFinder finder2(real);

        vector<string> test_cities{"alpha"};
        vector<string> real_cities{"Tokyo"};

        int test_result = finder1.total_population(test_cities);
        int real_result = finder2.total_population(real_cities);

        EXPECT_EQ(1, test_result);
        EXPECT_EQ(37400068, real_result);
    }
    catch (const exception &e)
    {
        cout << "  Exception: " << e.what() << "\n";
    }
}

// ============================================================================
// Main Entry Point
// ============================================================================

int main()
{
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║  SINGLETON PATTERN - TESTING & DEPENDENCY INJECTION       ║\n";
    cout << "║           Coding Exercise - Manual Test Suite             ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";

    cout << "\n"
         << string(60, '=') << "\n";
    cout << "Running Test Suite\n";
    cout << string(60, '=') << "\n";

    // Database Tests
    test_singleton_instance();
    test_contains_expected_data();

    // Singleton RecordFinder (Bad)
    cout << "\n--- Singleton RecordFinder (Tightly Coupled) ---\n";
    test_singleton_total_population();

    // Dependency Injection RecordFinder (Good)
    cout << "\n--- ConfigurableRecordFinder (Loosely Coupled) ---\n";
    test_dependency_injection_total_population();
    test_dependency_injection_multiple_cities();
    test_empty_list();
    test_single_city();

    // Integration Tests
    cout << "\n--- Integration Tests ---\n";
    test_configurable_with_dummy();
    test_configurable_with_singleton();
    test_configurable_with_configurable();

    // Comparison Tests
    cout << "\n--- Comparison Tests ---\n";
    test_same_code_multiple_databases();

    // Summary
    cout << "\n"
         << string(60, '=') << "\n";
    cout << "Test Results Summary\n";
    cout << string(60, '=') << "\n";
    cout << "Test Cases: " << test_count << "\n";
    cout << "Assertions Passed: " << passed_count << "\n";

    if (passed_count == 14 && test_count == 11)
    {
        cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        cout << "║  ALL TESTS PASSED! ✓                                       ║\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n";
        return 0;
    }
    else
    {
        cout << "Expected 14 assertions, got " << passed_count << "\n";
        cout << "\n✗ Some tests failed\n";
        return 1;
    }
}
