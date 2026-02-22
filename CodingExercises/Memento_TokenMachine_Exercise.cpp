#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// ============================================================================
// MEMENTO PATTERN - TOKEN MACHINE
// ============================================================================
// Problem:
// - TokenMachine manages a collection of Token objects
// - Each Token is a reference type (shared_ptr) with a numerical value
// - Need to capture snapshots of the system state
// - CRITICAL: Token values can be modified after snapshot is taken
// - Memento must contain COPIES of tokens, not references
// - This ensures the snapshot is immutable even if original tokens change
//
// Example:
// - Create memento with token value = 5
// - Later, modify the original token to value = 10
// - Memento must still show value = 5 (not affected by later changes)
// ============================================================================

struct Token
{
    int value;

    explicit Token(int value) : value(value) {}

    // Explicit copy constructor for deep copying
    Token(const Token &other) : value(other.value) {}
};

struct Memento
{
    vector<shared_ptr<Token>> tokens;

    // Constructor to deep copy tokens
    explicit Memento(const vector<shared_ptr<Token>> &source_tokens)
    {
        // Create copies of all tokens to preserve snapshot
        for (const auto &token : source_tokens)
        {
            tokens.push_back(make_shared<Token>(*token)); // Deep copy!
        }
    }

    // Default constructor for empty memento
    Memento() = default;

    // Get token at index
    shared_ptr<Token> get_token(size_t index) const
    {
        if (index < tokens.size())
            return tokens[index];
        return nullptr;
    }

    // Get number of tokens in snapshot
    size_t size() const
    {
        return tokens.size();
    }
};

struct TokenMachine
{
    vector<shared_ptr<Token>> tokens;

    // Add token by value (creates new Token internally)
    Memento add_token(int value)
    {
        return add_token(make_shared<Token>(value));
    }

    // Add token by shared_ptr and return memento snapshot
    // IMPORTANT: Must create deep copies of tokens in memento
    // so that future modifications to original tokens don't affect snapshot
    Memento add_token(const shared_ptr<Token> &token)
    {
        tokens.push_back(token);
        return Memento(tokens); // Creates deep copy snapshot
    }

    // Revert the system to a previous state represented by memento
    void revert(const Memento &m)
    {
        tokens.clear();
        // Deep copy tokens from memento
        for (const auto &token : m.tokens)
        {
            tokens.push_back(make_shared<Token>(*token));
        }
    }

    // Get token at index
    shared_ptr<Token> get_token(size_t index) const
    {
        if (index < tokens.size())
            return tokens[index];
        return nullptr;
    }

    // Get number of tokens
    size_t size() const
    {
        return tokens.size();
    }

    // Get total value of all tokens
    int total_value() const
    {
        int sum = 0;
        for (const auto &token : tokens)
        {
            sum += token->value;
        }
        return sum;
    }
};

// ============================================================================
// TEST FRAMEWORK
// ============================================================================

int g_test_count = 0;
int g_passed = 0;
int g_assertion_count = 0;

#define TEST(name)                                                 \
    {                                                              \
        g_test_count++;                                            \
        cout << "\nTest " << g_test_count << ": " << name << endl; \
    }
#define ASSERT_EQ_INT(actual, expected)                                    \
    {                                                                      \
        g_assertion_count++;                                               \
        if ((actual) == (expected))                                        \
        {                                                                  \
            cout << "  ✓ ASSERT_EQ(" << (actual) << ") passed" << endl;    \
        }                                                                  \
        else                                                               \
        {                                                                  \
            cout << "  ✗ ASSERT_EQ(" << (actual) << ") FAILED - expected " \
                 << (expected) << endl;                                    \
            return;                                                        \
        }                                                                  \
    }
#define TEST_END()  \
    {               \
        g_passed++; \
    }

// ============================================================================
// TOKEN MACHINE TESTS
// ============================================================================

void test_add_single_token()
{
    TEST("Add single token by value");

    TokenMachine tm;
    auto m = tm.add_token(5);

    ASSERT_EQ_INT(tm.size(), 1);
    ASSERT_EQ_INT(tm.get_token(0)->value, 5);
    ASSERT_EQ_INT(m.size(), 1);
    ASSERT_EQ_INT(m.get_token(0)->value, 5);
    TEST_END();
}

void test_add_multiple_tokens()
{
    TEST("Add multiple tokens");

    TokenMachine tm;
    tm.add_token(10);
    tm.add_token(20);
    tm.add_token(30);

    ASSERT_EQ_INT(tm.size(), 3);
    ASSERT_EQ_INT(tm.get_token(0)->value, 10);
    ASSERT_EQ_INT(tm.get_token(1)->value, 20);
    ASSERT_EQ_INT(tm.get_token(2)->value, 30);
    TEST_END();
}

