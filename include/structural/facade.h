#ifndef FACADE_PATTERN_H
#define FACADE_PATTERN_H

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>
#include <cstdint>

using namespace std;

// ============================================================================
// FACADE PATTERN
// ============================================================================
//
// INTENT:
//   Provide a unified, simplified interface to a set of interfaces in a
//   subsystem. Facade defines a higher-level interface that makes the
//   subsystem easier to use.
//
// KEY CONCEPTS:
//   1. Subsystems - Complex set of interrelated classes
//   2. Facade - Single entry point hiding subsystem complexity
//   3. Client - Uses facade instead of subsystems directly
//   4. Decoupling - Clients don't need to know subsystem details
//
// BENEFITS:
//   ✓ Shields clients from complex subsystem components
//   ✓ Promotes loose coupling between client and subsystems
//   ✓ Simplifies client code
//   ✓ One point of control for subsystem access
//   ✓ Reduces compile dependencies
//
// WHEN TO USE:
//   - Need to simplify complex subsystems
//   - Want to decouple clients from subsystems
//   - Need to layer subsystems (progressive disclosure)
//   - Have many interdependent components
//
// DIFFERENCE FROM DECORATOR:
//   - Decorator: adds functionality to single object
//   - Facade: simplifies interface to multiple objects/subsystems

namespace facade_solution
{

    // ============================================================================
    // EXAMPLE 1: Home Theater System Facade
    // ============================================================================

    namespace home_theater
    {

        // Subsystem 1: Power Amplifier
        struct Amplifier
        {
            bool on = false;
            int volume = 0;

            void power_on()
            {
                on = true;
                cout << "    Amplifier on\n";
            }

            void power_off()
            {
                on = false;
                cout << "    Amplifier off\n";
            }

            void set_volume(int vol)
            {
                volume = vol;
                cout << "    Amplifier volume: " << vol << "%\n";
            }
        };

        // Subsystem 2: DVD Player
        struct DvdPlayer
        {
            bool on = false;
            string current_movie;

            void power_on()
            {
                on = true;
                cout << "    DVD player on\n";
            }

            void power_off()
            {
                on = false;
                cout << "    DVD player off\n";
            }

            void play(const string &movie)
            {
                current_movie = movie;
                cout << "    Playing movie: " << movie << "\n";
            }

            void stop()
            {
                cout << "    DVD player stopped\n";
            }
        };

        // Subsystem 3: Projector
        struct Projector
        {
            bool on = false;

            void power_on()
            {
                on = true;
                cout << "    Projector on\n";
            }

            void power_off()
            {
                on = false;
                cout << "    Projector off\n";
            }

            void dim_lights(int level)
            {
                cout << "    Dimming lights to " << level << "%\n";
            }
        };

        // Subsystem 4: Screen
        struct Screen
        {
            bool down = false;

            void down_screen()
            {
                down = true;
                cout << "    Screen down\n";
            }

            void up_screen()
            {
                down = false;
                cout << "    Screen up\n";
            }
        };

        // FACADE: Home Theater - simplifies all components
        struct HomeTheater
        {
            Amplifier amplifier;
            DvdPlayer dvd;
            Projector projector;
            Screen screen;

            // Simple interface for complex operation
            void watch_movie(const string &movie)
            {
                cout << "  Getting ready to watch: " << movie << "\n";
                screen.down_screen();
                projector.dim_lights(10);
                amplifier.power_on();
                amplifier.set_volume(5);
                dvd.power_on();
                dvd.play(movie);
                projector.power_on();
            }

            void end_movie()
            {
                cout << "  Shutting down movie theater\n";
                dvd.stop();
                dvd.power_off();
                amplifier.power_off();
                projector.power_off();
                screen.up_screen();
            }
        };

    } // namespace home_theater

    // ============================================================================
    // EXAMPLE 2: Database Connection Facade
    // ============================================================================

    namespace database_facade
    {

        // Subsystem 1: Connection Pool
        struct ConnectionPool
        {
            int available_connections = 10;

            string get_connection()
            {
                available_connections--;
                return "Connection[" + to_string(11 - available_connections) + "]";
            }

            void release_connection(const string &conn)
            {
                available_connections++;
            }
        };

        // Subsystem 2: Query Parser
        struct QueryParser
        {
            string parse(const string &query)
            {
                return "Parsed: " + query;
            }
        };

        // Subsystem 3: Transaction Manager
        struct TransactionManager
        {
            bool in_transaction = false;

            void begin()
            {
                in_transaction = true;
                cout << "    Transaction started\n";
            }

            void commit()
            {
                in_transaction = false;
                cout << "    Transaction committed\n";
            }

            void rollback()
            {
                in_transaction = false;
                cout << "    Transaction rolled back\n";
            }
        };

        // FACADE: Database - simplifies complex operations
        struct Database
        {
            ConnectionPool pool;
            QueryParser parser;
            TransactionManager tm;

            // Simple query interface
            string query(const string &sql)
            {
                cout << "  Executing: " << sql << "\n";
                string conn = pool.get_connection();
                string parsed = parser.parse(sql);
                pool.release_connection(conn);
                return parsed + " (executed)";
            }

            // Simple transaction interface
            void execute_transaction(const vector<string> &queries)
            {
                cout << "  Running transaction\n";
                tm.begin();
                for (const auto &q : queries)
                {
                    query(q);
                }
                tm.commit();
            }
        };

    } // namespace database_facade

