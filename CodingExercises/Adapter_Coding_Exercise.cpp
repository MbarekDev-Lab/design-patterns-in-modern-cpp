#include <iostream>
#include <vector>
#include <memory>
#include <cassert>

using namespace std;

// ============================================================================
// ADAPTER CODING EXERCISE: Square to Rectangle Adapter
// ============================================================================
// Task: Adapt a Square interface to a Rectangle interface
// You have:
//   1. Rectangle protocol (interface)
//   2. Extension method on Rectangle
//   3. Square class with incompatible interface
// Goal: Create SquareToRectangleAdapter to make them compatible
// ============================================================================

// ============================================================================
// STEP 1: Define the Rectangle Protocol (Interface)
// ============================================================================

class Rectangle
{
public:
    virtual ~Rectangle() = default;
    virtual int get_width() const = 0;
    virtual int get_height() const = 0;

    // Extension method (works with any Rectangle)
    virtual int get_area() const
    {
        return get_width() * get_height();
    }
};

// ============================================================================
// STEP 2: The Square Class with Incompatible Interface
// ============================================================================

class Square
{
public:
    Square(int side) : side_(side) {}

    int get_side() const { return side_; }
    void set_side(int s) { side_ = s; }

private:
    int side_;
};

// ============================================================================
// STEP 3: Create SquareToRectangleAdapter
// ============================================================================
// YOUR TASK:
// Implement an adapter that:
//   1. Inherits from Rectangle
//   2. Wraps a Square (composition)
//   3. Adapts get_width() and get_height() to return side values
//   4. Allows the extension method get_area() to work automatically
// ============================================================================

class SquareToRectangleAdapter : public Rectangle
{
public:
    // Constructor takes a Square reference or pointer
    explicit SquareToRectangleAdapter(const shared_ptr<Square> &square)
        : square_(square)
    {
    }

    // Implement the Rectangle interface
    int get_width() const override
    {
        return square_->get_side();
    }

    int get_height() const override
    {
        return square_->get_side();
    }

    // Note: get_area() is inherited from Rectangle and automatically works
    // because it calls get_width() and get_height() which we've implemented

private:
    shared_ptr<Square> square_;
};

// ============================================================================
// ACTUAL RECTANGLE IMPLEMENTATIONS (for testing)
// ============================================================================

class ConcreteRectangle : public Rectangle
{
public:
    ConcreteRectangle(int width, int height) : width_(width), height_(height) {}

    int get_width() const override { return width_; }
    int get_height() const override { return height_; }

private:
    int width_, height_;
};

// ============================================================================
// TEST FRAMEWORK
// ============================================================================

struct TestResult
{
    int total = 0;
    int passed = 0;
    int failed = 0;

    void print_summary()
    {
        cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        cout << "║              TEST RESULTS SUMMARY                          ║\n";
        cout << "├────────────────────────────────────────────────────────────┤\n";
        cout << "│ Total Tests:  " << total << "\n";
        cout << "│ Passed:       " << passed << " ✓\n";
        cout << "│ Failed:       " << failed << " ✗\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n\n";
    }
};

TestResult results;

#define TEST_CASE(name)                    \
    cout << "\n[TEST] " << (name) << "\n"; \
    results.total++

#define ASSERT_EQ(actual, expected, message)                                                                   \
    if ((actual) == (expected))                                                                                \
    {                                                                                                          \
        cout << "  ✓ " << (message) << "\n";                                                                   \
        results.passed++;                                                                                      \
    }                                                                                                          \
    else                                                                                                       \
    {                                                                                                          \
        cout << "  ✗ FAILED: " << (message) << " (expected: " << (expected) << ", got: " << (actual) << ")\n"; \
        results.failed++;                                                                                      \
    }

#define ASSERT_TRUE(condition, message)              \
    if (condition)                                   \
    {                                                \
        cout << "  ✓ " << (message) << "\n";         \
        results.passed++;                            \
    }                                                \
    else                                             \
    {                                                \
        cout << "  ✗ FAILED: " << (message) << "\n"; \
        results.failed++;                            \
    }

// ============================================================================
// TESTS
// ============================================================================

void test_adapter_basic_width()
{
    TEST_CASE("Adapter - Basic width conversion");

    auto square = make_shared<Square>(50);
    SquareToRectangleAdapter adapter(square);

    ASSERT_EQ(adapter.get_width(), 50, "Width should be 50");
}

void test_adapter_basic_height()
{
    TEST_CASE("Adapter - Basic height conversion");

    auto square = make_shared<Square>(50);
    SquareToRectangleAdapter adapter(square);

    ASSERT_EQ(adapter.get_height(), 50, "Height should be 50");
}

void test_adapter_is_rectangle()
{
    TEST_CASE("Adapter - Is a Rectangle (polymorphic)");

    auto square = make_shared<Square>(40);
    shared_ptr<Rectangle> rect = make_shared<SquareToRectangleAdapter>(square);

    ASSERT_EQ(rect->get_width(), 40, "Can use as Rectangle type");
}

