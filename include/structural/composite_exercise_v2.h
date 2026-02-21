#pragma once
#include <string>
#include <vector>
#include <numeric>

using namespace std;

// ============================================================================
// COMPOSITE PATTERN EXERCISE: ContainsIntegers Hierarchy (Alternative Approach)
// ============================================================================
// This is an alternative Composite pattern implementation using polymorphic
// methods instead of iterators. Each concrete type implements its own sum() logic.
// ============================================================================

struct ContainsIntegers
{
    virtual int sum() = 0;
    virtual ~ContainsIntegers() = default;
};

// ============================================================================
// LEAF: SingleValue
// ============================================================================
// Represents a single integer value.
// Implements sum() to return the single value.
// ============================================================================
struct SingleValue : ContainsIntegers
{
    int value{0};

    SingleValue() = default;

    explicit SingleValue(const int value)
        : value{value}
    {
    }

    int sum() override
    {
        return value;
    }
};

// ============================================================================
// COMPOSITE: ManyValues
// ============================================================================
// Represents a collection of integer values (inherits from vector<int>).
// Implements sum() to return the sum of all values in the collection.
// ============================================================================
struct ManyValues : vector<int>, ContainsIntegers
{
    void add(const int value)
    {
        push_back(value);
    }

    int sum() override
    {
        return accumulate(begin(), end(), 0);
    }
};

// ============================================================================
// FUNCTION: sum
// ============================================================================
// Demonstrates the Composite pattern power:
// This function works uniformly with both SingleValue and ManyValues.
// It calls the sum() method on each item, regardless of type.
// This is cleaner than iteration-based approaches because each type
// handles its own aggregation logic.
// ============================================================================
int sum(const vector<ContainsIntegers *> items)
{
    int result{0};
    for (auto item : items)
        result += item->sum(); // Polymorphic call works for both types!
    return result;
}
