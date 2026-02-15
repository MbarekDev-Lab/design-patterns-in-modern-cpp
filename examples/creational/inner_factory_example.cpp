#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "../include/creational/inner_factory.h"

using namespace std;

int main()
{
    cout << "=== INNER FACTORY PATTERN ===" << endl
         << endl;

    // ========================================================================
    // VIOLATION: Static Factory Methods (Flat Organization)
    // ========================================================================
    cout << "--- VIOLATION: Flat Static Factories (Before) ---" << endl;
    cout << "All factory methods at class level - Hard to organize:" << endl;
    cout << "PointBad::NewCartesian(...)" << endl;
    cout << "PointBad::NewPolar(...)" << endl;
    cout << "PointBad::NewFromString(...)" << endl;
    cout << "Problem: No logical grouping, scales poorly with many factories" << endl
         << endl;

    auto pb1 = PointBad::NewCartesian(5, 10);
    auto pb2 = PointBad::NewPolar(5, M_PI_4);
    auto pb3 = PointBad::NewFromString("3.5,2.1");

    cout << "Result 1: " << pb1 << endl;
    cout << "Result 2: " << pb2 << endl;
    cout << "Result 3: " << pb3 << endl
         << endl;

    // ========================================================================
    // SOLUTION 1: Cartesian Factory (Nested Class)
    // ========================================================================
    cout << "--- SOLUTION 1: Cartesian Inner Factory ---" << endl;
    cout << "Organized factory class for rectangular coordinates:" << endl;
    cout << "Point::CartesianFactory::Create(x, y)" << endl
         << endl;

    auto p1 = Point::CartesianFactory::Create(5, 10);
    auto p2 = Point::CartesianFactory::Origin();
    auto p3 = Point::CartesianFactory::UnitX();
    auto p4 = Point::CartesianFactory::UnitY();

    cout << "Create(5, 10):   " << p1 << endl;
    cout << "Origin():        " << p2 << endl;
    cout << "UnitX():         " << p3 << endl;
    cout << "UnitY():         " << p4 << endl
         << endl;

    // ========================================================================
    // SOLUTION 2: Polar Factory (Nested Class)
    // ========================================================================
    cout << "--- SOLUTION 2: Polar Inner Factory ---" << endl;
    cout << "Specialized factory for polar/radial coordinates:" << endl;
    cout << "Point::PolarFactory::Create(r, theta)" << endl
         << endl;

    auto p5 = Point::PolarFactory::Create(5, M_PI_4);
    auto p6 = Point::PolarFactory::FromDegrees(10, 45);
    auto p7 = Point::PolarFactory::OnUnitCircle(M_PI / 6); // 30 degrees
    auto p8 = Point::PolarFactory::OnUnitCircle(M_PI_2);

    cout << "Create(5, π/4):           " << p5 << endl;
    cout << "FromDegrees(10, 45°):     " << p6 << endl;
    cout << "OnUnitCircle(π/6):        " << p7 << endl;
    cout << "OnUnitCircle(π/2):        " << p8 << endl
         << endl;

    // ========================================================================
    // SOLUTION 3: String Parser Factory
    // ========================================================================
    cout << "--- SOLUTION 3: String Parser Inner Factory ---" << endl;
    cout << "Different format parsers organized in one factory:" << endl
         << endl;

    auto p9 = Point::StringFactory::FromCSV("7.5,3.2");
    auto p10 = Point::StringFactory::FromSpaceSeparated("4.1 8.9");

    cout << "FromCSV(\"7.5,3.2\"):            " << p9 << endl;
    cout << "FromSpaceSeparated(\"4.1 8.9\"): " << p10 << endl
         << endl;

    // ========================================================================
    // SOLUTION 4: Advanced Factory (Complex Operations)
    // ========================================================================
    cout << "--- SOLUTION 4: Advanced Inner Factory ---" << endl;
    cout << "Complex geometric calculations:" << endl
         << endl;

    auto p11 = Point::AdvancedFactory::OnEllipse(5, 3, M_PI_4);
    auto p12 = Point::AdvancedFactory::OnEllipse(5, 3, M_PI_2);
    auto p13 = Point::AdvancedFactory::Lerp(Point::CartesianFactory::Create(0, 0),
                                            Point::CartesianFactory::Create(10, 10), 0.5f);

    cout << "OnEllipse(5, 3, π/4):     " << p11 << endl;
    cout << "OnEllipse(5, 3, π/2):     " << p12 << endl;
    cout << "Lerp((0,0), (10,10), 0.5):" << p13 << endl
         << endl;

    // ========================================================================
    // SOLUTION 5: Dynamic Factory with Type Registry
    // ========================================================================
    cout << "--- SOLUTION 5: Dynamic Type-Based Factory ---" << endl;
    cout << "Runtime selection of coordinate system:" << endl
         << endl;

    auto p14 = PointWithRegistry::Factory::Create(CoordinateSystem::Cartesian, 5, 10);
    auto p15 = PointWithRegistry::Factory::Create(CoordinateSystem::Polar, 5, M_PI_4);
    auto p16 = PointWithRegistry::Factory::Create(CoordinateSystem::Ellipse, 5, M_PI / 3, 3);

    cout << "Cartesian(5, 10):           " << p14 << endl;
    cout << "Polar(5, π/4):              " << p15 << endl;
    cout << "Ellipse(5, π/3, h=3):       " << p16 << endl
         << endl;

    // ========================================================================
    // BENEFITS SUMMARY
    // ========================================================================
    cout << "--- KEY BENEFITS OF INNER FACTORY PATTERN ---" << endl;
    cout << "✓ Organization: Factories grouped by responsibility" << endl;
    cout << "✓ Encapsulation: Each factory controls its creation logic" << endl;
    cout << "✓ Scalability: Easy to add new factories without cluttering main class" << endl;
    cout << "✓ Clarity: Intent is clear from factory class name" << endl;
    cout << "✓ Namespace: Factories don't pollute global space" << endl;
    cout << "✓ Extensibility: Can add new factories as nested classes" << endl;
    cout << "✓ Type Safety: Compile-time verification of factory calls" << endl;

    return 0;
}
