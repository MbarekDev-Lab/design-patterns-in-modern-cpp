#ifndef DECORATOR_PATTERN_H
#define DECORATOR_PATTERN_H

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <sstream>
#include <functional>
#include <cstdint>

using namespace std;

// ============================================================================
// DECORATOR PATTERN
// ============================================================================
//
// INTENT:
//   Attach additional responsibilities to an object dynamically.
//   Decorators provide a flexible alternative to subclassing for extending
//   functionality.
//
// KEY CONCEPTS:
//   1. Component - The object to which functionality is added
//   2. Decorator - Wraps the component, adding behavior
//   3. Dynamic - Behavior added at runtime, not compile-time
//   4. Transparent - Client often unaware of decoration
//
// BENEFITS:
//   ✓ Single Responsibility - each decorator adds one responsibility
//   ✓ Open-Closed Principle - open for extension, closed for modification
//   ✓ Flexible composition - combine decorators in any order
//   ✓ Avoid class explosion - no ColoredSquare, TransparentSquare, etc.
//   ✓ Runtime flexibility - add/remove decorators at runtime
//
// APPROACHES:
//   1. Wrapper/Composition - holding references/pointers to components
//   2. Mixin Inheritance - template inheritance patterns
//   3. Function Decoration - decorating callables and functions

namespace decorator_solution
{

    // ============================================================================
    // APPROACH 1: WRAPPER/COMPOSITION DECORATORS (Dynamic Decorators)
    // ============================================================================

    namespace wrapper
    {

        // Base component
        struct Shape
        {
            virtual string str() const = 0;
            virtual ~Shape() = default;
        };

        // Concrete components
        struct Circle : Shape
        {
            float radius;

            Circle() : radius(0) {}

            explicit Circle(float radius) : radius(radius) {}

            void resize(float factor)
            {
                radius *= factor;
            }

            string str() const override
            {
                ostringstream oss;
                oss << "A circle of radius " << radius;
                return oss.str();
            }
        };

        struct Square : Shape
        {
            float side;

            Square() : side(0) {}

            explicit Square(float side) : side(side) {}

            string str() const override
            {
                ostringstream oss;
                oss << "A square with side " << side;
                return oss.str();
            }
        };

        struct Rectangle : Shape
        {
            float width, height;

            Rectangle() : width(0), height(0) {}

            Rectangle(float w, float h) : width(w), height(h) {}

            string str() const override
            {
                ostringstream oss;
                oss << "A rectangle " << width << "x" << height;
                return oss.str();
            }
        };

        // Decorators - wrap existing shapes
        struct ColoredShape : Shape
        {
            Shape &shape;
            string color;

            ColoredShape(Shape &shape, const string &color)
                : shape(shape), color(color) {}

            string str() const override
            {
                ostringstream oss;
                oss << shape.str() << " has the color " << color;
                return oss.str();
            }
        };

        struct TransparentShape : Shape
        {
            Shape &shape;
            uint8_t transparency;

            TransparentShape(Shape &shape, uint8_t transparency)
                : shape(shape), transparency(transparency) {}

            string str() const override
            {
                ostringstream oss;
                oss << shape.str() << " has "
                    << (static_cast<float>(transparency) / 255.0f * 100.0f)
                    << "% transparency";
                return oss.str();
            }
        };

        struct BorderShape : Shape
        {
            Shape &shape;
            string border_char;

            BorderShape(Shape &shape, const string &border_char = "*")
                : shape(shape), border_char(border_char) {}

            string str() const override
            {
                ostringstream oss;
                oss << border_char << " " << shape.str() << " " << border_char;
                return oss.str();
            }
        };

    } // namespace wrapper

    // ============================================================================
    // APPROACH 2: MIXIN INHERITANCE (Template-based Decorators)
    // ============================================================================

    namespace mixin
    {

        struct Shape
        {
            virtual string str() const = 0;
            virtual ~Shape() = default;
        };

        struct Circle : Shape
        {
            float radius;

            Circle() : radius(0) {}

            explicit Circle(float radius) : radius(radius) {}

            string str() const override
            {
                ostringstream oss;
                oss << "A circle of radius " << radius;
                return oss.str();
            }
        };

        struct Square : Shape
        {
            float side;

            Square() : side(0) {}

            explicit Square(float side) : side(side) {}

            string str() const override
            {
                ostringstream oss;
                oss << "A square with side " << side;
                return oss.str();
            }
        };

        // ColoredShape2<T> inherits from T
        template <typename T>
        struct ColoredShape2 : T
        {
            static_assert(is_base_of<Shape, T>::value,
                          "Template argument must be a Shape");

            string color;

            ColoredShape2() {}

            template <typename... Args>
            ColoredShape2(const string &color, Args &&...args)
                : T(std::forward<Args>(args)...), color(color) {}

            string str() const override
            {
                ostringstream oss;
                oss << T::str() << " has the color " << color;
                return oss.str();
            }
        };

        // TransparentShape2<T> inherits from T
        template <typename T>
        struct TransparentShape2 : T
        {
            uint8_t transparency;

