#include <iostream>
#include <memory>
#include "../include/di_container.h"

using namespace std;
using namespace di;

// ============================================================================
// EXAMPLE CLASSES - Similar to Boost DI Example
// ============================================================================

struct Engine
{
    float volume = 5.0f;
    int horse_power = 400;

    Engine() { cout << "Engine created\n"; }

    friend ostream &operator<<(ostream &os, const Engine &obj)
    {
        return os << "Engine(volume: " << obj.volume
                  << ", hp: " << obj.horse_power << ")";
    }
};

struct ILogger
{
    virtual ~ILogger() = default;
    virtual void Log(const string &s) = 0;
};

struct ConsoleLogger : ILogger
{
    ConsoleLogger() { cout << "ConsoleLogger created\n"; }

    void Log(const string &s) override
    {
        cout << "[LOG] " << s << "\n";
    }
};

struct FileLogger : ILogger
{
    FileLogger() { cout << "FileLogger created\n"; }

    void Log(const string &s) override
    {
        cout << "[FILE LOG] " << s << "\n";
    }
};

struct Car
{
    shared_ptr<Engine> engine;
    shared_ptr<ILogger> logger;

    Car(shared_ptr<Engine> engine, shared_ptr<ILogger> logger)
        : engine(engine), logger(logger)
    {
        logger->Log("Car created with dependencies");
    }

    void drive() const
    {
        logger->Log("Driving with " + to_string(engine->horse_power) + "hp");
    }

    friend ostream &operator<<(ostream &os, const Car &obj)
    {
        return os << "Car with " << *obj.engine;
    }
};

// ============================================================================
// EXAMPLE 1: Manual Dependency Injection (What DI Container Avoids)
// ============================================================================

void example_1_manual_di()
{
    cout << "\n========== EXAMPLE 1: Manual Dependency Injection ==========\n";

    cout << "Creating dependencies manually:\n";
    auto engine = make_shared<Engine>();
    auto logger = make_shared<ConsoleLogger>();
    auto car = make_shared<Car>(engine, logger);

    cout << "\nUsing car:\n";
    car->drive();

    cout << "\nProblem: Tedious to manage all dependencies!\n";
}

// ============================================================================
// EXAMPLE 2: Using DI Container - Cleaner Approach
// ============================================================================

void example_2_di_container()
{
    cout << "\n========== EXAMPLE 2: Using DI Container ==========\n";

    cout << "Setting up DI container:\n";

    // Create injector with bindings
    Injector injector;

    // Bind interfaces to implementations
    injector.bind(
        bind<ILogger>().to<ConsoleLogger>());
    injector.bind(
        bind<Engine>().to<Engine>());

    cout << "\nCreating Car with automatic dependency resolution:\n";
    auto logger = injector.create<ConsoleLogger>();
    auto engine = injector.create<Engine>();
    auto car = make_shared<Car>(engine, logger);

    cout << "\nUsing car:\n";
    car->drive();

    cout << "\nBenefit: Container manages all dependency creation!\n";
}

// ============================================================================
// EXAMPLE 3: Singleton Pattern with DI
// ============================================================================

void example_3_singleton_in_di()
{
    cout << "\n========== EXAMPLE 3: Singleton with DI Container ==========\n";

    Injector injector;

    cout << "Getting singleton instances:\n";
    auto logger1 = injector.get_singleton<ConsoleLogger>();
    auto logger2 = injector.get_singleton<ConsoleLogger>();

    cout << "logger1 address: " << logger1.get() << "\n";
    cout << "logger2 address: " << logger2.get() << "\n";

    if (logger1.get() == logger2.get())
    {
        cout << "✓ Both references point to same instance (singleton)\n";
    }
}

// ============================================================================
// EXAMPLE 4: Multiple Implementations
// ============================================================================

void example_4_multiple_implementations()
{
    cout << "\n========== EXAMPLE 4: Swappable Implementations ==========\n";

    cout << "Setup 1: Using ConsoleLogger\n";
    {
        Injector injector1;
        injector1.bind(bind<ILogger>().to<ConsoleLogger>());

        auto logger = injector1.create<ConsoleLogger>();
        auto engine = injector1.create<Engine>();
        auto car = make_shared<Car>(engine, logger);
        car->drive();
    }

    cout << "\nSetup 2: Using FileLogger (different implementation)\n";
    {
        Injector injector2;
        injector2.bind(bind<ILogger>().to<FileLogger>());

        auto logger = injector2.create<FileLogger>();
        auto engine = injector2.create<Engine>();
        auto car = make_shared<Car>(engine, logger);
        car->drive();
    }

    cout << "\nBenefit: Same Car code works with different loggers!\n";
}

