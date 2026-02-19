#include "../include/structural/decorator.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <cassert>

using namespace std;

// ============================================================================
// Custom Assertion Framework
// ============================================================================

int assertions_passed = 0;
int assertions_failed = 0;

#define ASSERT_EQ(actual, expected) \
    do                              \
    {                               \
        if ((actual) == (expected)) \
        {                           \
            cout << "✓";            \
            assertions_passed++;    \
        }                           \
        else                        \
        {                           \
            cout << "✗";            \
            assertions_failed++;    \
        }                           \
    } while (0)

#define ASSERT_TRUE(condition) ASSERT_EQ((condition), true)

#define ASSERT_FALSE(condition) ASSERT_EQ((condition), false)

#define ASSERT_STRING_CONTAINS(str, substr)     \
    do                                          \
    {                                           \
        if ((str).find(substr) != string::npos) \
        {                                       \
            cout << "✓";                        \
            assertions_passed++;                \
        }                                       \
        else                                    \
        {                                       \
            cout << "✗";                        \
            assertions_failed++;                \
        }                                       \
    } while (0)

// ============================================================================
// TEST CASES
// ============================================================================

void test_wrapper_basic_decoration()
{
    using namespace decorator_solution::wrapper;

    // Test 1: Plain circle
    Circle c(5.0f);
    ASSERT_STRING_CONTAINS(c.str(), "radius 5");

    // Test 2: Colored circle
    ColoredShape red_c(c, "red");
    ASSERT_STRING_CONTAINS(red_c.str(), "red");
    ASSERT_STRING_CONTAINS(red_c.str(), "radius 5");

    // Test 3: Multiple decorators
    TransparentShape transparent_red(red_c, 128);
    ASSERT_STRING_CONTAINS(transparent_red.str(), "red");
    ASSERT_STRING_CONTAINS(transparent_red.str(), "transparency");

    cout << " Basic decoration";
}

void test_wrapper_different_shapes()
{
    using namespace decorator_solution::wrapper;

    Circle circle(3.0f);
    Square square(4.0f);
    Rectangle rect(5.0f, 6.0f);

    // Same decorator, different shapes
    ColoredShape colored_circle(circle, "blue");
    ColoredShape colored_square(square, "green");
    ColoredShape colored_rect(rect, "yellow");

    ASSERT_STRING_CONTAINS(colored_circle.str(), "circle");
    ASSERT_STRING_CONTAINS(colored_circle.str(), "blue");

    ASSERT_STRING_CONTAINS(colored_square.str(), "square");
    ASSERT_STRING_CONTAINS(colored_square.str(), "green");

    ASSERT_STRING_CONTAINS(colored_rect.str(), "rectangle");
    ASSERT_STRING_CONTAINS(colored_rect.str(), "yellow");

    cout << " Different shapes decoration";
}

void test_wrapper_decoration_order()
{
    using namespace decorator_solution::wrapper;

    Circle c(7.0f);

    // Order 1: Color then Transparent
    ColoredShape colored(c, "purple");
    TransparentShape transparent1(colored, 100);
    string result1 = transparent1.str();

    // Order 2: Transparent then Color (won't work the same way, but test both)
    Circle c2(7.0f);
    TransparentShape transparent2(c2, 100);
    ColoredShape colored2(transparent2, "purple");
    string result2 = colored2.str();

    // Both should have radius and colors/transparency info
    ASSERT_STRING_CONTAINS(result1, "radius");
    ASSERT_STRING_CONTAINS(result2, "radius");

    cout << " Decoration order flexibility";
}

void test_wrapper_border_decorator()
{
    using namespace decorator_solution::wrapper;

    Square s(5.0f);
    BorderShape bordered(s, "*");
    string result = bordered.str();

    ASSERT_STRING_CONTAINS(result, "*");
    ASSERT_STRING_CONTAINS(result, "square");

    cout << " Border decorator";
}

void test_mixin_basic()
{
    using namespace decorator_solution::mixin;

    ColoredShape2<Circle> colored_circle("red", 5.0f);
    ASSERT_STRING_CONTAINS(colored_circle.str(), "red");
    ASSERT_STRING_CONTAINS(colored_circle.str(), "5");

    cout << " Mixin basic decoration";
}

void test_mixin_nested()
{
    using namespace decorator_solution::mixin;

    // TransparentShape2<ColoredShape2<Square>>
    TransparentShape2<ColoredShape2<Square>> decorated(128, "blue", 10.0f);
    string result = decorated.str();

    ASSERT_STRING_CONTAINS(result, "blue");
    ASSERT_STRING_CONTAINS(result, "square");
    ASSERT_STRING_CONTAINS(result, "transparency");

    cout << " Mixin nested decorators";
}

