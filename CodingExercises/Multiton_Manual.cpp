#include <iostream>
#include <cassert>
#include <creational/multiton.h>
#include <string>
#include <memory>

using namespace std;
using namespace multiton_solution;

// ============================================================================
// MANUAL TEST FRAMEWORK
// ============================================================================

struct TestResult
{
    int total = 0;
    int passed = 0;
    int failed = 0;

    void print_summary()
    {
        cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        cout << "║              TEST RESULTS SUMMARY                          ║\n";
        cout << "├────────────────────────────────────────────────────────────┤\n";
        cout << "│ Total Tests:  " << total << "\n";
        cout << "│ Passed:       " << passed << " ✓\n";
        cout << "│ Failed:       " << failed << " ✗\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    }
};

TestResult test_results;

#define TEST_CASE(name)                    \
    cout << "\n[TEST] " << (name) << "\n"; \
    test_results.total++

#define ASSERT_TRUE(condition, message)              \
    if (condition)                                   \
    {                                                \
        cout << "  ✓ " << (message) << "\n";         \
        test_results.passed++;                       \
    }                                                \
    else                                             \
    {                                                \
        cout << "  ✗ FAILED: " << (message) << "\n"; \
        test_results.failed++;                       \
    }

#define ASSERT_EQ(actual, expected, message)                                                                   \
    if ((actual) == (expected))                                                                                \
    {                                                                                                          \
        cout << "  ✓ " << (message) << "\n";                                                                   \
        test_results.passed++;                                                                                 \
    }                                                                                                          \
    else                                                                                                       \
    {                                                                                                          \
        cout << "  ✗ FAILED: " << (message) << " (expected: " << (expected) << ", got: " << (actual) << ")\n"; \
        test_results.failed++;                                                                                 \
    }

#define ASSERT_NE(actual, expected, message)                               \
    if ((actual) != (expected))                                            \
    {                                                                      \
        cout << "  ✓ " << (message) << "\n";                               \
        test_results.passed++;                                             \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        cout << "  ✗ FAILED: " << (message) << " (should not be equal)\n"; \
        test_results.failed++;                                             \
    }

// ============================================================================
// TEST CLASSES
// ============================================================================

class TestService : public Multiton<TestService, string>
{
public:
    TestService(const string &name) : name_(name), instance_num_(++instance_count_)
    {
    }

    string get_name() const { return name_; }
    int get_instance_num() const { return instance_num_; }

