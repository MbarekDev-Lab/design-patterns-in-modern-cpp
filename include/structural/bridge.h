#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>

// ============================================================================
// BRIDGE PATTERN
// ============================================================================
// The Bridge pattern decouples an abstraction from its implementation so that
// the two can vary independently. It uses a pointer to an implementation class
// (Pimpl idiom) allowing you to change the implementation without modifying
// the public interface.
//
// Key Concepts:
// 1. Abstraction - The public interface (Person, Device, etc.)
// 2. Implementation - The actual implementation details (PersonImpl, DeviceImpl)
// 3. Bridge - A pointer to the implementation that allows swapping
// 4. Decouple - Interface and implementation can evolve independently
// ============================================================================

namespace pimpl_wrapper
{
    // ========================================================================
    // PIMPL WRAPPER - Generic template-based PIMPL pattern
    // ========================================================================
    // This template can wrap any implementation class, providing:
    // - Automatic memory management with unique_ptr
    // - Operator overloading for transparent access
    // - Construction forwarding with perfect forwarding
    // ========================================================================

    template <typename T>
    class PimplWrapper
    {
    private:
        std::unique_ptr<T> impl;

    public:
        // Default constructor
        PimplWrapper()
            : impl(std::make_unique<T>())
        {
        }

        // Destructor
        ~PimplWrapper() = default;

        // Perfect forwarding constructor for custom arguments
        template <typename... Args>
        explicit PimplWrapper(Args &&...args)
            : impl(std::make_unique<T>(std::forward<Args>(args)...))
        {
        }

        // Pointer access
        T *operator->()
        {
            return impl.get();
        }

        const T *operator->() const
        {
            return impl.get();
        }

        // Dereference access
        T &operator*()
        {
            return *impl.get();
        }

        const T &operator*() const
        {
            return *impl.get();
        }

        // Move semantics
        PimplWrapper(PimplWrapper &&) = default;
        PimplWrapper &operator=(PimplWrapper &&) = default;

        // Delete copy semantics
        PimplWrapper(const PimplWrapper &) = delete;
        PimplWrapper &operator=(const PimplWrapper &) = delete;
    };

} // namespace pimpl_wrapper

namespace bridge_violation
{
    // ========================================================================
    // PROBLEM: Implementation mixed with interface
    // ========================================================================

    class ReportGenerator
    {
    public:
        void generate_as_pdf()
        {
            std::cout << "Generating PDF with embedded implementation...\n";
            // PDF-specific logic directly in the class
            std::cout << "  - Creating PDF document\n";
            std::cout << "  - Adding content\n";
            std::cout << "  - Compressing\n";
        }

        void generate_as_html()
        {
            std::cout << "Generating HTML with embedded implementation...\n";
            // HTML-specific logic directly in the class
            std::cout << "  - Creating HTML structure\n";
            std::cout << "  - Adding styles\n";
            std::cout << "  - Minifying\n";
        }

        void generate_as_json()
        {
            std::cout << "Generating JSON with embedded implementation...\n";
            // JSON-specific logic directly in the class
            std::cout << "  - Creating JSON structure\n";
            std::cout << "  - Formatting\n";
            std::cout << "  - Serializing\n";
        }

        // Problem: Every new format requires modifying this class
        // violates Open/Closed Principle
    };

    // ========================================================================
    // REFERENCE EXAMPLE: Simple Pimpl Bridge
    // ========================================================================

    class Person
    {
    public:
        std::string name;

        class PersonImpl;
        PersonImpl *impl;

        Person();
        ~Person();

        void greet();
    };

} // namespace bridge_violation

namespace bridge_solution
{
    // ========================================================================
    // SOLUTION 1: Classic Pimpl Bridge Pattern
    // ========================================================================

    class Person
    {
    public:
        std::string name;

        class PersonImpl;
        std::unique_ptr<PersonImpl> impl;

        Person();
        ~Person();

        void greet();
    };

    // Implementation can be hidden and varied
    class Person::PersonImpl
    {
    public:
        void greet(Person *p);
    };
    // Inline implementations
    inline Person::Person()
        : impl(std::make_unique<PersonImpl>())
    {
    }

    inline Person::~Person() = default;

    inline void Person::greet()
    {
        impl->greet(this);
    }

    inline void Person::PersonImpl::greet(Person *p)
    {
        std::cout << "Hello " << p->name << "\n";
    }

    // ========================================================================
    // SOLUTION 2: PIMPL Wrapper-Based Bridge
    // ========================================================================
    // Using the template-based PIMPL wrapper for cleaner, reusable code

    class Window
    {
    public:
        class WindowImpl;

        Window();
        ~Window();

        void set_title(const std::string &title);
        void show();
        void hide();

    private:
        pimpl_wrapper::PimplWrapper<WindowImpl> pimpl;
        friend class WindowImpl;
    };

    class Window::WindowImpl
    {
    public:
        std::string title;
        bool visible = false;

        void show()
        {
            visible = true;
            std::cout << "    Window '" << title << "' is now visible\n";
        }

        void hide()
        {
            visible = false;
            std::cout << "    Window '" << title << "' is now hidden\n";
        }
    };

    inline Window::Window() = default;
    inline Window::~Window() = default;

    inline void Window::set_title(const std::string &title)
    {
        pimpl->title = title;
    }

    inline void Window::show()
    {
        pimpl->show();
    }

    inline void Window::hide()
    {
        pimpl->hide();
    }