void test_mixin_triple_nested()
{
    using namespace decorator_solution::mixin;

    // PatternShape2<TransparentShape2<ColoredShape2<Circle>>>
    PatternShape2<TransparentShape2<ColoredShape2<Circle>>> decorated(
        "dots", 50, "gold", 3.0f);

    string result = decorated.str();
    ASSERT_STRING_CONTAINS(result, "gold");
    ASSERT_STRING_CONTAINS(result, "dots");
    ASSERT_STRING_CONTAINS(result, "transparency");
    ASSERT_STRING_CONTAINS(result, "circle");

    cout << " Mixin triple nesting";
}

void test_mixin_pattern_decorator()
{
    using namespace decorator_solution::mixin;

    PatternShape2<Circle> striped_circle("stripes", 4.0f);
    ASSERT_STRING_CONTAINS(striped_circle.str(), "stripes");
    ASSERT_STRING_CONTAINS(striped_circle.str(), "circle");

    cout << " Mixin pattern decorator";
}

void test_function_logger_basic()
{
    using namespace decorator_solution::function_decorator;

    // Test that function logger can be created and called
    auto func = []() { /* silent call */ };
    auto logged = make_logger2(func, "TestFunc");

    // Just verify it compiles and can be called without errors
    logged();
    ASSERT_TRUE(true);

    cout << " Function logger basic";
}

void test_function_logger3_signature()
{
    using namespace decorator_solution::function_decorator;

    // Logger2 works with void() functions
    auto void_func = []() { /* silent call */ };

    auto logged_func = make_logger2(void_func, "TestFunc");

    // Just verify it can be called without errors
    logged_func();
    ASSERT_TRUE(true);

    cout << " Function logger with void signature";
}

void test_violation_minimal_classes()
{
    using namespace decorator_violation;

    // Without decorator: need different classes
    Circle plain(5);
    ColoredCircle colored(5, "red");
    TransparentCircle transparent(5, 128);
    ColoredTransparentCircle both(5, "red", 128);

    ASSERT_STRING_CONTAINS(plain.str(), "radius");
    ASSERT_STRING_CONTAINS(colored.str(), "red");
    ASSERT_STRING_CONTAINS(transparent.str(), "transparency");
    ASSERT_STRING_CONTAINS(both.str(), "red");

    cout << " Violation pattern shows class explosion";
}

void test_wrapper_transparency_calculation()
{
    using namespace decorator_solution::wrapper;

    Circle c(1.0f);
    TransparentShape t(c, 255); // 100% opacity
    string result = t.str();

    // Should contain "100%" for full opacity
    ASSERT_STRING_CONTAINS(result, "100");

    Circle c2(1.0f);
    TransparentShape t2(c2, 0); // 0% opacity (transparent)
    string result2 = t2.str();
    ASSERT_STRING_CONTAINS(result2, "0");

    cout << " Transparency calculation";
}

void test_wrapper_chaining()
{
    using namespace decorator_solution::wrapper;

    Circle original(2.0f);

    ColoredShape step1(original, "cyan");
    TransparentShape step2(step1, 192);
    BorderShape step3(step2, "#");

    string final_result = step3.str();

    ASSERT_STRING_CONTAINS(final_result, "cyan");
    ASSERT_STRING_CONTAINS(final_result, "transparency");
    ASSERT_STRING_CONTAINS(final_result, "#");

    cout << " Decorator chaining";
}

void test_wrapper_rectangle()
{
    using namespace decorator_solution::wrapper;

    Rectangle rect(10.0f, 20.0f);
    ASSERT_STRING_CONTAINS(rect.str(), "10x20");

    ColoredShape colored(rect, "orange");
    ASSERT_STRING_CONTAINS(colored.str(), "10x20");
    ASSERT_STRING_CONTAINS(colored.str(), "orange");

    cout << " Rectangle decoration";
}

void test_multiple_instances_independent()
{
    using namespace decorator_solution::wrapper;

    Circle c1(5.0f);
    Circle c2(10.0f);

    ColoredShape red_c1(c1, "red");
    ColoredShape blue_c2(c2, "blue");

    ASSERT_STRING_CONTAINS(red_c1.str(), "red");
    ASSERT_STRING_CONTAINS(red_c1.str(), "5");
    ASSERT_STRING_CONTAINS(blue_c2.str(), "blue");
    ASSERT_STRING_CONTAINS(blue_c2.str(), "10");

    cout << " Multiple instances independence";
}

