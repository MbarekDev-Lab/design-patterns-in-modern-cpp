#include "../include/structural/bridge.h"
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>

using namespace std;

// ============================================================================
// BRIDGE PATTERN EXAMPLES
// ============================================================================

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
// EXAMPLE 2: PIMPL Wrapper-Based Bridge Pattern
// ============================================================================

void example2_pimpl_wrapper()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 2: PIMPL Wrapper-Based Bridge\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "Pattern: Template-based PIMPL wrapper for cleaner code\n";
    cout << "Benefit: Reusable wrapper for any PIMPL pattern\n\n";

    bridge_solution::Window window1;
    window1.set_title("Main Window");
    cout << "Window 1:\n";
    window1.show();
    window1.hide();

    bridge_solution::Window window2;
    window2.set_title("Settings");
    cout << "\nWindow 2:\n";
    window2.show();

    cout << "\n";
}

// ============================================================================
// EXAMPLE 3: Violation - Mixed Implementation and Interface
// ============================================================================

void example3_violation()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 3: Violation - Tightly Coupled Implementation\n";
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
// EXAMPLE 4: Solution - Abstract Implementation Bridge
// ============================================================================

void example4_abstract_bridge()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 4: Abstract Implementation Bridge\n";
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

void example5_device_bridge()
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
// EXAMPLE 6: Shape-Renderer Bridge
// ============================================================================

void example6_shape_renderer()
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

void example7_mixed_shapes_renderers()
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
// EXAMPLE 8: Real-World - Database Connection Bridge
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

void example8_database_bridge()
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
// EXAMPLE 9: Shape Bridge with String Rendering
// ============================================================================

void example9_shape_bridge_string()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 8: Shape Bridge with String Rendering\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "Pattern: Shape abstraction with Renderer bridge\n";
    cout << "Benefit: Renderers determine HOW to draw\n";
    cout << "         Shapes determine WHAT to draw\n\n";

    // Define renderers
    struct Renderer
    {
        virtual string what_to_render_as() const = 0;
        virtual ~Renderer() = default;
    };

    struct RasterRenderer : Renderer
    {
        string what_to_render_as() const override
        {
            return "pixels";
        }
    };

    struct VectorRenderer : Renderer
    {
        string what_to_render_as() const override
        {
            return "lines";
        }
    };

    // Define shapes
    struct Shape
    {
        string name;
        const Renderer &renderer;

        Shape(const Renderer &r) : renderer(r) {}

        string str() const
        {
            ostringstream oss;
            oss << "Drawing " << name << " as " << renderer.what_to_render_as();
            return oss.str();
        }
    };

    struct Triangle : Shape
    {
        explicit Triangle(const Renderer &renderer) : Shape(renderer)
        {
            name = "Triangle";
        }
    };

    struct Square : Shape
    {
        explicit Square(const Renderer &renderer) : Shape(renderer)
        {
            name = "Square";
        }
    };

    struct Circle : Shape
    {
        explicit Circle(const Renderer &renderer) : Shape(renderer)
        {
            name = "Circle";
        }
    };

    // Usage
    RasterRenderer raster;
    VectorRenderer vector;

    Triangle t1(raster);
    Triangle t2(vector);

    Square s1(raster);
    Square s2(vector);

    Circle c(vector);

    cout << t1.str() << "\n";
    cout << t2.str() << "\n";
    cout << s1.str() << "\n";
    cout << s2.str() << "\n";
    cout << c.str() << "\n";

    cout << "\n";
}

// ============================================================================
// EXAMPLE 10: Circle Bridge with Draw and Resize
// ============================================================================

void example10_circle_bridge_render()
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "EXAMPLE 9: Circle Bridge with Draw and Resize Operations\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    cout << "Pattern: Shape with Renderer bridge + operations\n";
    cout << "Benefit: Renderer is abstracted from shape operations\n\n";

    // Renderer interface
    struct CircleRenderer
    {
        virtual void render_circle(float x, float y, float radius) = 0;
        virtual ~CircleRenderer() = default;
    };

    struct VectorCircleRenderer : CircleRenderer
    {
        void render_circle(float x, float y, float radius) override
        {
            cout << "  Drawing vector circle at (" << x << "," << y
                 << ") with radius " << radius << "\n";
        }
    };

    struct RasterCircleRenderer : CircleRenderer
    {
        void render_circle(float x, float y, float radius) override
        {
            cout << "  Rasterizing circle at (" << x << "," << y
                 << ") with radius " << radius << "\n";
        }
    };

    // Shape abstraction
    struct Shape
    {
    protected:
        CircleRenderer &renderer;
        Shape(CircleRenderer &r) : renderer(r) {}

    public:
        virtual ~Shape() = default;
        virtual void draw() = 0;
        virtual void resize(float factor) = 0;
    };

    struct CircleShape : Shape
    {
        float x, y, radius;

        CircleShape(CircleRenderer &renderer, float x, float y, float r)
            : Shape(renderer), x(x), y(y), radius(r) {}

        void draw() override
        {
            renderer.render_circle(x, y, radius);
        }

        void resize(float factor) override
        {
            radius *= factor;
        }
    };

    // Usage
    VectorCircleRenderer vector_renderer;
    RasterCircleRenderer raster_renderer;

    CircleShape my_circle(vector_renderer, 10.0f, 10.0f, 5.0f);

    cout << "Initial circle (vector rendered):\n";
    my_circle.draw();

    cout << "\nAfter resize(2):\n";
    my_circle.resize(2.0f);
    my_circle.draw();

    cout << "\nSame circle with raster renderer:\n";
    CircleShape raster_circle(raster_renderer, 10.0f, 10.0f, 10.0f);
    raster_circle.draw();

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
    example2_pimpl_wrapper();
    example3_violation();
    example4_abstract_bridge();
    example5_device_bridge();
    example6_shape_renderer();
    example7_mixed_shapes_renderers();
    example8_database_bridge();
    example9_shape_bridge_string();
    example10_circle_bridge_render();

    cout << "═══════════════════════════════════════════════════════════\n";
    cout << "All examples completed successfully!\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    return 0;
}