void test_token_total_value()
{
    TEST("Total value of all tokens");

    TokenMachine tm;
    tm.add_token(5);
    tm.add_token(10);
    tm.add_token(15);

    ASSERT_EQ_INT(tm.total_value(), 30);
    TEST_END();
}

void test_memento_snapshot()
{
    TEST("Memento captures snapshot at time of creation");

    TokenMachine tm;
    auto m1 = tm.add_token(5);  // Snapshot: [5]
    auto m2 = tm.add_token(10); // Snapshot: [5, 10]

    ASSERT_EQ_INT(m1.size(), 1);
    ASSERT_EQ_INT(m1.get_token(0)->value, 5);

    ASSERT_EQ_INT(m2.size(), 2);
    ASSERT_EQ_INT(m2.get_token(0)->value, 5);
    ASSERT_EQ_INT(m2.get_token(1)->value, 10);
    TEST_END();
}

void test_memento_is_immutable()
{
    TEST("Memento snapshot is not affected by future token changes");

    TokenMachine tm;
    auto token = make_shared<Token>(5);
    auto m = tm.add_token(token); // Snapshot: [5]

    // Now modify the original token
    token->value = 100;

    // Memento should still have original value (deep copy!)
    ASSERT_EQ_INT(m.get_token(0)->value, 5);    // Memento unchanged
    ASSERT_EQ_INT(tm.get_token(0)->value, 100); // Machine updated
    TEST_END();
}

void test_mixed_token_additions()
{
    TEST("Add tokens both by value and by shared_ptr");

    TokenMachine tm;
    tm.add_token(10); // By value

    auto token = make_shared<Token>(20);
    auto m = tm.add_token(token); // By shared_ptr

    ASSERT_EQ_INT(tm.size(), 2);
    ASSERT_EQ_INT(tm.get_token(0)->value, 10);
    ASSERT_EQ_INT(tm.get_token(1)->value, 20);

    // Modify the original shared_ptr token
    token->value = 200;

    // Machine reflects change
    ASSERT_EQ_INT(tm.get_token(1)->value, 200);

    // But memento has original value (deep copy)
    ASSERT_EQ_INT(m.get_token(1)->value, 20);
    TEST_END();
}

void test_revert_to_memento()
{
    TEST("Revert machine to previous state");

    TokenMachine tm;
    auto m1 = tm.add_token(5);  // [5]
    auto m2 = tm.add_token(10); // [5, 10]
    tm.add_token(15);           // [5, 10, 15]

    ASSERT_EQ_INT(tm.size(), 3);

    // Revert to m2
    tm.revert(m2);

    ASSERT_EQ_INT(tm.size(), 2);
    ASSERT_EQ_INT(tm.get_token(0)->value, 5);
    ASSERT_EQ_INT(tm.get_token(1)->value, 10);

    // Revert to m1
    tm.revert(m1);

    ASSERT_EQ_INT(tm.size(), 1);
    ASSERT_EQ_INT(tm.get_token(0)->value, 5);
    TEST_END();
}

void test_revert_creates_independent_copy()
{
    TEST("Reverted tokens are independent of memento");

    TokenMachine tm;
    auto token = make_shared<Token>(5);
    auto m = tm.add_token(token);

    // Modify original token
    token->value = 100;
    ASSERT_EQ_INT(tm.get_token(0)->value, 100);
    ASSERT_EQ_INT(m.get_token(0)->value, 5);

    // Revert to memento
    tm.revert(m);
    ASSERT_EQ_INT(tm.get_token(0)->value, 5);

    // Modify the reverted token in machine
    tm.get_token(0)->value = 50;

    // Memento should still have original value
    ASSERT_EQ_INT(m.get_token(0)->value, 5);
    ASSERT_EQ_INT(tm.get_token(0)->value, 50);
    TEST_END();
}

void test_multiple_reverts()
{
    TEST("Multiple reverts to same memento");

    TokenMachine tm;
    auto m1 = tm.add_token(5);
    auto m2 = tm.add_token(10);
    auto m3 = tm.add_token(15);

    // Revert multiple times to m2
    tm.revert(m2);
    ASSERT_EQ_INT(tm.size(), 2);

    tm.add_token(20); // [5, 10, 20]
    ASSERT_EQ_INT(tm.size(), 3);

    tm.revert(m2); // Back to [5, 10]
    ASSERT_EQ_INT(tm.size(), 2);

    tm.revert(m1); // Back to [5]
    ASSERT_EQ_INT(tm.size(), 1);
    TEST_END();
}