// ============================================================================
// EXAMPLE 5: Constructor-Based Dependency Resolution
// ============================================================================

void example_5_constructor_injection()
{
    cout << "\n========== EXAMPLE 5: Constructor-Based Injection ==========\n";

    cout << "Scenario: Car requires Engine and ILogger in constructor\n";
    cout << "DI Container automatically resolves these dependencies\n\n";

    Injector injector;

    auto logger = injector.create<ConsoleLogger>();
    auto engine = injector.create<Engine>();

    cout << "Creating Car with resolved dependencies:\n";
    auto car = make_shared<Car>(engine, logger);

    cout << "✓ All dependencies elegantly injected!\n";
}

// ============================================================================
// EXAMPLE 6: Real-world URL-based Dependency Example
// ============================================================================

struct IDataFetcher
{
    virtual ~IDataFetcher() = default;
    virtual string fetch(const string &url) = 0;
};

struct HttpDataFetcher : IDataFetcher
{
    string fetch(const string &url) override
    {
        return "HTTP fetched from: " + url;
    }
};

struct MockDataFetcher : IDataFetcher
{
    string fetch(const string &url) override
    {
        return "Mock data for: " + url;
    }
};

struct ApiClient
{
    shared_ptr<IDataFetcher> fetcher;

    ApiClient(shared_ptr<IDataFetcher> fetcher)
        : fetcher(fetcher) {}

    void get_data(const string &url) const
    {
        cout << "Client: " << fetcher->fetch(url) << "\n";
    }
};

void example_6_real_world()
{
    cout << "\n========== EXAMPLE 6: Real-world API Client ==========\n";

    cout << "Production setup (HTTP fetcher):\n";
    {
        Injector injector;
        auto fetcher = make_shared<HttpDataFetcher>();
        auto client = make_shared<ApiClient>(fetcher);
        client->get_data("https://api.example.com/data");
    }

    cout << "\nTest setup (Mock fetcher):\n";
    {
        Injector injector;
        auto fetcher = make_shared<MockDataFetcher>();
        auto client = make_shared<ApiClient>(fetcher);
        client->get_data("https://api.example.com/data");
    }

    cout << "\nBenefit: Easy to test with mock implementations!\n";
}

// ============================================================================
// EXAMPLE 7: Comparison - Manual vs DI Container
// ============================================================================

void example_7_comparison()
{
    cout << "\n========== EXAMPLE 7: Manual vs DI Container ==========\n";

    cout << "MANUAL APPROACH:\n";
    cout << "  Code: auto logger = make_shared<ConsoleLogger>();\n";
    cout << "  Code: auto engine = make_shared<Engine>();\n";
    cout << "  Code: auto car = make_shared<Car>(engine, logger);\n";
    cout << "  Issues:\n";
    cout << "    - Repetitive boilerplate\n";
    cout << "    - Hard to manage complex graphs\n";
    cout << "    - Difficult to test with mocks\n";
    cout << "    - Changes require code modifications\n\n";

    cout << "DI CONTAINER APPROACH:\n";
    cout << "  Code: injector.bind(bind<ILogger>().to<ConsoleLogger>());\n";
    cout << "  Code: auto car = injector.create<Car>();\n";
    cout << "  Benefits:\n";
    cout << "    - Automatic dependency resolution\n";
    cout << "    - Handles complex object graphs\n";
    cout << "    - Easy to swap implementations for testing\n";
    cout << "    - Centralizes configuration\n";
}

// ============================================================================
// Main Entry Point
// ============================================================================

int main()
{
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║      DEPENDENCY INJECTION CONTAINER PATTERN               ║\n";
    cout << "║  Custom Implementation (inspired by Boost DI)             ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";

    example_1_manual_di();
    example_2_di_container();
    example_3_singleton_in_di();
    example_4_multiple_implementations();
    example_5_constructor_injection();
    example_6_real_world();
    example_7_comparison();

    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                    KEY TAKEAWAYS                           ║\n";
    cout << "├────────────────────────────────────────────────────────────┤\n";
    cout << "│ 1. DI Container automates dependency resolution            │\n";
    cout << "│ 2. Promotes loose coupling and testability                 │\n";
    cout << "│ 3. Simplifies managing complex object graphs               │\n";
    cout << "│ 4. Makes it easy to swap implementations (mocking)         │\n";
    cout << "│ 5. Centralizes configuration in one place                  │\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    return 0;
}
