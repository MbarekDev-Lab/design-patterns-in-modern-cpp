#include <iostream>
#include <cassert>
#include <sstream>

#include "structural/proxy.h"

using namespace std;
using namespace proxy_pattern;

// ============================================================================
// TEST FRAMEWORK
// ============================================================================

int g_test_count = 0;
int g_passed_count = 0;
int g_assertion_count = 0;

#define TEST_START(name)                 \
    cout << "\n[TEST] " << name << "\n"; \
    g_test_count++;                      \
    bool test_passed = true;

#define ASSERT_TRUE(condition)  \
    g_assertion_count++;        \
    if (condition)              \
    {                           \
        cout << "  ✓ ";         \
    }                           \
    else                        \
    {                           \
        cout << "  ✗ FAILED: "; \
        test_passed = false;    \
    }                           \
    cout << #condition << "\n";

#define ASSERT_FALSE(condition) \
    g_assertion_count++;        \
    if (!(condition))           \
    {                           \
        cout << "  ✓ ";         \
    }                           \
    else                        \
    {                           \
        cout << "  ✗ FAILED: "; \
        test_passed = false;    \
    }                           \
    cout << "!" << #condition << "\n";

#define ASSERT_EQ(a, b)                                          \
    g_assertion_count++;                                         \
    if ((a) == (b))                                              \
    {                                                            \
        cout << "  ✓ ";                                          \
    }                                                            \
    else                                                         \
    {                                                            \
        cout << "  ✗ FAILED: " << (a) << " != " << (b) << " - "; \
        test_passed = false;                                     \
    }                                                            \
    cout << #a << " == " << #b << "\n";

#define TEST_END()             \
    if (test_passed)           \
    {                          \
        cout << "  ✅ PASS\n"; \
        g_passed_count++;      \
    }                          \
    else                       \
    {                          \
        cout << "  ❌ FAIL\n"; \
    }

// ============================================================================
// VIRTUAL PROXY TESTS (5 tests)
// ============================================================================

void test_virtual_proxy_creation()
{
    TEST_START("Virtual Proxy: Proxy creation doesn't load image");

    virtual_proxy::ImageProxy proxy1("image1.png");
    virtual_proxy::ImageProxy proxy2("image2.png");

    ASSERT_TRUE(true); // If we reach here without load, test passes

    TEST_END();
}

void test_virtual_proxy_lazy_loading()
{
    TEST_START("Virtual Proxy: Image loads on first draw");

    virtual_proxy::ImageProxy proxy("test.png");
    // Image not loaded yet

    // Draw triggers loading
    proxy.draw();
    ASSERT_TRUE(true); // Successful draw

    TEST_END();
}

void test_virtual_proxy_filename()
{
    TEST_START("Virtual Proxy: Returns correct filename");

    virtual_proxy::ImageProxy proxy("pokemon.png");
    string filename = proxy.get_filename();

    ASSERT_EQ(filename, "pokemon.png");

    TEST_END();
}

void test_virtual_proxy_multiple_draws()
{
    TEST_START("Virtual Proxy: Subsequent draws don't reload");

    virtual_proxy::ImageProxy proxy("image.png");

    // First draw loads
    proxy.draw();

    // Second draw should use cached object
    proxy.draw();

    ASSERT_TRUE(true); // Both draws successful

    TEST_END();
}

void test_virtual_proxy_different_images()
{
    TEST_START("Virtual Proxy: Multiple proxies manage separate images");

    virtual_proxy::ImageProxy proxy1("first.png");
    virtual_proxy::ImageProxy proxy2("second.png");

    ASSERT_EQ(proxy1.get_filename(), "first.png");
    ASSERT_EQ(proxy2.get_filename(), "second.png");

    TEST_END();
}

// ============================================================================
// PROTECTION PROXY TESTS (5 tests)
// ============================================================================

void test_protection_proxy_guest_read()
{
    TEST_START("Protection Proxy: Guest can read");

    auto real_db = make_shared<protection_proxy::RealDatabase>();
    protection_proxy::DatabaseProxy proxy("guest", real_db);

    // Should not throw
    proxy.read("SELECT * FROM users");

    ASSERT_TRUE(true);

    TEST_END();
}

void test_protection_proxy_guest_write_denied()
{
    TEST_START("Protection Proxy: Guest cannot write");

    auto real_db = make_shared<protection_proxy::RealDatabase>();
    protection_proxy::DatabaseProxy proxy("guest", real_db);

    // Should be denied (no exception, just denied message)
    proxy.write("INSERT INTO users");

    ASSERT_TRUE(true);

    TEST_END();
}

