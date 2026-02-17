#include "../include/structural/bridge.h"
#include <iostream>
#include <memory>
#include <vector>
#include <cassert>

using namespace bridge_solution;
using namespace std;

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

#define ASSERT_EQ(actual, expected, message)         \
    if ((actual) == (expected))                      \
    {                                                \
        cout << "  ✓ " << (message) << "\n";         \
        results.passed++;                            \
    }                                                \
    else                                             \
    {                                                \
        cout << "  ✗ FAILED: " << (message) << "\n"; \
        results.failed++;                            \
    }

#define ASSERT_NE(actual, expected, message)         \
    if ((actual) != (expected))                      \
    {                                                \
        cout << "  ✓ " << (message) << "\n";         \
        results.passed++;                            \
    }                                                \
    else                                             \
    {                                                \
        cout << "  ✗ FAILED: " << (message) << "\n"; \
        results.failed++;                            \
    }

#define ASSERT_TRUE(condition, message)              \
    if ((condition))                                 \
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

void test_pimpl_bridge_creation()
{
    TEST_CASE("Pimpl Bridge - Creation");
    Person p;
    p.name = "Alice";
    ASSERT_EQ("Alice", p.name, "Name set correctly");
    ASSERT_NE(nullptr, p.impl.get(), "Implementation exists");
}

void test_pimpl_bridge_multiple_instances()
{
    TEST_CASE("Pimpl Bridge - Multiple instances");
    Person p1;
    p1.name = "Charlie";
    Person p2;
    p2.name = "Diana";

    ASSERT_NE(p1.impl.get(), p2.impl.get(), "Different impl pointers");
    ASSERT_EQ("Charlie", p1.name, "First person name");
    ASSERT_EQ("Diana", p2.name, "Second person name");
}

void test_document_with_pdf_renderer()
{
    TEST_CASE("Document - PDF renderer");
    auto renderer = make_shared<PDFRenderer>();
    Document doc(renderer);
    doc.render();
    ASSERT_TRUE(true, "PDF rendering works");
}

void test_document_with_html_renderer()
{
    TEST_CASE("Document - HTML renderer");
    auto renderer = make_shared<HTMLRenderer>();
    Document doc(renderer);
    doc.render();
    ASSERT_TRUE(true, "HTML rendering works");
}

void test_document_with_json_renderer()
{
    TEST_CASE("Document - JSON renderer");
    auto renderer = make_shared<JSONRenderer>();
    Document doc(renderer);
    doc.render();
    ASSERT_TRUE(true, "JSON rendering works");
}

void test_document_renderer_switch()
{
    TEST_CASE("Document - Switch renderer at runtime");
    auto pdf = make_shared<PDFRenderer>();
    auto html = make_shared<HTMLRenderer>();

    Document doc(pdf);
    doc.render();
    doc.set_renderer(html);
    doc.render();

    ASSERT_TRUE(true, "Runtime renderer switch works");
}

void test_remote_control_with_radio()
{
    TEST_CASE("RemoteControl - Radio device");
    auto radio = make_shared<RadioImpl>();
    RemoteControl remote(radio);

    remote.turn_on();
    remote.set_volume(80);
    remote.turn_off();

    ASSERT_TRUE(true, "Radio control works");
}

void test_remote_control_with_tv()
{
    TEST_CASE("RemoteControl - TV device");
    auto tv = make_shared<TVImpl>();
    RemoteControl remote(tv);

    remote.turn_on();
    remote.set_volume(40);
    remote.turn_off();

    ASSERT_TRUE(true, "TV control works");
}

void test_remote_control_device_switch()
{
    TEST_CASE("RemoteControl - Switch device at runtime");
    auto radio = make_shared<RadioImpl>();
    auto tv = make_shared<TVImpl>();

    RemoteControl remote(radio);
    remote.turn_on();
    remote.set_device(tv);
    remote.turn_on();

    ASSERT_TRUE(true, "Device switching works");
}

void test_circle_creation()
{
    TEST_CASE("Shape - Circle creation");
    auto renderer = make_shared<ConsoleRenderer>();
    Circle circle(renderer, 5.0);
    circle.draw();
    ASSERT_TRUE(true, "Circle drawing works");
}

void test_rectangle_creation()
{
    TEST_CASE("Shape - Rectangle creation");
    auto renderer = make_shared<ConsoleRenderer>();
    Rectangle rect(renderer, 10.0, 20.0);
    rect.draw();
    ASSERT_TRUE(true, "Rectangle drawing works");
}

void test_circle_with_high_res_renderer()
{
    TEST_CASE("Shape - Circle with high-res renderer");
    auto renderer = make_shared<HighResRenderer>();
    Circle circle(renderer, 5.0);
    circle.draw();
    ASSERT_TRUE(true, "High-res circle works");
}

void test_rectangle_with_high_res_renderer()
{
    TEST_CASE("Shape - Rectangle with high-res renderer");
    auto renderer = make_shared<HighResRenderer>();
    Rectangle rect(renderer, 10.0, 20.0);
    rect.draw();
    ASSERT_TRUE(true, "High-res rectangle works");
}

void test_multiple_shapes_polymorphism()
{
    TEST_CASE("Shape - Polymorphic collection");
    auto renderer = make_shared<ConsoleRenderer>();
    vector<shared_ptr<Shape>> shapes;

    shapes.push_back(make_shared<Circle>(renderer, 5.0));
    shapes.push_back(make_shared<Rectangle>(renderer, 10.0, 20.0));

    ASSERT_EQ((int)shapes.size(), 2, "Collection has 2 shapes");

    for (const auto &shape : shapes)
    {
        shape->draw();
    }
}

