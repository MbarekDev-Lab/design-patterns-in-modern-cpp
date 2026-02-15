#include "creational/builder.h"
#include <iostream>

using namespace builder_pattern;

int main()
{
    std::cout << "=== Builder Pattern (Creational) ===\n\n";

    // ============ VIOLATION: Manual Construction ============
    std::cout << "VIOLATION - Manual HTML Construction:\n";
    std::cout << "--------------------------------------\n";
    {
        std::cout << "Without Builder Pattern - verbose and error-prone:\n\n";
        std::cout << build_html_manual();

        std::cout << "Problems:\n";
        std::cout << "  1. String concatenation is verbose\n";
        std::cout << "  2. Easy to forget closing tags\n";
        std::cout << "  3. Hard to read and maintain\n";
        std::cout << "  4. No structure or validation\n";
        std::cout << "  5. Difficult to build complex structures\n\n";
    }

    // ============ SOLUTION: Builder Pattern ============
    std::cout << "SOLUTION - Builder Pattern:\n";
    std::cout << "--------------------------------------\n";

    // Simple list builder
    std::cout << "Example 1: Simple fluent interface (reference-based):\n";
    {
        HtmlBuilder builder("ul");
        builder.add_child("li", "Hello")
            .add_child("li", "World")
            .add_child("li", "from Builder Pattern");

        std::cout << builder.str();
    }

    // Pointer-based chaining
    std::cout << "Example 2: Pointer-based chaining:\n";
    {
        HtmlBuilder builder("ol");
        auto result = builder.add_child_ptr("li", "First")
                          ->add_child_ptr("li", "Second")
                          ->add_child_ptr("li", "Third");

        std::cout << result->str();
    }

    // Complex nested structure
    std::cout << "Example 3: Complex nested HTML structure:\n";
    {
        HtmlBuilder builder("div");
        builder.add_child("h1", "Welcome")
            .add_child("p", "This is a paragraph")
            .add_child("p", "Another paragraph");

        std::cout << builder.str();
    }

    // Document builder
    std::cout << "Example 4: Document Builder (specialized builder):\n";
    {
        auto doc = DocumentBuilder()
                       .add_title("My Blog Post")
                       .add_paragraph("This is the introduction paragraph.")
                       .add_paragraph("Here's some more content.")
                       .add_list_item("Point 1")
                       .add_list_item("Point 2")
                       .add_list_item("Point 3")
                       .build();

        std::cout << doc.get_html();
    }

    // Using implicit conversion
    std::cout << "Example 5: Implicit conversion to HtmlElement:\n";
    {
        HtmlElement result = HtmlBuilder("nav")
                                 .add_child("a", "Home")
                                 .add_child("a", "About")
                                 .add_child("a", "Contact");

        std::cout << result.str();
    }

    // ============ Key Benefits ============
    std::cout << "=== Key Benefits of Builder Pattern ===\n";
    std::cout << "1. Readability: Clear, fluent interface\n";
    std::cout << "2. Step-by-step: Build complex objects gradually\n";
    std::cout << "3. No telescoping constructors: Avoid constructor overload explosion\n";
    std::cout << "4. Optional parameters: Easy to add/skip optional elements\n";
    std::cout << "5. Immutability: Can build immutable objects\n";
    std::cout << "6. Method chaining: Elegant, expressive code\n";
    std::cout << "7. Validation: Can validate at each step\n";

    return 0;
}