    static int get_instance_count() { return instance_count_; }
    static void reset_count() { instance_count_ = 0; }

private:
    string name_;
    int instance_num_;
    static int instance_count_;
};
int TestService::instance_count_ = 0;

enum class TestEnum
{
    FIRST,
    SECOND,
    THIRD
};

class EnumService : public EnumMultiton<EnumService, TestEnum>
{
public:
    void set_value(int val) { value_ = val; }
    int get_value() const { return value_; }

private:
    int value_ = 0;
};

// ============================================================================
// TESTS
// ============================================================================

void test_same_key_returns_same_instance()
{
    TEST_CASE("Same key returns same instance");
    TestService::clear_all();
    TestService::reset_count();

    auto service1 = TestService::get("service_a");
    auto service2 = TestService::get("service_a");

    ASSERT_TRUE(service1 == service2, "Same key should return same instance");
    ASSERT_TRUE(service1.get() == service2.get(), "Pointers should be identical");
}

void test_different_keys_return_different_instances()
{
    TEST_CASE("Different keys return different instances");
    TestService::clear_all();
    TestService::reset_count();

    auto service_a = TestService::get("service_a");
    auto service_b = TestService::get("service_b");
    auto service_c = TestService::get("service_c");

    ASSERT_TRUE(service_a != service_b, "Different keys should create different instances");
    ASSERT_TRUE(service_b != service_c, "Different keys should create different instances");
    ASSERT_TRUE(service_a != service_c, "Different keys should create different instances");
}

void test_instance_count_matches_unique_keys()
{
    TEST_CASE("Instance count matches number of unique keys");
    TestService::clear_all();
    TestService::reset_count();

    TestService::get("key1");
    TestService::get("key2");
    TestService::get("key3");

    ASSERT_EQ(TestService::get_instance_count(), 3, "Should have exactly 3 instances for 3 keys");
}

void test_instance_names_preserved()
{
    TEST_CASE("Instance names are preserved");
    TestService::clear_all();
    TestService::reset_count();

    auto service_a = TestService::get("service_a");
    auto service_b = TestService::get("service_b");

    ASSERT_TRUE(service_a->get_name() == "service_a", "Service A name preserved");
    ASSERT_TRUE(service_b->get_name() == "service_b", "Service B name preserved");
}

void test_repeated_requests_do_not_increase_count()
{
    TEST_CASE("Repeated requests do not increase instance count");
    TestService::clear_all();
    TestService::reset_count();

    TestService::get("service_x");
    TestService::get("service_x");
    TestService::get("service_x");

    ASSERT_EQ(TestService::get_instance_count(), 1, "Requesting same key 3 times should create only 1 instance");
}

void test_instance_numbers_sequential()
{
    TEST_CASE("Instance numbers are assigned sequentially");
    TestService::clear_all();
    TestService::reset_count();

    auto service1 = TestService::get("first");
    auto service2 = TestService::get("second");
    auto service3 = TestService::get("third");

    ASSERT_EQ(service1->get_instance_num(), 1, "First instance number is 1");
    ASSERT_EQ(service2->get_instance_num(), 2, "Second instance number is 2");
    ASSERT_EQ(service3->get_instance_num(), 3, "Third instance number is 3");
}

void test_enum_keys_work_correctly()
{
    TEST_CASE("Enum keys work correctly");

    auto service1 = EnumService::get(TestEnum::FIRST);
    auto service2 = EnumService::get(TestEnum::SECOND);
    auto service3 = EnumService::get(TestEnum::THIRD);

    ASSERT_TRUE(service1 != service2, "Different enum values return different instances");
    ASSERT_TRUE(service2 != service3, "Different enum values return different instances");
}

void test_enum_instance_preserves_state()
{
    TEST_CASE("Enum instance preserves state");

    auto service1 = EnumService::get(TestEnum::FIRST);
    service1->set_value(42);

    auto service1_again = EnumService::get(TestEnum::FIRST);
    ASSERT_EQ(service1_again->get_value(), 42, "State should be preserved across multiple gets");
}

void test_enum_values_return_different_instances()
{
    TEST_CASE("Different enum values return different instances");

    auto first = EnumService::get(TestEnum::FIRST);
    auto second = EnumService::get(TestEnum::SECOND);
    auto third = EnumService::get(TestEnum::THIRD);

    first->set_value(1);
    second->set_value(2);
    third->set_value(3);

    ASSERT_EQ(first->get_value(), 1, "First value is 1");
    ASSERT_EQ(second->get_value(), 2, "Second value is 2");
    ASSERT_EQ(third->get_value(), 3, "Third value is 3");
}

void test_clear_all_removes_instances()
{
    TEST_CASE("Clear all removes all instances");
    TestService::clear_all();
    TestService::reset_count();

    TestService::get("a");
    TestService::get("b");

    const auto &all_before = TestService::get_all_instances();
    ASSERT_EQ(all_before.size(), 2, "Should have 2 instances before clear");

    TestService::clear_all();

    const auto &all_after = TestService::get_all_instances();
    ASSERT_EQ(all_after.size(), 0, "After clear_all, map should be empty");
}

void test_get_all_instances_returns_correct_collection()
{
    TEST_CASE("Get all instances returns correct collection");
    TestService::clear_all();
    TestService::reset_count();

    TestService::get("first");
    TestService::get("second");
    TestService::get("third");

    const auto &all_instances = TestService::get_all_instances();
    ASSERT_EQ(all_instances.size(), 3, "Should return all 3 instances");
}

void test_instances_remain_valid_after_additional_creates()
{
    TEST_CASE("Instances remain valid after additional creates");
    TestService::clear_all();
    TestService::reset_count();

    auto first = TestService::get("first");
    auto first_ptr = first.get();

    TestService::get("second");
    TestService::get("third");

    auto first_again = TestService::get("first");
    ASSERT_TRUE(first_again.get() == first_ptr, "Original instance should remain unchanged");
}

void test_multiple_clients_access_same_instance()
{
    TEST_CASE("Multiple clients can access same instance");
    TestService::clear_all();
    TestService::reset_count();

    auto client1_service = TestService::get("database");
    auto client2_service = TestService::get("database");
    auto client3_service = TestService::get("database");

    ASSERT_TRUE(client1_service == client2_service, "Client 1 and 2 get same instance");
    ASSERT_TRUE(client2_service == client3_service, "Client 2 and 3 get same instance");
    ASSERT_EQ(TestService::get_instance_count(), 1, "Should have only 1 instance");
}

void test_shared_ownership_works_correctly()
{
    TEST_CASE("Shared ownership works correctly");
    TestService::clear_all();
    TestService::reset_count();

    {
        auto service = TestService::get("shared");
        ASSERT_EQ(TestService::get_instance_count(), 1, "Instance created in scope");
    } // service goes out of scope

    // Instance should still exist because Multiton holds reference
    auto service_again = TestService::get("shared");
    ASSERT_EQ(TestService::get_instance_count(), 1, "Instance should persist even after local scope exit");
}

void test_multi_tenant_scenario()
{
    TEST_CASE("Real-world multi-tenant scenario");
    TestService::clear_all();
    TestService::reset_count();

    auto tenant_a_db = TestService::get("tenant_a_db");
    auto tenant_b_db = TestService::get("tenant_b_db");
    auto tenant_a_cache = TestService::get("tenant_a_cache");
    auto tenant_b_cache = TestService::get("tenant_b_cache");

    ASSERT_EQ(TestService::get_instance_count(), 4, "Should have 4 instances for 4 resources");

    // Verify each tenant's resources are independent
    auto tenant_a_db_again = TestService::get("tenant_a_db");
    auto tenant_b_db_again = TestService::get("tenant_b_db");

    ASSERT_TRUE(tenant_a_db == tenant_a_db_again, "Tenant A DB instance reused");
    ASSERT_TRUE(tenant_b_db == tenant_b_db_again, "Tenant B DB instance reused");
    ASSERT_TRUE(tenant_a_db != tenant_b_db, "Different tenants have different instances");
}

void test_many_keys()
{
    TEST_CASE("Many keys can be created (stress test)");
    TestService::clear_all();
    TestService::reset_count();

    for (int i = 0; i < 100; ++i)
    {
        TestService::get("key_" + to_string(i));
    }

    ASSERT_EQ(TestService::get_instance_count(), 100, "Should create 100 instances");
}

void test_rapid_repeated_access()
{
    TEST_CASE("Rapid repeated access (1000 accesses)");
    TestService::clear_all();
    TestService::reset_count();

    auto service = TestService::get("repeated");

    for (int i = 0; i < 1000; ++i)
    {
        auto same_service = TestService::get("repeated");
        if (service != same_service)
        {
            cout << "  ✗ FAILED: access #" << i << " returned different instance\n";
            test_results.failed++;
            return;
        }
    }

    cout << "  ✓ All 1000 accesses returned same instance\n";
    test_results.passed++;
    ASSERT_EQ(TestService::get_instance_count(), 1, "Should have only 1 instance");
}

void test_empty_string_key()
{
    TEST_CASE("Empty string key");
    TestService::clear_all();
    TestService::reset_count();

    auto service = TestService::get("");
    auto service_again = TestService::get("");

    ASSERT_TRUE(service == service_again, "Empty string key should work");
    ASSERT_EQ(TestService::get_instance_count(), 1, "Should have 1 instance for empty string key");
}

void test_keys_with_special_characters()
{
    TEST_CASE("Keys with special characters");
    TestService::clear_all();
    TestService::reset_count();

    auto s1 = TestService::get("key:with:colons");
    auto s2 = TestService::get("key@with@symbols");
    auto s3 = TestService::get("key with spaces");

    ASSERT_TRUE(s1 != s2, "Keys with different special chars are different");
    ASSERT_TRUE(s2 != s3, "Keys with different special chars are different");
    ASSERT_EQ(TestService::get_instance_count(), 3, "Should have 3 instances");
}

void test_case_sensitivity()
{
    TEST_CASE("Keys are case sensitive");
    TestService::clear_all();
    TestService::reset_count();

    auto service_upper = TestService::get("SERVICE");
    auto service_lower = TestService::get("service");
    auto service_mixed = TestService::get("Service");

    ASSERT_TRUE(service_upper != service_lower, "Upper and lower case are different");
    ASSERT_TRUE(service_lower != service_mixed, "Different cases are different");
    ASSERT_EQ(TestService::get_instance_count(), 3, "Should have 3 different instances");
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║          MULTITON PATTERN - MANUAL TEST SUITE             ║\n";
    cout << "║  20 Comprehensive Tests                                   ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";

    test_same_key_returns_same_instance();
    test_different_keys_return_different_instances();
    test_instance_count_matches_unique_keys();
    test_instance_names_preserved();
    test_repeated_requests_do_not_increase_count();
    test_instance_numbers_sequential();
    test_enum_keys_work_correctly();
    test_enum_instance_preserves_state();
    test_enum_values_return_different_instances();
    test_clear_all_removes_instances();
    test_get_all_instances_returns_correct_collection();
    test_instances_remain_valid_after_additional_creates();
    test_multiple_clients_access_same_instance();
    test_shared_ownership_works_correctly();
    test_multi_tenant_scenario();
    test_many_keys();
    test_rapid_repeated_access();
    test_empty_string_key();
    test_keys_with_special_characters();
    test_case_sensitivity();

    test_results.print_summary();
    return test_results.failed == 0 ? 0 : 1;
}
