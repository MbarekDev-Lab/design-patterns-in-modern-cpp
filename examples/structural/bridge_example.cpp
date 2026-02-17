#include "../include/structural/bridge.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// ============================================================================
// BRIDGE PATTERN EXAMPLES
// ============================================================================

// Implementations for the simple Pimpl bridge (from reference)
void bridge_solution::Person::PersonImpl::greet(Person *p)
{
    printf("Hello %s\n", p->name.c_str());
}

bridge_solution::Person::Person()
    : impl(std::make_unique<PersonImpl>())
{
}

bridge_solution::Person::~Person() = default;

void bridge_solution::Person::greet()
{
    impl->greet(this);
}

// ============================================================================
// EXAMPLE 1: Simple Pimpl Bridge (Reference Implementation)
// ============================================================================

void example1_simple_pimpl()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 1: Simple Pimpl Bridge\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "Pattern: Pointer to Implementation (Pimpl)\n";
    cout << "Benefit: Implementation details are hidden\n\n";

    bridge_solution::Person p1;
    p1.name = "Alice";
    p1.greet();

    bridge_solution::Person p2;
    p2.name = "Bob";
    p2.greet();

    cout << "\n";
}

// ============================================================================
// EXAMPLE 2: Violation - Mixed Implementation and Interface
// ============================================================================

void example2_violation()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 2: Violation - Tightly Coupled Implementation\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "Problem: All implementations mixed into single class\n";
    cout << "- Classes get bloated with multiple responsibilities\n";
    cout << "- Hard to modify one format without affecting others\n";
    cout << "- Violates Open/Closed Principle\n\n";

    bridge_violation::ReportGenerator report;
    cout << "Problem with multiple implementations:\n\n";
    report.generate_as_pdf();
    cout << "\n";
    report.generate_as_html();
    cout << "\n";
    report.generate_as_json();

    cout << "\n";
}

// ============================================================================
// EXAMPLE 3: Solution - Abstract Implementation Bridge
// ============================================================================

void example3_abstract_bridge()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 3: Abstract Implementation Bridge\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "Solution: Separate interface from implementation\n";
    cout << "- Each implementation by a separate class\n";
    cout << "- Easy to add new implementations\n";
    cout << "- Follows Open/Closed Principle\n\n";

    auto pdf_renderer = make_shared<bridge_solution::PDFRenderer>();
    auto html_renderer = make_shared<bridge_solution::HTMLRenderer>();
    auto json_renderer = make_shared<bridge_solution::JSONRenderer>();

    bridge_solution::Document doc(pdf_renderer);

    cout << "Rendering with PDF:\n";
    doc.render();

    cout << "\nSwitching to HTML renderer:\n";
    doc.set_renderer(html_renderer);
    doc.render();

    cout << "\nSwitching to JSON renderer:\n";
    doc.set_renderer(json_renderer);
    doc.render();

    cout << "\n";
}

// ============================================================================
// EXAMPLE 4: Device-Renderer Bridge
// ============================================================================

void example4_device_bridge()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 4: Device-Renderer Bridge (Remote Control)\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "Single remote control, multiple devices\n";
    cout << "- Same interface (RemoteControl)\n";
    cout << "- Different implementations (RadioImpl, TVImpl, PhoneImpl)\n\n";

    auto radio = make_shared<bridge_solution::RadioImpl>();
    auto tv = make_shared<bridge_solution::TVImpl>();
    auto phone = make_shared<bridge_solution::PhoneImpl>();

    bridge_solution::RemoteControl remote(radio);

    cout << "Controlling Radio:\n";
    remote.turn_on();
    remote.set_volume(75);
    remote.turn_off();

    cout << "\nSwitching to TV:\n";
    remote.set_device(tv);
    remote.turn_on();
    remote.set_volume(50);
    remote.turn_off();

    cout << "\nSwitching to Phone:\n";
    remote.set_device(phone);
    remote.turn_on();
    remote.set_volume(30);
    remote.turn_off();

    cout << "\n";
}

// ============================================================================
// EXAMPLE 5: Shape-Renderer Bridge
// ============================================================================

void example5_shape_renderer()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 5: Shape-Renderer Bridge\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "Problem: If we hardcoded rendering strategy:\n";
    cout << "- Shape classes would depend on rendering implementation\n";
    cout << "- Adding new rendering mode requires modifying shapes\n\n";

    cout << "Solution: Bridge pattern separates them\n";
    cout << "- Shapes evolve independently from renderers\n";
    cout << "- Easy to add new renderers or shapes\n\n";

    auto console_renderer = make_shared<bridge_solution::ConsoleRenderer>();
    auto high_res_renderer = make_shared<bridge_solution::HighResRenderer>();

    bridge_solution::Circle circle(console_renderer, 5.5);
    bridge_solution::Rectangle rect(console_renderer, 10.0, 20.0);

    cout << "Using Console Renderer:\n";
    circle.draw();
    rect.draw();

    cout << "\nSwitching to High-Res Renderer:\n";
    circle = bridge_solution::Circle(high_res_renderer, 5.5);
    rect = bridge_solution::Rectangle(high_res_renderer, 10.0, 20.0);
    circle.draw();
    rect.draw();

    cout << "\n";
}