    // ============================================================================
    // EXAMPLE 3: HTTP API Facade
    // ============================================================================

    namespace http_facade
    {

        // Subsystem 1: Request Builder
        struct RequestBuilder
        {
            string method;
            string path;
            string body;

            string build()
            {
                return method + " " + path + " with body: " + body;
            }
        };

        // Subsystem 2: Client Session
        struct ClientSession
        {
            bool connected = false;

            void connect(const string &host)
            {
                connected = true;
                cout << "    Connected to " << host << "\n";
            }

            void disconnect()
            {
                connected = false;
                cout << "    Disconnected\n";
            }
        };

        // Subsystem 3: Response Parser
        struct ResponseParser
        {
            string parse(const string &response)
            {
                return "Parsed response: " + response;
            }
        };

        // FACADE: HTTP Client - simplifies API calls
        struct HttpClient
        {
            RequestBuilder request_builder;
            ClientSession session;
            ResponseParser parser;
            string host;

            HttpClient(const string &host) : host(host) {}

            string post(const string &path, const string &body)
            {
                cout << "  POST " << path << "\n";
                session.connect(host);
                request_builder.method = "POST";
                request_builder.path = path;
                request_builder.body = body;
                string response = "HTTP 200 OK";
                session.disconnect();
                return parser.parse(response);
            }

            string get(const string &path)
            {
                cout << "  GET " << path << "\n";
                session.connect(host);
                request_builder.method = "GET";
                request_builder.path = path;
                string response = "HTTP 200 OK";
                session.disconnect();
                return parser.parse(response);
            }
        };

    } // namespace http_facade

    // ============================================================================
    // EXAMPLE 4: Graphics Library Facade (similar to Bloom)
    // ============================================================================

    namespace graphics_facade
    {

        // Subsystem 1: Shader Manager
        struct ShaderManager
        {
            bool shader_compiled = false;

            void compile_shader(const string &src)
            {
                cout << "    Compiling shader\n";
                shader_compiled = true;
            }

            void use_shader()
            {
                cout << "    Using shader program\n";
            }
        };

        // Subsystem 2: Texture Manager
        struct TextureManager
        {
            int loaded_textures = 0;

            int load_texture(const string &file)
            {
                loaded_textures++;
                cout << "    Loading texture: " << file << "\n";
                return loaded_textures;
            }

            void bind_texture(int texture_id)
            {
                cout << "    Binding texture " << texture_id << "\n";
            }
        };

        // Subsystem 3: Vertex Buffer
        struct VertexBuffer
        {
            bool initialized = false;

            void create_buffer(const vector<float> &vertices)
            {
                cout << "    Creating vertex buffer with " << vertices.size()
                     << " vertices\n";
                initialized = true;
            }

            void bind()
            {
                cout << "    Binding vertex buffer\n";
            }
        };

        // Subsystem 4: Frame Buffer
        struct FrameBuffer
        {
            bool bound = false;

            void bind_fb()
            {
                bound = true;
                cout << "    Binding framebuffer\n";
            }

            void clear()
            {
                cout << "    Clearing framebuffer\n";
            }
        };

        // FACADE: Graphics Engine
        struct GraphicsEngine
        {
            ShaderManager shader_manager;
            TextureManager texture_manager;
            VertexBuffer vertex_buffer;
            FrameBuffer frame_buffer;

            void initialize()
            {
                cout << "  Initializing graphics engine\n";
                shader_manager.compile_shader("default.glsl");
                frame_buffer.bind_fb();
            }

            void draw(const vector<float> &vertices, const string &texture_file)
            {
                cout << "  Drawing geometry\n";
                frame_buffer.bind_fb();
                frame_buffer.clear();
                shader_manager.use_shader();
                int tex_id = texture_manager.load_texture(texture_file);
                texture_manager.bind_texture(tex_id);
                vertex_buffer.create_buffer(vertices);
                vertex_buffer.bind();
            }

            void teardown()
            {
                cout << "  Tearing down graphics engine\n";
            }
        };

    } // namespace graphics_facade

} // namespace facade_solution

// ============================================================================
// VIOLATION - WITHOUT FACADE (Complex Client Code)
// ============================================================================

namespace facade_violation
{

    namespace home_theater_bad
    {

        struct Amplifier
        {
            bool on = false;
            int volume = 0;
            void power_on() { on = true; }
            void power_off() { on = false; }
            void set_volume(int v) { volume = v; }
        };

        struct DvdPlayer
        {
            bool on = false;
            string movie;
            void power_on() { on = true; }
            void power_off() { on = false; }
            void play(const string &m) { movie = m; }
            void stop() {}
        };

        struct Projector
        {
            bool on = false;
            void power_on() { on = true; }
            void power_off() { on = false; }
            void dim_lights(int l) {}
        };

        struct Screen
        {
            bool down = false;
            void lower() { down = true; }
            void raise() { down = false; }
        };

        // WITHOUT FACADE - Client needs to manage ALL components
        void watch_movie_bad(Amplifier &amp, DvdPlayer &dvd,
                             Projector &proj, Screen &scr, const string &movie)
        {
            // Complex, error-prone client code
            scr.lower();
            proj.dim_lights(10);
            amp.power_on();
            amp.set_volume(5);
            dvd.power_on();
            dvd.play(movie);
            proj.power_on();
            // ... and cleanup is also client's responsibility!
        }

    } // namespace home_theater_bad

} // namespace facade_violation

#endif // FACADE_PATTERN_H
