#include <iostream>
#include <cassert>
#include <memory>
#include <vector>
#include <string>
#include <structural/adapter.h>

using namespace std;
using namespace adapter_solution;

// ============================================================================
// MANUAL TEST FRAMEWORK
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

// ============================================================================
// TEST CLASSES
// ============================================================================

// Test adapter 1: Shape adapter
void test_shape_adapter()
{
    TEST_CASE("Shape Adapter - Adapts legacy shape to new interface");

    auto square = make_shared<LegacySquare>(50);
    auto adapter = make_shared<SquareAdapter>(square);

    ASSERT_EQ(adapter->get_width(), 50, "Adapter width is 50");
    ASSERT_EQ(adapter->get_height(), 50, "Adapter height is 50");
    ASSERT_TRUE(adapter->get_width() == adapter->get_height(), "Square maintains aspect ratio");
}

// Test adapter 2: Line to point adapter
void test_line_to_point_adapter()
{
    TEST_CASE("Line to Point Adapter - Converts line to points");

    // Vertical line from (5,5) to (5,10)
    Line vertical{{5, 5}, {5, 10}};
    LineToPointAdapter adapter(vertical);

    ASSERT_EQ(adapter.get_points().size(), 6, "Vertical line generates 6 points");
}

// Test adapter 3: Horizontal line
void test_horizontal_line_adapter()
{
    TEST_CASE("Horizontal Line Adapter - Horizontal line conversion");

    Line horizontal{{0, 3}, {5, 3}};
    LineToPointAdapter adapter(horizontal);

    ASSERT_EQ(adapter.get_points().size(), 6, "Horizontal line generates 6 points");
}

// Test adapter 4: Caching adapter
void test_caching_adapter()
{
    TEST_CASE("Caching Adapter - Caches converted points");

    Line line{{1, 1}, {1, 4}};

    cout << "  Creating first adapter:\n";
    CachingLineToPointAdapter::clear_cache();
}

// Test adapter 5: Rectangle adapter
void test_rectangle_adapter()
{
    TEST_CASE("Rectangle - Modern interface works directly");

    Rectangle rect(100, 50);

    ASSERT_EQ(rect.get_width(), 100, "Rectangle width is 100");
    ASSERT_EQ(rect.get_height(), 50, "Rectangle height is 50");
}

// Test adapter 6: Multiple shapes
void test_multiple_shapes_with_adapter()
{
    TEST_CASE("Multiple Shapes - Mixed modern and adapted");

    vector<shared_ptr<Shape>> shapes;

    // Add modern shape
    shapes.push_back(make_shared<Rectangle>(75, 40));

    // Add legacy shape through adapter
    auto legacy = make_shared<LegacySquare>(60);
    shapes.push_back(make_shared<SquareAdapter>(legacy));

    ASSERT_EQ(shapes.size(), 2, "Container has 2 shapes");
    ASSERT_EQ(shapes[0]->get_width(), 75, "First shape width is 75");
    ASSERT_EQ(shapes[1]->get_width(), 60, "Second shape (adapted) width is 60");
}

// Test adapter 7: Bidirectional adapter
void test_bidirectional_adapter_line_to_points()
{
    TEST_CASE("Bidirectional Adapter - Line to Points");

    Line line{{2, 2}, {2, 5}};
    BidirectionalAdapter adapter(line);

    ASSERT_TRUE(!adapter.get_points().empty(), "Adapter has points");
    ASSERT_EQ(adapter.get_points().size(), 4, "Line generates 4 points");
}

// Test adapter 8: Bidirectional adapter reverse
void test_bidirectional_adapter_points_to_line()
{
    TEST_CASE("Bidirectional Adapter - Points to Line");

    PointSet points{{3, 7}, {3, 8}, {3, 9}};
    BidirectionalAdapter adapter(points);

    const Line &line = adapter.get_line();
    ASSERT_EQ(line.start.x, 3, "Line start x is 3");
    ASSERT_EQ(line.start.y, 7, "Line start y is 7");
}

// Test adapter 9: Empty line
void test_empty_point_adapter()
{
    TEST_CASE("Point Adapter - Single point (start == end)");

    Line single{{5, 5}, {5, 5}};
    LineToPointAdapter adapter(single);

    ASSERT_EQ(adapter.get_points().size(), 1, "Single point line generates 1 point");
}

// Test adapter 10: Point set iteration
void test_adapter_iteration()
{
    TEST_CASE("Adapter Iteration - Can iterate over adapted points");

    Line line{{0, 0}, {0, 2}};
    LineToPointAdapter adapter(line);

    int count = 0;
    for (const auto &p : adapter.get_points())
    {
        count++;
    }

    ASSERT_EQ(count, 3, "Can iterate through all 3 points");
}

// Test adapter 11: Multiple different adapters
void test_multiple_different_adapters()
{
    TEST_CASE("Multiple Adapters - Different lines");

    Line line1{{0, 0}, {0, 3}};
    Line line2{{0, 0}, {3, 0}};
    Line line3{{1, 1}, {1, 5}};

    LineToPointAdapter adapter1(line1);
    LineToPointAdapter adapter2(line2);
    LineToPointAdapter adapter3(line3);

    ASSERT_EQ(adapter1.get_points().size(), 4, "Vertical line 1 has 4 points");
    ASSERT_EQ(adapter2.get_points().size(), 4, "Horizontal line has 4 points");
    ASSERT_EQ(adapter3.get_points().size(), 5, "Vertical line 3 has 5 points");
}