void test_adapter_extension_method()
{
    TEST_CASE("Adapter - Extension method works (area calculation)");

    auto square = make_shared<Square>(10);
    SquareToRectangleAdapter adapter(square);

    // get_area() is inherited from Rectangle
    int area = adapter.get_area();
    ASSERT_EQ(area, 100, "Area of 10x10 square should be 100");
}

void test_adapter_different_sizes()
{
    TEST_CASE("Adapter - Various square sizes");

    auto square1 = make_shared<Square>(5);
    auto square2 = make_shared<Square>(15);
    auto square3 = make_shared<Square>(30);

    SquareToRectangleAdapter adapter1(square1);
    SquareToRectangleAdapter adapter2(square2);
    SquareToRectangleAdapter adapter3(square3);

    ASSERT_EQ(adapter1.get_area(), 25, "5x5 area is 25");
    ASSERT_EQ(adapter2.get_area(), 225, "15x15 area is 225");
    ASSERT_EQ(adapter3.get_area(), 900, "30x30 area is 900");
}

void test_adapter_in_polymorphic_collection()
{
    TEST_CASE("Adapter - Works in polymorphic collection");

    vector<shared_ptr<Rectangle>> rectangles;

    // Add real rectangles
    rectangles.push_back(make_shared<ConcreteRectangle>(10, 20));
    rectangles.push_back(make_shared<ConcreteRectangle>(5, 15));

    // Add adapted squares
    auto square1 = make_shared<Square>(12);
    auto square2 = make_shared<Square>(8);
    rectangles.push_back(make_shared<SquareToRectangleAdapter>(square1));
    rectangles.push_back(make_shared<SquareToRectangleAdapter>(square2));

    ASSERT_EQ(rectangles.size(), 4, "Collection has 4 rectangles");

    // All can be used through Rectangle interface
    int total_area = 0;
    for (const auto &rect : rectangles)
    {
        total_area += rect->get_area();
    }

    ASSERT_EQ(total_area, 200 + 75 + 144 + 64, "Total area calculation works");
}

void test_adapter_maintains_width_height()
{
    TEST_CASE("Adapter - Width equals height (square property)");

    auto square = make_shared<Square>(25);
    SquareToRectangleAdapter adapter(square);

    ASSERT_TRUE(adapter.get_width() == adapter.get_height(),
                "Adapted square maintains equal width and height");
}

void test_adapter_does_not_modify_original()
{
    TEST_CASE("Adapter - Doesn't modify original square");

    auto square = make_shared<Square>(50);
    SquareToRectangleAdapter adapter(square);

    // Verify original square is unchanged
    ASSERT_EQ(square->get_side(), 50, "Original square unchanged");
}

void test_concrete_rectangle()
{
    TEST_CASE("Rectangle - Concrete implementation works");

    ConcreteRectangle rect(10, 20);

    ASSERT_EQ(rect.get_width(), 10, "Width is 10");
    ASSERT_EQ(rect.get_height(), 20, "Height is 20");
    ASSERT_EQ(rect.get_area(), 200, "Area is 200");
}

void test_two_adapters_are_independent()
{
    TEST_CASE("Adapter - Multiple adapters are independent");

    auto square1 = make_shared<Square>(10);
    auto square2 = make_shared<Square>(20);

    SquareToRectangleAdapter adapter1(square1);
    SquareToRectangleAdapter adapter2(square2);

    ASSERT_EQ(adapter1.get_area(), 100, "First adapter area is 100");
    ASSERT_EQ(adapter2.get_area(), 400, "Second adapter area is 400");
}

// ============================================================================
// MAIN - Example Usage
// ============================================================================