    // ========================================================================
    // SOLUTION 3: Abstract Implementation Bridge
    // ========================================================================
    // Allows swappable implementations through inheritance

    class Renderer
    {
    public:
        virtual ~Renderer() = default;
        virtual void render_header() = 0;
        virtual void render_content() = 0;
        virtual void render_footer() = 0;
    };

    class PDFRenderer : public Renderer
    {
    public:
        void render_header() override { std::cout << "  [PDF] Header\n"; }
        void render_content() override { std::cout << "  [PDF] Content\n"; }
        void render_footer() override { std::cout << "  [PDF] Footer\n"; }
    };

    class HTMLRenderer : public Renderer
    {
    public:
        void render_header() override { std::cout << "  [HTML] <header>\n"; }
        void render_content() override { std::cout << "  [HTML] <body>\n"; }
        void render_footer() override { std::cout << "  [HTML] <footer>\n"; }
    };

    class JSONRenderer : public Renderer
    {
    public:
        void render_header() override { std::cout << "  [JSON] {\"header\": {\n"; }
        void render_content() override { std::cout << "    \"content\": {...}\n"; }
        void render_footer() override { std::cout << "  }}\n"; }
    };

    // Abstraction uses the bridge
    class Document
    {
    public:
        explicit Document(std::shared_ptr<Renderer> renderer)
            : impl_(renderer)
        {
        }

        void render()
        {
            impl_->render_header();
            impl_->render_content();
            impl_->render_footer();
        }

        // Can change implementation at runtime
        void set_renderer(std::shared_ptr<Renderer> renderer)
        {
            impl_ = renderer;
        }

    private:
        std::shared_ptr<Renderer> impl_;
    };

    // ========================================================================
    // SOLUTION 3: Device with Bridge Pattern
    // ========================================================================

    class DeviceImplementation
    {
    public:
        virtual ~DeviceImplementation() = default;
        virtual void power_on() = 0;
        virtual void power_off() = 0;
        virtual void set_volume(int level) = 0;
    };

    class RadioImpl : public DeviceImplementation
    {
    public:
        void power_on() override { std::cout << "    Radio powered on\n"; }
        void power_off() override { std::cout << "    Radio powered off\n"; }
        void set_volume(int level) override
        {
            std::cout << "    Radio volume: " << level << "%\n";
        }
    };

    class TVImpl : public DeviceImplementation
    {
    public:
        void power_on() override { std::cout << "    TV powered on\n"; }
        void power_off() override { std::cout << "    TV powered off\n"; }
        void set_volume(int level) override
        {
            std::cout << "    TV volume: " << level << "%\n";
        }
    };

    class PhoneImpl : public DeviceImplementation
    {
    public:
        void power_on() override { std::cout << "    Phone powered on\n"; }
        void power_off() override { std::cout << "    Phone powered off\n"; }
        void set_volume(int level) override
        {
            std::cout << "    Phone volume: " << level << "%\n";
        }
    };

    // Abstraction - Remote control
    class RemoteControl
    {
    public:
        explicit RemoteControl(std::shared_ptr<DeviceImplementation> device)
            : impl_(device)
        {
        }

        void turn_on() { impl_->power_on(); }
        void turn_off() { impl_->power_off(); }
        void set_volume(int level) { impl_->set_volume(level); }

        void set_device(std::shared_ptr<DeviceImplementation> device)
        {
            impl_ = device;
        }

    private:
        std::shared_ptr<DeviceImplementation> impl_;
    };

    // ========================================================================
    // SOLUTION 4: Shape with Renderer Bridge
    // ========================================================================

    class ShapeRenderer
    {
    public:
        virtual ~ShapeRenderer() = default;
        virtual void draw_circle(double radius) = 0;
        virtual void draw_rectangle(double width, double height) = 0;
    };

    class ConsoleRenderer : public ShapeRenderer
    {
    public:
        void draw_circle(double radius) override
        {
            std::cout << "  Drawing circle with radius " << radius << " in console\n";
        }

        void draw_rectangle(double width, double height) override
        {
            std::cout << "  Drawing rectangle " << width << "x" << height
                      << " in console\n";
        }
    };

    class HighResRenderer : public ShapeRenderer
    {
    public:
        void draw_circle(double radius) override
        {
            std::cout << "  Drawing high-res circle with radius " << radius << "\n";
        }

        void draw_rectangle(double width, double height) override
        {
            std::cout << "  Drawing high-res rectangle " << width << "x" << height
                      << "\n";
        }
    };

    class Shape
    {
    public:
        explicit Shape(std::shared_ptr<ShapeRenderer> renderer)
            : impl_(renderer)
        {
        }

        virtual ~Shape() = default;
        virtual void draw() = 0;

    protected:
        std::shared_ptr<ShapeRenderer> impl_;
    };

    class Circle : public Shape
    {
    public:
        Circle(std::shared_ptr<ShapeRenderer> renderer, double radius)
            : Shape(renderer), radius_(radius)
        {
        }

        void draw() override { impl_->draw_circle(radius_); }

    private:
        double radius_;
    };

    class Rectangle : public Shape
    {
    public:
        Rectangle(std::shared_ptr<ShapeRenderer> renderer, double width,
                  double height)
            : Shape(renderer), width_(width), height_(height)
        {
        }

        void draw() override { impl_->draw_rectangle(width_, height_); }

    private:
        double width_, height_;
    };

} // namespace bridge_solution