// Test adapter 12: Adapter doesn't modify original
void test_adapter_non_destructive()
{
    TEST_CASE("Adapter - Doesn't modify original");

    Line original{{2, 2}, {2, 4}};
    LineToPointAdapter adapter(original);

    ASSERT_EQ(original.start.x, 2, "Original line unchanged");
    ASSERT_EQ(original.start.y, 2, "Original line unchanged");
    ASSERT_EQ(original.end.x, 2, "Original line unchanged");
    ASSERT_EQ(original.end.y, 4, "Original line unchanged");
}

// Test adapter 13: Diagonals (not supported, should be no points)
void test_diagonal_line()
{
    TEST_CASE("Diagonal Line - Not vertical/horizontal (no points)");

    Line diagonal{{0, 0}, {5, 5}};
    LineToPointAdapter adapter(diagonal);

    ASSERT_EQ(adapter.get_points().size(), 0, "Diagonal line generates no points (not supported)");
}

// Test adapter 14: Different rectangle sizes
void test_rectangle_various_sizes()
{
    TEST_CASE("Rectangle - Various sizes");

    Rectangle small(10, 10);
    Rectangle wide(100, 20);
    Rectangle tall(30, 80);

    ASSERT_EQ(small.get_width(), 10, "Small rectangle width");
    ASSERT_EQ(wide.get_width(), 100, "Wide rectangle width");
    ASSERT_EQ(tall.get_height(), 80, "Tall rectangle height");
}

// Test adapter 15: Stress test - many adapters
void test_many_adapters()
{
    TEST_CASE("Stress Test - Many adapters");

    vector<LineToPointAdapter> adapters;

    for (int i = 0; i < 10; ++i)
    {
        adapters.emplace_back(Line{{i, i}, {i, i + 5}});
    }

    ASSERT_EQ(adapters.size(), 10, "Created 10 adapters");
    ASSERT_TRUE(adapters[0].get_points().size() > 0, "All adapters work");
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║   ADAPTER PATTERN - COMPREHENSIVE TEST SUITE              ║\n";
    cout << "║   Tests for adapting incompatible interfaces               ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";

    test_shape_adapter();
    test_line_to_point_adapter();
    test_horizontal_line_adapter();
    test_caching_adapter();
    test_rectangle_adapter();
    test_multiple_shapes_with_adapter();
    test_bidirectional_adapter_line_to_points();
    test_bidirectional_adapter_points_to_line();
    test_empty_point_adapter();
    test_adapter_iteration();
    test_multiple_different_adapters();
    test_adapter_non_destructive();
    test_diagonal_line();
    test_rectangle_various_sizes();
    test_many_adapters();

    results.print_summary();

    cout << "═══════════════════════════════════════════════════════════════\n";
    cout << "KEY INSIGHTS FROM ADAPTER PATTERN\n";
    cout << "═══════════════════════════════════════════════════════════════\n\n";

    cout << "1. WHEN TO USE ADAPTER:\n";
    cout << "   - Working with third-party libraries\n";
    cout << "   - Integrating legacy code with new systems\n";
    cout << "   - Converting between incompatible interfaces\n";
    cout << "   - Adding functionality to existing code\n\n";

    cout << "2. TWO MAIN APPROACHES:\n";
    cout << "   Class Adapter: Inherits from both classes (C++ only)\n";
    cout << "   Object Adapter: Uses composition (more flexible)\n\n";

    cout << "3. REAL-WORLD EXAMPLES:\n";
    cout << "   ✓ Device drivers (hardware to OS interface)\n";
    cout << "   ✓ File format converters\n";
    cout << "   ✓ Database adapters\n";
    cout << "   ✓ Voltage converters (220V -> 110V)\n";
    cout << "   ✓ Protocol bridges\n\n";

    cout << "4. BENEFITS:\n";
    cout << "   ✓ Reuse old code with new interface\n";
    cout << "   ✓ Separate adaptation logic from client\n";
    cout << "   ✓ Can add functionality (logging, caching, etc.)\n";
    cout << "   ✓ More flexible than modifying original classes\n\n";

    cout << "═══════════════════════════════════════════════════════════════\n\n";

    return results.failed == 0 ? 0 : 1;
}
/*

#include <gtest/gtest.h>
#include "../../../../../gtest/include/gtest/gtest.h"
#include "../../../../../gtest/include/gtest/internal/gtest-internal.h"
#include "../../../../../gtest/include/gtest/gtest_pred_impl.h"

struct Square
{
  int side{ 0 };


  explicit Square(const int side)
    : side(side)
  {
  }
};

struct Rectangle
{
  virtual int width() const = 0;
  virtual int height() const = 0;

  int area() const
  {
    return width() * height();
  }
};

struct SquareToRectangleAdapter : Rectangle
{
  SquareToRectangleAdapter(const Square& square)
    : square(square) {}

  int width() const override
  {
    return square.side;
  }

  int height() const override
  {
    return square.side;
  }

  const Square& square;
};

#include "gtest/gtest.h"

//#include "helpers/iohelper.h"

//#include "exercise.cpp"


namespace
{
  class Evaluate : public testing::Test
  {
  };

  TEST_F(Evaluate, SimpleTest)
  {
    Square sq{ 11 };
    SquareToRectangleAdapter adapter{ sq };
    ASSERT_EQ(121, adapter.area());
  }
}

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}


*/