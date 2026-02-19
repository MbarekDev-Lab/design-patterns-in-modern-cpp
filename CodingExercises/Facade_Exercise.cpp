#include "../include/structural/facade.h"
#include <iostream>
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

// ============================================================================
// TEST CASES
// ============================================================================

void test_home_theater_basic()
{
    using namespace facade_solution::home_theater;

    HomeTheater theater;

    // Test components exist
    ASSERT_FALSE(theater.amplifier.on);
    ASSERT_FALSE(theater.dvd.on);
    ASSERT_FALSE(theater.projector.on);

    cout << " Home theater initialization";
}

void test_home_theater_movie_setup()
{
    using namespace facade_solution::home_theater;

    HomeTheater theater;

    // Before watching
    ASSERT_FALSE(theater.amplifier.on);
    ASSERT_FALSE(theater.dvd.on);

    // Watch movie (complex operation hidden)
    theater.watch_movie("Avatar");

    // After watching - subsystems should be on
    ASSERT_TRUE(theater.amplifier.on);
    ASSERT_TRUE(theater.dvd.on);
    ASSERT_TRUE(theater.projector.on);
    ASSERT_EQ(theater.dvd.current_movie, "Avatar");
    ASSERT_TRUE(theater.screen.down);

    cout << " Home theater movie setup";
}

void test_home_theater_movie_cleanup()
{
    using namespace facade_solution::home_theater;

    HomeTheater theater;
    theater.watch_movie("Inception");

    // After setup, everything on
    ASSERT_TRUE(theater.amplifier.on);

    // End movie (also complex, hidden by facade)
    theater.end_movie();

    // Everything should be off now
    ASSERT_FALSE(theater.dvd.on);
    ASSERT_FALSE(theater.amplifier.on);
    ASSERT_FALSE(theater.projector.on);
    ASSERT_FALSE(theater.screen.down);

    cout << " Home theater cleanup";
}

void test_database_connection_pool()
{
    using namespace facade_solution::database_facade;

    Database db;

    // Pool starts with some connections
    ASSERT_EQ(db.pool.available_connections, 10);

    // Get connection
    string conn = db.pool.get_connection();
    ASSERT_EQ(db.pool.available_connections, 9);

    // Release connection
    db.pool.release_connection(conn);
    ASSERT_EQ(db.pool.available_connections, 10);

    cout << " Database connection pool";
}

void test_database_query()
{
    using namespace facade_solution::database_facade;

    Database db;

    // Query should return parsed result
    string result = db.query("SELECT * FROM users");
    ASSERT_TRUE(!result.empty());
    ASSERT_TRUE(result.find("executed") != string::npos);

    cout << " Database query execution";
}

void test_database_transaction()
{
    using namespace facade_solution::database_facade;

    Database db;

    ASSERT_FALSE(db.tm.in_transaction);

    // Transaction should manage state
    vector<string> queries = {"INSERT INTO users", "UPDATE users"};
    db.execute_transaction(queries);

    // After transaction, should be completed
    ASSERT_FALSE(db.tm.in_transaction);

    cout << " Database transaction management";
}

void test_http_client_get()
{
    using namespace facade_solution::http_facade;

    HttpClient client("api.example.com");

    cout << " HTTP client GET";
}

void test_http_client_post()
{
    using namespace facade_solution::http_facade;

    HttpClient client("api.example.com");

    cout << " HTTP client POST";
}

void test_graphics_engine_initialization()
{
    using namespace facade_solution::graphics_facade;

    GraphicsEngine engine;

    // Before init
    ASSERT_FALSE(engine.frame_buffer.bound);

    cout << " Graphics engine initialization";
}

void test_graphics_engine_shader()
{
    using namespace facade_solution::graphics_facade;

    GraphicsEngine engine;

    ASSERT_FALSE(engine.shader_manager.shader_compiled);

    cout << " Graphics shader compilation";
}

void test_graphics_engine_texture()
{
    using namespace facade_solution::graphics_facade;

    GraphicsEngine engine;

    ASSERT_EQ(engine.texture_manager.loaded_textures, 0);

    int tex_id = engine.texture_manager.load_texture("test.png");
    ASSERT_EQ(engine.texture_manager.loaded_textures, 1);
    ASSERT_EQ(tex_id, 1);

    cout << " Graphics texture loading";
}

void test_graphics_engine_draw()
{
    using namespace facade_solution::graphics_facade;

    GraphicsEngine engine;

    vector<float> vertices = {0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f};

    // Draw should coordinate all subsystems
    engine.draw(vertices, "texture.png");

    ASSERT_TRUE(engine.frame_buffer.bound);
    ASSERT_TRUE(engine.vertex_buffer.initialized);
    ASSERT_EQ(engine.texture_manager.loaded_textures, 1);

    cout << " Graphics drawing";
}

void test_amplifier_state()
{
    using namespace facade_solution::home_theater;

    Amplifier amp;

    ASSERT_FALSE(amp.on);
    ASSERT_EQ(amp.volume, 0);

    amp.power_on();
    ASSERT_TRUE(amp.on);

    amp.set_volume(50);
    ASSERT_EQ(amp.volume, 50);

    cout << " Amplifier state management";
}

void test_dvd_player_state()
{
    using namespace facade_solution::home_theater;

    DvdPlayer dvd;

    ASSERT_FALSE(dvd.on);
    ASSERT_EQ(dvd.current_movie, "");

    dvd.power_on();
    ASSERT_TRUE(dvd.on);

    dvd.play("Movie1");
    ASSERT_EQ(dvd.current_movie, "Movie1");

    cout << " DVD player state management";
}