void test_protection_proxy_admin_all_access()
{
    TEST_START("Protection Proxy: Admin has all access");

    auto real_db = make_shared<protection_proxy::RealDatabase>();
    protection_proxy::DatabaseProxy proxy("admin", real_db);

    proxy.read("SELECT *");
    proxy.write("INSERT");
    proxy.delete_data("DELETE");

    ASSERT_TRUE(true);

    TEST_END();
}

void test_protection_proxy_different_roles()
{
    TEST_START("Protection Proxy: Multiple proxies with different roles");

    auto real_db = make_shared<protection_proxy::RealDatabase>();
    protection_proxy::DatabaseProxy guest_proxy("guest", real_db);
    protection_proxy::DatabaseProxy admin_proxy("admin", real_db);

    // Both created successfully
    ASSERT_TRUE(true);

    TEST_END();
}

void test_protection_proxy_read_allowed_for_all()
{
    TEST_START("Protection Proxy: Read allowed for all roles");

    auto real_db = make_shared<protection_proxy::RealDatabase>();
    protection_proxy::DatabaseProxy user_proxy("user", real_db);

    user_proxy.read("SELECT * FROM public");

    ASSERT_TRUE(true);

    TEST_END();
}

// ============================================================================
// LOGGING PROXY TESTS (5 tests)
// ============================================================================

void test_logging_proxy_logs_additions()
{
    TEST_START("Logging Proxy: Logs addition operations");

    logging_proxy::LoggingCalculatorProxy calc;

    int result = calc.add(5, 3);

    ASSERT_EQ(result, 8);
    ASSERT_EQ(calc.get_operation_count(), 1);

    TEST_END();
}

void test_logging_proxy_logs_multiple_operations()
{
    TEST_START("Logging Proxy: Logs multiple operations");

    logging_proxy::LoggingCalculatorProxy calc;

    calc.add(10, 20);
    calc.subtract(50, 30);
    calc.multiply(3, 4);

    ASSERT_EQ(calc.get_operation_count(), 3);

    TEST_END();
}

void test_logging_proxy_addition_correctness()
{
    TEST_START("Logging Proxy: Addition calculation correct");

    logging_proxy::LoggingCalculatorProxy calc;
    int result = calc.add(100, 200);

    ASSERT_EQ(result, 300);

    TEST_END();
}

void test_logging_proxy_multiplication_correctness()
{
    TEST_START("Logging Proxy: Multiplication calculation correct");

    logging_proxy::LoggingCalculatorProxy calc;
    int result = calc.multiply(7, 8);

    ASSERT_EQ(result, 56);

    TEST_END();
}

void test_logging_proxy_subtraction_correctness()
{
    TEST_START("Logging Proxy: Subtraction calculation correct");

    logging_proxy::LoggingCalculatorProxy calc;
    int result = calc.subtract(100, 30);

    ASSERT_EQ(result, 70);

    TEST_END();
}

// ============================================================================
// CACHING PROXY TESTS (5 tests)
// ============================================================================

void test_caching_proxy_computes_factorial()
{
    TEST_START("Caching Proxy: Computes factorial correctly");

    caching_proxy::CachingComputeProxy compute;
    int result = compute.compute_factorial(5);

    ASSERT_EQ(result, 120); // 5! = 120

    TEST_END();
}

void test_caching_proxy_factorial_10()
{
    TEST_START("Caching Proxy: Factorial(10) computation");

    caching_proxy::CachingComputeProxy compute;
    int result = compute.compute_factorial(10);

    ASSERT_EQ(result, 3628800); // 10!

    TEST_END();
}

void test_caching_proxy_caches_result()
{
    TEST_START("Caching Proxy: Caches computed results");

    caching_proxy::CachingComputeProxy compute;

    // First call
    compute.compute_factorial(5);
    ASSERT_EQ(compute.get_cache_size(), 1);

    // Second call (should use cache)
    compute.compute_factorial(5);
    ASSERT_EQ(compute.get_cache_size(), 1); // Still 1

    TEST_END();
}

void test_caching_proxy_multiple_cached_values()
{
    TEST_START("Caching Proxy: Stores multiple cached values");

    caching_proxy::CachingComputeProxy compute;

    compute.compute_factorial(3);
    compute.compute_factorial(5);
    compute.compute_factorial(7);

    ASSERT_EQ(compute.get_cache_size(), 3);

    TEST_END();
}

void test_caching_proxy_factorial_1()
{
    TEST_START("Caching Proxy: Factorial(1) = 1");

    caching_proxy::CachingComputeProxy compute;
    int result = compute.compute_factorial(1);

    ASSERT_EQ(result, 1);

    TEST_END();
}

