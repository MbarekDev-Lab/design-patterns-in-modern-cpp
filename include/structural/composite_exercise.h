#pragma once
#include <vector>
#include <numeric>
#include <memory>

using namespace std;

// ============================================================================
// COMPOSITE PATTERN EXERCISE: ValueContainer Hierarchy
// ============================================================================
// This exercise demonstrates the Composite pattern applied to value containers.
// The pattern allows treating individual values and collections uniformly.
// ============================================================================

// Common interface for both leaf (SingleValue) and composite (ManyValues)
struct ValueContainer
{
    virtual vector<int>::iterator begin() = 0;
    virtual vector<int>::iterator end() = 0;
    virtual ~ValueContainer() = default;
};

// ============================================================================
// LEAF: SingleValue
// ============================================================================
// Represents a single value wrapped in a container interface.
// Even a single value implements the same interface as a collection.
// ============================================================================
struct SingleValue : ValueContainer
{
    int value;

    explicit SingleValue(int value) : value(value) {}

private:
    vector<int> data{value}; // Wrap single value in a vector for interface compatibility

public:
    vector<int>::iterator begin() override { return data.begin(); }
    vector<int>::iterator end() override { return data.end(); }
};

// ============================================================================
// COMPOSITE: ManyValues
// ============================================================================
// Represents a collection of values.
// Inherits from both ValueContainer (provides interface) and vector<int> (provides storage).
// This allows it to be used wherever ValueContainer is expected.
// ============================================================================
struct ManyValues : ValueContainer, vector<int>
{
    // Provide the iterator interface from the vector
    vector<int>::iterator begin() override
    {
        return vector<int>::begin();
    }

    vector<int>::iterator end() override
    {
        return vector<int>::end();
    }

    // Add convenience method for adding values
    void add(int value)
    {
        push_back(value);
    }
};

// ============================================================================
// FUNCTION: sum
// ============================================================================
// Demonstrates the power of the Composite pattern:
// This function works uniformly with both SingleValue and ManyValues objects.
// It doesn't need to know the concrete types - just the ValueContainer interface.
// ============================================================================
int sum(vector<ValueContainer *> items)
{
    int result = 0;

    // Iterate through all containers
    for (auto item : items)
    {
        // Use the iterator interface to access values
        // Works the same whether item is SingleValue or ManyValues!
        for (int i : *item)
        {
            result += i;
        }
    }

    return result;
}





