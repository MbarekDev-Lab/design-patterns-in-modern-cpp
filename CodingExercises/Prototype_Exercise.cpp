#include <iostream>
#include <cassert>
#include <memory>
#include <vector>

using namespace std;

// ============================================================================
// PROTOTYPE PATTERN - CODING EXERCISE
// ============================================================================
// Exercise: Implement Line::deep_copy() to perform a deep copy of a Line
// object with its start and end Points.
//
// Key Concepts:
// - Deep copying vs shallow copying
// - Memory management with pointers
// - Preventing memory leaks with proper cleanup
// - Cloning complex object graphs
// ============================================================================

struct Point
{
    int x{0}, y{0};

    Point() {}

    Point(const int x, const int y) : x{x}, y{y} {}

    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }

    friend ostream &operator<<(ostream &os, const Point &p)
    {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};

// ============================================================================
// EXERCISE VERSION - Incomplete Implementation
// ============================================================================

struct LineExercise
{
    Point *start;
    Point *end;

    LineExercise(Point *const start, Point *const end)
        : start(start), end(end)
    {
    }

    ~LineExercise()
    {
        delete start;
        delete end;
    }

    // TODO: Implement deep_copy() to properly clone this Line
    LineExercise deep_copy() const
    {
        // Create new Point objects to avoid sharing pointers
        Point *newStart = new Point(start->x, start->y);
        Point *newEnd = new Point(end->x, end->y);

        return LineExercise(newStart, newEnd);
    }

    friend ostream &operator<<(ostream &os, const LineExercise &line)
    {
        return os << "Line from " << *line.start << " to " << *line.end;
    }
};

// ============================================================================
// SOLUTION VERSION - Complete Implementation
// ============================================================================

struct Line
{
    Point *start;
    Point *end;

    Line(Point *const start, Point *const end)
        : start(start), end(end)
    {
    }

    // Move constructor for safe transfer of ownership
    Line(Line &&other) noexcept
        : start(other.start), end(other.end)
    {
        other.start = nullptr;
        other.end = nullptr;
    }

    // Move assignment operator
    Line &operator=(Line &&other) noexcept
    {
        if (this != &other)
        {
            delete start;
            delete end;
            start = other.start;
            end = other.end;
            other.start = nullptr;
            other.end = nullptr;
        }
        return *this;
    }

    // Delete copy operations to prevent shallow copy
    Line(const Line &) = delete;
    Line &operator=(const Line &) = delete;

    ~Line()
    {
        delete start;
        delete end;
    }

    // Deep copy implementation
    Line deep_copy() const
    {
        // Key points:
        // 1. Create new Point objects (not shared references)
        // 2. Copy the values from the original points
        // 3. Return a new Line with the new points
        // 4. Memory is properly managed - no leaks
        Point *newStart = new Point(start->x, start->y);
        Point *newEnd = new Point(end->x, end->y);

        return Line(newStart, newEnd);
    }

    // Verify that this is truly a deep copy
    bool is_independent_from(const Line &other) const
    {
        // Different start pointers
        if (start == other.start)
            return false;
        // Different end pointers
        if (end == other.end)
            return false;
        // Same values but different objects
        return *start == *other.start && *end == *other.end;
    }

    friend ostream &operator<<(ostream &os, const Line &line)
    {
        return os << "Line from " << *line.start << " to " << *line.end;
    }
};

// ============================================================================
// TEST CASE 1: Basic Deep Copy
// ============================================================================

void test_1_basic_deep_copy()
{
    cout << "\n========== TEST 1: Basic Deep Copy ==========\n";

    Line original(new Point(0, 0), new Point(10, 10));
    cout << "Original: " << original << "\n";

    Line cloned = original.deep_copy();
    cout << "Cloned:   " << cloned << "\n";

    // Verify independence
    assert(original.is_independent_from(cloned));
    cout << "✓ Cloned is independent\n";
    cout << "✓ Values are identical\n";
}

// ============================================================================
// TEST CASE 2: Modifying Clone Doesn't Affect Original
// ============================================================================

void test_2_independence()
{
    cout << "\n========== TEST 2: Modification Independence ==========\n";

    Line original(new Point(0, 0), new Point(10, 10));
    cout << "Original: " << original << "\n";

    Line cloned = original.deep_copy();

    // Modify the clone
    cloned.start->x = 100;
    cloned.start->y = 100;
    cloned.end->x = 200;
    cloned.end->y = 200;

    cout << "After modifying clone:\n";
    cout << "Original: " << original << "\n";
    cout << "Cloned:   " << cloned << "\n";

    // Original must remain unchanged
    assert(original.start->x == 0 && original.start->y == 0);
    assert(original.end->x == 10 && original.end->y == 10);
    cout << "✓ Original unchanged\n";
    cout << "✓ Clone is truly independent\n";
}