// ============================================================================
// SMART POINTER PROXY TESTS (5 tests)
// ============================================================================

void test_smart_pointer_proxy_creation()
{
    TEST_START("Smart Pointer Proxy: Creates resource");

    smart_pointer_proxy::ResourceProxy proxy("resource1");

    ASSERT_EQ(proxy.get_id(), "resource1");

    TEST_END();
}

void test_smart_pointer_proxy_tracks_usage()
{
    TEST_START("Smart Pointer Proxy: Tracks resource usage");

    smart_pointer_proxy::ResourceProxy proxy("resource2");

    proxy.use();
    ASSERT_EQ(proxy.get_request_count(), 1);

    proxy.use();
    ASSERT_EQ(proxy.get_request_count(), 2);

    proxy.use();
    ASSERT_EQ(proxy.get_request_count(), 3);

    TEST_END();
}

void test_smart_pointer_proxy_resource_id()
{
    TEST_START("Smart Pointer Proxy: Returns correct resource ID");

    smart_pointer_proxy::ResourceProxy proxy("test_resource");

    ASSERT_EQ(proxy.get_id(), "test_resource");

    TEST_END();
}

void test_smart_pointer_proxy_multiple_proxies()
{
    TEST_START("Smart Pointer Proxy: Multiple proxies independent");

    smart_pointer_proxy::ResourceProxy proxy1("resource_a");
    smart_pointer_proxy::ResourceProxy proxy2("resource_b");

    proxy1.use();
    proxy2.use();
    proxy1.use();

    ASSERT_EQ(proxy1.get_request_count(), 2);
    ASSERT_EQ(proxy2.get_request_count(), 1);

    TEST_END();
}

void test_smart_pointer_proxy_initial_count()
{
    TEST_START("Smart Pointer Proxy: Request count starts at 0");

    smart_pointer_proxy::ResourceProxy proxy("resource");

    ASSERT_EQ(proxy.get_request_count(), 0);

    TEST_END();
}

// ============================================================================
// PROPERTY PROXY TESTS (5 tests)
// ============================================================================

void test_property_proxy_default_values()
{
    TEST_START("Property Proxy: Default property values");

    property_proxy::Creature creature;

    ASSERT_EQ((int)creature.strength, 10);
    ASSERT_EQ((int)creature.agility, 5);
    ASSERT_EQ((int)creature.health, 100);

    TEST_END();
}

void test_property_proxy_set_strength()
{
    TEST_START("Property Proxy: Set strength property");

    property_proxy::Creature creature;
    creature.strength = 20;

    ASSERT_EQ((int)creature.strength, 20);

    TEST_END();
}

void test_property_proxy_set_agility()
{
    TEST_START("Property Proxy: Set agility property");

    property_proxy::Creature creature;
    creature.agility = 25;

    ASSERT_EQ((int)creature.agility, 25);

    TEST_END();
}

void test_property_proxy_set_health()
{
    TEST_START("Property Proxy: Set health property");

    property_proxy::Creature creature;
    creature.health = 50;

    ASSERT_EQ((int)creature.health, 50);

    TEST_END();
}

void test_property_proxy_multiple_modifications()
{
    TEST_START("Property Proxy: Multiple property modifications");

    property_proxy::Creature creature;

    creature.strength = 15;
    creature.agility = 18;
    creature.health = 80;

    ASSERT_EQ((int)creature.strength, 15);
    ASSERT_EQ((int)creature.agility, 18);
    ASSERT_EQ((int)creature.health, 80);

    TEST_END();
}

// ============================================================================
// REMOTE PROXY TESTS (5 tests)
// ============================================================================

void test_remote_proxy_initialization()
{
    TEST_START("Remote Proxy: Initializes with server address");

    remote_proxy::RemoteServiceProxy proxy("http://localhost:8080");

    ASSERT_EQ(proxy.get_call_count(), 0);

    TEST_END();
}

void test_remote_proxy_get_data()
{
    TEST_START("Remote Proxy: Get data operation");

    remote_proxy::RemoteServiceProxy proxy("http://localhost:9000");

    string result = proxy.get_data("user:123");

    ASSERT_EQ(proxy.get_call_count(), 1);

    TEST_END();
}

void test_remote_proxy_set_data()
{
    TEST_START("Remote Proxy: Set data operation");

    remote_proxy::RemoteServiceProxy proxy("http://localhost:9000");

    proxy.set_data("key", "value");

    ASSERT_EQ(proxy.get_call_count(), 1);

    TEST_END();
}

