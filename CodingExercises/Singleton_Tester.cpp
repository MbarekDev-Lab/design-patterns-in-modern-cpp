#include <iostream>
#include <functional>
#include <cassert>
#include <memory>
#include <string>

using namespace std;

// ============================================================================
// SINGLETON TESTER - TEST SUITE
// ============================================================================

struct SingletonTester
{
    template <typename T>
    bool is_singleton(function<T *()> factory)
    {
        T *instance1 = factory();
        T *instance2 = factory();
        return instance1 == instance2;
    }
};

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

TestResult results;

#define TEST_CASE(name)                    \
    cout << "\n[TEST] " << (name) << "\n"; \
    results.total++

#define ASSERT_TRUE(condition, message)              \
    if (condition)                                   \
    {                                                \
        cout << "  ✓ " << (message) << "\n";         \
        results.passed++;                            \
    }                                                \
    else                                             \
    {                                                \
        cout << "  ✗ FAILED: " << (message) << "\n"; \
        results.failed++;                            \
    }

#define ASSERT_EQ(actual, expected, message)         \
    if ((actual) == (expected))                      \
    {                                                \
        cout << "  ✓ " << (message) << "\n";         \
        results.passed++;                            \
    }                                                \
    else                                             \
    {                                                \
        cout << "  ✗ FAILED: " << (message) << "\n"; \
        results.failed++;                            \
    }

// ============================================================================
// TEST CLASSES
// ============================================================================

// Class 1: Static Singleton
class StaticSingleton
{
public:
    static StaticSingleton *get_instance()
    {
        static StaticSingleton instance;
        return &instance;
    }

    int get_id() const { return id_; }
    StaticSingleton() : id_(++counter_) {}

private:
    static int counter_;
    int id_;
};
int StaticSingleton::counter_ = 0;

// Class 2: Lazy Initialized Singleton
class LazyInitSingleton
{
public:
    static LazyInitSingleton *get_instance()
    {
        if (!instance_)
        {
            instance_ = new LazyInitSingleton();
        }
        return instance_;
    }

    int get_id() const { return id_; }
    LazyInitSingleton() : id_(++counter_) {}

    static void reset()
    {
        delete instance_;
        instance_ = nullptr;
    }

private:
    static int counter_;
    static LazyInitSingleton *instance_;
    int id_;
};
int LazyInitSingleton::counter_ = 0;
LazyInitSingleton *LazyInitSingleton::instance_ = nullptr;

// Class 3: Shared Ptr Singleton
class SharedPtrSingletonClass
{
public:
    static shared_ptr<SharedPtrSingletonClass> get_instance()
    {
        if (!instance_)
        {
            instance_ = make_shared<SharedPtrSingletonClass>();
        }
        return instance_;
    }

    int get_id() const { return id_; }
    SharedPtrSingletonClass() : id_(++counter_) {}

private:
    static int counter_;
    static shared_ptr<SharedPtrSingletonClass> instance_;
    int id_;
};
int SharedPtrSingletonClass::counter_ = 0;
shared_ptr<SharedPtrSingletonClass> SharedPtrSingletonClass::instance_ = nullptr;

// Class 4: FALSE Singleton - Always creates new
class FalseSingletonClass
{
public:
    static FalseSingletonClass *create()
    {
        return new FalseSingletonClass();
    }

    int get_id() const { return id_; }
    FalseSingletonClass() : id_(++counter_) {}

private:
    static int counter_;
    int id_;
};
int FalseSingletonClass::counter_ = 0;

// Class 5: Prototype - Always creates new with clone
class PrototypeClass
{
public:
    static PrototypeClass *create_copy()
    {
        if (!prototype_)
            prototype_ = new PrototypeClass();
        return new PrototypeClass(*prototype_); // Return a copy
    }

    int get_id() const { return id_; }
    PrototypeClass() : id_(++counter_) {}

private:
    static int counter_;
    static PrototypeClass *prototype_;
    int id_;
};
int PrototypeClass::counter_ = 0;
PrototypeClass *PrototypeClass::prototype_ = nullptr;

// Class 6: Double-checked Locking Singleton
class ThreadSafeSingleton
{
public:
    static ThreadSafeSingleton *get_instance()
    {
        if (!instance_) // First check (unlocked)
        {
            instance_ = new ThreadSafeSingleton();
        }
        return instance_;
    }

