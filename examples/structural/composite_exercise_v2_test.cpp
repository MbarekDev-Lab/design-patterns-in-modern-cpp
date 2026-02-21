#include <iostream>
#include "../include/structural/composite_exercise_v2.h"

// ============================================================================
// TEST: Composite Pattern Exercise V2
// ============================================================================
// This test demonstrates the Composite pattern in action.
// It shows how a single interface (ContainsIntegers) works uniformly
// with both individual values (SingleValue) and collections (ManyValues).
// ============================================================================

int main()
{
    cout << "=== COMPOSITE PATTERN EXERCISE V2 TEST ===" << endl;
    cout << endl;

    // Test Case 1: Simple Test with mixed types
    {
        cout << "Test Case 1: SingleValue + ManyValues" << endl;
        SingleValue single_value{11};
        ManyValues other_values;
        other_values.add(22);
        other_values.add(33);

        int result = sum({&single_value, &other_values});
        int expected = 66;

        cout << "  SingleValue: 11" << endl;
        cout << "  ManyValues: [22, 33]" << endl;
        cout << "  Expected sum: " << expected << endl;
        cout << "  Actual sum: " << result << endl;

        if (result == expected)
        {
            cout << "  ✓ PASS" << endl;
        }
        else
        {
            cout << "  ✗ FAIL" << endl;
            return 1;
        }
        cout << endl;
    }

    // Test Case 2: Multiple SingleValues
    {
        cout << "Test Case 2: Multiple SingleValues" << endl;
        SingleValue sv1{5};
        SingleValue sv2{10};
        SingleValue sv3{15};

        int result = sum({&sv1, &sv2, &sv3});
        int expected = 30;

        cout << "  Values: 5 + 10 + 15" << endl;
        cout << "  Expected sum: " << expected << endl;
        cout << "  Actual sum: " << result << endl;

        if (result == expected)
        {
            cout << "  ✓ PASS" << endl;
        }
        else
        {
            cout << "  ✗ FAIL" << endl;
            return 1;
        }
        cout << endl;
    }

    // Test Case 3: Multiple ManyValues
    {
        cout << "Test Case 3: Multiple ManyValues" << endl;
        ManyValues mv1;
        mv1.add(1);
        mv1.add(2);

        ManyValues mv2;
        mv2.add(3);
        mv2.add(4);

        int result = sum({&mv1, &mv2});
        int expected = 10;

        cout << "  ManyValues [1, 2] + [3, 4]" << endl;
        cout << "  Expected sum: " << expected << endl;
        cout << "  Actual sum: " << result << endl;

        if (result == expected)
        {
            cout << "  ✓ PASS" << endl;
        }
        else
        {
            cout << "  ✗ FAIL" << endl;
            return 1;
        }
        cout << endl;
    }

    // Test Case 4: Complex Mix
    {
        cout << "Test Case 4: Complex Mix" << endl;
        SingleValue sv{100};
        ManyValues mv1;
        mv1.add(10);
        mv1.add(20);

        ManyValues mv2;
        mv2.add(5);
        mv2.add(5);

        int result = sum({&sv, &mv1, &mv2});
        int expected = 140;

        cout << "  SingleValue(100) + ManyValues([10,20]) + ManyValues([5,5])" << endl;
        cout << "  Expected sum: " << expected << endl;
        cout << "  Actual sum: " << result << endl;

        if (result == expected)
        {
            cout << "  ✓ PASS" << endl;
        }
        else
        {
            cout << "  ✗ FAIL" << endl;
            return 1;
        }
        cout << endl;
    }

    cout << "=== ALL TESTS PASSED ===" << endl;
    return 0;
}