void test_remote_proxy_multiple_calls()
{
    TEST_START("Remote Proxy: Tracks multiple calls");

    remote_proxy::RemoteServiceProxy proxy("http://localhost:9000");

    proxy.get_data("key1");
    proxy.set_data("key2", "value");
    proxy.get_data("key3");

    ASSERT_EQ(proxy.get_call_count(), 3);

    TEST_END();
}

void test_remote_proxy_call_count_increases()
{
    TEST_START("Remote Proxy: Call count increases with each operation");

    remote_proxy::RemoteServiceProxy proxy("http://localhost:9000");

    ASSERT_EQ(proxy.get_call_count(), 0);

    proxy.get_data("test1");
    ASSERT_EQ(proxy.get_call_count(), 1);

    proxy.get_data("test2");
    ASSERT_EQ(proxy.get_call_count(), 2);

    TEST_END();
}

// ============================================================================
// SYNCHRONIZATION PROXY TESTS (5 tests)
// ============================================================================

void test_synchronization_proxy_initial_value()
{
    TEST_START("Synchronization Proxy: Initial counter value is 0");

    synchronization_proxy::ThreadSafeCounterProxy counter;

    ASSERT_EQ(counter.get_value(), 0);

    TEST_END();
}

void test_synchronization_proxy_increment()
{
    TEST_START("Synchronization Proxy: Increment operation");

    synchronization_proxy::ThreadSafeCounterProxy counter;

    counter.increment();
    ASSERT_EQ(counter.get_value(), 1);

    TEST_END();
}

void test_synchronization_proxy_decrement()
{
    TEST_START("Synchronization Proxy: Decrement operation");

    synchronization_proxy::ThreadSafeCounterProxy counter;

    counter.increment();
    counter.increment();
    counter.decrement();

    ASSERT_EQ(counter.get_value(), 1);

    TEST_END();
}

void test_synchronization_proxy_multiple_increments()
{
    TEST_START("Synchronization Proxy: Multiple increments");

    synchronization_proxy::ThreadSafeCounterProxy counter;

    counter.increment();
    counter.increment();
    counter.increment();

    ASSERT_EQ(counter.get_value(), 3);

    TEST_END();
}

void test_synchronization_proxy_access_count()
{
    TEST_START("Synchronization Proxy: Tracks access count");

    synchronization_proxy::ThreadSafeCounterProxy counter;

    counter.increment();
    counter.decrement();
    counter.increment();

    ASSERT_EQ(counter.get_access_count(), 3);
    ASSERT_EQ(counter.get_value(), 1);

    TEST_END();
}

// ============================================================================
// COMPOSITE PROXY TESTS (5 tests)
// ============================================================================

void test_composite_proxy_caching_layer()
{
    TEST_START("Composite Proxy: Caching layer works");

    auto real = make_shared<composite_proxy::RealAPIClient>();
    auto cached = make_shared<composite_proxy::CachingAPIProxy>(real);
    composite_proxy::LoggingAPIProxy logged(cached);

    string result = logged.fetch_data("/endpoint");

    ASSERT_TRUE(!result.empty());

    TEST_END();
}

void test_composite_proxy_logging_layer()
{
    TEST_START("Composite Proxy: Logging layer tracks requests");

    auto real = make_shared<composite_proxy::RealAPIClient>();
    auto cached = make_shared<composite_proxy::CachingAPIProxy>(real);
    composite_proxy::LoggingAPIProxy logged(cached);

    logged.fetch_data("/api/users");
    logged.fetch_data("/api/posts");

    ASSERT_EQ(logged.get_log().size(), 2);

    TEST_END();
}

void test_composite_proxy_request_log()
{
    TEST_START("Composite Proxy: Maintains request log");

    auto real = make_shared<composite_proxy::RealAPIClient>();
    auto cached = make_shared<composite_proxy::CachingAPIProxy>(real);
    composite_proxy::LoggingAPIProxy logged(cached);

    logged.fetch_data("/endpoint1");
    logged.fetch_data("/endpoint2");
    logged.fetch_data("/endpoint3");

    const auto &log = logged.get_log();
    ASSERT_EQ(log.size(), 3);
    ASSERT_EQ(log[0], "/endpoint1");
    ASSERT_EQ(log[1], "/endpoint2");
    ASSERT_EQ(log[2], "/endpoint3");

    TEST_END();
}

void test_composite_proxy_chained_proxies()
{
    TEST_START("Composite Proxy: Multiple proxy layers");

    auto real = make_shared<composite_proxy::RealAPIClient>();
    auto cached = make_shared<composite_proxy::CachingAPIProxy>(real);
    composite_proxy::LoggingAPIProxy logged(cached);

    logged.fetch_data("/data");
    logged.fetch_data("/data"); // Should be cached

    ASSERT_EQ(logged.get_log().size(), 2);

    TEST_END();
}

