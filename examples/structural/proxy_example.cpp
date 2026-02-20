#include <iostream>
#include <string>
#include <memory>

#include "structural/proxy.h"

using namespace std;
using namespace proxy_pattern;

// ============================================================================
// EXAMPLE 1: VIRTUAL PROXY (Lazy Loading Images)
// ============================================================================

void example1_virtual_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 1: VIRTUAL PROXY (Lazy Loading)\n";
    cout << string(70, '=') << "\n\n";

    cout << "Creating proxy objects (images NOT loaded yet):\n";
    virtual_proxy::ImageProxy image1("pokemon.png");
    virtual_proxy::ImageProxy image2("landscape.jpg");
    virtual_proxy::ImageProxy image3("portrait.png");

    cout << "\nFirst access to image1 (triggers loading):\n";
    image1.draw();

    cout << "\nSecond access to image1 (already loaded, no reload):\n";
    image1.draw();

    cout << "\nFirst access to image2 (triggers loading):\n";
    image2.draw();

    cout << "\nBENEFIT: Heavy image loading deferred until first use\n";
}

// ============================================================================
// EXAMPLE 2: PROTECTION PROXY (Access Control)
// ============================================================================

void example2_protection_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 2: PROTECTION PROXY (Access Control)\n";
    cout << string(70, '=') << "\n\n";

    auto real_db = make_shared<protection_proxy::RealDatabase>();

    cout << "User with 'guest' role:\n";
    protection_proxy::DatabaseProxy guest_db("guest", real_db);
    guest_db.read("SELECT * FROM users");   // Allowed
    guest_db.write("INSERT INTO users..."); // Denied
    guest_db.delete_data("DELETE FROM..."); // Denied

    cout << "\nUser with 'admin' role:\n";
    protection_proxy::DatabaseProxy admin_db("admin", real_db);
    admin_db.read("SELECT * FROM users");   // Allowed
    admin_db.write("INSERT INTO users..."); // Allowed
    admin_db.delete_data("DELETE FROM..."); // Allowed

    cout << "\nBENEFIT: Role-based access control transparent to client\n";
}

// ============================================================================
// EXAMPLE 3: LOGGING PROXY (Instrumentation)
// ============================================================================

void example3_logging_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 3: LOGGING PROXY (Instrumentation)\n";
    cout << string(70, '=') << "\n\n";

    logging_proxy::LoggingCalculatorProxy calc;

    cout << "Performing calculations:\n";
    int result1 = calc.add(10, 20);
    cout << "Result: " << result1 << "\n\n";

    int result2 = calc.multiply(5, 6);
    cout << "Result: " << result2 << "\n\n";

    int result3 = calc.subtract(100, 30);
    cout << "Result: " << result3 << "\n\n";

    cout << "Audit trail:\n";
    calc.print_log();
    cout << "\nTotal operations logged: " << calc.get_operation_count() << "\n";

    cout << "\nBENEFIT: Transparently logs all operations for debugging/auditing\n";
}

// ============================================================================
// EXAMPLE 4: CACHING PROXY
// ============================================================================

void example4_caching_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 4: CACHING PROXY\n";
    cout << string(70, '=') << "\n\n";

    caching_proxy::CachingComputeProxy compute;

    cout << "First call to compute_factorial(5):\n";
    int result1 = compute.compute_factorial(5);
    cout << "Result: " << result1 << "\n\n";

    cout << "Second call to compute_factorial(5) (cached):\n";
    int result2 = compute.compute_factorial(5);
    cout << "Result: " << result2 << "\n\n";

    cout << "First call to compute_factorial(10):\n";
    int result3 = compute.compute_factorial(10);
    cout << "Result: " << result3 << "\n\n";

    cout << "Cache statistics: " << compute.get_cache_size() << " items cached\n";

    cout << "\nBENEFIT: Expensive computations cached for subsequent accesses\n";
}

// ============================================================================
// EXAMPLE 5: SMART POINTER PROXY
// ============================================================================

