#include "../include/structural/facade.h"
#include <iostream>

using namespace std;

// ============================================================================
// FACADE PATTERN EXAMPLES
// ============================================================================

void print_header(const string &title)
{
    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << title << "\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";
}

// ============================================================================
// EXAMPLE 1: Violation - Complex Manual Subsystem Usage
// ============================================================================

void example1_violation()
{
    print_header("EXAMPLE 1: Violation - Complex Manual Subsystem Usage");

    cout << "Problem: Client must manage all subsystem components\n";
    cout << "Complex, error-prone, hard to maintain\n\n";

    using namespace facade_violation::home_theater_bad;

    Amplifier amp;
    DvdPlayer dvd;
    Projector proj;
    Screen scr;

    cout << "Starting movie manually (complex):\n";
    scr.lower();
    proj.dim_lights(10);
    amp.power_on();
    amp.set_volume(5);
    dvd.power_on();
    dvd.play("Avatar");
    proj.power_on();

    cout << "\nNote: Client handles EVERY detail\n";
    cout << "Order matters! Easy to make mistakes.\n";

    cout << "\nStopping movie (also manual, also complex):\n";
    dvd.stop();
    dvd.power_off();
    amp.power_off();
    proj.power_off();
    scr.raise();

    cout << "\n✗ Violates DRY principle - code duplication\n";
    cout << "✗ Client tightly coupled to subsystems\n";
}

// ============================================================================
// EXAMPLE 2: Home Theater Facade
// ============================================================================

void example2_home_theater()
{
    print_header("EXAMPLE 2: Home Theater Facade");

    cout << "Solution: Single facade manages complex subsystem\n";
    cout << "Client code is simple and maintainable\n\n";

    using namespace facade_solution::home_theater;

    HomeTheater theater;

    cout << "Starting movie (simple facade call):\n";
    theater.watch_movie("Avatar");

    cout << "\nEnding movie (simple facade call):\n";
    theater.end_movie();

    cout << "\n✓ Single responsibility in client code\n";
    cout << "✓ Subsystems managed by facade\n";
}

// ============================================================================
// EXAMPLE 3: Database Facade
// ============================================================================

void example3_database_facade()
{
    print_header("EXAMPLE 3: Database Facade");

    cout << "Simplifies: connection pooling, parsing, transactions\n\n";

    using namespace facade_solution::database_facade;

    Database db;

    cout << "Simple query:\n";
    string result1 = db.query("SELECT * FROM users");
    cout << "Result: " << result1 << "\n\n";

    cout << "Complex transaction (facade handles everything):\n";
    vector<string> queries = {
        "INSERT INTO users VALUES (1, 'Alice')",
        "INSERT INTO users VALUES (2, 'Bob')",
        "UPDATE users SET active = 1"};
    db.execute_transaction(queries);

    cout << "\n✓ Transaction details hidden\n";
    cout << "✓ Connection management automatic\n";
}

// ============================================================================
// EXAMPLE 4: HTTP Client Facade
// ============================================================================

void example4_http_facade()
{
    print_header("EXAMPLE 4: HTTP Client Facade");

    cout << "Simplifies: connection, request building, parsing\n\n";

    using namespace facade_solution::http_facade;

    HttpClient client("api.example.com");

    cout << "GET request:\n";
    string resp1 = client.get("/api/users");
    cout << "Response: " << resp1 << "\n\n";

    cout << "POST request:\n";
    string resp2 = client.post("/api/users", "{\"name\": \"Alice\"}");
    cout << "Response: " << resp2 << "\n\n";

    cout << "✓ No need to build requests manually\n";
    cout << "✓ Connection management transparent\n";
}

// ============================================================================
// EXAMPLE 5: Graphics Engine Facade
// ============================================================================

void example5_graphics_facade()
{
    print_header("EXAMPLE 5: Graphics Engine Facade (like Bloom)");

    cout << "Simplifies: shaders, textures, buffers, framebuffers\n\n";

    using namespace facade_solution::graphics_facade;

    GraphicsEngine engine;

    cout << "Initialize engine:\n";
    engine.initialize();

    cout << "\nDraw with geometry:\n";
    vector<float> vertices = {0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f};
    engine.draw(vertices, "texture.png");

    cout << "\nCleanup:\n";
    engine.teardown();

    cout << "\n✓ Complex graphics setup in few lines\n";
    cout << "✓ Shader/texture coordination automatic\n";
}

// ============================================================================
// EXAMPLE 6: Facade vs Direct Usage Comparison
// ============================================================================

