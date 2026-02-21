#include <iostream>
#include "../include/structural/composite_exercise_v2.h"

int main()
{
    cout << "=== COMPOSITE PATTERN EXERCISE V2: Polymorphic sum() ===" << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 1: Single Value
    // ========================================================================
    cout << "--- EXAMPLE 1: Single Value ---" << endl;
    SingleValue single_value{11};
    cout << "SingleValue: 11" << endl;
    cout << "sum(): " << single_value.sum() << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 2: Many Values (Collection)
    // ========================================================================
    cout << "--- EXAMPLE 2: Collection of Values ---" << endl;
    ManyValues many_values;
    many_values.add(22);
    many_values.add(33);
    cout << "ManyValues: [22, 33]" << endl;
    cout << "sum(): " << many_values.sum() << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 3: Mixed - Single and Many Together (Composite Pattern Power!)
    // ========================================================================
    cout << "--- EXAMPLE 3: Composite Pattern Power ---" << endl;
    cout << "Mixing SingleValue and ManyValues in one container" << endl;

    vector<ContainsIntegers *> items{&single_value, &many_values};

    cout << "Items:" << endl;
    cout << "  [0] SingleValue(11)" << endl;
    cout << "  [1] ManyValues([22, 33])" << endl;
    cout << endl;

    int total = sum(items);
    cout << "Total sum: " << total << endl;
    cout << "Calculation: 11 + (22 + 33) = 11 + 55 = 66" << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 4: Multiple Collections
    // ========================================================================
    cout << "--- EXAMPLE 4: Multiple Collections ---" << endl;
    SingleValue sv1{5}, sv2{10};
    ManyValues mv1, mv2;
    mv1.add(1);
    mv1.add(2);
    mv1.add(3);
    mv2.add(100);
    mv2.add(200);

    vector<ContainsIntegers *> complex{&sv1, &mv1, &sv2, &mv2};
    cout << "Items: sv1(5), mv1([1,2,3]), sv2(10), mv2([100,200])" << endl;
    cout << "Total sum: " << sum(complex) << endl;
    cout << "Calculation: 5 + (1+2+3) + 10 + (100+200) = 321" << endl;

    return 0;
}