void test_complex_token_modification_scenario()
{
    TEST("Complex: Token added, modified, reverted, modified again");

    TokenMachine tm;
    auto token = make_shared<Token>(10);
    auto m1 = tm.add_token(token); // Snapshot: [10]

    // Modify token to 20
    token->value = 20;
    ASSERT_EQ_INT(tm.get_token(0)->value, 20);
    ASSERT_EQ_INT(m1.get_token(0)->value, 10); // Memento unchanged

    // Add another token
    auto m2 = tm.add_token(30); // Snapshot: [20, 30]
    ASSERT_EQ_INT(m2.get_token(0)->value, 20);
    ASSERT_EQ_INT(m2.get_token(1)->value, 30);

    // Modify first token again
    token->value = 100;
    ASSERT_EQ_INT(tm.get_token(0)->value, 100);
    ASSERT_EQ_INT(m1.get_token(0)->value, 10); // Still original
    ASSERT_EQ_INT(m2.get_token(0)->value, 20); // Memento2 unchanged

    // Revert to m1
    tm.revert(m1);
    ASSERT_EQ_INT(tm.size(), 1);
    ASSERT_EQ_INT(tm.get_token(0)->value, 10);
    TEST_END();
}

void test_empty_machine_revert()
{
    TEST("Revert to empty state");

    TokenMachine tm;
    Memento empty;

    tm.add_token(5);
    tm.add_token(10);
    ASSERT_EQ_INT(tm.size(), 2);

    tm.revert(empty);
    ASSERT_EQ_INT(tm.size(), 0);
    TEST_END();
}

void test_shared_ptr_semantics()
{
    TEST("Verify tokens are properly shared/copied");

    TokenMachine tm;
    auto token1 = make_shared<Token>(10);
    auto token2 = make_shared<Token>(20);

    auto m = tm.add_token(token1);
    tm.add_token(token2);

    // Modify token1
    token1->value = 100;

    // Machine sees change
    ASSERT_EQ_INT(tm.get_token(0)->value, 100);

    // Memento is independent
    ASSERT_EQ_INT(m.get_token(0)->value, 10);

    // Revert creates new copies
    tm.revert(m);
    ASSERT_EQ_INT(tm.size(), 1);

    // Revert creates new Token shared_ptr, not the same object
    tm.get_token(0)->value = 50;
    ASSERT_EQ_INT(m.get_token(0)->value, 10); // Memento unaffected
    TEST_END();
}

void test_memento_addition_sequence()
{
    TEST("Sequence of tokens with memento snapshots");

    TokenMachine tm;
    vector<Memento> m;

    m.push_back(tm.add_token(1)); // [1]
    m.push_back(tm.add_token(2)); // [1, 2]
    m.push_back(tm.add_token(3)); // [1, 2, 3]
    m.push_back(tm.add_token(4)); // [1, 2, 3, 4]
    m.push_back(tm.add_token(5)); // [1, 2, 3, 4, 5]

    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ_INT(m[i].size(), i + 1);
    }

    // Verify each memento has correct values
    ASSERT_EQ_INT(m[0].get_token(0)->value, 1);
    ASSERT_EQ_INT(m[1].get_token(0)->value, 1);
    ASSERT_EQ_INT(m[1].get_token(1)->value, 2);
    ASSERT_EQ_INT(m[4].get_token(4)->value, 5);
    TEST_END();
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main()
{
    cout << "\n======================================================================\n";
    cout << "MEMENTO CODING EXERCISE - TOKEN MACHINE SNAPSHOTS\n";
    cout << "======================================================================\n";

    test_add_single_token();
    test_add_multiple_tokens();
    test_token_total_value();
    test_memento_snapshot();
    test_memento_is_immutable();
    test_mixed_token_additions();
    test_revert_to_memento();
    test_revert_creates_independent_copy();
    test_multiple_reverts();
    test_complex_token_modification_scenario();
    test_empty_machine_revert();
    test_shared_ptr_semantics();
    test_memento_addition_sequence();

    cout << "\n======================================================================\n";
    cout << "TEST SUMMARY\n";
    cout << "======================================================================\n";
    cout << "Tests Run:     " << g_test_count << "\n";
    cout << "Tests Passed:  " << g_passed << "\n";
    cout << "Tests Failed:  " << (g_test_count - g_passed) << "\n";
    cout << "Assertions:    " << g_assertion_count << "\n";
    cout << "Status:        " << (g_passed == g_test_count ? "✅ ALL TESTS PASSED" : "❌ SOME TESTS FAILED")
         << " (" << g_passed << "/" << g_test_count << ")\n";
    cout << "======================================================================\n\n";

    return g_passed == g_test_count ? 0 : 1;
}