            TransparentShape2() : transparency(255) {}

            template <typename... Args>
            TransparentShape2(uint8_t transparency, Args &&...args)
                : T(std::forward<Args>(args)...), transparency(transparency) {}

            string str() const override
            {
                ostringstream oss;
                oss << T::str() << " has "
                    << (static_cast<float>(transparency) / 255.0f * 100.0f)
                    << "% transparency";
                return oss.str();
            }
        };

        // PatternShape2<T> inherits from T
        template <typename T>
        struct PatternShape2 : T
        {
            string pattern;

            PatternShape2() {}

            template <typename... Args>
            PatternShape2(const string &pattern, Args &&...args)
                : T(std::forward<Args>(args)...), pattern(pattern) {}

            string str() const override
            {
                ostringstream oss;
                oss << T::str() << " with pattern " << pattern;
                return oss.str();
            }
        };

    } // namespace mixin

    // ============================================================================
    // APPROACH 3: FUNCTION DECORATORS
    // ============================================================================

    namespace function_decorator
    {

        // Basic Logger with std::function
        struct Logger
        {
            function<void()> func;
            string name;

            Logger(const function<void()> &func, const string &name)
                : func(func), name(name) {}

            void operator()() const
            {
                cout << "Entering " << name << endl;
                func();
                cout << "Exiting " << name << endl;
            }
        };

        // Template-based Logger for any callable
        template <typename Func>
        struct Logger2
        {
            Func func;
            string name;

            Logger2(const Func &func, const string &name)
                : func(func), name(name) {}

            void operator()() const
            {
                cout << "Entering " << name << endl;
                func();
                cout << "Exiting " << name << endl;
            }
        };

        template <typename Func>
        auto make_logger2(Func func, const string &name)
        {
            return Logger2<Func>{func, name};
        }

        // Specialized Logger for function signatures
        template <typename>
        struct Logger3;

        template <typename R, typename... Args>
        struct Logger3<R(Args...)>
        {
            function<R(Args...)> func;
            string name;

            Logger3(function<R(Args...)> func, const string &name)
                : func(func), name(name) {}

            R operator()(Args... args) const
            {
                cout << "Entering " << name << endl;
                R result = func(args...);
                cout << "Exiting " << name << endl;
                return result;
            }
        };

        template <typename R, typename... Args>
        auto make_logger3(R (*func)(Args...), const string &name)
        {
            return Logger3<R(Args...)>(
                std::function<R(Args...)>(func),
                name);
        }

        // Timer decorator
        template <typename Func>
        struct Timer
        {
            Func func;
            string name;

            Timer(const Func &func, const string &name)
                : func(func), name(name) {}

            void operator()() const
            {
                cout << "Starting timer for " << name << endl;
                func();
                cout << "Completed " << name << endl;
            }
        };

    } // namespace function_decorator

} // namespace decorator_solution

// ============================================================================
// VIOLATION - WITHOUT DECORATOR (Problem: Class Explosion)
// ============================================================================

namespace decorator_violation
{

    struct Shape
    {
        virtual string str() const = 0;
        virtual ~Shape() = default;
    };

    // PROBLEM: Need a new class for every combination!
    struct Circle : Shape
    {
        float radius;
        Circle() : radius(0) {}
        explicit Circle(float radius) : radius(radius) {}
        string str() const override
        {
            return string("A circle of radius ") + to_string(radius);
        }
    };

    // ColoredCircle - one combination
    struct ColoredCircle : Shape
    {
        float radius;
        string color;
        ColoredCircle(float radius, const string &color)
            : radius(radius), color(color) {}
        string str() const override
        {
            return string("A circle of radius ") + to_string(radius) +
                   " has the color " + color;
        }
    };

    // TransparentCircle - another combination
    struct TransparentCircle : Shape
    {
        float radius;
        uint8_t transparency;
        TransparentCircle(float radius, uint8_t transparency)
            : radius(radius), transparency(transparency) {}
        string str() const override
        {
            ostringstream oss;
            oss << "A circle of radius " << radius << " has "
                << (static_cast<float>(transparency) / 255.0f * 100.0f)
                << "% transparency";
            return oss.str();
        }
    };

    // ColoredTransparentCircle - EXPLOSION!
    struct ColoredTransparentCircle : Shape
    {
        float radius;
        string color;
        uint8_t transparency;
        ColoredTransparentCircle(float radius, const string &color,
                                 uint8_t transparency)
            : radius(radius), color(color), transparency(transparency) {}
        string str() const override
        {
            ostringstream oss;
            oss << "A circle of radius " << radius << " has the color "
                << color << " and "
                << (static_cast<float>(transparency) / 255.0f * 100.0f)
                << "% transparency";
            return oss.str();
        }
    };

    // Square, ColoredSquare, TransparentSquare,
    // ColoredTransparentSquare - and so on...
    // THIS DOESN'T SCALE!

} // namespace decorator_violation

#endif // DECORATOR_PATTERN_H