void test_mixin_circle_access()
{
    using namespace decorator_solution::mixin;

    ColoredShape2<Circle> c("red", 5.0f);
    // Should be able to access Circle properties through inheritance
    c.radius = 7.0f;
    ASSERT_EQ(c.radius, 7.0f);

    cout << " Mixin property access";
}

void test_mixin_square_access()
{
    using namespace decorator_solution::mixin;

    ColoredShape2<Square> s("blue", 8.0f);
    s.side = 12.0f;
    ASSERT_EQ(s.side, 12.0f);

    cout << " Mixin square property access";
}

void test_decorator_benefits_summary()
{
    using namespace decorator_solution::wrapper;

    // Test that decorator pattern provides benefits

    // 1. No class explosion
    // With decorator: 2 shapes + 3 decorators = 5 classes
    // With inheritance: 2 * 3! = way more classes needed

    Circle c1(1);
    Square c2(2);
    Rectangle c3(3, 4);

    ColoredShape d1(c1, "a");
    TransparentShape d2(d1, 1);
    BorderShape d3(d2, "x");

    ASSERT_STRING_CONTAINS(d3.str(), "circle");
    ASSERT_STRING_CONTAINS(d3.str(), "a");
    ASSERT_STRING_CONTAINS(d3.str(), "transparency");
    ASSERT_STRING_CONTAINS(d3.str(), "x");

    cout << " Decorator benefits proof";
}

void test_decorator_composition_flexibility()
{
    using namespace decorator_solution::wrapper;

    // Can compose in any order
    Circle original(3.0f);

    // Path 1: Color -> Transparent -> Border
    ColoredShape path1_step1(original, "red");
    TransparentShape path1_step2(path1_step1, 100);
    BorderShape path1_final(path1_step2, "*");

    ASSERT_STRING_CONTAINS(path1_final.str(), "red");
    ASSERT_STRING_CONTAINS(path1_final.str(), "transparency");
    ASSERT_STRING_CONTAINS(path1_final.str(), "*");

    // Path 2: Different order
    Circle original2(3.0f);
    BorderShape path2_step1(original2, "*");
    ColoredShape path2_step2(path2_step1, "red");
    TransparentShape path2_final(path2_step2, 100);

    ASSERT_STRING_CONTAINS(path2_final.str(), "red");
    ASSERT_STRING_CONTAINS(path2_final.str(), "*");

    cout << " Composition flexibility";
}

// ============================================================================
// FLOWER DECORATOR EXERCISE - Smart Color Deduplication
// ============================================================================

namespace flower_exercise
{

    // Base component
    struct Flower
    {
        virtual string str() = 0;
        virtual ~Flower() = default;
    };

    // Concrete component
    struct Rose : Flower
    {
        string str() override
        {
            return "A rose";
        }
    };

    // Color decorator - avoids duplicating colors, handles "and" properly
    struct RedFlower : Flower
    {
        Flower &flower;

        RedFlower(Flower &flower) : flower(flower) {}

        string str() override
        {
            string s = flower.str();

            // Already red - don't duplicate
            if (s.find("red") != string::npos)
                return s;

            // Blue exists, add "and red"
            if (s.find("blue") != string::npos)
                return s + " and red";

            // First color - use "that is"
            return s + " that is red";
        }
    };

    // Another color decorator
    struct BlueFlower : Flower
    {
        Flower &flower;

        BlueFlower(Flower &flower) : flower(flower) {}

        string str() override
        {
            string s = flower.str();

            // Already blue - don't duplicate
            if (s.find("blue") != string::npos)
                return s;

            // Red exists, add "and blue"
            if (s.find("red") != string::npos)
                return s + " and blue";

            // First color - use "that is"
            return s + " that is blue";
        }
    };

} // namespace flower_exercise

