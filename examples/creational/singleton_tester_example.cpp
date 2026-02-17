#include <iostream>
#include <functional>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// ============================================================================
// SINGLETON TESTER
// ============================================================================
// Tests whether a factory function produces singleton instances by comparing
// if multiple calls return the same pointer (same memory address).
// ============================================================================

struct SingletonTester
{
    template <typename T>
    bool is_singleton(function<T *()> factory)
    {
        // Call factory twice and compare pointers
        T *instance1 = factory();
        T *instance2 = factory();

        // If both calls return the same pointer, it's a singleton
        return instance1 == instance2;
    }
};

// ============================================================================
// TEST CASES
// ============================================================================

// Case 1: True Singleton - Classic pattern with static instance
class TrueSingleton
{
public:
    static TrueSingleton *get_instance()
    {
        static TrueSingleton instance;
        return &instance;
    }

    void log(const string &msg) const { cout << "  [TrueSingleton] " << msg << "\n"; }

    TrueSingleton() = default;

private:
};

// Case 2: False Singleton - Creates new instance every time
class FakeSingleton
{
public:
    static FakeSingleton *get_instance()
    {
        return new FakeSingleton(); // ❌ Creates new instance each time!
    }

    void log(const string &msg) const { cout << "  [FakeSingleton] " << msg << "\n"; }

    FakeSingleton() = default;

private:
};

// Case 3: Shared Pointer Singleton
class SharedPtrSingleton
{
public:
    static shared_ptr<SharedPtrSingleton> get_instance()
    {
        if (!instance_)
        {
            instance_ = make_shared<SharedPtrSingleton>();
        }
        return instance_;
    }

    void log(const string &msg) const { cout << "  [SharedPtrSingleton] " << msg << "\n"; }

    // Constructor must be public for make_shared to work
    SharedPtrSingleton() = default;

private:
    static shared_ptr<SharedPtrSingleton> instance_;
};
shared_ptr<SharedPtrSingleton> SharedPtrSingleton::instance_ = nullptr;

// Case 4: Pointer-based Singleton with new
class PointerSingleton
{
public:
    static PointerSingleton *get_instance()
    {
        if (!instance_)
        {
            instance_ = new PointerSingleton();
        }
        return instance_;
    }

    void log(const string &msg) const { cout << "  [PointerSingleton] " << msg << "\n"; }

    static void reset()
    {
        delete instance_;
        instance_ = nullptr;
    }

    PointerSingleton() = default;

private:
    static PointerSingleton *instance_;
};
PointerSingleton *PointerSingleton::instance_ = nullptr;

// Case 5: Meyer's Singleton (local static)
class MeyersSingleton
{
public:
    static MeyersSingleton &get_instance()
    {
        static MeyersSingleton instance;
        return instance;
    }

    void log(const string &msg) const { cout << "  [MeyersSingleton] " << msg << "\n"; }

    MeyersSingleton() = default;

private:
};

// Case 6: Not a Singleton - Creates prototype copies
class NotASingleton
{
public:
    static NotASingleton *create()
    {
        return new NotASingleton(); // Always creates a new instance
    }

    void log(const string &msg) const { cout << "  [NotASingleton] " << msg << "\n"; }

    NotASingleton() = default;

private:
};

// Case 7: Singleton with Reset (for testing purposes)
class ResettableSingleton
{
public:
    static ResettableSingleton *get_instance()
    {
        if (!instance_)
        {
            instance_ = new ResettableSingleton();
        }
        return instance_;
    }

    void log(const string &msg) const { cout << "  [ResettableSingleton] " << msg << "\n"; }

    static void reset()
    {
        delete instance_;
        instance_ = nullptr;
    }

    ResettableSingleton() = default;

private:
    static ResettableSingleton *instance_;
};
ResettableSingleton *ResettableSingleton::instance_ = nullptr;

// Case 8: Template Singleton
template <typename T>
class GenericSingleton
{
public:
    static GenericSingleton<T> *get_instance()
    {
        if (!instance_)
        {
            instance_ = new GenericSingleton<T>();
        }
        return instance_;
    }

    static void reset()
    {
        delete instance_;
        instance_ = nullptr;
    }

    GenericSingleton() = default;

private:
    static GenericSingleton<T> *instance_;
};

template <typename T>
GenericSingleton<T> *GenericSingleton<T>::instance_ = nullptr;

// ============================================================================
// TEST RUNNER
// ============================================================================

