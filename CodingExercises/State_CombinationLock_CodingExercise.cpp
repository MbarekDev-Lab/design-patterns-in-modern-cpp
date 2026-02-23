/*
 * State Pattern - Combination Lock Coding Exercise
 *
 * Problem: Implement a combination lock that maintains state based on digits entered.
 *
 * State Rules:
 * 1. At startup or after locking: status = "LOCKED"
 * 2. As digits are entered: status shows the entered digits
 * 3. If correct combination entered: status = "OPEN"
 * 4. If wrong digit entered: status = "ERROR"
 *
 * Example:
 * CombinationLock cl({1,2,3});          // LOCKED
 * cl.enter_digit(1);                     // "1"
 * cl.enter_digit(2);                     // "12"
 * cl.enter_digit(3);                     // "OPEN"
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
        // Transition from LOCKED state (first digit)
        if (status == "LOCKED")
            status = "";

        // Append the entered digit to status
        status += to_string(digit);

        // Check if this digit matches the expected digit
        if (combination[digits_entered] != digit)
        {
            failed = true;
        }

        digits_entered++;

        // Check if we've entered all digits
        if (digits_entered == static_cast<int>(combination.size()))
        {
            status = failed ? "ERROR" : "OPEN";
        }
    }

    // Helper method to reset the lock for testing
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
    CombinationLock cl({1, 2, 3});
    assert(cl.status == "LOCKED");
    cout << "✓ Lock starts in LOCKED state" << endl;
}

void test_single_digit_entry()
{
    CombinationLock cl({1, 2, 3});
    cl.enter_digit(1);
    assert(cl.status == "1");
    cout << "✓ Single digit shows in status" << endl;
}

void test_two_digits_entry()
{
    CombinationLock cl({1, 2, 3});
    cl.enter_digit(1);
    assert(cl.status == "1");
    cl.enter_digit(2);
    assert(cl.status == "12");
    cout << "✓ Multiple digits accumulate in status" << endl;
}

void test_correct_combination_unlocks()
{
    CombinationLock cl({1, 2, 3});
    assert(cl.status == "LOCKED");

    cl.enter_digit(1);
    assert(cl.status == "1");

    cl.enter_digit(2);
    assert(cl.status == "12");

    cl.enter_digit(3);
    assert(cl.status == "OPEN");

    cout << "✓ Correct combination opens lock (OPEN)" << endl;
}

void test_wrong_first_digit()
{
    CombinationLock cl({1, 2, 3});

    cl.enter_digit(9);
    assert(cl.status == "9");

    cl.enter_digit(2);
    assert(cl.status == "92");

    cl.enter_digit(3);
    assert(cl.status == "ERROR");

    cout << "✓ Wrong first digit results in ERROR" << endl;
}

void test_wrong_second_digit()
{
    CombinationLock cl({1, 2, 3});

    cl.enter_digit(1);
    assert(cl.status == "1");

    cl.enter_digit(9);
    assert(cl.status == "19");

    cl.enter_digit(3);
    assert(cl.status == "ERROR");

    cout << "✓ Wrong second digit results in ERROR" << endl;
}

void test_wrong_last_digit()
{
    CombinationLock cl({1, 2, 3});

    cl.enter_digit(1);
    assert(cl.status == "1");

    cl.enter_digit(2);
    assert(cl.status == "12");

    cl.enter_digit(5);
    assert(cl.status == "ERROR");

    cout << "✓ Wrong last digit results in ERROR" << endl;
}

void test_longer_combination()
{
    CombinationLock cl({5, 3, 7, 2});

    cl.enter_digit(5);
    assert(cl.status == "5");

    cl.enter_digit(3);
    assert(cl.status == "53");

    cl.enter_digit(7);
    assert(cl.status == "537");

    cl.enter_digit(2);
    assert(cl.status == "OPEN");

    cout << "✓ Longer combination (4 digits) works correctly" << endl;
}

void test_single_digit_combination_correct()
{
    CombinationLock cl({7});

    cl.enter_digit(7);
    assert(cl.status == "OPEN");

    cout << "✓ Single digit correct combination opens" << endl;
}

void test_single_digit_combination_wrong()
{
    CombinationLock cl({7});

    cl.enter_digit(3);
    assert(cl.status == "ERROR");

    cout << "✓ Single digit wrong combination shows ERROR" << endl;
}

void test_large_combination()
{
    CombinationLock cl({1, 1, 2, 3, 5, 8, 13});

    // Enter correct digits
    cl.enter_digit(1);
    assert(cl.status == "1");
    cl.enter_digit(1);
    assert(cl.status == "11");
    cl.enter_digit(2);
    assert(cl.status == "112");
    cl.enter_digit(3);
    assert(cl.status == "1123");
    cl.enter_digit(5);
    assert(cl.status == "11235");
    cl.enter_digit(8);
    assert(cl.status == "112358");
    cl.enter_digit(13);
    assert(cl.status == "OPEN");

    cout << "✓ Large combination (7+ digits) works" << endl;
}

void test_all_zeros()
{
    CombinationLock cl({0, 0, 0});

    cl.enter_digit(0);
    assert(cl.status == "0");

    cl.enter_digit(0);
    assert(cl.status == "00");

    cl.enter_digit(0);
    assert(cl.status == "OPEN");

    cout << "✓ All zeros combination works" << endl;
}

void test_consecutive_same_digits()
{
    CombinationLock cl({5, 5, 5});

    cl.enter_digit(5);
    assert(cl.status == "5");

    cl.enter_digit(5);
    assert(cl.status == "55");

    cl.enter_digit(5);
    assert(cl.status == "OPEN");

    cout << "✓ Consecutive same digits work" << endl;
}

void test_reset_functionality()
{
    CombinationLock cl({1, 2, 3});

    cl.enter_digit(1);
    assert(cl.status == "1");

    cl.reset_lock();
    assert(cl.status == "LOCKED");

    // Can try again after reset
    cl.enter_digit(1);
    assert(cl.status == "1");
    cl.enter_digit(2);
    assert(cl.status == "12");
    cl.enter_digit(3);
    assert(cl.status == "OPEN");

    cout << "✓ Reset functionality works" << endl;
}

void test_double_zero_then_different()
{
    CombinationLock cl({0, 0, 5});

    cl.enter_digit(0);
    assert(cl.status == "0");

    cl.enter_digit(0);
    assert(cl.status == "00");

    cl.enter_digit(5);
    assert(cl.status == "OPEN");

    cout << "✓ Double zero followed by different digit works" << endl;
}

void test_large_digit_values()
{
    CombinationLock cl({99, 88, 77});

    cl.enter_digit(99);
    assert(cl.status == "99");

    cl.enter_digit(88);
    assert(cl.status == "9988");

    cl.enter_digit(77);
    assert(cl.status == "OPEN");

    cout << "✓ Large digit values (multi-digit numbers) work" << endl;
}

void test_state_transitions_sequence()
{
    cout << "\n--- State Transition Sequence ---" << endl;
    CombinationLock cl({1, 2, 3});

    cout << "Initial state: " << cl.status << endl;
    assert(cl.status == "LOCKED");

    cl.enter_digit(1);
    cout << "After digit 1: " << cl.status << endl;
    assert(cl.status == "1");

    cl.enter_digit(2);
    cout << "After digit 2: " << cl.status << endl;
    assert(cl.status == "12");

    cl.enter_digit(3);
    cout << "After digit 3: " << cl.status << endl;
    assert(cl.status == "OPEN");

    cout << "✓ Complete state transition sequence verified" << endl;
}

int main()
{
    cout << "State Pattern - Combination Lock Coding Exercise\n"
         << endl;

    try
    {
        cout << "Running comprehensive tests...\n"
             << endl;

        test_lock_starts_locked();
        test_single_digit_entry();
        test_two_digits_entry();
        test_correct_combination_unlocks();
        test_wrong_first_digit();
        test_wrong_second_digit();
        test_wrong_last_digit();
        test_longer_combination();
        test_single_digit_combination_correct();
        test_single_digit_combination_wrong();
        test_large_combination();
        test_all_zeros();
        test_consecutive_same_digits();
        test_reset_functionality();
        test_double_zero_then_different();
        test_large_digit_values();
        test_state_transitions_sequence();

        cout << "\n✅ All 17 tests passed!" << endl;
        cout << "\nState Machine for Combination Lock:" << endl;
        cout << "  LOCKED ----enter_digit----> <digits>" << endl;
        cout << "                                 |" << endl;
        cout << "                                 v" << endl;
        cout << "                       <continue entering digits>" << endl;
        cout << "                                 |" << endl;
        cout << "                _________________|" << endl;
        cout << "               |                 |" << endl;
        cout << "               v                 v" << endl;
        cout << "             OPEN            ERROR" << endl;
        cout << "         (correct)         (incorrect)" << endl;
        cout << "\nKey Implementation Details:" << endl;
        cout << "  • LOCKED state: initial status" << endl;
        cout << "  • Transition to digit display: status += to_string(digit)" << endl;
        cout << "  • Track progress: digits_entered counter" << endl;
        cout << "  • Check correctness: compare with combination[index]" << endl;
        cout << "  • Final state: OPEN (all correct) or ERROR (any wrong)" << endl;
        cout << "  • Reset capability: restore to LOCKED state" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Test failed with exception: " << e.what() << endl;
        return 1;
    }
}
