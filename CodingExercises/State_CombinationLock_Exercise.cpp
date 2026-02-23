/*
 * State Pattern - Combination Lock Coding Exercise
 *
 * Problem: Implement a combination lock that maintains its internal state
 * as digits are entered. The lock must track:
 * - Current status (LOCKED, digits entered, OPEN, or ERROR)
 * - Which digits have been entered
 * - Whether the combination is correct
 *
 * This demonstrates state-based transitions:
 * LOCKED -> (digits entered) -> OPEN or ERROR
 */

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

class CombinationLock
{
    vector<int> combination;
    int digits_entered{0};
    bool failed{false};

    void reset()
    {
        status = "LOCKED";
        digits_entered = 0;
        failed = false;
    }

public:
    string status;

    CombinationLock(const vector<int> &combination)
        : combination(combination)
    {
        reset();
    }

    void enter_digit(int digit)
    {
        if (status == "LOCKED")
            status = "";

        status += to_string(digit);

        if (combination[digits_entered] != digit)
        {
            failed = true;
        }

        digits_entered++;

        if (digits_entered == combination.size())
            status = failed ? "ERROR" : "OPEN";
    }

    void reset_lock()
    {
        reset();
    }
};

// ============================================================================
// TEST SUITE
// ============================================================================

void test_lock_starts_locked()
{
    CombinationLock lock({1, 2, 3});
    assert(lock.status == "LOCKED");
    cout << "✓ Lock starts in LOCKED state" << endl;
}

void test_success_path()
{
    CombinationLock lock({1, 2, 3});
    assert(lock.status == "LOCKED");

    lock.enter_digit(1);
    assert(lock.status == "1");

    lock.enter_digit(2);
    assert(lock.status == "12");

    lock.enter_digit(3);
    assert(lock.status == "OPEN");

    cout << "✓ Correct combination opens lock" << endl;
}

void test_failure_path()
{
    CombinationLock lock({1, 2, 3});
    assert(lock.status == "LOCKED");

    lock.enter_digit(1);
    assert(lock.status == "1");

    lock.enter_digit(2);
    assert(lock.status == "12");

    lock.enter_digit(5);
    assert(lock.status == "ERROR");

    cout << "✓ Wrong digit produces ERROR state" << endl;
}

void test_first_digit_wrong()
{
    CombinationLock lock({1, 2, 3});

    lock.enter_digit(9);
    assert(lock.status == "9");

    lock.enter_digit(2);
    assert(lock.status == "92");

    lock.enter_digit(3);
    assert(lock.status == "ERROR");

    cout << "✓ First digit wrong produces ERROR" << endl;
}

void test_second_digit_wrong()
{
    CombinationLock lock({1, 2, 3});

    lock.enter_digit(1);
    assert(lock.status == "1");

    lock.enter_digit(9);
    assert(lock.status == "19");

    lock.enter_digit(3);
    assert(lock.status == "ERROR");

    cout << "✓ Second digit wrong produces ERROR" << endl;
}

void test_longer_combination()
{
    CombinationLock lock({5, 3, 7, 2});
    assert(lock.status == "LOCKED");

    lock.enter_digit(5);
    assert(lock.status == "5");

    lock.enter_digit(3);
    assert(lock.status == "53");

    lock.enter_digit(7);
    assert(lock.status == "537");

    lock.enter_digit(2);
    assert(lock.status == "OPEN");

    cout << "✓ Longer combination (4 digits) works" << endl;
}

void test_single_digit_combination()
{
    CombinationLock lock({7});
    assert(lock.status == "LOCKED");

    lock.enter_digit(7);
    assert(lock.status == "OPEN");

    cout << "✓ Single digit combination works" << endl;
}

void test_single_digit_wrong()
{
    CombinationLock lock({7});
    assert(lock.status == "LOCKED");

    lock.enter_digit(3);
    assert(lock.status == "ERROR");

    cout << "✓ Single digit wrong produces ERROR" << endl;
}

void test_reset_functionality()
{
    CombinationLock lock({1, 2, 3});

    lock.enter_digit(1);
    assert(lock.status == "1");

    lock.reset_lock();
    assert(lock.status == "LOCKED");

    // Now try correct combination
    lock.enter_digit(1);
    lock.enter_digit(2);
    lock.enter_digit(3);
    assert(lock.status == "OPEN");

    cout << "✓ Reset functionality works" << endl;
}

int main()
{
    cout << "State Pattern - Combination Lock Exercise\n"
         << endl;

    try
    {
        cout << "Running tests...\n"
             << endl;

        test_lock_starts_locked();
        test_success_path();
        test_failure_path();
        test_first_digit_wrong();
        test_second_digit_wrong();
        test_longer_combination();
        test_single_digit_combination();
        test_single_digit_wrong();
        test_reset_functionality();

        cout << "\n✅ All 9 tests passed!" << endl;
        cout << "\nState transitions:" << endl;
        cout << "  • LOCKED -> (enter digit) -> digit string" << endl;
        cout << "  • digit string -> OPEN (all correct)" << endl;
        cout << "  • digit string -> ERROR (wrong digit)" << endl;
        cout << "  • Any state -> LOCKED (via reset)" << endl;
        cout << "\nKey concepts:" << endl;
        cout << "  • Status string represents state" << endl;
        cout << "  • Transitions based on digit input" << endl;
        cout << "  • State changes when complete combination entered" << endl;
        cout << "  • Failed flag triggers ERROR state" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
}
