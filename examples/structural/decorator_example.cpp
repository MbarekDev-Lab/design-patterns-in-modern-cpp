#include "../include/structural/decorator.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// ============================================================================
// DECORATOR PATTERN EXAMPLES
// ============================================================================

void print_header(const string &title)
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << title << "\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";
}

// ============================================================================
// EXAMPLE 1: Violation - Class Explosion Without Decorator
// ============================================================================

void example1_violation()
{
    print_header("EXAMPLE 1: Violation - Class Explosion Without Decorator");

    cout << "Problem: Need a new class for every feature combination\n";
    cout << "Without Decorator Pattern:\n\n";

    using namespace decorator_violation;

    Circle plain_circle(5);
    cout << plain_circle.str() << "\n";

    ColoredCircle red_circle(5, "red");
    cout << red_circle.str() << "\n";

    TransparentCircle half_visible(5, 128);
    cout << half_visible.str() << "\n";

    ColoredTransparentCircle red_half_visible(5, "red", 128);
    cout << red_half_visible.str() << "\n";

    cout << "\nClass count: 4 for Circle variations alone!\n";
    cout << "Add Square, Rectangle... and we have EXPLOSION ✗\n";
}

// ============================================================================
// EXAMPLE 2: Wrapper/Composition Decorators
// ============================================================================

void example2_wrapper_decorators()
{
    print_header("EXAMPLE 2: Wrapper/Composition Decorators");

    cout << "Solution: Wrap shapes with decorator objects\n";
    cout << "Benefits: Reusable decorators for ANY shape\n\n";

    using namespace decorator_solution::wrapper;

    Circle circle(5);
    cout << "1. " << circle.str() << "\n";

    ColoredShape red_circle(circle, "red");
    cout << "2. " << red_circle.str() << "\n";

    TransparentShape red_half_visible(red_circle, 128);
    cout << "3. " << red_half_visible.str() << "\n";

    BorderShape bordered(red_half_visible, "█");
    cout << "4. " << bordered.str() << "\n";

    cout << "\nDecorate Square:\n";
    Square square(10);
    cout << "1. " << square.str() << "\n";

    ColoredShape blue_square(square, "blue");
    cout << "2. " << blue_square.str() << "\n";

    TransparentShape blue_transparent(blue_square, 64);
    cout << "3. " << blue_transparent.str() << "\n";

    cout << "\n✓ Same decorators, different shapes!\n";
}

// ============================================================================
// EXAMPLE 3: Mixin Inheritance Decorators
// ============================================================================

void example3_mixin_inheritance()
{
    print_header("EXAMPLE 3: Mixin Inheritance Decorators");

    cout << "Pattern: Template inheritance - T inherits from decorator\n";
    cout << "Benefit: Compile-time composition, full type information\n\n";

    using namespace decorator_solution::mixin;

    // ColoredShape2<Circle> - inherits from Circle
    ColoredShape2<Circle> green_circle("green", 7.5f);
    cout << green_circle.str() << "\n";

    // TransparentShape2<ColoredShape2<Square>> - nested decorators
    TransparentShape2<ColoredShape2<Square>> blue_invisible_square(
        25, "blue", 15.0f);
    cout << blue_invisible_square.str() << "\n";

    // PatternShape2 - another decorator
    PatternShape2<Circle> striped_circle("stripes", 4.0f);
    cout << striped_circle.str() << "\n";

    // Complex nesting
    PatternShape2<TransparentShape2<ColoredShape2<Circle>>>
        complex("dots", 50, "gold", 3.0f);
    cout << complex.str() << "\n";

    cout << "\n✓ Type-safe, no runtime overhead (all inlined)\n";
}

// ============================================================================
// Global Functions for Decoration
// ============================================================================

double add_numbers(double a, double b)
{
    cout << "        " << a << " + " << b << " = " << (a + b) << "\n";
    return a + b;
}

double multiply_numbers(double a, double b)
{
    cout << "        " << a << " * " << b << " = " << (a * b) << "\n";
    return a * b;
}

// ============================================================================
// EXAMPLE 4: Function Decorators - Logging
// ============================================================================

void example4_function_decorators()
{
    print_header("EXAMPLE 4: Function Decorators - Logging");

    cout << "Pattern: Wrap functions to add cross-cutting concerns\n";
    cout << "Example: Add logging to any function\n\n";

    using namespace decorator_solution::function_decorator;

    // Lambda decoration with Logger2 (void functions)
    auto hello_func = []()
    { cout << "    Hello from decorated function!\n"; };
    auto logged_hello = make_logger2(hello_func, "HelloFunction");

    cout << "Calling logged function:\n";
    logged_hello();

    cout << "\nDecorating another void function:\n";
    auto greet = []()
    { cout << "    Greetings from decorator!\n"; };
    auto logged_greet = make_logger2(greet, "GreetFunction");
    logged_greet();
}

// ============================================================================
// EXAMPLE 5: Function Decorators with Return Values
// ============================================================================

void example5_function_decorator_return()
{
    print_header("EXAMPLE 5: Function Decorators with Return Values");

    cout << "Pattern: Logger3 specialization for function signatures\n";
    cout << "Benefit: Handles return values and parameters properly\n\n";

    using namespace decorator_solution::function_decorator;

    auto logged_add = make_logger3(add_numbers, "Addition");

    cout << "Calling decorated add function:\n";
    double result = logged_add(5.0, 3.0);
    cout << "Result captured: " << result << "\n";

    cout << "\nDecorating multiply:\n";
    auto logged_mul = make_logger3(multiply_numbers, "Multiplication");
    double prod = logged_mul(6.0, 7.0);
    cout << "Result: " << prod << "\n";
}