void test_flower_decorator_exercise()
{
    using namespace flower_exercise;

    // Test case 1: Plain rose
    Rose rose;
    ASSERT_EQ(rose.str(), "A rose");

    // Test case 2: Red flower
    RedFlower red_rose(rose);
    ASSERT_EQ(red_rose.str(), "A rose that is red");

    // Test case 3: Double red (no duplication)
    RedFlower double_red(red_rose);
    ASSERT_EQ(double_red.str(), "A rose that is red");

    // Test case 4: Red then blue (compound color)
    BlueFlower blue_red(red_rose);
    ASSERT_EQ(blue_red.str(), "A rose that is red and blue");

    // Test case 5: Blue then red (opposite order)
    Rose rose2;
    BlueFlower blue_rose(rose2);
    RedFlower red_blue(blue_rose);
    ASSERT_EQ(red_blue.str(), "A rose that is blue and red");

    // Test case 6: Double blue (no duplication)
    BlueFlower double_blue(blue_rose);
    ASSERT_EQ(double_blue.str(), "A rose that is blue");

    // Test case 7: Complex nesting - Red, Blue, Red
    Rose rose3;
    RedFlower step1(rose3);
    BlueFlower step2(step1);
    RedFlower step3(step2);
    // Should be "A rose that is red and blue" (red doesn't duplicate)
    ASSERT_EQ(step3.str(), "A rose that is red and blue");

    // Test case 8: Triple nesting with mixed colors
    Rose rose4;
    BlueFlower b1(rose4); // "A rose that is blue"
    RedFlower r1(b1);     // "A rose that is blue and red"
    BlueFlower b2(r1);    // "A rose that is blue and red" (blue no dup)
    ASSERT_EQ(b2.str(), "A rose that is blue and red");

    cout << " Flower decorator exercise";
}

// ============================================================================
// TEST RUNNER
// ============================================================================

int main()
{
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║          DECORATOR PATTERN - TEST SUITE                   ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n\n";

    cout << "Running tests...\n\n";

    test_wrapper_basic_decoration();
    cout << "\n";
    test_wrapper_different_shapes();
    cout << "\n";
    test_wrapper_decoration_order();
    cout << "\n";
    test_wrapper_border_decorator();
    cout << "\n";
    test_mixin_basic();
    cout << "\n";
    test_mixin_nested();
    cout << "\n";
    test_mixin_triple_nested();
    cout << "\n";
    test_mixin_pattern_decorator();
    cout << "\n";
    test_function_logger_basic();
    cout << "\n";
    test_function_logger3_signature();
    cout << "\n";
    test_violation_minimal_classes();
    cout << "\n";
    test_wrapper_transparency_calculation();
    cout << "\n";
    test_wrapper_chaining();
    cout << "\n";
    test_wrapper_rectangle();
    cout << "\n";
    test_multiple_instances_independent();
    cout << "\n";
    test_mixin_circle_access();
    cout << "\n";
    test_mixin_square_access();
    cout << "\n";
    test_decorator_benefits_summary();
    cout << "\n";
    test_decorator_composition_flexibility();
    cout << "\n";
    test_flower_decorator_exercise();
    cout << "\n\n";

    // Summary
    cout << "═══════════════════════════════════════════════════════════\n";
    cout << "TEST SUMMARY\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "┌───────────────────────────────────────────────────────────┐\n";
    cout << "│ Results                                                   │\n";
    cout << "└───────────────────────────────────────────────────────────┘\n\n";

    cout << "│ Passed:       " << assertions_passed << " ✓\n";
    cout << "│ Failed:       " << assertions_failed << " ✗\n";

    if (assertions_failed == 0)
    {
        cout << "\n│ Status: ALL TESTS PASSED ✓\n";
    }
    else
    {
        cout << "\n│ Status: SOME TESTS FAILED ✗\n";
    }

    cout << "└───────────────────────────────────────────────────────────┘\n\n";

    cout << "KEY LEARNING POINTS\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "1. DECORATOR PATTERN STRUCTURE:\n";
    cout << "   - Component: Object receiving functionality\n";
    cout << "   - Decorator: Wrapper adding behavior\n";
    cout << "   - Both implement same interface\n";
    cout << "   - Decorator holds reference to component\n\n";

    cout << "2. THREE MAIN APPROACHES:\n";
    cout << "   - Wrapper/Composition: Dynamic, runtime flexibility\n";
    cout << "   - Mixin Inheritance: Compile-time, type-safe\n";
    cout << "   - Function Decorators: Cross-cutting concerns\n\n";

    cout << "3. WHEN TO USE:\n";
    cout << "   - Need to add features without modifying class\n";
    cout << "   - Want to combine features flexibly\n";
    cout << "   - Avoid class explosion (ColoredRedBorderedSquare)\n";
    cout << "   - Apply cross-cutting concerns (logging, timing)\n\n";

    cout << "4. ADVANTAGES OVER INHERITANCE:\n";
    cout << "   - No class hierarchy explosion\n";
    cout << "   - Single Responsibility Principle\n";
    cout << "   - Open/Closed Principle\n";
    cout << "   - Flexible runtime composition\n\n";

    cout << "═══════════════════════════════════════════════════════════\n\n";

    return 0;
}