void example6_comparison()
{
    print_header("EXAMPLE 6: Facade vs Direct Usage");

    cout << "WITHOUT FACADE - Many lines, lots of complexity:\n\n";

    cout << "  Amplifier amp;                 // Create component\n";
    cout << "  DvdPlayer dvd;                 // Create component\n";
    cout << "  Projector proj;                // Create component\n";
    cout << "  Screen scr;                    // Create component\n";
    cout << "  scr.lower();                   // 4 manual steps\n";
    cout << "  proj.dim_lights(10);           //\n";
    cout << "  amp.power_on();                //\n";
    cout << "  amp.set_volume(5);             //\n";
    cout << "  dvd.power_on();                // More manual steps\n";
    cout << "  dvd.play(\"Avatar\");           //\n";
    cout << "  proj.power_on();               //\n\n";

    cout << "WITH FACADE - One simple call:\n\n";
    cout << "  HomeTheater theater;           // Single facade\n";
    cout << "  theater.watch_movie(\"Avatar\"); // One method\n\n";

    cout << "✓ 10+ lines reduced to 2 lines\n";
    cout << "✓ Error-prone steps hidden\n";
    cout << "✓ Concerns organized\n";
}

// ============================================================================
// EXAMPLE 7: Multiple Facades for Different Use Cases
// ============================================================================

void example7_multiple_facades()
{
    print_header("EXAMPLE 7: Multiple Facades for Different Use Cases");

    cout << "Can have multiple facades exposing different views\n";
    cout << "of the same complex subsystem\n\n";

    using namespace facade_solution::home_theater;

    HomeTheater theater;

    cout << "Facade Method 1: watch_movie()\n";
    cout << "  - Coordinates: screen, projector, dvd, amp\n";
    cout << "  - Complex setup with specific parameters\n\n";

    cout << "Could also have:\n";
    cout << "  - listen_to_music() facade\n";
    cout << "    Coordinates: amp, speakers (no projector/dvd)\n";
    cout << "  - watch_news() facade\n";
    cout << "    Coordinates: projector, tuner (different setup)\n";
    cout << "\nEach facade presents one aspect of subsystem\n";
    cout << "Clients only see what they need\n";
}

// ============================================================================
// EXAMPLE 8: Progressive Disclosure with Facades
// ============================================================================

void example8_progressive_disclosure()
{
    print_header("EXAMPLE 8: Progressive Disclosure with Facades");

    cout << "Beginners: Use simple facade methods\n";
    cout << "Advanced users: Access subsystems directly\n\n";

    using namespace facade_solution::home_theater;

    HomeTheater theater;

    cout << "Simple User Level:\n";
    cout << "  theater.watch_movie(\"Inception\");\n";
    cout << "  theater.end_movie();\n\n";

    cout << "Advanced User Level:\n";
    cout << "  theater.amplifier.set_volume(75);\n";
    cout << "  theater.dvd.play(\"Custom Movie\");\n";
    cout << "  // Direct subsystem access if needed\n\n";

    cout << "✓ Easy to use for common cases\n";
    cout << "✓ Powerful for expert users\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║              FACADE PATTERN EXAMPLES                      ║\n";
    cout << "║  Provide unified interface to complex subsystems          ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";

    cout << "\n┌───────────────────────────────────────────────────────────┐\n";
    cout << "│ PATTERN OVERVIEW                                          │\n";
    cout << "└───────────────────────────────────────────────────────────┘\n\n";

    cout << "Intent:\n";
    cout << "  Provide a unified, simplified interface to a set of\n";
    cout << "  interfaces in a subsystem. Facade defines a higher-level\n";
    cout << "  interface that makes the subsystem easier to use.\n\n";

    cout << "Key Concepts:\n";
    cout << "  1. Subsystems - Complex set of interrelated classes\n";
    cout << "  2. Facade - Single entry point hiding complexity\n";
    cout << "  3. Client - Uses facade instead of subsystems\n";
    cout << "  4. Decoupling - Clients don't need subsystem details\n\n";

    cout << "Benefits:\n";
    cout << "  ✓ Shields clients from complex components\n";
    cout << "  ✓ Promotes loose coupling\n";
    cout << "  ✓ Simplifies client code\n";
    cout << "  ✓ One point of control\n";
    cout << "  ✓ Reduces compile dependencies\n\n";

    cout << "When to Use:\n";
    cout << "  - Simplify complex subsystems\n";
    cout << "  - Decouple clients from implementations\n";
    cout << "  - Layer subsystems\n";
    cout << "  - Many interdependent components\n\n";

    // Run examples
    example1_violation();
    example2_home_theater();
    example3_database_facade();
    example4_http_facade();
    example5_graphics_facade();
    example6_comparison();
    example7_multiple_facades();
    example8_progressive_disclosure();

    cout << "\n═══════════════════════════════════════════════════════════\n";
    cout << "All examples completed successfully!\n";
    cout << "═══════════════════════════════════════════════════════════\n\n";

    return 0;
}