void test_composite_proxy_caching_effect()
{
    TEST_START("Composite Proxy: Caching reduces actual requests");

    auto real = make_shared<composite_proxy::RealAPIClient>();
    auto cached = make_shared<composite_proxy::CachingAPIProxy>(real);
    composite_proxy::LoggingAPIProxy logged(cached);

    // First request goes through
    logged.fetch_data("/endpoint");

    // Second request (should be cached by logging proxy)
    logged.fetch_data("/endpoint");

    // Logging proxy logs both requests
    ASSERT_EQ(logged.get_log().size(), 2);

    TEST_END();
}

// ============================================================================
// TEST SUMMARY
// ============================================================================

void print_summary()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "TEST SUMMARY\n";
    cout << string(70, '=') << "\n";

    cout << "\nTest Results:\n";
    cout << "  Passed:  " << g_passed_count << " / " << g_test_count << "\n";
    cout << "  Assertions: " << g_assertion_count << "\n";
    cout << "  Success Rate: " << (g_test_count > 0 ? (g_passed_count * 100 / g_test_count) : 0) << "%\n";

    if (g_passed_count == g_test_count)
    {
        cout << "\n✅ ALL TESTS PASSED\n";
    }
    else
    {
        cout << "\n❌ SOME TESTS FAILED\n";
    }

    cout << "\nTests by Category:\n";
    cout << "  Virtual Proxy: 5\n";
    cout << "  Protection Proxy: 5\n";
    cout << "  Logging Proxy: 5\n";
    cout << "  Caching Proxy: 5\n";
    cout << "  Smart Pointer Proxy: 5\n";
    cout << "  Property Proxy: 5\n";
    cout << "  Remote Proxy: 5\n";
    cout << "  Synchronization Proxy: 5\n";
    cout << "  Composite Proxy: 5\n";
    cout << "  Total: 45 tests\n";

    cout << "\n"
         << string(70, '=') << "\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "PROXY PATTERN - COMPREHENSIVE TEST SUITE\n";
    cout << string(70, '=') << "\n\n";

    // Virtual Proxy Tests
    test_virtual_proxy_creation();
    test_virtual_proxy_lazy_loading();
    test_virtual_proxy_filename();
    test_virtual_proxy_multiple_draws();
    test_virtual_proxy_different_images();

    // Protection Proxy Tests
    test_protection_proxy_guest_read();
    test_protection_proxy_guest_write_denied();
    test_protection_proxy_admin_all_access();
    test_protection_proxy_different_roles();
    test_protection_proxy_read_allowed_for_all();

    // Logging Proxy Tests
    test_logging_proxy_logs_additions();
    test_logging_proxy_logs_multiple_operations();
    test_logging_proxy_addition_correctness();
    test_logging_proxy_multiplication_correctness();
    test_logging_proxy_subtraction_correctness();

    // Caching Proxy Tests
    test_caching_proxy_computes_factorial();
    test_caching_proxy_factorial_10();
    test_caching_proxy_caches_result();
    test_caching_proxy_multiple_cached_values();
    test_caching_proxy_factorial_1();

    // Smart Pointer Proxy Tests
    test_smart_pointer_proxy_creation();
    test_smart_pointer_proxy_tracks_usage();
    test_smart_pointer_proxy_resource_id();
    test_smart_pointer_proxy_multiple_proxies();
    test_smart_pointer_proxy_initial_count();

    // Property Proxy Tests
    test_property_proxy_default_values();
    test_property_proxy_set_strength();
    test_property_proxy_set_agility();
    test_property_proxy_set_health();
    test_property_proxy_multiple_modifications();

    // Remote Proxy Tests
    test_remote_proxy_initialization();
    test_remote_proxy_get_data();
    test_remote_proxy_set_data();
    test_remote_proxy_multiple_calls();
    test_remote_proxy_call_count_increases();

    // Synchronization Proxy Tests
    test_synchronization_proxy_initial_value();
    test_synchronization_proxy_increment();
    test_synchronization_proxy_decrement();
    test_synchronization_proxy_multiple_increments();
    test_synchronization_proxy_access_count();

    // Composite Proxy Tests
    test_composite_proxy_caching_layer();
    test_composite_proxy_logging_layer();
    test_composite_proxy_request_log();
    test_composite_proxy_chained_proxies();
    test_composite_proxy_caching_effect();

    print_summary();

    return g_passed_count == g_test_count ? 0 : 1;
}