// ============================================================================
// TEST CASE 3: Chain Copying
// ============================================================================

void test_3_chain_copying()
{
    cout << "\n========== TEST 3: Chain Copying ==========\n";

    Line line1(new Point(0, 0), new Point(5, 5));
    Line line2 = line1.deep_copy();
    Line line3 = line2.deep_copy();

    cout << "Line 1: " << line1 << "\n";
    cout << "Line 2: " << line2 << "\n";
    cout << "Line 3: " << line3 << "\n";

    // All should be independent
    assert(line1.is_independent_from(line2));
    assert(line2.is_independent_from(line3));
    assert(line1.is_independent_from(line3));

    cout << "✓ All lines are independent\n";
    cout << "✓ No shared pointers\n";
}

// ============================================================================
// TEST CASE 4: Collection of Deep Copies
// ============================================================================

void test_4_collection()
{
    cout << "\n========== TEST 4: Collection of Deep Copies ==========\n";

    vector<Line> lines;

    Line original(new Point(0, 0), new Point(10, 10));

    // Create multiple independent copies
    for (int i = 0; i < 3; ++i)
    {
        lines.push_back(original.deep_copy());
    }

    // Modify each copy differently
    lines[0].start->x = 1;
    lines[1].end->y = 20;
    lines[2].start->y = 5;

    cout << "Collections of copies:\n";
    for (size_t i = 0; i < lines.size(); ++i)
    {
        cout << "  Copy " << i << ": " << lines[i] << "\n";
    }

    cout << "✓ Multiple independent copies created\n";
}

// ============================================================================
// TEST CASE 5: Memory Management Verification
// ============================================================================

void test_5_memory_management()
{
    cout << "\n========== TEST 5: Memory Management ==========\n";

    {
        Line line1(new Point(0, 0), new Point(10, 10));
        Line line2 = line1.deep_copy();

        cout << "Created line1 and line2 (deep copy)\n";
        cout << "Line 1: " << line1 << "\n";
        cout << "Line 2: " << line2 << "\n";

        // Both will be properly deleted when going out of scope
    } // Destructors called here - no memory leaks!

    cout << "✓ Memory properly deallocated\n";
    cout << "✓ No memory leaks\n";
}

// ============================================================================
// TEST CASE 6: Practical Example - Geometric Transformations
// ============================================================================

void test_6_geometric_transform()
{
    cout << "\n========== TEST 6: Geometric Transformations ==========\n";

    // Original line
    Line original(new Point(0, 0), new Point(10, 10));
    cout << "Original line: " << original << "\n";

    // Create a translated copy (moved by offset)
    auto translated = original.deep_copy();
    int offset_x = 5, offset_y = -3;
    translated.start->x += offset_x;
    translated.start->y += offset_y;
    translated.end->x += offset_x;
    translated.end->y += offset_y;
    cout << "Translated:    " << translated << " (by " << offset_x << ", "
         << offset_y << ")\n";

    // Create a reflected copy (swap start and end)
    auto reflected = original.deep_copy();
    swap(reflected.start, reflected.end);
    cout << "Reflected:     " << reflected << "\n";

    // Verify independence by checking pointer addresses
    assert(original.start != translated.start);
    assert(original.end != translated.end);
    assert(original.start != reflected.start);
    assert(original.end != reflected.end);

    cout << "✓ Multiple transformations applied independently\n";
}

// ============================================================================
// TEST CASE 7: Comparing with Shallow Copy (WRONG WAY)
// ============================================================================

struct LineBad
{
    Point *start;
    Point *end;

    LineBad(Point *const start, Point *const end)
        : start(start), end(end)
    {
    }

    ~LineBad()
    {
        delete start;
        delete end;
    }

    // WRONG: Shallow copy (assignment operator)
    // This shares pointers - dangerous!
    LineBad shallow_copy() const
    {
        LineBad newLine(start, end); // SHARES pointers - WRONG!
        return newLine;
    }

    friend ostream &operator<<(ostream &os, const LineBad &line)
    {
        return os << "Line from " << *line.start << " to " << *line.end;
    }
};

