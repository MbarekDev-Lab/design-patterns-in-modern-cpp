#include <iostream>
#include "../include/structural/composite_exercise.h"

int main()
{
    // ========================================================================
    // EXAMPLE 1: Single values (leaf nodes)
    // ========================================================================
    SingleValue sv1(1), sv2(2), sv3(3);

    cout << "=== EXAMPLE 1: Single Values ===" << endl;
    vector<ValueContainer *> values{&sv1, &sv2, &sv3};
    cout << "Sum of single values [1, 2, 3]: " << sum(values) << endl
         << endl;

    // ========================================================================
    // EXAMPLE 2: Collection of values (composite)
    // ========================================================================
    ManyValues mv1;
    mv1.add(10);
    mv1.add(20);
    mv1.add(30);

    cout << "=== EXAMPLE 2: Collection of Values ===" << endl;
    vector<ValueContainer *> collections{&mv1};
    cout << "Sum of collection [10, 20, 30]: " << sum(collections) << endl
         << endl;

    // ========================================================================
    // EXAMPLE 3: Mixed - Single values and collections together
    // ========================================================================
    cout << "=== EXAMPLE 3: Mixed Single Values and Collections ===" << endl;
    ManyValues mv2;
    mv2.add(100);
    mv2.add(200);

    vector<ValueContainer *> mixed{&sv1, &mv1, &sv2, &mv2, &sv3};
    // Contains:
    // - sv1 (1)
    // - mv1 (10, 20, 30)
    // - sv2 (2)
    // - mv2 (100, 200)
    // - sv3 (3)
    // Total: 1 + 10 + 20 + 30 + 2 + 100 + 200 + 3 = 366

    cout << "Sum of mixed items: " << sum(mixed) << endl;
    cout << "Expected: 1 + (10+20+30) + 2 + (100+200) + 3 = 366" << endl;

    return 0;
}
