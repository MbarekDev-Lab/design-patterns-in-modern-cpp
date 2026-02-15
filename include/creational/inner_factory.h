#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>

// ============================================================================
// VIOLATION: Static Factory Methods Only
// ============================================================================

class PointBad
{
private:
    float x, y;

    PointBad(float x, float y) : x{x}, y{y} {}

public:
    float get_x() const { return x; }
    float get_y() const { return y; }

    // PROBLEM: Factory methods cluttered at class level
    // PROBLEM: No logical grouping or organization
    // PROBLEM: Hard to extend with more factory types

    static PointBad NewCartesian(float x, float y)
    {
        return PointBad{x, y};
    }

    static PointBad NewPolar(float r, float theta)
    {
        return PointBad{r * cos(theta), r * sin(theta)};
    }

    static PointBad NewFromString(const std::string &str)
    {
        // Parse "x,y" format
        size_t comma = str.find(',');
        float x = std::stof(str.substr(0, comma));
        float y = std::stof(str.substr(comma + 1));
        return PointBad{x, y};
    }

    friend std::ostream &operator<<(std::ostream &os, const PointBad &obj)
    {
        return os << "Point(x: " << obj.x << ", y: " << obj.y << ")";
    }
};

// ============================================================================
// SOLUTION: Inner Factory Pattern - Nested Factory Classes
// ============================================================================

class Point
{
private:
    float x, y;

    Point(float x, float y) : x{x}, y{y} {}

public:
    float get_x() const { return x; }
    float get_y() const { return y; }

    friend std::ostream &operator<<(std::ostream &os, const Point &obj)
    {
        return os << "Point(x: " << obj.x << ", y: " << obj.y << ")";
    }

    // ========================================================================
    // SOLUTION: Inner Factory Classes - Organized and Extensible
    // ========================================================================

    // Cartesian Factory - handles rectangular/x-y coordinates
    class CartesianFactory
    {
    public:
        static Point Create(float x, float y)
        {
            return Point{x, y};
        }

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

    // Polar Factory - handles radial/angular coordinates
    class PolarFactory
    {
    public:
        static Point Create(float r, float theta)
        {
            return Point{r * cos(theta), r * sin(theta)};
        }

        static Point FromDegrees(float r, float degrees)
        {
            float radians = degrees * M_PI / 180.0f;
            return Point{r * cos(radians), r * sin(radians)};
        }

        static Point OnUnitCircle(float theta)
        {
            return Point{cos(theta), sin(theta)};
        }
    };

    // String Parser Factory
    class StringFactory
    {
    public:
        // Parse "x,y" format
        static Point FromCSV(const std::string &csv)
        {
            size_t comma = csv.find(',');
            float x = std::stof(csv.substr(0, comma));
            float y = std::stof(csv.substr(comma + 1));
            return Point{x, y};
        }

        // Parse "x y" format (space-separated)
        static Point FromSpaceSeparated(const std::string &str)
        {
            size_t space = str.find(' ');
            float x = std::stof(str.substr(0, space));
            float y = std::stof(str.substr(space + 1));
            return Point{x, y};
        }
    };

    // Advanced Factory - for complex scenarios
    class AdvancedFactory
    {
    public:
        // Create point at intersection of two circles
        static Point CircleIntersection(float r1, float r2, float distance)
        {
            // Simplified: intersect two circles centered at origin and (distance, 0)
            float a = (distance * distance + r1 * r1 - r2 * r2) / (2 * distance);
            float h = sqrt(r1 * r1 - a * a);
            return Point{a, h};
        }

        // Create point at specific angle on ellipse
        static Point OnEllipse(float a, float b, float theta)
        {
            return Point{a * cos(theta), b * sin(theta)};
        }

        // Interpolate between two points
        static Point Lerp(const Point &p1, const Point &p2, float t)
        {
            return Point{
                p1.x + (p2.x - p1.x) * t,
                p1.y + (p2.y - p1.y) * t};
        }
    };
};

// ============================================================================
// ALTERNATIVE: Dynamic Factory with Type Registry
// ============================================================================

enum class CoordinateSystem
{
    Cartesian,
    Polar,
    Ellipse
};

class PointWithRegistry
{
private:
    float x, y;

    PointWithRegistry(float x, float y) : x{x}, y{y} {}

public:
    float get_x() const { return x; }
    float get_y() const { return y; }

    // Factory registry - can be extended at runtime
    class Factory
    {
    public:
        static PointWithRegistry Create(CoordinateSystem system, float a, float b, float c = 0)
        {
            switch (system)
            {
            case CoordinateSystem::Cartesian:
                return PointWithRegistry{a, b};

            case CoordinateSystem::Polar:
                return PointWithRegistry{a * cos(b), a * sin(b)};

            case CoordinateSystem::Ellipse:
                return PointWithRegistry{a * cos(b), c * sin(b)};

            default:
                return PointWithRegistry{0, 0};
            }
        }
    };

    friend std::ostream &operator<<(std::ostream &os, const PointWithRegistry &obj)
    {
        return os << "Point(x: " << obj.x << ", y: " << obj.y << ")";
    }
};
