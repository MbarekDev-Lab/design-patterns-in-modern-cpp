#include "creational/html_elements.h"
#include <iostream>

using namespace html;

int main()
{
    std::cout << "=== Type-Safe HTML Elements Pattern ===\n\n";

    // ============ Example 1: Simple Tags ============
    std::cout << "Example 1: Simple HTML elements:\n";
    std::cout << "--------------------------------------\n";
    {
        std::cout << P("Hello, World!\n");
        std::cout << H1("Welcome to C++\n");
    }

    // ============ Example 2: Self-Closing Tags ============
    std::cout << "Example 2: Self-closing image tag:\n";
    std::cout << "--------------------------------------\n";
    {
        IMG img("https://example.com/image.png");
        img.alt("Example Image").width(200).height(150);
        std::cout << img;
    }

    // ============ Example 3: Nested Elements ============
    std::cout << "Example 3: Nested elements (list):\n";
    std::cout << "--------------------------------------\n";
    {
        UL list{
            LI("Item 1"),
            LI("Item 2"),
            LI("Item 3")};
        std::cout << list;
    }

    // ============ Example 4: Complex Structure ============
    std::cout << "Example 4: Complex nested structure:\n";
    std::cout << "--------------------------------------\n";
    {
        DIV container;
        container.css_class("main-container");
        container.id("content");

        P intro("Welcome to our website!");
        std::cout << container;
        std::cout << intro;
    }

    // ============ Example 5: Form with Attributes ============
    std::cout << "Example 5: Form with input elements:\n";
    std::cout << "--------------------------------------\n";
    {
        FORM form;
        form.action("/submit").method("POST");

        INPUT name_field;
        name_field.input_type("text")
            .name("username")
            .placeholder("Enter your name");

        INPUT email_field;
        email_field.input_type("email")
            .name("email")
            .placeholder("Enter your email");

        BUTTON submit_btn("Submit");
        submit_btn.button_type("submit");

        std::cout << form;
        std::cout << name_field;
        std::cout << email_field;
        std::cout << submit_btn;
    }

    // ============ Example 6: Heading with Link ============
    std::cout << "Example 6: Heading with link:\n";
    std::cout << "--------------------------------------\n";
    {
        H2 title("Read More");
        A link("https://example.com", "Click here");
        std::cout << title;
        std::cout << link;
    }

    // ============ Example 7: Picture with Caption ============
    std::cout << "Example 7: Picture with caption:\n";
    std::cout << "--------------------------------------\n";
    {
        DIV figure;
        figure.css_class("figure");

        IMG photo("https://example.com/photo.jpg");
        photo.alt("Beautiful landscape").width(400).height(300);

        P caption("A beautiful sunset over the mountains");

        std::cout << figure;
        std::cout << photo;
        std::cout << caption;
    }

    // ============ Example 8: Ordered List ============
    std::cout << "Example 8: Ordered list (numbered):\n";
    std::cout << "--------------------------------------\n";
    {
        OL steps{
            LI("Initialize the project"),
            LI("Install dependencies"),
            LI("Build the application"),
            LI("Test the code"),
            LI("Deploy to production")};
        std::cout << steps;
    }

    // ============ Benefits of This Pattern ============
    std::cout << "=== Key Benefits ===\n";
    std::cout << "1. Type Safety: Compile-time checking for tag types\n";
    std::cout << "2. Natural Syntax: Code reads like HTML\n";
    std::cout << "3. Flexibility: Supports text, attributes, and children\n";
    std::cout << "4. Method Chaining: Fluent API for adding attributes\n";
    std::cout << "5. Reusability: Create complex elements easily\n";
    std::cout << "6. No Strings: No magic string tag names\n";
    std::cout << "7. Validation: Invalid tag types caught at compile time\n";
    std::cout << "8. Clean Output: Properly formatted HTML\n";

    return 0;
}