    int get_id() const { return id_; }
    ThreadSafeSingleton() : id_(++counter_) {}

    static void reset()
    {
        delete instance_;
        instance_ = nullptr;
    }

private:
    static int counter_;
    static ThreadSafeSingleton *instance_;
    int id_;
};
int ThreadSafeSingleton::counter_ = 0;
ThreadSafeSingleton *ThreadSafeSingleton::instance_ = nullptr;

// ============================================================================
// TESTS
// ============================================================================

void test_static_singleton()
{
    TEST_CASE("Static Singleton - Meyer's pattern");
    SingletonTester tester;

    function<StaticSingleton *()> factory = []()
    { return StaticSingleton::get_instance(); };

    bool result = tester.is_singleton(factory);
    ASSERT_TRUE(result, "Static singleton should be recognized");
}

void test_lazy_init_singleton()
{
    TEST_CASE("Lazy Initialized Singleton");
    SingletonTester tester;
    LazyInitSingleton::reset();

    function<LazyInitSingleton *()> factory = []()
    { return LazyInitSingleton::get_instance(); };

    bool result = tester.is_singleton(factory);
    ASSERT_TRUE(result, "Lazy init singleton should be recognized");

    LazyInitSingleton::reset();
}

void test_shared_ptr_singleton()
{
    TEST_CASE("Shared Ptr Singleton");
    SingletonTester tester;

    // Use .get() to convert shared_ptr to raw pointer
    function<SharedPtrSingletonClass *()> factory = []()
    { return SharedPtrSingletonClass::get_instance().get(); };

    bool result = tester.is_singleton(factory);
    ASSERT_TRUE(result, "Shared ptr singleton should be recognized");
}

void test_false_singleton()
{
    TEST_CASE("False Singleton - Always creates new");
    SingletonTester tester;

    function<FalseSingletonClass *()> factory = []()
    { return FalseSingletonClass::create(); };

    bool result = tester.is_singleton(factory);
    ASSERT_TRUE(!result, "False singleton should NOT be recognized");
}

void test_prototype_pattern()
{
    TEST_CASE("Prototype Pattern - Always creates copies");
    SingletonTester tester;

    function<PrototypeClass *()> factory = []()
    { return PrototypeClass::create_copy(); };

    bool result = tester.is_singleton(factory);
    ASSERT_TRUE(!result, "Prototype pattern should NOT be recognized as singleton");
}

void test_thread_safe_singleton()
{
    TEST_CASE("Thread Safe Singleton - Double checked locking");
    SingletonTester tester;
    ThreadSafeSingleton::reset();

    function<ThreadSafeSingleton *()> factory = []()
    { return ThreadSafeSingleton::get_instance(); };

    bool result = tester.is_singleton(factory);
    ASSERT_TRUE(result, "Thread safe singleton should be recognized");

    ThreadSafeSingleton::reset();
}

void test_pointer_addresses()
{
    TEST_CASE("Verify pointer addresses differ for false singletons");
    FalseSingletonClass *f1 = FalseSingletonClass::create();
    FalseSingletonClass *f2 = FalseSingletonClass::create();

    ASSERT_TRUE(f1 != f2, "False singleton addresses should be different");
    ASSERT_TRUE(f1->get_id() != f2->get_id(), "Instances should have different IDs");

    delete f1;
    delete f2;
}

void test_pointer_addresses_true_singleton()
{
    TEST_CASE("Verify pointer addresses same for true singletons");
    StaticSingleton *s1 = StaticSingleton::get_instance();
    StaticSingleton *s2 = StaticSingleton::get_instance();

    ASSERT_TRUE(s1 == s2, "True singleton addresses should be identical");
    ASSERT_TRUE(s1->get_id() == s2->get_id(), "Instances should have same ID");
}

void test_multiple_calls()
{
    TEST_CASE("Multiple calls to singleton factory");
    LazyInitSingleton::reset();
    SingletonTester tester;

    function<LazyInitSingleton *()> factory = []()
    { return LazyInitSingleton::get_instance(); };

    LazyInitSingleton *p1 = factory();
    LazyInitSingleton *p2 = factory();
    LazyInitSingleton *p3 = factory();
    LazyInitSingleton *p4 = factory();

    ASSERT_TRUE(p1 == p2, "First and second calls same");
    ASSERT_TRUE(p2 == p3, "Second and third calls same");
    ASSERT_TRUE(p3 == p4, "Third and fourth calls same");

    LazyInitSingleton::reset();
}

