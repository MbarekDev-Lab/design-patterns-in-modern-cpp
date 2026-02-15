#include <iostream>
#include "../include/creational/factory_method.h"

using namespace std;

int main()
{
    cout << "=== FACTORY METHOD PATTERN ===" << endl
         << endl;

    // ========================================================================
    // VIOLATION: Constructor Overloading Ambiguity
    // ========================================================================
    cout << "--- VIOLATION: Ambiguous Constructor (Before) ---" << endl;
    cout << "Problem: PointBad(x, y) - unclear if cartesian or polar coordinates" << endl;
    cout << "Can't add overload without type parameter conflicting with existing constructor" << endl
         << endl;

    PointBad pb1{5, 10};    // Cartesian? Polar? Unknown!
    PointBad pb2{5, 3.14f}; // Completely unclear!

    cout << "Result 1: " << pb1 << " (Intent unclear)" << endl;
    cout << "Result 2: " << pb2 << " (Intent unclear)" << endl
         << endl;

    // ========================================================================
    // SOLUTION 1: Point Factory with Coordinate Systems
    // ========================================================================
    cout << "--- SOLUTION 1: Factory Methods (Clear Intent) ---" << endl;
    cout << "Point::NewCartesian(5, 10)  // Explicit: X=5, Y=10" << endl;
    cout << "Point::NewPolar(5, PI/4)    // Explicit: R=5, Theta=PI/4" << endl
         << endl;

    auto p1 = Point::NewCartesian(5, 10);
    cout << "Cartesian(5, 10): " << p1 << endl;

    auto p2 = Point::NewPolar(5, M_PI_4); // r=5, theta=45°
    cout << "Polar(5, π/4):    " << p2 << endl;

    auto p3 = Point::NewPolar(10, M_PI_2); // r=10, theta=90°
    cout << "Polar(10, π/2):   " << p3 << endl
         << endl;

    // ========================================================================
    // SOLUTION 2: Color Factory with Predefined Colors
    // ========================================================================
    cout << "--- SOLUTION 2: Predefined Object Factory ---" << endl;
    cout << "Factory methods return common instances with meaningful names:" << endl
         << endl;

    auto red = Color::Red();
    auto green = Color::Green();
    auto blue = Color::Blue();
    auto white = Color::White();

    cout << "Red:   " << red << endl;
    cout << "Green: " << green << endl;
    cout << "Blue:  " << blue << endl;
    cout << "White: " << white << endl
         << endl;

    // ========================================================================
    // SOLUTION 3: Color from Hex String
    // ========================================================================
    cout << "--- SOLUTION 3: Factory from Alternative Format ---" << endl;
    cout << "FromHex(\"FF5733\") creates custom color from hex string:" << endl
         << endl;

    auto custom1 = Color::FromHex("FF5733");
    auto custom2 = Color::FromHex("33FF57");
    auto custom3 = Color::FromHex("3357FF");

    cout << "FromHex(\"FF5733\"): " << custom1 << endl;
    cout << "FromHex(\"33FF57\"): " << custom2 << endl;
    cout << "FromHex(\"3357FF\"): " << custom3 << endl
         << endl;

    // ========================================================================
    // SOLUTION 4: Complex Factory - Database Connections
    // ========================================================================
    cout << "--- SOLUTION 4: Complex Factory with Configuration ---" << endl;
    cout << "Different database types require different connection logic:" << endl
         << endl;

    auto mysql_db = DatabaseConnection::MySQL("localhost", 3306, "myapp");
    auto postgres_db = DatabaseConnection::PostgreSQL("db.example.com", 5432, "analytics");
    auto sqlite_db = DatabaseConnection::SQLite("./data.db");

    cout << "MySQL:      " << mysql_db << endl;
    cout << "PostgreSQL: " << postgres_db << endl;
    cout << "SQLite:     " << sqlite_db << endl
         << endl;

    // ========================================================================
    // KEY BENEFITS SUMMARY
    // ========================================================================
    cout << "--- KEY BENEFITS OF FACTORY METHOD PATTERN ---" << endl;
    cout << "✓ Clear Intent: Method names describe what inputs mean" << endl;
    cout << "✓ No Ambiguity: Compiler knows exactly which factory to call" << endl;
    cout << "✓ Encapsulation: Private constructor prevents direct instantiation" << endl;
    cout << "✓ Polymorphism: Can return different types based on parameters" << endl;
    cout << "✓ Validation: Factory can validate inputs before creating objects" << endl;
    cout << "✓ Predefined Objects: Common instances available as named factories" << endl;
    cout << "✓ Complex Logic: Construction logic hidden from caller" << endl;

    return 0;
}