void test_projector_state()
{
    using namespace facade_solution::home_theater;

    Projector proj;

    ASSERT_FALSE(proj.on);

    proj.power_on();
    ASSERT_TRUE(proj.on);

    cout << " Projector state management";
}

void test_screen_state()
{
    using namespace facade_solution::home_theater;

    Screen scr;

    ASSERT_FALSE(scr.down);

    scr.down_screen();
    ASSERT_TRUE(scr.down);

    scr.up_screen();
    ASSERT_FALSE(scr.down);

    cout << " Screen state management";
}

void test_facade_hides_complexity()
{
    using namespace facade_solution::home_theater;

    HomeTheater theater;

    // Without facade, client would need to:
    // 1. Know about Amplifier, DvdPlayer, Projector, Screen
    // 2. Know correct order of operations
    // 3. Handle all state management
    // 4. Duplicated in every watch_movie call
    //
    // With facade, client just calls:
    theater.watch_movie("Inception");

    // Verify all state is correct without client managing it
    ASSERT_TRUE(theater.amplifier.on);
    ASSERT_TRUE(theater.dvd.on);
    ASSERT_TRUE(theater.projector.on);
    ASSERT_TRUE(theater.screen.down);
    ASSERT_EQ(theater.dvd.current_movie, "Inception");

    cout << " Facade encapsulation";
}

void test_query_parser()
{
    using namespace facade_solution::database_facade;

    QueryParser parser;

    string parsed = parser.parse("SELECT * FROM users");
    ASSERT_TRUE(parsed.find("Parsed") != string::npos);

    cout << " Query parser functionality";
}

void test_vertex_buffer()
{
    using namespace facade_solution::graphics_facade;

    VertexBuffer vb;

    ASSERT_FALSE(vb.initialized);

    vector<float> vertices = {0.0f, 1.0f, 2.0f};
    vb.create_buffer(vertices);

    ASSERT_TRUE(vb.initialized);

    cout << " Vertex buffer creation";
}

void test_connection_pool_multiple_operations()
{
    using namespace facade_solution::database_facade;

    ConnectionPool pool;
    const int initial = pool.available_connections;

    string c1 = pool.get_connection();
    string c2 = pool.get_connection();
    ASSERT_EQ(pool.available_connections, initial - 2);

    pool.release_connection(c1);
    ASSERT_EQ(pool.available_connections, initial - 1);

    pool.release_connection(c2);
    ASSERT_EQ(pool.available_connections, initial);

    cout << " Connection pool operations";
}

void test_transaction_state_transitions()
{
    using namespace facade_solution::database_facade;

    TransactionManager tm;

    ASSERT_FALSE(tm.in_transaction);

    tm.begin();
    ASSERT_TRUE(tm.in_transaction);

    tm.commit();
    ASSERT_FALSE(tm.in_transaction);

    tm.begin();
    ASSERT_TRUE(tm.in_transaction);

    tm.rollback();
    ASSERT_FALSE(tm.in_transaction);

    cout << " Transaction state transitions";
}

// ============================================================================
// TEST RUNNER
// ============================================================================

int main()
{
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║          FACADE PATTERN - TEST SUITE                      ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n\n";

    cout << "Running tests...\n\n";

    test_home_theater_basic();
    cout << "\n";
    test_home_theater_movie_setup();
    cout << "\n";
    test_home_theater_movie_cleanup();
    cout << "\n";
    test_database_connection_pool();
    cout << "\n";
    test_database_query();
    cout << "\n";
    test_database_transaction();
    cout << "\n";
    test_http_client_get();
    cout << "\n";
    test_http_client_post();
    cout << "\n";
    test_graphics_engine_initialization();
    cout << "\n";
    test_graphics_engine_shader();
    cout << "\n";
    test_graphics_engine_texture();
    cout << "\n";
    test_graphics_engine_draw();
    cout << "\n";
    test_amplifier_state();
    cout << "\n";
    test_dvd_player_state();
    cout << "\n";
    test_projector_state();
    cout << "\n";
    test_screen_state();
    cout << "\n";
    test_facade_hides_complexity();
    cout << "\n";
    test_query_parser();
    cout << "\n";
    test_vertex_buffer();
    cout << "\n";
    test_connection_pool_multiple_operations();
    cout << "\n";
    test_transaction_state_transitions();
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

    cout << "1. FACADE PATTERN BENEFITS:\n";
    cout << "   - Simplifies client code dramatically\n";
    cout << "   - Hides subsystem complexity\n";
    cout << "   - Single point of entry\n";
    cout << "   - Reduces coupling\n\n";

    cout << "2. WHEN TO USE FACADE:\n";
    cout << "   - Complex subsystems with many components\n";
    cout << "   - Need to simplify client interactions\n";
    cout << "   - Want to layer subsystems\n";
    cout << "   - Provide simplified API\n\n";

    cout << "3. FACADE vs DECORATOR:\n";
    cout << "   - Facade: simplifies interface to MULTIPLE objects\n";
    cout << "   - Decorator: adds functionality to SINGLE object\n\n";

    cout << "4. REAL-WORLD EXAMPLES:\n";
    cout << "   - Libraries (STL hides complex memory management)\n";
    cout << "   - Frameworks (provide simple entry points)\n";
    cout << "   - APIs (HTTP clients, database wrappers)\n";
    cout << "   - Graphics engines (like Bloom application)\n\n";

    cout << "═══════════════════════════════════════════════════════════\n\n";

    return 0;
}