void example5_smart_pointer_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 5: SMART POINTER PROXY (RAII)\n";
    cout << string(70, '=') << "\n\n";

    {
        cout << "Creating resource proxy:\n";
        smart_pointer_proxy::ResourceProxy proxy("DatabaseConnection");

        cout << "\nUsing resource:\n";
        proxy.use();
        proxy.use();
        proxy.use();

        cout << "\nRequest count: " << proxy.get_request_count() << "\n";

        cout << "\nResource ID: " << proxy.get_id() << "\n";

        cout << "\nProxy going out of scope...\n";
    }

    cout << "Resource automatically cleaned up!\n";
    cout << "\nBENEFIT: Automatic lifetime management (no manual delete needed)\n";
}

// ============================================================================
// EXAMPLE 6: PROPERTY PROXY
// ============================================================================

void example6_property_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 6: PROPERTY PROXY\n";
    cout << string(70, '=') << "\n\n";

    property_proxy::Creature creature;

    cout << "Initial creature stats:\n";
    creature.print_stats();

    cout << "\nSetting agility to 20:\n";
    creature.agility = 20;

    cout << "\nReading agility:\n";
    int agility_value = creature.agility;

    cout << "\nModifying strength:\n";
    creature.strength = 15;

    cout << "\nFinal stats:\n";
    creature.print_stats();

    cout << "\nBENEFIT: Properties control attribute access with logging/validation\n";
}

// ============================================================================
// EXAMPLE 7: REMOTE PROXY
// ============================================================================

void example7_remote_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 7: REMOTE PROXY (Network Transparency)\n";
    cout << string(70, '=') << "\n\n";

    remote_proxy::RemoteServiceProxy service("http://api.example.com:8080");

    cout << "Making remote calls (transparent):\n\n";

    cout << "Call 1: Getting user data:\n";
    string data1 = service.get_data("user:123");
    cout << "Response: " << data1 << "\n\n";

    cout << "Call 2: Updating configuration:\n";
    service.set_data("config:theme", "dark");

    cout << "\nCall 3: Getting configuration:\n";
    string data2 = service.get_data("config:theme");
    cout << "Response: " << data2 << "\n\n";

    cout << "Total network calls made: " << service.get_call_count() << "\n";

    cout << "\nBENEFIT: Remote objects accessed like local objects\n";
}

// ============================================================================
// EXAMPLE 8: SYNCHRONIZATION PROXY (Thread-Safe)
// ============================================================================

void example8_synchronization_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 8: SYNCHRONIZATION PROXY (Thread-Safe Access)\n";
    cout << string(70, '=') << "\n\n";

    synchronization_proxy::ThreadSafeCounterProxy counter;

    cout << "Initial value: " << counter.get_value() << "\n\n";

    cout << "Incrementing counter:\n";
    counter.increment();
    counter.increment();

    cout << "\nDecrementing counter:\n";
    counter.decrement();

    cout << "\nReading final value:\n";
    int final_value = counter.get_value();
    cout << "Final value: " << final_value << "\n";

    cout << "\nTotal synchronized accesses: " << counter.get_access_count() << "\n";

    cout << "\nBENEFIT: Thread-safe access without explicit locks in client code\n";
}

// ============================================================================
// EXAMPLE 9: COMPOSITE PROXY (Layered Multiple Proxies)
// ============================================================================

void example9_composite_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 9: COMPOSITE PROXY (Layered Protection)\n";
    cout << string(70, '=') << "\n\n";

    cout << "Building proxy chain: Logging -> Caching -> Real API\n\n";

    auto real_api = make_shared<composite_proxy::RealAPIClient>();
    auto cached_api = make_shared<composite_proxy::CachingAPIProxy>(real_api);
    composite_proxy::LoggingAPIProxy logged_api(cached_api);

    cout << "Request 1: /users/123\n";
    logged_api.fetch_data("/users/123");

    cout << "\nRequest 2: /users/456\n";
    logged_api.fetch_data("/users/456");

    cout << "\nRequest 3: /users/123 (cached)\n";
    logged_api.fetch_data("/users/123");

    cout << "\nRequest 4: /users/456 (cached)\n";
    logged_api.fetch_data("/users/456");

    cout << "\nRequest log:\n";
    int count = 1;
    for (const auto &endpoint : logged_api.get_log())
    {
        cout << "  " << count++ << ". " << endpoint << "\n";
    }

    cout << "\nBENEFIT: Multiple layers of functionality (logging + caching + real service)\n";
}

