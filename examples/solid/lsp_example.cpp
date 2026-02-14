#include "solid/lsp.h"
#include <iostream>

using namespace lsp;

int main()
{
    std::cout << "=== Liskov Substitution Principle (LSP) ===\n\n";

    // ============ VIOLATION: The Problem ============
    std::cout << "VIOLATION - Square inheriting from Rectangle:\n";
    std::cout << "--------------------------------------\n";
    {
        RectangleBad r{5, 5};
        std::cout << "Testing Rectangle(5, 5):\n";
        process_rectangle_bad(r);

        std::cout << "\nTesting SquareBad(5):\n";
        SquareBad s{5};
        process_rectangle_bad(s);

        std::cout << "\nProblem Explanation:\n";
        std::cout << "  1. process_rectangle_bad() expects independent width/height\n";
        std::cout << "  2. When set_height(10) is called, it should ONLY change height\n";
        std::cout << "  3. But SquareBad also changes width! ❌\n";
        std::cout << "  4. This violates LSP - SquareBad is NOT a proper Rectangle\n";
        std::cout << "  5. We cannot safely substitute SquareBad for Rectangle\n\n";
    }

    // ============ SOLUTION: The Correct Way ============
    std::cout << "SOLUTION - Proper abstraction with abstract Shape:\n";
    std::cout << "--------------------------------------\n";
    {
        std::cout << "Rectangle(5, 5):\n";
        Rectangle rect{5, 5};
        rect.set_width(5);
        rect.set_height(10);
        std::cout << "  Area: " << rect.area() << " (Expected: 50) ✓\n";

        std::cout << "\nSquare(5):\n";
        Square square{5};
        square.set_side(5);
        std::cout << "  Area: " << square.area() << " (Expected: 25) ✓\n";

        std::cout << "\nBenefit:\n";
        std::cout << "  1. Square and Rectangle have separate interfaces\n";
        std::cout << "  2. Each class respects its own contract\n";
        std::cout << "  3. Generic Shape* can work with any shape\n";
        std::cout << "  4. LSP is respected - each subclass can be substituted\n\n";
    }

    // ============ Polymorphic Usage ============
    std::cout << "Polymorphic Usage with abstract Shape:\n";
    std::cout << "--------------------------------------\n";
    {
        Rectangle rect{5, 10};
        Square square{7};

        std::cout << "Created shapes:\n";
        print_shape_area(rect);
        print_shape_area(square);

        std::cout << "\nKey Point:\n";
        std::cout << "  1. Both Rectangle and Square are Shapes\n";
        std::cout << "  2. print_shape_area takes const Shape&\n";
        std::cout << "  3. No issues with substitutability\n";
        std::cout << "  4. Each class maintains its invariants ✓\n\n";
    }

    // ============ Why LSP Matters ============
    std::cout << "=== Key Insights ===\n";
    std::cout << "LSP VIOLATION (Square extends Rectangle):\n";
    std::cout << "  - Breaks user expectations\n";
    std::cout << "  - Cannot substitute Square for Rectangle safely\n";
    std::cout << "  - Hidden side effects (set_width changes height)\n";
    std::cout << "  - Code using Rectangle may behave incorrectly with Square\n\n";

    std::cout << "LSP COMPLIANCE (Abstract Shape):\n";
    std::cout << "  - Each class has clear, predictable behavior\n";
    std::cout << "  - Subclasses can be substituted safely\n";
    std::cout << "  - No hidden side effects\n";
    std::cout << "  - Intent is clear through class design\n";

    return 0;
}