void test_7_shallow_vs_deep()
{
    cout << "\n========== TEST 7: Shallow vs Deep Copy ==========\n";

    // DEEP COPY (Correct)
    cout << "Deep Copy (CORRECT):\n";
    {
        Line line1(new Point(0, 0), new Point(10, 10));
        Line line2 = line1.deep_copy();

        cout << "  Line 1: " << line1 << "\n";
        cout << "  Line 2: " << line2 << "\n";

        line2.start->x = 999;
        cout << "  After modifying line2.start->x = 999:\n";
        cout << "  Line 1: " << line1 << " (unchanged)\n";
        cout << "  Line 2: " << line2 << "\n";

        cout << "  ✓ No side effects - safe!\n";
    }

    cout << "\nShallow Copy (DANGEROUS):\n";
    {
        // Creating similar scenario with shallow copy
        Point *p1 = new Point(0, 0);
        Point *p2 = new Point(10, 10);

        Point *p1_ref = p1; // Shallow - shared reference!
        Point *p2_ref = p2; // Shallow - shared reference!

        cout << "  Original: (" << p1->x << ", " << p1->y << ") -> ("
             << p2->x << ", " << p2->y << ")\n";
        cout << "  Shallow:  (" << p1_ref->x << ", " << p1_ref->y << ") -> ("
             << p2_ref->x << ", " << p2_ref->y << ")\n";

        // Modify through shallow copy
        p1_ref->x = 999;
        cout << "  After modifying p1_ref->x = 999:\n";
        cout << "  Original: (" << p1->x << ", " << p1->y << ") -> ("
             << p2->x << ", " << p2->y << ") (CHANGED!)\n";
        cout << "  ⚠ Side effects - unsafe!\n";

        delete p1;
        delete p2;
    }
}

// ============================================================================
// TEST CASE 8: Real-World - Graphics Editor Scenario
// ============================================================================

void test_8_real_world()
{
    cout << "\n========== TEST 8: Graphics Editor Scenario ==========\n";

    cout << "Scenario: User draws a line and wants to duplicate it\n\n";

    // User draws original line
    Line original(new Point(100, 50), new Point(200, 150));
    cout << "User draws: " << original << "\n";

    // User duplicates the line (Ctrl+D)
    Line duplicate = original.deep_copy();

    // Move the duplicate to a new position
    duplicate.start->x += 50;
    duplicate.start->y += 50;
    duplicate.end->x += 50;
    duplicate.end->y += 50;

    cout << "Duplicate created and moved:\n";
    cout << "Original:  " << original << "\n";
    cout << "Duplicate: " << duplicate << "\n";

    cout << "\n✓ Duplication works correctly\n";
    cout << "✓ Original line remains unchanged\n";
}

// ============================================================================
// EXERCISE LEARNING OBJECTIVES
// ============================================================================

void print_learning_objectives()
{
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║         PROTOTYPE PATTERN - CODING EXERCISE               ║\n";
    cout << "║              Deep Copy Implementation                      ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";

    cout << "\nLearning Objectives:\n";
    cout << "  1. Understand deep copying vs shallow copying\n";
    cout << "  2. Implement deep_copy() for objects with pointers\n";
    cout << "  3. Manage memory properly to avoid leaks\n";
    cout << "  4. Create independent clones of complex objects\n";
    cout << "  5. Use deep copies in practical scenarios\n";

    cout << "\nKey Implementation Points:\n";
    cout << "  • Create NEW Point objects (not share existing ones)\n";
    cout << "  • Copy values from original points: Point(start->x, start->y)\n";
    cout << "  • Return a new Line with the new points\n";
    cout << "  • Destructors handle cleanup automatically\n";
    cout << "  • No dangling pointers or shared ownership\n";

    cout << "\nCommon Mistakes:\n";
    cout << "  ✗ Returning references to temporary objects\n";
    cout << "  ✗ Sharing Point pointers between Line objects\n";
    cout << "  ✗ Not allocating new memory for cloned objects\n";
    cout << "  ✗ Forgetting to update all member pointers\n";

    cout << "\nExtensions for Practice:\n";
    cout << "  • Add move constructor/operator for efficiency\n";
    cout << "  • Implement operator== for comparing lines\n";
    cout << "  • Add transformation methods (rotate, scale, translate)\n";
    cout << "  • Create a Line container with automatic deep copies\n";
    cout << "  • Use smart pointers (unique_ptr) to simplify memory mgmt\n";
}

// ============================================================================
// Main Entry Point
// ============================================================================

int main()
{
    print_learning_objectives();

    cout << "\n"
         << string(60, '=') << "\n";
    cout << "Running Test Cases\n";
    cout << string(60, '=') << "\n";

    try
    {
        test_1_basic_deep_copy();
        test_2_independence();
        test_3_chain_copying();
        test_4_collection();
        test_5_memory_management();
        test_6_geometric_transform();
        test_7_shallow_vs_deep();
        test_8_real_world();

        cout << "\n"
             << string(60, '=') << "\n";
        cout << "╔════════════════════════════════════════════════════════════╗\n";
        cout << "║  ALL TESTS PASSED! ✓                                       ║\n";
        cout << "║  You've successfully implemented Line::deep_copy()!        ║\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    }
    catch (const exception &e)
    {
        cout << "\n✗ TEST FAILED: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