// ============================================================================
// EXAMPLE 6: Complex Multi-Decorator Stacking
// ============================================================================

void example6_multi_decorator_stacking()
{
    print_header("EXAMPLE 6: Complex Multi-Decorator Stacking");

    cout << "Pattern: Chain multiple decorators in any order\n";
    cout << "Benefit: Flexible composition of behaviors\n\n";

    using namespace decorator_solution::wrapper;

    Rectangle rect(8, 5);
    cout << "Base:   " << rect.str() << "\n";

    ColoredShape colored(rect, "purple");
    cout << "Color:  " << colored.str() << "\n";

    TransparentShape transparent(colored, 192);
    cout << "Trans:  " << transparent.str() << "\n";

    BorderShape bordered(transparent, "═");
    cout << "Border: " << bordered.str() << "\n";

    cout << "\nAlternative order:\n";
    BorderShape b1(rect, "▓");
    cout << "1st:    " << b1.str() << "\n";

    TransparentShape t1(b1, 100);
    cout << "2nd:    " << t1.str() << "\n";

    ColoredShape c1(t1, "gold");
    cout << "3rd:    " << c1.str() << "\n";

    cout << "\n✓ Same decorators, different order, different results!\n";
}

// ============================================================================
// EXAMPLE 7: Real-World - UI Component Decoration
// ============================================================================

void example7_ui_components()
{
    print_header("EXAMPLE 7: Real-World - UI Component Decoration");

    cout << "Pattern: Decorate UI components with visual effects\n";
    cout << "Example: Button with border, color, shadow\n\n";

    using namespace decorator_solution::wrapper;

    // Model a button as a shape
    struct Button : Shape
    {
        string text;
        Button(const string &text) : text(text) {}
        string str() const override
        {
            return "Button[" + text + "]";
        }
    };

    Button submit_btn("Submit");
    cout << "Plain:      " << submit_btn.str() << "\n";

    ColoredShape blue_btn(submit_btn, "blue");
    cout << "Colored:    " << blue_btn.str() << "\n";

    TransparentShape hover_effect(blue_btn, 220);
    cout << "Hover:      " << hover_effect.str() << "\n";

    BorderShape bordered_btn(hover_effect, "╔═╗");
    cout << "Bordered:   " << bordered_btn.str() << "\n";

    cout << "\nEach decorator adds one responsibility:\n";
    cout << "- ColoredShape adds color\n";
    cout << "- TransparentShape adds opacity\n";
    cout << "- BorderShape adds border\n";
}

// ============================================================================
// EXAMPLE 8: Decorator vs Composition vs Inheritance
// ============================================================================

void example8_pattern_comparison()
{
    print_header("EXAMPLE 8: Decorator vs Alternatives");

    cout << "Comparison of approaches:\n\n";

    cout << "1. INHERITANCE (Bad for this case):\n";
    cout << "   class RedCircle : public Circle { }\n";
    cout << "   class FramedRedCircle : public RedCircle { }\n";
    cout << "   problem: creates class hierarchy explosion\n\n";

    cout << "2. COMPOSITION (Without Decorator):\n";
    cout << "   class Circle { Shape &inner; }\n";
    cout << "   problem: requires defining all combinations\n\n";

    cout << "3. DECORATOR (Best for this case):\n";
    cout << "   class ColoredShape : public Shape {\n";
    cout << "       Shape &shape;\n";
    cout << "   }\n";
    cout << "   benefit: combine any decorators with any shape\n\n";

    cout << "Decorator Advantages:\n";
    cout << "✓ Open/Closed Principle - add features without modifying existing\n";
    cout << "✓ Single Responsibility - each decorator handles one aspect\n";
    cout << "✓ Runtime flexibility - decorators added/removed at runtime\n";
    cout << "✓ No combinatorial explosion - N shapes + M decorators = N+M classes\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║              DECORATOR PATTERN EXAMPLES                   ║\n";
    cout << "║  Attach additional responsibilities dynamically           ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    cout << "\n┌───────────────────────────────────────────────────────────┐\n";
    cout << "│ PATTERN OVERVIEW                                          │\n";
    cout << "└───────────────────────────────────────────────────────────┘\n\n";

    cout << "Intent:\n";
    cout << "  Attach additional responsibilities to an object dynamically.\n";
    cout << "  Provide a flexible alternative to subclassing for extending\n";
    cout << "  functionality.\n\n";

    cout << "Key Concepts:\n";
    cout << "  1. Component - The object receiving functionality\n";
    cout << "  2. Decorator - Wraps the component, adding behavior\n";
    cout << "  3. Dynamic - Composition at runtime, not compile-time\n";
    cout << "  4. Transparent - Decorator and component share interface\n\n";

    cout << "Benefits:\n";
    cout << "  ✓ Single Responsibility Principle\n";
    cout << "  ✓ Open/Closed Principle (open for extension, closed for mod)\n";
    cout << "  ✓ Flexible combination of behaviors\n";
    cout << "  ✓ Avoid class explosion (N+M vs N*M)\n";
    cout << "  ✓ Runtime flexibility\n\n";

    cout << "Three Main Approaches:\n";
    cout << "  1. Wrapper/Composition - holds references to components\n";
    cout << "  2. Mixin Inheritance - template inheritance patterns\n";
    cout << "  3. Function Decorators - wraps callables and functions\n\n";

    // Run examples
    example1_violation();
    example2_wrapper_decorators();
    example3_mixin_inheritance();
    example4_function_decorators();
    example5_function_decorator_return();
    example6_multi_decorator_stacking();
    example7_ui_components();
    example8_pattern_comparison();

    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "All examples completed successfully!\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    return 0;
}
