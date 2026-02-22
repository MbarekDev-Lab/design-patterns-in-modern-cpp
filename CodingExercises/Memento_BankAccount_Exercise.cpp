#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// ============================================================================
// MEMENTO PATTERN - BANKING APPLICATION
// ============================================================================
// Problem:
// - Bank account needs to track state changes
// - Users need ability to undo/redo transactions
// - Internal state must be protected (encapsulation)
// - Need to save and restore account states
//
// Solution: Memento Pattern
// - Memento: Captures account balance at a point in time
// - BankAccount: Creates and uses mementos
// - BankAccount2: Adds undo/redo with memento history
// ============================================================================

// MEMENTO - Captures the state of a BankAccount at a point in time
class Memento
{
    int balance;

public:
    explicit Memento(int balance)
        : balance(balance)
    {
    }

    int get_balance() const
    {
        return balance;
    }

    friend class BankAccount;
    friend class BankAccount2;
};

// ============================================================================
// BANKACCOUNT - Basic memento support (save/restore)
// ============================================================================
class BankAccount
{
    int balance = 0;

public:
    explicit BankAccount(const int balance)
        : balance(balance)
    {
    }

    // Deposit and return a memento of the new state
    Memento deposit(int amount)
    {
        balance += amount;
        return Memento{balance};
    }

    // Restore account to a previous state
    void restore(const Memento &m)
    {
        balance = m.balance;
    }

    int get_balance() const
    {
        return balance;
    }

    void set_balance(int b)
    {
        balance = b;
    }

    friend ostream &operator<<(ostream &os, const BankAccount &obj)
    {
        return os << "balance: " << obj.balance;
    }
};

// ============================================================================
// BANKACCOUNT2 - Undo/Redo support with memento history
// ============================================================================
class BankAccount2
{
    int balance = 0;
    vector<shared_ptr<Memento>> changes;
    int current = 0;

public:
    explicit BankAccount2(const int balance)
        : balance(balance)
    {
        changes.emplace_back(make_shared<Memento>(balance));
        current = 0;
    }

    // Deposit and save state to history
    shared_ptr<Memento> deposit(int amount)
    {
        balance += amount;
        auto m = make_shared<Memento>(balance);
        changes.push_back(m);
        ++current;
        return m;
    }

    // Undo one transaction
    shared_ptr<Memento> undo()
    {
        if (current > 0)
        {
            --current;
            auto m = changes[current];
            balance = m->balance;
            return m;
        }
        return {};
    }

    // Redo one transaction
    shared_ptr<Memento> redo()
    {
        if (current + 1 < changes.size())
        {
            ++current;
            auto m = changes[current];
            balance = m->balance;
            return m;
        }
        return {};
    }

    // Restore to a specific memento
    void restore(const shared_ptr<Memento> &m)
    {
        if (m)
        {
            balance = m->balance;
            changes.push_back(m);
            current = changes.size() - 1;
        }
    }

    int get_balance() const
    {
        return balance;
    }

    bool can_undo() const
    {
        return current > 0;
    }

    bool can_redo() const
    {
        return current + 1 < changes.size();
    }

    int get_history_size() const
    {
        return changes.size();
    }

    int get_current_index() const
    {
        return current;
    }

