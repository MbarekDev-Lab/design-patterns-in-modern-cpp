#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

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

    // PROBLEM: Constructor ambiguity - which float, float constructor should be used?
    // For cartesian: (x, y)
    // For polar: (r, theta)
    // Compiler can't distinguish intent!
    PointBad(float a, float b)
        : x{a}, y{b}
    {
        // Is this cartesian or polar? Unclear!
    }

    // PROBLEM: We can't even add a second constructor with same signature
    // PointBad(float a, float b, PointTypeBad type)  // Would need type param
    // But then callers might forget type or mix them up

    friend std::ostream &operator<<(std::ostream &os, const PointBad &obj)
    {
        return os << "x: " << obj.x << " y: " << obj.y;
    }
};

// ============================================================================
// SOLUTION: Factory Method Pattern
// ============================================================================

enum class PointType
{
    cartesian,
    polar
};

class Point
{
private:
    float x, y;

    // SOLUTION: Private constructor prevents ambiguous direct instantiation
    Point(float x, float y) : x{x}, y{y} {}

public:
    float get_x() const { return x; }
    float get_y() const { return y; }

    // SOLUTION: Static factory methods with clear intent
    // Each method name describes exactly what inputs mean

    static Point NewCartesian(float x, float y)
    {
        return Point{x, y};
    }

    static Point NewPolar(float r, float theta)
    {
        // Convert from polar to cartesian
        return Point{r * cos(theta), r * sin(theta)};
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "Point(x: " << x << ", y: " << y << ")";
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &obj)
    {
        return os << obj.to_string();
    }
};

// ============================================================================
// ADDITIONAL EXAMPLES: Complex Object Creation
// ============================================================================

class Color
{
private:
    int r, g, b;

    Color(int r, int g, int b) : r{r}, g{g}, b{b} {}

public:
    int get_r() const { return r; }
    int get_g() const { return g; }
    int get_b() const { return b; }

    // Factory methods for common colors
    static Color Red() { return Color{255, 0, 0}; }
    static Color Green() { return Color{0, 255, 0}; }
    static Color Blue() { return Color{0, 0, 255}; }
    static Color White() { return Color{255, 255, 255}; }
    static Color Black() { return Color{0, 0, 0}; }

    // Factory method from hex string
    static Color FromHex(const std::string &hex)
    {
        // Simple hex parsing: "RRGGBB"
        int value = std::stoi(hex, nullptr, 16);
        int r = (value >> 16) & 0xFF;
        int g = (value >> 8) & 0xFF;
        int b = value & 0xFF;
        return Color{r, g, b};
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "RGB(" << r << ", " << g << ", " << b << ")";
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const Color &obj)
    {
        return os << obj.to_string();
    }
};

// ============================================================================
// COMPLEX FACTORY: Database Connection
// ============================================================================

class DatabaseConnection
{
private:
    std::string connection_string;
    std::string db_type;

    DatabaseConnection(const std::string &type, const std::string &conn_str)
        : db_type{type}, connection_string{conn_str} {}

public:
    const std::string &get_type() const { return db_type; }
    const std::string &get_connection_string() const { return connection_string; }

    // Factory methods for different database types
    static DatabaseConnection MySQL(const std::string &host, int port, const std::string &db)
    {
        std::string conn = "mysql://" + host + ":" + std::to_string(port) + "/" + db;
        return DatabaseConnection{"MySQL", conn};
    }

    static DatabaseConnection PostgreSQL(const std::string &host, int port, const std::string &db)
    {
        std::string conn = "postgresql://" + host + ":" + std::to_string(port) + "/" + db;
        return DatabaseConnection{"PostgreSQL", conn};
    }

    static DatabaseConnection SQLite(const std::string &filename)
    {
        std::string conn = "sqlite://" + filename;
        return DatabaseConnection{"SQLite", conn};
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "[" << db_type << "] " << connection_string;
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const DatabaseConnection &obj)
    {
        return os << obj.to_string();
    }
};