void test_mixed_singleton_types()
{
    TEST_CASE("Compare different singleton implementations");
    LazyInitSingleton::reset();
    ThreadSafeSingleton::reset();

    SingletonTester tester;

    function<LazyInitSingleton *()> lazy_factory = []()
    { return LazyInitSingleton::get_instance(); };
    function<ThreadSafeSingleton *()> thread_safe_factory = []()
    { return ThreadSafeSingleton::get_instance(); };

    bool lazy_result = tester.is_singleton(lazy_factory);
    bool thread_safe_result = tester.is_singleton(thread_safe_factory);

    ASSERT_TRUE(lazy_result, "Lazy init is singleton");
    ASSERT_TRUE(thread_safe_result, "Thread safe is singleton");

    LazyInitSingleton::reset();
    ThreadSafeSingleton::reset();
}

void test_lambda_factory()
{
    TEST_CASE("Test with lambda factory");
    LazyInitSingleton::reset();
    SingletonTester tester;

    int call_count = 0;

    // Lambda that tracks calls
    function<LazyInitSingleton *()> factory = [&call_count]()
    {
        call_count++;
        return LazyInitSingleton::get_instance();
    };

    bool result = tester.is_singleton(factory);

    ASSERT_TRUE(result, "Should detect singleton with lambda");
    ASSERT_EQ(call_count, 2, "Factory should be called exactly twice");

    LazyInitSingleton::reset();
}

void test_false_positive_detection()
{
    TEST_CASE("Ensure false positives are not detected");
    SingletonTester tester;

    // This should NOT appear to be a singleton
    function<PrototypeClass *()> factory = []()
    { return PrototypeClass::create_copy(); };

    // Get two instances and verify they have different addresses
    PrototypeClass *p1 = factory();
    PrototypeClass *p2 = factory();

    ASSERT_TRUE(p1 != p2, "Different instances created");
    ASSERT_TRUE(!tester.is_singleton(factory), "Prototype not recognized as singleton");

    delete p1;
    delete p2;
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║   SINGLETON TESTER - COMPREHENSIVE TEST SUITE              ║\n";
    cout << "║   Tests for detecting singleton instances                  ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";

    test_static_singleton();
    test_lazy_init_singleton();
    test_shared_ptr_singleton();
    test_false_singleton();
    test_prototype_pattern();
    test_thread_safe_singleton();
    test_pointer_addresses();
    test_pointer_addresses_true_singleton();
    test_multiple_calls();
    test_mixed_singleton_types();
    test_lambda_factory();
    test_false_positive_detection();

    results.print_summary();

    cout << "═══════════════════════════════════════════════════════════════\n";
    cout << "KEY INSIGHTS FROM SINGLETON TESTER\n";
    cout << "═══════════════════════════════════════════════════════════════\n\n";

    cout << "1. HOW IT WORKS:\n";
    cout << "   - Calls factory twice\n";
    cout << "   - Compares pointer values\n";
    cout << "   - Same pointer = singleton\n";
    cout << "   - Different pointers = not singleton\n\n";

    cout << "2. WHAT IT DETECTS:\n";
    cout << "   ✓ Static local variables (Meyer's singleton)\n";
    cout << "   ✓ Lazy initialization with null check\n";
    cout << "   ✓ Shared pointer singletons (use .get())\n";
    cout << "   ✗ Factories that use 'new' each time\n";
    cout << "   ✗ Prototype pattern (creates copies)\n\n";

    cout << "3. WHY THIS WORKS:\n";
    cout << "   - Memory addresses are unique per object\n";
    cout << "   - Pointer comparison is reliable\n";
    cout << "   - No need to know implementation details\n";
    cout << "   - Works with any singleton pattern variant\n\n";

    cout << "4. PRACTICAL IMPLICATIONS:\n";
    cout << "   - Easy to verify singleton behavior at runtime\n";
    cout << "   - Can validate singleton implementations\n";
    cout << "   - Useful for testing frameworks\n";
    cout << "   - Helps catch incorrect singleton implementations\n\n";

    cout << "═══════════════════════════════════════════════════════════════\n\n";

    return results.failed == 0 ? 0 : 1;
}