// ============================================================================
// COMPARISON & ANALYSIS
// ============================================================================

void example10_comparison()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 10: PROXY PATTERN ANALYSIS & BENEFITS\n";
    cout << string(70, '=') << "\n\n";

    cout << "PROXY TYPES & USE CASES:\n\n";

    cout << "1. VIRTUAL PROXY:\n";
    cout << "   - Use Case: Lazy loading expensive objects\n";
    cout << "   - Example: Loading large images only when displayed\n";
    cout << "   - Benefit: Faster application startup\n\n";

    cout << "2. PROTECTION PROXY:\n";
    cout << "   - Use Case: Access control based on permissions\n";
    cout << "   - Example: Database access with role-based restrictions\n";
    cout << "   - Benefit: Centralized authorization logic\n\n";

    cout << "3. LOGGING PROXY:\n";
    cout << "   - Use Case: Tracking object usage and operations\n";
    cout << "   - Example: Recording all calculator operations\n";
    cout << "   - Benefit: Debugging and auditing\n\n";

    cout << "4. CACHING PROXY:\n";
    cout << "   - Use Case: Caching expensive computation results\n";
    cout << "   - Example: Factorial computation caching\n";
    cout << "   - Benefit: Reduced computation time for repeated calls\n\n";

    cout << "5. REMOTE PROXY:\n";
    cout << "   - Use Case: Representing remote objects locally\n";
    cout << "   - Example: Web service client proxies\n";
    cout << "   - Benefit: Network operations transparent to client\n\n";

    cout << "6. SMART POINTER PROXY:\n";
    cout << "   - Use Case: Automatic memory management\n";
    cout << "   - Example: Using shared_ptr/unique_ptr\n";
    cout << "   - Benefit: No manual delete, exception safe\n\n";

    cout << "7. PROPERTY PROXY:\n";
    cout << "   - Use Case: Controlled attribute access\n";
    cout << "   - Example: Creature properties with logging\n";
    cout << "   - Benefit: Validation and notification on changes\n\n";

    cout << "8. SYNCHRONIZATION PROXY:\n";
    cout << "   - Use Case: Thread-safe object access\n";
    cout << "   - Example: Counter with automatic locking\n";
    cout << "   - Benefit: Simplified multithreading\n\n";

    cout << "9. COMPOSITE PROXY:\n";
    cout << "   - Use Case: Multiple layers of protection/functionality\n";
    cout << "   - Example: API client with logging and caching\n";
    cout << "   - Benefit: Extensible and composable proxies\n\n";

    cout << "KEY PROXY BENEFITS:\n";
    cout << "✓ Lazy initialization of expensive objects\n";
    cout << "✓ Transparent access control and authorization\n";
    cout << "✓ Automatic resource management\n";
    cout << "✓ Caching and performance optimization\n";
    cout << "✓ Logging and monitoring capabilities\n";
    cout << "✓ Remote object transparency\n";
    cout << "✓ Thread-safe access protection\n";
    cout << "✓ Separation of concerns\n\n";

    cout << "TRADE-OFFS TO CONSIDER:\n";
    cout << "✗ Additional layer of indirection\n";
    cout << "✗ Slightly slower object access\n";
    cout << "✗ More classes to maintain\n";
    cout << "✗ Complexity in proxy chains\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "\n"
         << string(70, '*') << "\n";
    cout << "PROXY PATTERN - COMPREHENSIVE EXAMPLES\n";
    cout << string(70, '*') << "\n";

    example1_virtual_proxy();
    example2_protection_proxy();
    example3_logging_proxy();
    example4_caching_proxy();
    example5_smart_pointer_proxy();
    example6_property_proxy();
    example7_remote_proxy();
    example8_synchronization_proxy();
    example9_composite_proxy();
    example10_comparison();

    cout << "\n"
         << string(70, '*') << "\n";
    cout << "END OF EXAMPLES\n";
    cout << string(70, '*') << "\n\n";

    return 0;
}