    friend ostream &operator<<(ostream &os, const BankAccount2 &obj)
    {
        return os << "balance: " << obj.balance;
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
#define ASSERT_TRUE(condition)                        \
    {                                                 \
        g_assertion_count++;                          \
        if ((condition))                              \
        {                                             \
            cout << "  ✓ ASSERT_TRUE passed" << endl; \
        }                                             \
        else                                          \
        {                                             \
            cout << "  ✗ ASSERT_TRUE FAILED" << endl; \
            return;                                   \
        }                                             \
    }
#define ASSERT_FALSE(condition)                        \
    {                                                  \
        g_assertion_count++;                           \
        if (!(condition))                              \
        {                                              \
            cout << "  ✓ ASSERT_FALSE passed" << endl; \
        }                                              \
        else                                           \
        {                                              \
            cout << "  ✗ ASSERT_FALSE FAILED" << endl; \
            return;                                    \
        }                                              \
    }
#define TEST_END()  \
    {               \
        g_passed++; \
    }

// ============================================================================
// BANKACCOUNT TESTS (Basic Memento)
// ============================================================================

void test_basic_deposit()
{
    TEST("Basic Deposit: Account starts at 100, deposits 50");

    BankAccount ba{100};
    ASSERT_EQ_INT(ba.get_balance(), 100);

    ba.deposit(50);
    ASSERT_EQ_INT(ba.get_balance(), 150);
    TEST_END();
}

void test_memento_capture()
{
    TEST("Memento: Save balance after deposit");

    BankAccount ba{100};
    auto memento = ba.deposit(50);

    ASSERT_EQ_INT(ba.get_balance(), 150);
    ASSERT_EQ_INT(memento.get_balance(), 150);
    TEST_END();
}

void test_restore_basic()
{
    TEST("Restore: Revert to previous balance");

    BankAccount ba{100};
    auto m1 = ba.deposit(50); // 150
    ASSERT_EQ_INT(ba.get_balance(), 150);

    ba.deposit(25); // 175
    ASSERT_EQ_INT(ba.get_balance(), 175);

    ba.restore(m1); // Restore to 150
    ASSERT_EQ_INT(ba.get_balance(), 150);
    TEST_END();
}

void test_multiple_mementos()
{
    TEST("Multiple Mementos: Save and restore different states");

    BankAccount ba{100};
    auto m1 = ba.deposit(50);  // 150
    auto m2 = ba.deposit(75);  // 225
    auto m3 = ba.deposit(100); // 325

    ASSERT_EQ_INT(ba.get_balance(), 325);

    ba.restore(m2); // Restore to 225
    ASSERT_EQ_INT(ba.get_balance(), 225);

    ba.restore(m1); // Restore to 150
    ASSERT_EQ_INT(ba.get_balance(), 150);

    ba.restore(m3); // Restore to 325
    ASSERT_EQ_INT(ba.get_balance(), 325);
    TEST_END();
}

void test_negative_balance_restoration()
{
    TEST("Negative: Restore account with negative balance");

    BankAccount ba{100};
    auto m1 = ba.deposit(-200); // -100

    ASSERT_EQ_INT(ba.get_balance(), -100);

    ba.deposit(50); // -50
    ASSERT_EQ_INT(ba.get_balance(), -50);

    ba.restore(m1); // Restore to -100
    ASSERT_EQ_INT(ba.get_balance(), -100);
    TEST_END();
}

// ============================================================================
// BANKACCOUNT2 TESTS (Undo/Redo)
// ============================================================================

void test_undoredo_initial_state()
{
    TEST("UndoRedo: Account initializes with memento history");

    BankAccount2 ba{100};

    ASSERT_EQ_INT(ba.get_balance(), 100);
    ASSERT_EQ_INT(ba.get_history_size(), 1);
    ASSERT_EQ_INT(ba.get_current_index(), 0);
    ASSERT_FALSE(ba.can_undo());
    ASSERT_FALSE(ba.can_redo());
    TEST_END();
}

void test_deposit_and_undo()
{
    TEST("UndoRedo: Deposit then undo");

    BankAccount2 ba{100};
    ba.deposit(50); // 150

    ASSERT_EQ_INT(ba.get_balance(), 150);
    ASSERT_TRUE(ba.can_undo());
    ASSERT_FALSE(ba.can_redo());

    ba.undo();
    ASSERT_EQ_INT(ba.get_balance(), 100);
    ASSERT_FALSE(ba.can_undo());
    ASSERT_TRUE(ba.can_redo());
    TEST_END();
}

void test_deposit_undo_redo()
{
    TEST("UndoRedo: Deposit, undo, then redo");

    BankAccount2 ba{100};
    ba.deposit(50); // 150
    ba.deposit(25); // 175
    ASSERT_EQ_INT(ba.get_balance(), 175);

    ba.undo(); // Back to 150
    ASSERT_EQ_INT(ba.get_balance(), 150);

    ba.undo(); // Back to 100
    ASSERT_EQ_INT(ba.get_balance(), 100);

    ba.redo(); // Forward to 150
    ASSERT_EQ_INT(ba.get_balance(), 150);

    ba.redo(); // Forward to 175
    ASSERT_EQ_INT(ba.get_balance(), 175);
    TEST_END();
}

void test_complex_undo_redo()
{
    TEST("UndoRedo: Complex sequence with multiple undos");

    BankAccount2 ba{100};
    ba.deposit(50);  // 150
    ba.deposit(25);  // 175
    ba.deposit(100); // 275

    ASSERT_EQ_INT(ba.get_history_size(), 4); // Initial + 3 deposits

    ba.undo(); // 175
    ASSERT_EQ_INT(ba.get_balance(), 175);

    ba.undo(); // 150
    ASSERT_EQ_INT(ba.get_balance(), 150);

    ba.redo(); // 175
    ASSERT_EQ_INT(ba.get_balance(), 175);

    ba.undo(); // 150
    ASSERT_EQ_INT(ba.get_balance(), 150);

    ba.deposit(200); // 350
    ASSERT_EQ_INT(ba.get_balance(), 350);
    TEST_END();
}

void test_undo_redo_limits()
{
    TEST("UndoRedo: Cannot undo/redo beyond limits");

    BankAccount2 ba{100};
    ba.deposit(50);

    // Undo twice (should only go back once)
    ba.undo();
    ASSERT_FALSE(ba.can_undo());

    auto result = ba.undo(); // Should return empty
    ASSERT_EQ_INT(ba.get_balance(), 100);

    // Redo twice (should only go forward once)
    ba.redo();
    ASSERT_FALSE(ba.can_redo());

    auto result2 = ba.redo(); // Should return empty
    ASSERT_EQ_INT(ba.get_balance(), 150);
    TEST_END();
}

void test_negative_deposits_undo_redo()
{
    TEST("UndoRedo: Negative deposits (withdrawals) with undo/redo");

    BankAccount2 ba{100};
    ba.deposit(-30); // 70
    ba.deposit(-20); // 50
    ASSERT_EQ_INT(ba.get_balance(), 50);

    ba.undo();
    ASSERT_EQ_INT(ba.get_balance(), 70);

    ba.undo();
    ASSERT_EQ_INT(ba.get_balance(), 100);

    ba.redo();
    ASSERT_EQ_INT(ba.get_balance(), 70);
    TEST_END();
}

void test_history_size_tracking()
{
    TEST("UndoRedo: History size grows with deposits");

    BankAccount2 ba{100};
    ASSERT_EQ_INT(ba.get_history_size(), 1);

    ba.deposit(50);
    ASSERT_EQ_INT(ba.get_history_size(), 2);

    ba.deposit(25);
    ASSERT_EQ_INT(ba.get_history_size(), 3);

    ba.deposit(75);
    ASSERT_EQ_INT(ba.get_history_size(), 4);
    TEST_END();
}

void test_sequential_operations()
{
    TEST("UndoRedo: Sequential deposit, undo, deposit, redo");

    BankAccount2 ba{100};
    ba.deposit(50); // 150
    ba.deposit(50); // 200
    ASSERT_EQ_INT(ba.get_balance(), 200);

    ba.undo(); // 150
    ASSERT_EQ_INT(ba.get_balance(), 150);

    ba.deposit(30); // 180
    ASSERT_EQ_INT(ba.get_balance(), 180);

    ba.redo(); // Should have no effect (history was branched)
    ASSERT_EQ_INT(ba.get_balance(), 180);
    TEST_END();
}

void test_restore_explicit()
{
    TEST("UndoRedo: Explicitly restore from memento");

    BankAccount2 ba{100};
    auto m1 = ba.deposit(50);  // 150
    auto m2 = ba.deposit(100); // 250

    ASSERT_EQ_INT(ba.get_balance(), 250);

    ba.restore(m1);
    ASSERT_EQ_INT(ba.get_balance(), 150);
    TEST_END();
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main()
{
    cout << "\n======================================================================\n";
    cout << "MEMENTO CODING EXERCISE - BANK ACCOUNT STATE MANAGEMENT\n";
    cout << "======================================================================\n";

    // BankAccount tests
    test_basic_deposit();
    test_memento_capture();
    test_restore_basic();
    test_multiple_mementos();
    test_negative_balance_restoration();

    // BankAccount2 tests
    test_undoredo_initial_state();
    test_deposit_and_undo();
    test_deposit_undo_redo();
    test_complex_undo_redo();
    test_undo_redo_limits();
    test_negative_deposits_undo_redo();
    test_history_size_tracking();
    test_sequential_operations();
    test_restore_explicit();

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