// ============================================================================
// EXAMPLE 6: Multiple Shapes with Different Renderers
// ============================================================================

void example6_mixed_shapes_renderers()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 6: Mixed Shapes and Renderers\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "Mixing different shapes and renderers:\n";
    cout << "- Shapes can use different renderers\n";
    cout << "- Renderers can be swapped independently\n\n";

    auto console = make_shared<bridge_solution::ConsoleRenderer>();
    auto high_res = make_shared<bridge_solution::HighResRenderer>();

    vector<shared_ptr<bridge_solution::Shape>> shapes;

    shapes.push_back(make_shared<bridge_solution::Circle>(console, 3.0));
    shapes.push_back(make_shared<bridge_solution::Circle>(high_res, 5.0));
    shapes.push_back(make_shared<bridge_solution::Rectangle>(console, 10.0, 15.0));
    shapes.push_back(make_shared<bridge_solution::Rectangle>(high_res, 20.0, 30.0));

    cout << "Drawing " << shapes.size() << " shapes:\n";
    for (const auto &shape : shapes)
    {
        shape->draw();
    }

    cout << "\n";
}

// ============================================================================
// EXAMPLE 7: Real-World - Database Connection Bridge
// ============================================================================

class DatabaseConnection
{
public:
    virtual ~DatabaseConnection() = default;
    virtual void connect() = 0;
    virtual void execute(const string &query) = 0;
    virtual void disconnect() = 0;
};

class MySQLConnection : public DatabaseConnection
{
public:
    void connect() override { cout << "    MySQL: Connecting...\n"; }
    void execute(const string &query) override
    {
        cout << "    MySQL: Executing: " << query << "\n";
    }
    void disconnect() override { cout << "    MySQL: Disconnecting...\n"; }
};

class PostgreSQLConnection : public DatabaseConnection
{
public:
    void connect() override { cout << "    PostgreSQL: Connecting...\n"; }
    void execute(const string &query) override
    {
        cout << "    PostgreSQL: Executing: " << query << "\n";
    }
    void disconnect() override { cout << "    PostgreSQL: Disconnecting...\n"; }
};

class DatabaseService
{
public:
    explicit DatabaseService(std::shared_ptr<DatabaseConnection> conn)
        : impl_(conn)
    {
    }

    void query(const string &sql)
    {
        impl_->connect();
        impl_->execute(sql);
        impl_->disconnect();
    }

    void switch_database(std::shared_ptr<DatabaseConnection> conn)
    {
        impl_ = conn;
    }

private:
    std::shared_ptr<DatabaseConnection> impl_;
};

void example7_database_bridge()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 7: Real-World - Database Connection Bridge\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "Application uses DatabaseService for queries\n";
    cout << "But underlying database can be changed at runtime\n\n";

    auto mysql = make_shared<MySQLConnection>();
    auto postgres = make_shared<PostgreSQLConnection>();

    DatabaseService service(mysql);

    cout << "Using MySQL:\n";
    service.query("SELECT * FROM users");

    cout << "\nSwitching to PostgreSQL:\n";
    service.switch_database(postgres);
    service.query("SELECT * FROM users");

    cout << "\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "\n╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║              BRIDGE PATTERN EXAMPLES                      ║\n";
    cout << "║  Decouple abstraction from implementation                 ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    cout << "\n┌───────────────────────────────────────────────────────────┐\n";
    cout << "│ PATTERN OVERVIEW                                          │\n";
    cout << "└───────────────────────────────────────────────────────────┘\n\n";

    cout << "Intent:\n";
    cout << "  Decouple an abstraction from its implementation so the two\n";
    cout << "  can vary independently.\n\n";

    cout << "Key Concepts:\n";
    cout << "  1. Abstraction - The public interface\n";
    cout << "  2. Implementation - The concrete implementation\n";
    cout << "  3. Bridge - A pointer/reference to implementation\n";
    cout << "  4. Separation - Allow evolution independently\n\n";

    cout << "Benefits:\n";
    cout << "  ✓ Reduces coupling between abstraction and implementation\n";
    cout << "  ✓ Improves extensibility (easy to add implementations)\n";
    cout << "  ✓ Follows Open/Closed Principle\n";
    cout << "  ✓ Can change implementation at runtime\n";
    cout << "  ✓ Hides implementation details (Pimpl idiom)\n\n";

    cout << "When to Use:\n";
    cout << "  - You need to avoid permanent binding between interface\n";
    cout << "    and implementation\n";
    cout << "  - Changes in implementation shouldn't affect clients\n";
    cout << "  - You want to share implementations among multiple objects\n";
    cout << "  - You need to support multiple implementations\n";
    cout << "  - Reducing compile dependencies (header files)\n\n";

    // Run examples
    example1_simple_pimpl();
    example2_violation();
    example3_abstract_bridge();
    example4_device_bridge();
    example5_shape_renderer();
    example6_mixed_shapes_renderers();
    example7_database_bridge();

    cout << "═══════════════════════════════════════════════════════════\n";
    cout << "All examples completed successfully!\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    return 0;
}
