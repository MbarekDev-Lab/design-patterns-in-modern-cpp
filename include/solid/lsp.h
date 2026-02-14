#ifndef LSP_H
#define LSP_H

#include <iostream>

/**
 * Liskov Substitution Principle (LSP)
 *
 * "Objects in a program should be replaceable with instances of their subtypes
 * without altering the correctness of the program."
 *
 * In other words: Derived classes must be usable in place of base classes
 * without breaking the expected behavior.
 *
 * This example shows:
 * 1. VIOLATION: Square inherits from Rectangle but violates user expectations
 * 2. SOLUTION: Use proper abstraction with an abstract Shape base class
 */

namespace lsp
{

    // ============ VIOLATION: Square inherits from Rectangle ============
    /**
     * ❌ PROBLEM: The user expects Rectangle to have independent width and height
     * But Square violates this by keeping width == height always
     * This breaks LSP - Square cannot be substituted for Rectangle safely
     */

    class RectangleBad
    {
    protected:
        int width;
        int height;

    public:
        RectangleBad(int width, int height) : width(width), height(height) {}

        virtual ~RectangleBad() = default;

        int get_width() const { return width; }
        virtual void set_width(int width) { this->width = width; }

        int get_height() const { return height; }
        virtual void set_height(int height) { this->height = height; }

        int area() const { return width * height; }
    };

    /**
     * ❌ VIOLATION OF LSP
     * Square modifies the behavior set_width() and set_height()
     * Setting width also sets height, and vice versa
     * This breaks the contract that width and height are independent
     */
    class SquareBad : public RectangleBad
    {
    public:
        explicit SquareBad(int size) : RectangleBad(size, size) {}

        void set_width(int width) override
        {
            this->width = width;
            this->height = width; // ❌ Breaks expectation: height changed too!
        }

        void set_height(int height) override
        {
            this->height = height;
            this->width = height; // ❌ Breaks expectation: width changed too!
        }
    };

    // ============ SOLUTION: Proper abstraction with composition ============

    /**
     * ✅ SOLUTION: Abstract base class for shapes
     * All derived classes have consistent behavior
     */
    class Shape
    {
    public:
        virtual ~Shape() = default;
        virtual int area() const = 0;
    };

    /**
     * ✅ CORRECT: Rectangle is its own class
     * Width and height are independent
     */
    class Rectangle : public Shape
    {
    protected:
        int width;
        int height;

    public:
        Rectangle(int width, int height) : width(width), height(height) {}

        virtual ~Rectangle() = default;

        int get_width() const { return width; }
        virtual void set_width(int width) { this->width = width; }

        int get_height() const { return height; }
        virtual void set_height(int height) { this->height = height; }

        int area() const override { return width * height; }
    };

    /**
     * ✅ CORRECT: Square is its own class
     * Does NOT inherit from Rectangle
     * Size is the only parameter, no behavioral confusion
     */
    class Square : public Shape
    {
    private:
        int side;

    public:
        explicit Square(int side) : side(side) {}

        virtual ~Square() = default;

        int get_side() const { return side; }
        void set_side(int side) { this->side = side; }

        int area() const override { return side * side; }
    };

    /**
     * ✅ CORRECT: This function expects a Shape, not Rectangle
     * Works correctly with any Shape subclass
     */
    inline void print_shape_area(const Shape &shape)
    {
        std::cout << "Shape area: " << shape.area() << std::endl;
    }

    /**
     * ❌ VIOLATION VERSION (shows the problem)
     * This function expects a Rectangle with independent width/height
     * But if you pass a SquareBad, it violates the contract
     */
    inline void process_rectangle_bad(RectangleBad &r)
    {
        int w = r.get_width();
        r.set_height(10);

        std::cout << "Expected area = " << (w * 10)
                  << ", got " << r.area();

        if ((w * 10) != r.area())
        {
            std::cout << " ❌ VIOLATION! Area mismatch!\n";
        }
        else
        {
            std::cout << " ✓\n";
        }
    }

} // namespace lsp

#endif // LSP_H