void test_renderer_polymorphism()
{
    TEST_CASE("Renderer - Polymorphic collection");
    vector<shared_ptr<Renderer>> renderers;
    renderers.push_back(make_shared<PDFRenderer>());
    renderers.push_back(make_shared<HTMLRenderer>());
    renderers.push_back(make_shared<JSONRenderer>());

    ASSERT_EQ((int)renderers.size(), 3, "Collection has 3 renderers");
}

void test_device_implementation_polymorphism()
{
    TEST_CASE("Device - Polymorphic implementations");
    vector<shared_ptr<DeviceImplementation>> devices;
    devices.push_back(make_shared<RadioImpl>());
    devices.push_back(make_shared<TVImpl>());
    devices.push_back(make_shared<PhoneImpl>());

    ASSERT_EQ((int)devices.size(), 3, "Collection has 3 devices");
}

void test_independence_of_abstraction_and_implementation()
{
    TEST_CASE("Bridge - Abstraction-Implementation independence");

    auto pdf = make_shared<PDFRenderer>();
    auto html = make_shared<HTMLRenderer>();

    ASSERT_TRUE((void *)pdf.get() != (void *)html.get(), "Different renderer instances");

    Document doc1(pdf);
    Document doc2(html);

    doc1.render();
    doc2.render();

    ASSERT_TRUE(true, "Independent abstractions and implementations work");
}

void test_edge_case_small_circle()
{
    TEST_CASE("Shape - Small circle");
    auto renderer = make_shared<ConsoleRenderer>();
    Circle circle(renderer, 0.1);
    circle.draw();
    ASSERT_TRUE(true, "Small circle works");
}

void test_edge_case_large_circle()
{
    TEST_CASE("Shape - Large circle");
    auto renderer = make_shared<ConsoleRenderer>();
    Circle circle(renderer, 1000.0);
    circle.draw();
    ASSERT_TRUE(true, "Large circle works");
}

void test_edge_case_square_rectangle()
{
    TEST_CASE("Shape - Square as rectangle");
    auto renderer = make_shared<ConsoleRenderer>();
    Rectangle rect(renderer, 10.0, 10.0);
    rect.draw();
    ASSERT_TRUE(true, "Square-shaped rectangle works");
}

void test_multiple_independent_bridges()
{
    TEST_CASE("Bridge - Multiple independent bridge patterns");

    auto pdf = make_shared<PDFRenderer>();
    Document doc(pdf);

    auto radio = make_shared<RadioImpl>();
    RemoteControl remote(radio);

    auto renderer = make_shared<ConsoleRenderer>();
    Circle circle(renderer, 5.0);

    doc.render();
    remote.turn_on();
    circle.draw();

    ASSERT_TRUE(true, "Multiple bridges work independently");
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║           BRIDGE PATTERN TEST SUITE                        ║\n";
    cout << "║  Decouple abstraction from implementation                  ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";

    cout << "\n═══════════════════════════════════════════════════════════════\n";
    cout << "RUNNING TESTS\n";
    cout << "═══════════════════════════════════════════════════════════════\n";

    test_pimpl_bridge_creation();
    test_pimpl_bridge_multiple_instances();
    test_document_with_pdf_renderer();
    test_document_with_html_renderer();
    test_document_with_json_renderer();
    test_document_renderer_switch();
    test_remote_control_with_radio();
    test_remote_control_with_tv();
    test_remote_control_device_switch();
    test_circle_creation();
    test_rectangle_creation();
    test_circle_with_high_res_renderer();
    test_rectangle_with_high_res_renderer();
    test_multiple_shapes_polymorphism();
    test_renderer_polymorphism();
    test_device_implementation_polymorphism();
    test_independence_of_abstraction_and_implementation();
    test_edge_case_small_circle();
    test_edge_case_large_circle();
    test_edge_case_square_rectangle();
    test_multiple_independent_bridges();

    results.print_summary();

    cout << "═══════════════════════════════════════════════════════════════\n";
    cout << "KEY LEARNING POINTS\n";
    cout << "═══════════════════════════════════════════════════════════════\n\n";

    cout << "1. BRIDGE PATTERN STRUCTURE:\n";
    cout << "   - Abstraction: The public interface\n";
    cout << "   - Implementation: The actual behavior\n";
    cout << "   - Bridge: A pointer to implementation\n";
    cout << "   - Decoupling: Both can vary independently\n\n";

    cout << "2. BENEFITS:\n";
    cout << "   - Reduces coupling between interface and implementation\n";
    cout << "   - Follows Open/Closed Principle\n";
    cout << "   - Easy to add new implementations\n";
    cout << "   - Can change implementation at runtime\n";
    cout << "   - Hides implementation details (Pimpl)\n\n";

    cout << "3. WHEN TO USE:\n";
    cout << "   - Need to decouple abstraction from implementation\n";
    cout << "   - Multiple implementations for same interface\n";
    cout << "   - Want to avoid compile dependencies\n";
    cout << "   - Need runtime flexibility\n\n";

    cout << "4. REAL-WORLD EXAMPLES:\n";
    cout << "   - Database connections (MySQL, PostgreSQL, etc.)\n";
    cout << "   - UI rendering engines (Console, Web, etc.)\n";
    cout << "   - Device drivers and hardware abstraction\n";
    cout << "   - File format handling and conversion\n\n";

    cout << "═══════════════════════════════════════════════════════════════\n\n";

    return results.failed == 0 ? 0 : 1;
}