int main()
{
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║          SINGLETON TESTER - Validation Exercise           ║\n";
    cout << "║  Tests whether factories produce singleton instances      ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    SingletonTester tester;
    int tests_passed = 0;
    int tests_total = 0;

    // Test 1: True Singleton (Static instance)
    {
        cout << "TEST 1: TrueSingleton (static instance)\n";
        tests_total++;

        function<TrueSingleton *()> factory = []()
        { return TrueSingleton::get_instance(); };

        bool result = tester.is_singleton(factory);
        cout << "  Result: " << (result ? "✓ PASS (is singleton)" : "✗ FAIL (not singleton)") << "\n";
        cout << "  Expected: true (returns same static instance)\n";

        if (result)
            tests_passed++;
        cout << "\n";
    }

    // Test 2: Fake Singleton (creates new each time)
    {
        cout << "TEST 2: FakeSingleton (allocates new each time)\n";
        tests_total++;

        function<FakeSingleton *()> factory = []()
        { return FakeSingleton::get_instance(); };

        bool result = tester.is_singleton(factory);
        cout << "  Result: " << (result ? "✓ PASS (is singleton)" : "✗ FAIL (not singleton)") << "\n";
        cout << "  Expected: false (creates new instance with 'new')\n";

        if (!result)
            tests_passed++;
        cout << "\n";
    }

    // Test 3: Shared Pointer Singleton
    {
        cout << "TEST 3: SharedPtrSingleton (shared_ptr lazy init)\n";
        tests_total++;

        // Need to cast shared_ptr to raw pointer for comparison
        function<SharedPtrSingleton *()> factory = []()
        { return SharedPtrSingleton::get_instance().get(); };

        bool result = tester.is_singleton(factory);
        cout << "  Result: " << (result ? "✓ PASS (is singleton)" : "✗ FAIL (not singleton)") << "\n";
        cout << "  Expected: true (lazy initialization returns same instance)\n";

        if (result)
            tests_passed++;
        cout << "\n";
    }

    // Test 4: Pointer Singleton
    {
        cout << "TEST 4: PointerSingleton (pointer lazy init)\n";
        tests_total++;

        function<PointerSingleton *()> factory = []()
        { return PointerSingleton::get_instance(); };

        bool result = tester.is_singleton(factory);
        cout << "  Result: " << (result ? "✓ PASS (is singleton)" : "✗ FAIL (not singleton)") << "\n";
        cout << "  Expected: true (lazy initialization returns same pointer)\n";

        if (result)
            tests_passed++;

        PointerSingleton::reset();
        cout << "\n";
    }

    // Test 5: Meyer's Singleton (reference version)
    {
        cout << "TEST 5: MeyersSingleton (Meyer's pattern - reference)\n";
        tests_total++;

        function<MeyersSingleton *()> factory = []()
        { return &MeyersSingleton::get_instance(); };

        bool result = tester.is_singleton(factory);
        cout << "  Result: " << (result ? "✓ PASS (is singleton)" : "✗ FAIL (not singleton)") << "\n";
        cout << "  Expected: true (static local variable, same address)\n";

        if (result)
            tests_passed++;
        cout << "\n";
    }

    // Test 6: Not a Singleton (factory creates new)
    {
        cout << "TEST 6: NotASingleton (factory always creates new)\n";
        tests_total++;

        function<NotASingleton *()> factory = []()
        { return NotASingleton::create(); };

        bool result = tester.is_singleton(factory);
        cout << "  Result: " << (result ? "✓ PASS (is singleton)" : "✗ FAIL (not singleton)") << "\n";
        cout << "  Expected: false (creates new instance each time)\n";

        if (!result)
            tests_passed++;
        cout << "\n";
    }

    // Test 7: Resettable Singleton (fresh state)
    {
        cout << "TEST 7: ResettableSingleton (lazy init, resettable)\n";
        tests_total++;

        ResettableSingleton::reset(); // Start fresh

        function<ResettableSingleton *()> factory = []()
        { return ResettableSingleton::get_instance(); };

        bool result = tester.is_singleton(factory);
        cout << "  Result: " << (result ? "✓ PASS (is singleton)" : "✗ FAIL (not singleton)") << "\n";
        cout << "  Expected: true (lazy init ensures same instance)\n";

        if (result)
            tests_passed++;

        ResettableSingleton::reset();
        cout << "\n";
    }

    // Test 8: Generic Singleton
    {
        cout << "TEST 8: GenericSingleton<int> (template-based)\n";
        tests_total++;

        GenericSingleton<int>::reset(); // Start fresh

        function<GenericSingleton<int> *()> factory = []()
        { return GenericSingleton<int>::get_instance(); };

        bool result = tester.is_singleton(factory);
        cout << "  Result: " << (result ? "✓ PASS (is singleton)" : "✗ FAIL (not singleton)") << "\n";
        cout << "  Expected: true (template static instance)\n";

        if (result)
            tests_passed++;

        GenericSingleton<int>::reset();
        cout << "\n";
    }

    // ========================================================================
    // ANALYSIS
    // ========================================================================
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                   TEST RESULTS                             ║\n";
    cout << "├────────────────────────────────────────────────────────────┤\n";
    cout << "│ Total:  " << tests_total << "    Passed: " << tests_passed << "    Failed: " << (tests_total - tests_passed) << "\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    cout << "═══════════════════════════════════════════════════════════════\n";
    cout << "UNDERSTANDING THE SINGLETON TESTER\n";
    cout << "═══════════════════════════════════════════════════════════════\n\n";

    cout << "The key insight: A true singleton returns the SAME POINTER on\n";
    cout << "multiple calls. The tester exploits this by:\n\n";

    cout << "  1. Call factory() twice -> get instance1 and instance2\n";
    cout << "  2. Compare pointers: instance1 == instance2\n";
    cout << "  3. If true, it's a singleton; if false, it's not\n\n";

    cout << "WHY THIS WORKS:\n";
    cout << "  - Singletons manage their own creation and caching\n";
    cout << "  - Multiple calls to get_instance() return the same object\n";
    cout << "  - Different objects have different memory addresses\n";
    cout << "  - Same object always has the same address\n\n";

    cout << "KEY OBSERVATIONS:\n";
    cout << "  ✓ Static variables (Meyer's pattern) work perfectly\n";
    cout << "  ✓ Lazy initialization with null check works\n";
    cout << "  ✓ Both raw pointers and shared_ptr work (compare .get())\n";
    cout << "  ✗ Factories that create with 'new' each time fail the test\n";
    cout << "  ✗ Prototype factories fail (different instances)\n\n";

    cout << "═══════════════════════════════════════════════════════════════\n\n";

    return 0;
}
