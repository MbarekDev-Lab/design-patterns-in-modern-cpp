#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <sstream>

using namespace std;

// ============================================================================
// VIOLATION: Constructor Overloading Ambiguity
// ============================================================================

enum class PointTypeBad
{
    cartesian,
    polar
};

class PointBad
{
public:
    float x, y;

    // PROBLEM: Constructor ambiguity
    // Same signature (float, float) could mean cartesian or polar
    // Caller must always remember which is which
    PointBad(float a, float b)
        : x{a}, y{b}
    {
    }

    friend ostream &operator<<(ostream &os, const PointBad &obj)
    {
        return os << "Point(x: " << obj.x << ", y: " << obj.y << ")";
    }
};

// ============================================================================
// SOLUTION: Factory Method with Friend Class
// ============================================================================

enum class PointType
{
    cartesian,
    polar
};

class Point
{
    float x, y;

    // SOLUTION: Private constructor prevents direct instantiation
    Point(float x, float y) : x{x}, y{y} {}

public:
    float get_x() const { return x; }
    float get_y() const { return y; }

    // Friend class gets access to private constructor
    friend class PointFactory;

    friend ostream &operator<<(ostream &os, const Point &obj)
    {
        return os << "Point(x: " << obj.x << ", y: " << obj.y << ")";
    }
};

// SOLUTION: Factory class with clear intent methods
class PointFactory
{
public:
    // Factory method for cartesian coordinates
    static Point NewCartesian(float x, float y)
    {
        return Point{x, y};
    }

    // Factory method for polar coordinates
    static Point NewPolar(float r, float theta)
    {
        return Point{r * cos(theta), r * sin(theta)};
    }

    // Additional factory methods
    static Point Origin()
    {
        return Point{0, 0};
    }

    static Point UnitX()
    {
        return Point{1, 0};
    }

    static Point UnitY()
    {
        return Point{0, 1};
    }
};

int main()
{
    cout << "=== FACTORY METHOD PATTERN (Friend Class Approach) ===" << endl
         << endl;

    // ========================================================================
    // VIOLATION: Ambiguous Constructor
    // ========================================================================
    cout << "--- VIOLATION: Ambiguous Constructor ---" << endl;
    cout << "Problem: PointBad(5, 10) - unclear if cartesian or polar" << endl;
    cout << "Result: ";

    PointBad pb1{5, 10};
    cout << pb1 << endl;

    cout << "       : PointBad(5, 3.14) - even more confusing!" << endl;
    cout << "Result: ";

    PointBad pb2{5, 3.14f};
    cout << pb2 << endl
         << endl;

    // ========================================================================
    // SOLUTION 1: Simple Cartesian Factory
    // ========================================================================
    cout << "--- SOLUTION 1: Cartesian Factory ---" << endl;
    cout << "Clear intent: NewCartesian(5, 10)" << endl;

    auto p1 = PointFactory::NewCartesian(5, 10);
    cout << "Result: " << p1 << endl
         << endl;

    // ========================================================================
    // SOLUTION 2: Polar Factory with Angle Conversion
    // ========================================================================
    cout << "--- SOLUTION 2: Polar Factory ---" << endl;
    cout << "Clear intent: NewPolar(r, theta)" << endl;
    cout << "Automatic conversion from polar to cartesian:" << endl;

    auto p2 = PointFactory::NewPolar(5, M_PI_4); // r=5, theta=45°
    cout << "NewPolar(5, π/4):   " << p2 << endl;

    auto p3 = PointFactory::NewPolar(10, M_PI_2); // r=10, theta=90°
    cout << "NewPolar(10, π/2):  " << p3 << endl;

    auto p4 = PointFactory::NewPolar(7, M_PI); // r=7, theta=180°
    cout << "NewPolar(7, π):     " << p4 << endl
         << endl;

    // ========================================================================
    // SOLUTION 3: Common Point Factories
    // ========================================================================
    cout << "--- SOLUTION 3: Named Point Factories ---" << endl;
    cout << "Convenient named factories for common points:" << endl;

    auto origin = PointFactory::Origin();
    auto unit_x = PointFactory::UnitX();
    auto unit_y = PointFactory::UnitY();

    cout << "Origin():  " << origin << endl;
    cout << "UnitX():   " << unit_x << endl;
    cout << "UnitY():   " << unit_y << endl
         << endl;

    // ========================================================================
    // SOLUTION 4: Multiple Conversions
    // ========================================================================
    cout << "--- SOLUTION 4: Complex Polar Coordinates ---" << endl;

    auto p5 = PointFactory::NewPolar(1, 0);            // (1, 0)
    auto p6 = PointFactory::NewPolar(1, M_PI / 2);     // (0, 1)
    auto p7 = PointFactory::NewPolar(1, M_PI);         // (-1, 0)
    auto p8 = PointFactory::NewPolar(1, 3 * M_PI / 2); // (0, -1)

    cout << "Unit circle points:" << endl;
    cout << "  0°:   " << p5 << endl;
    cout << "  90°:  " << p6 << endl;
    cout << "  180°: " << p7 << endl;
    cout << "  270°: " << p8 << endl
         << endl;

    // ========================================================================
    // SOLUTION 5: Mixed Creation
    // ========================================================================
    cout << "--- SOLUTION 5: Creating Multiple Points ---" << endl;

    Point manhattan[4] = {
        PointFactory::NewCartesian(0, 0),
        PointFactory::NewCartesian(5, 0),
        PointFactory::NewCartesian(5, 5),
        PointFactory::NewCartesian(0, 5)};

    cout << "Rectangle corners:" << endl;
    for (int i = 0; i < 4; ++i)
    {
        cout << "  Corner " << (i + 1) << ": " << manhattan[i] << endl;
    }

    cout << endl;

    Point circle[4] = {
        PointFactory::NewPolar(5, 0),
        PointFactory::NewPolar(5, M_PI / 2),
        PointFactory::NewPolar(5, M_PI),
        PointFactory::NewPolar(5, 3 * M_PI / 2)};

    cout << "Circle points (radius 5):" << endl;
    for (int i = 0; i < 4; ++i)
    {
        cout << "  Point " << (i + 1) << ": " << circle[i] << endl;
    }

    cout << endl
         << endl;

    // ========================================================================
    // KEY BENEFITS
    // ========================================================================
    cout << "--- KEY BENEFITS OF FACTORY METHOD PATTERN ---" << endl;
    cout << "✓ Clear Intent: Method name describes coordinate system" << endl;
    cout << "✓ No Ambiguity: Compiler knows exactly what's being created" << endl;
    cout << "✓ Encapsulation: Private constructor prevents misuse" << endl;
    cout << "✓ Conversion Logic: Hidden in factory, not in constructor" << endl;
    cout << "✓ Consistency: All Point creation goes through factory" << endl;
    cout << "✓ Named Factories: Origin(), UnitX(), UnitY() for common cases" << endl;
    cout << "✓ Type Safety: Compiler ensures correct usage" << endl;

    return 0;
}
