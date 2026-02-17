#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <map>

using namespace std;

// ============================================================================
// ADAPTER PATTERN
// ============================================================================
// Adapter (also called Wrapper) converts the interface of a class into
// another interface clients expect. It lets classes work together that
// couldn't otherwise because of incompatible interfaces.
// ============================================================================

namespace adapter_violation
{
    // ========================================================================
    // PROBLEM: Incompatible interfaces that can't work together
    // ========================================================================

    // Old interface: Works with Squares
    class Square
    {
    public:
        Square(int side) : side_(side) {}
        int get_side() const { return side_; }
        void set_side(int s) { side_ = s; }
        int get_area() const { return side_ * side_; }

    private:
        int side_;
    };

    // New interface: Needs Rectangles with width/height
    class RectangleProcessor
    {
    public:
        void process_width_height(int width, int height)
        {
            cout << "Processing rectangle: " << width << "x" << height << "\n";
        }
    };

    // ❌ PROBLEM: Square and RectangleProcessor don't work together!
    // Square uses side, but RectangleProcessor expects width/height
    // Caller adds complexity to bridge them

} // namespace adapter_violation

namespace adapter_solution
{
    // ========================================================================
    // SOLUTION: Adapter Pattern
    // ========================================================================

    // Case 1: Class Adapter (using inheritance)
    // Adapts interface by inheriting from both the client interface and adaptee

    class Shape
    {
    public:
        virtual ~Shape() = default;
        virtual void render() = 0;
        virtual int get_width() const = 0;
        virtual int get_height() const = 0;
    };

    // Modern shape with standard interface
    class Rectangle : public Shape
    {
    public:
        Rectangle(int width, int height) : width_(width), height_(height) {}

        void render() override
        {
            cout << "Rendering Rectangle: " << width_ << "x" << height_ << "\n";
        }

        int get_width() const override { return width_; }
        int get_height() const override { return height_; }

    private:
        int width_, height_;
    };

    // Legacy shape with different interface
    class LegacySquare
    {
    public:
        LegacySquare(int side) : side_(side) {}
        int get_side() const { return side_; }

    private:
        int side_;
    };

    // ✓ Class Adapter: Makes LegacySquare compatible with Shape interface
    class SquareAdapter : public Shape
    {
    public:
        SquareAdapter(const shared_ptr<LegacySquare> &square)
            : square_(square) {}

        void render() override
        {
            cout << "Rendering Adapted Square: " << square_->get_side()
                 << "x" << square_->get_side() << "\n";
        }

        int get_width() const override { return square_->get_side(); }
        int get_height() const override { return square_->get_side(); }

    private:
        shared_ptr<LegacySquare> square_;
    };

    // ========================================================================
    // CASE 2: Object Adapter (using composition, more flexible)
    // ========================================================================

    // Line interface (source, what we have)
    struct Point
    {
        int x, y;
    };

    struct Line
    {
        Point start, end;
    };

    // Point interface (target, what we need)
    typedef vector<Point> PointSet;

    // Make Line comparable for use in maps
    inline bool operator<(const Line &a, const Line &b)
    {
        if (a.start.x != b.start.x)
            return a.start.x < b.start.x;
        if (a.start.y != b.start.y)
            return a.start.y < b.start.y;
        if (a.end.x != b.end.x)
            return a.end.x < b.end.x;
        return a.end.y < b.end.y;
    }

    inline bool operator==(const Line &a, const Line &b)
    {
        return a.start.x == b.start.x && a.start.y == b.start.y &&
               a.end.x == b.end.x && a.end.y == b.end.y;
    }

    // ✓ Object Adapter: Converts Line to PointSet
    class LineToPointAdapter
    {
    public:
        explicit LineToPointAdapter(const Line &line)
        {
            adapt_line(line);
        }

        PointSet &get_points() { return points_; }
        const PointSet &get_points() const { return points_; }

        auto begin() { return points_.begin(); }
        auto end() { return points_.end(); }

    private:
        PointSet points_;

        void adapt_line(const Line &line)
        {
            int left = min(line.start.x, line.end.x);
            int right = max(line.start.x, line.end.x);
            int top = min(line.start.y, line.end.y);
            int bottom = max(line.start.y, line.end.y);

            if (left == right) // Vertical line
            {
                for (int y = top; y <= bottom; ++y)
                {
                    points_.push_back({left, y});
                }
            }
            else if (top == bottom) // Horizontal line
            {
                for (int x = left; x <= right; ++x)
                {
                    points_.push_back({x, top});
                }
            }
        }
    };

    // ========================================================================
    // CASE 3: Caching Adapter
    // ========================================================================

    class CachingLineToPointAdapter
    {
    public:
        explicit CachingLineToPointAdapter(const Line &line) : line_(line)
        {
            if (cache_.find(line) == cache_.end())
            {
                cout << "  [Cache] Computing points for line ("
                     << line.start.x << "," << line.start.y << ") -> ("
                     << line.end.x << "," << line.end.y << ")\n";

                adapt_line(line);
                cache_[line] = points_;
            }
            else
            {
                cout << "  [Cache] Using cached points for line\n";
                points_ = cache_[line];
            }
        }

        PointSet &get_points() { return points_; }
        const PointSet &get_points() const { return points_; }

        static void clear_cache() { cache_.clear(); }

    private:
        PointSet points_;
        const Line &line_;

        // Static cache across all instances
        static map<Line, PointSet> cache_;

        void adapt_line(const Line &line)
        {
            int left = min(line.start.x, line.end.x);
            int right = max(line.start.x, line.end.x);
            int top = min(line.start.y, line.end.y);
            int bottom = max(line.start.y, line.end.y);

            if (left == right)
            {
                for (int y = top; y <= bottom; ++y)
                {
                    points_.push_back({left, y});
                }
            }
            else if (top == bottom)
            {
                for (int x = left; x <= right; ++x)
                {
                    points_.push_back({x, top});
                }
            }
        }
    };

    // Static member initialization
    map<Line, PointSet> CachingLineToPointAdapter::cache_;

    // ========================================================================
    // CASE 4: Two-way Adapter
    // ========================================================================

    class BidirectionalAdapter
    {
    public:
        // Can construct from either type
        BidirectionalAdapter(const Line &line)
        {
            adapt_line_to_points(line);
        }

        BidirectionalAdapter(const PointSet &points)
        {
            adapt_points_to_line(points);
        }

        const PointSet &get_points() const { return points_; }
        const Line &get_line() const { return line_; }

    private:
        PointSet points_;
        Line line_;

        void adapt_line_to_points(const Line &line)
        {
            line_ = line;
            int left = min(line.start.x, line.end.x);
            int right = max(line.start.x, line.end.x);
            int top = min(line.start.y, line.end.y);
            int bottom = max(line.start.y, line.end.y);

            if (left == right)
            {
                for (int y = top; y <= bottom; ++y)
                    points_.push_back({left, y});
            }
            else if (top == bottom)
            {
                for (int x = left; x <= right; ++x)
                    points_.push_back({x, top});
            }
        }

        void adapt_points_to_line(const PointSet &points)
        {
            if (points.empty())
                return;

            points_ = points;
            line_.start = points.front();
            line_.end = points.back();
        }
    };

} // namespace adapter_solution