int main()
{
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║   ADAPTER CODING EXERCISE: Square to Rectangle          ║\n";
    cout << "║   Adapt Square interface to Rectangle interface          ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";

    cout << "\n═══════════════════════════════════════════════════════════════\n";
    cout << "EXERCISE OVERVIEW\n";
    cout << "═══════════════════════════════════════════════════════════════\n\n";

    cout << "GIVEN:\n";
    cout << "  1. Rectangle protocol (abstract interface)\n";
    cout << "     - get_width() - returns width\n";
    cout << "     - get_height() - returns height\n";
    cout << "     - get_area() - extension method (calculates area)\n\n";

    cout << "  2. Square class (incompatible interface)\n";
    cout << "     - get_side() - returns side length\n";
    cout << "     - set_side() - sets side length\n\n";

    cout << "TASK:\n";
    cout << "  Create SquareToRectangleAdapter that:\n";
    cout << "     1. Inherits from Rectangle interface\n";
    cout << "     2. Wraps a Square instance\n";
    cout << "     3. Implements get_width() -> returns square's side\n";
    cout << "     4. Implements get_height() -> returns square's side\n";
    cout << "     5. Automatically supports get_area() through inheritance\n\n";

    cout << "BENEFITS:\n";
    cout << "  ✓ Square can be used where Rectangle is expected\n";
    cout << "  ✓ No modification to Square class needed\n";
    cout << "  ✓ All Rectangle methods work automatically\n";
    cout << "  ✓ Clean separation of concerns\n\n";

    cout << "═══════════════════════════════════════════════════════════════\n\n";

    // ========================================================================
    // EXAMPLE 1: Basic Usage
    // ========================================================================
    cout << "EXAMPLE 1: Basic Adapter Usage\n";
    cout << "───────────────────────────────\n";
    {
        auto square = make_shared<Square>(50);
        cout << "Created Square with side = 50\n";

        SquareToRectangleAdapter adapter(square);
        cout << "Created SquareToRectangleAdapter\n\n";

        cout << "Using Rectangle interface:\n";
        cout << "  Width: " << adapter.get_width() << "\n";
        cout << "  Height: " << adapter.get_height() << "\n";
        cout << "  Area (extension): " << adapter.get_area() << "\n\n";
    }

    // ========================================================================
    // EXAMPLE 2: Polymorphic Usage
    // ========================================================================
    cout << "EXAMPLE 2: Polymorphic Collection\n";
    cout << "───────────────────────────────────\n";
    {
        vector<shared_ptr<Rectangle>> shapes;

        // Mix real rectangles and adapted squares
        shapes.push_back(make_shared<ConcreteRectangle>(100, 50));
        shapes.push_back(make_shared<SquareToRectangleAdapter>(
            make_shared<Square>(75)));
        shapes.push_back(make_shared<ConcreteRectangle>(30, 60));
        shapes.push_back(make_shared<SquareToRectangleAdapter>(
            make_shared<Square>(40)));

        cout << "Created collection of 4 shapes (2 rectangles, 2 adapted squares)\n\n";

        cout << "Shape details:\n";
        for (size_t i = 0; i < shapes.size(); ++i)
        {
            auto &shape = shapes[i];
            cout << "  Shape " << (i + 1) << ": " << shape->get_width() << "x"
                 << shape->get_height() << " (area: " << shape->get_area() << ")\n";
        }
        cout << "\n";
    }

    // ========================================================================
    // EXAMPLE 3: Comparing with/without adapter
    // ========================================================================
    cout << "EXAMPLE 3: With Adapter vs Without\n";
    cout << "────────────────────────────────────\n";
    {
        auto square = make_shared<Square>(25);

        cout << "Without adapter - can't use Square as Rectangle:\n";
        cout << "  Square has get_side() method\n";
        cout << "  Rectangle expects get_width()/get_height()\n";
        cout << "  Can't mix in polymorphic collections\n\n";

        cout << "With adapter - Square works as Rectangle:\n";
        SquareToRectangleAdapter adapter(square);
        shared_ptr<Rectangle> rect = make_shared<SquareToRectangleAdapter>(square);
        cout << "  Can call get_width(): " << rect->get_width() << "\n";
        cout << "  Can call get_height(): " << rect->get_height() << "\n";
        cout << "  Can call get_area(): " << rect->get_area() << "\n";
        cout << "  Can mix in polymorphic collections\n\n";
    }

    // ========================================================================
    // RUN ALL TESTS
    // ========================================================================
    cout << "═══════════════════════════════════════════════════════════════\n";
    cout << "RUNNING TEST SUITE\n";
    cout << "═══════════════════════════════════════════════════════════════\n";

    test_adapter_basic_width();
    test_adapter_basic_height();
    test_adapter_is_rectangle();
    test_adapter_extension_method();
    test_adapter_different_sizes();
    test_adapter_in_polymorphic_collection();
    test_adapter_maintains_width_height();
    test_adapter_does_not_modify_original();
    test_concrete_rectangle();
    test_two_adapters_are_independent();

    results.print_summary();

    cout << "═══════════════════════════════════════════════════════════════\n";
    cout << "KEY LEARNING POINTS\n";
    cout << "═══════════════════════════════════════════════════════════════\n\n";

    cout << "1. ADAPTER PATTERN STRUCTURE:\n";
    cout << "   - Implement target interface (Rectangle)\n";
    cout << "   - Wrap incompatible class (Square)\n";
    cout << "   - Translate method calls (side -> width/height)\n\n";

    cout << "2. BENEFITS OF THIS APPROACH:\n";
    cout << "   - No modification to Square class\n";
    cout << "   - Automatic support for inherited methods (area)\n";
    cout << "   - Works with polymorphism\n";
    cout << "   - Can create multiple independent adapters\n\n";

    cout << "3. WHEN TO USE:\n";
    cout << "   - Integrating incompatible libraries\n";
    cout << "   - Working with legacy code\n";
    cout << "   - Implementing common interface for diverse types\n";
    cout << "   - Avoiding circular dependencies\n\n";

    cout << "4. REAL-WORLD EXAMPLES:\n";
    cout << "   - Iterator adapters\n";
    cout << "   - File system adapters\n";
    cout << "   - Network protocol adapters\n";
    cout << "   - Database driver adapters\n\n";

    cout << "═══════════════════════════════════════════════════════════════\n\n";

    return results.failed == 0 ? 0 : 1;
}
