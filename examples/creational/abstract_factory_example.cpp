#include <iostream>
#include "../include/creational/abstract_factory.h"

using namespace std;

int main()
{
    cout << "=== ABSTRACT FACTORY PATTERN ===" << endl
         << endl;

    // ========================================================================
    // VIOLATION: Hard-coded Object Creation
    // ========================================================================
    cout << "--- VIOLATION: Hard-coded Theme Creation ---" << endl;
    cout << "Problem: Switching themes requires changing code in many places" << endl;
    cout << "Must instantiate each product individually and manage consistency" << endl
         << endl;

    ButtonBad button_bad;
    TextBoxBad textbox_bad;
    cout << "Button:  ";
    button_bad.Draw();
    cout << "\nTextBox: ";
    textbox_bad.Draw();
    cout << "\n\n";

    // ========================================================================
    // SOLUTION 1: Windows Theme Factory
    // ========================================================================
    cout << "--- SOLUTION 1: Windows UI Factory ---" << endl;
    cout << "All Windows components created together, consistency guaranteed:" << endl
         << endl;

    WindowsUIFactory windows_factory;
    Application windows_app{windows_factory};
    windows_app.Draw();
    cout << endl;

    // ========================================================================
    // SOLUTION 2: macOS Theme Factory
    // ========================================================================
    cout << "--- SOLUTION 2: macOS UI Factory ---" << endl;
    cout << "Switch theme by changing factory, no other code changes:" << endl
         << endl;

    MacUIFactory mac_factory;
    Application mac_app{mac_factory};
    mac_app.Draw();
    cout << endl;

    // ========================================================================
    // SOLUTION 3: Linux Theme Factory
    // ========================================================================
    cout << "--- SOLUTION 3: Linux UI Factory ---" << endl;
    cout << "Seamless theme switching with same Application class:" << endl
         << endl;

    LinuxUIFactory linux_factory;
    Application linux_app{linux_factory};
    linux_app.Draw();
    cout << endl;

    // ========================================================================
    // SOLUTION 4: Runtime Factory Selection
    // ========================================================================
    cout << "--- SOLUTION 4: Runtime Factory Selection ---" << endl;
    cout << "Choose factory at runtime based on platform:" << endl
         << endl;

    std::string platform = "macos";

    std::unique_ptr<UIFactory> factory;

    if (platform == "windows")
    {
        factory = std::make_unique<WindowsUIFactory>();
        cout << "Using Windows theme\n";
    }
    else if (platform == "macos")
    {
        factory = std::make_unique<MacUIFactory>();
        cout << "Using macOS theme\n";
    }
    else
    {
        factory = std::make_unique<LinuxUIFactory>();
        cout << "Using Linux theme\n";
    }

    Application runtime_app{*factory};
    runtime_app.Draw();
    cout << endl
         << endl;

    // ========================================================================
    // SOLUTION 5: Database Factory Example
    // ========================================================================
    cout << "--- SOLUTION 5: Database Factory (Another Domain) ---" << endl;
    cout << "Same pattern applies to database abstraction:" << endl
         << endl;

    cout << "MySQL Setup:\n";
    MySQLFactory mysql_factory;
    DatabaseManager mysql_db{mysql_factory};
    mysql_db.Setup();
    mysql_db.ExecuteQuery("SELECT * FROM users");
    cout << endl;

    cout << "PostgreSQL Setup:\n";
    PostgreSQLFactory postgres_factory;
    DatabaseManager postgres_db{postgres_factory};
    postgres_db.Setup();
    postgres_db.ExecuteQuery("SELECT * FROM orders");
    cout << endl
         << endl;

    // ========================================================================
    // SOLUTION 6: Multiple Platforms at Once
    // ========================================================================
    cout << "--- SOLUTION 6: Multiple Applications with Different Themes ---" << endl;
    cout << "Create multiple themed applications simultaneously:" << endl
         << endl;

    cout << "App 1:\n";
    WindowsUIFactory factory1;
    Application app1{factory1};
    app1.Draw();

    cout << "\nApp 2:\n";
    MacUIFactory factory2;
    Application app2{factory2};
    app2.Draw();

    cout << "\nApp 3:\n";
    LinuxUIFactory factory3;
    Application app3{factory3};
    app3.Draw();

    cout << endl
         << endl;

    // ========================================================================
    // KEY BENEFITS SUMMARY
    // ========================================================================
    cout << "--- KEY BENEFITS OF ABSTRACT FACTORY PATTERN ---" << endl;
    cout << "✓ Consistency: Related products created together" << endl;
    cout << "✓ Loose Coupling: Application doesn't know concrete classes" << endl;
    cout << "✓ Easy Switching: Change factory = change all products" << endl;
    cout << "✓ Scalability: Add new families (themes) without modifying existing code" << endl;
    cout << "✓ Type Safety: Products guaranteed to be compatible" << endl;
    cout << "✓ Maintainability: Centralized object creation for product families" << endl;
    cout << "✓ Testability: Easy to create mock factories for testing" << endl;

    return 0;
}
