#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

// ============================================================================
// VIOLATION: Hard-coded Creation of Related Objects
// ============================================================================

// GUI Elements - Platform Specific
class ButtonBad
{
public:
    virtual void Draw() const { std::cout << "Generic Button"; }
    virtual ~ButtonBad() = default;
};

class TextBoxBad
{
public:
    virtual void Draw() const { std::cout << "Generic TextBox"; }
    virtual ~TextBoxBad() = default;
};

// PROBLEM: Hard to switch between themes/platforms
// Must change code in multiple places
// No type safety for related objects

// ============================================================================
// SOLUTION: Abstract Factory Pattern
// ============================================================================

// Abstract Products
class Button
{
public:
    virtual void Draw() const = 0;
    virtual ~Button() = default;
};

class TextBox
{
public:
    virtual void Draw() const = 0;
    virtual ~TextBox() = default;
};

class Window
{
public:
    virtual void Draw() const = 0;
    virtual ~Window() = default;
};

// Concrete Products - Windows Theme
class WindowsButton : public Button
{
public:
    void Draw() const override
    {
        std::cout << "🪟 Windows Button (Blue gradient)";
    }
};

class WindowsTextBox : public TextBox
{
public:
    void Draw() const override
    {
        std::cout << "🪟 Windows TextBox (Beveled border)";
    }
};

class WindowsWindow : public Window
{
public:
    void Draw() const override
    {
        std::cout << "🪟 Windows Window (Classic title bar)";
    }
};

// Concrete Products - macOS Theme
class MacButton : public Button
{
public:
    void Draw() const override
    {
        std::cout << "🍎 macOS Button (Rounded, subtle shadow)";
    }
};

class MacTextBox : public TextBox
{
public:
    void Draw() const override
    {
        std::cout << "🍎 macOS TextBox (Minimal borders)";
    }
};

class MacWindow : public Window
{
public:
    void Draw() const override
    {
        std::cout << "🍎 macOS Window (Native traffic lights)";
    }
};

// Concrete Products - Linux/GTK Theme
class LinuxButton : public Button
{
public:
    void Draw() const override
    {
        std::cout << "🐧 Linux Button (Flat design)";
    }
};

class LinuxTextBox : public TextBox
{
public:
    void Draw() const override
    {
        std::cout << "🐧 Linux TextBox (Simple outline)";
    }
};

class LinuxWindow : public Window
{
public:
    void Draw() const override
    {
        std::cout << "🐧 Linux Window (GNOME style)";
    }
};

// Abstract Factory - defines interface for creating families of objects
class UIFactory
{
public:
    virtual std::unique_ptr<Button> CreateButton() const = 0;
    virtual std::unique_ptr<TextBox> CreateTextBox() const = 0;
    virtual std::unique_ptr<Window> CreateWindow() const = 0;
    virtual ~UIFactory() = default;
};

// Concrete Factories - Windows
class WindowsUIFactory : public UIFactory
{
public:
    std::unique_ptr<Button> CreateButton() const override
    {
        return std::make_unique<WindowsButton>();
    }

    std::unique_ptr<TextBox> CreateTextBox() const override
    {
        return std::make_unique<WindowsTextBox>();
    }

    std::unique_ptr<Window> CreateWindow() const override
    {
        return std::make_unique<WindowsWindow>();
    }
};

// Concrete Factories - macOS
class MacUIFactory : public UIFactory
{
public:
    std::unique_ptr<Button> CreateButton() const override
    {
        return std::make_unique<MacButton>();
    }

    std::unique_ptr<TextBox> CreateTextBox() const override
    {
        return std::make_unique<MacTextBox>();
    }

    std::unique_ptr<Window> CreateWindow() const override
    {
        return std::make_unique<MacWindow>();
    }
};

// Concrete Factories - Linux
class LinuxUIFactory : public UIFactory
{
public:
    std::unique_ptr<Button> CreateButton() const override
    {
        return std::make_unique<LinuxButton>();
    }

    std::unique_ptr<TextBox> CreateTextBox() const override
    {
        return std::make_unique<LinuxTextBox>();
    }

    std::unique_ptr<Window> CreateWindow() const override
    {
        return std::make_unique<LinuxWindow>();
    }
};

// ============================================================================
// Application Class - Uses Abstract Factory
// ============================================================================

class Application
{
private:
    std::unique_ptr<Button> button;
    std::unique_ptr<TextBox> textbox;
    std::unique_ptr<Window> window;

public:
    Application(const UIFactory &factory)
    {
        button = factory.CreateButton();
        textbox = factory.CreateTextBox();
        window = factory.CreateWindow();
    }

    void Draw() const
    {
        std::cout << "Drawing UI:\n";
        std::cout << "  Button:  ";
        button->Draw();
        std::cout << "\n";

        std::cout << "  TextBox: ";
        textbox->Draw();
        std::cout << "\n";

        std::cout << "  Window:  ";
        window->Draw();
        std::cout << "\n";
    }
};

// ============================================================================
// Database Connection Factory Example
// ============================================================================

// Abstract Products for Database
class Connection
{
public:
    virtual void Connect() const = 0;
    virtual ~Connection() = default;
};

class Query
{
public:
    virtual void Execute(const std::string &sql) const = 0;
    virtual ~Query() = default;
};

// MySQL Implementation
class MySQLConnection : public Connection
{
public:
    void Connect() const override
    {
        std::cout << "MySQL: Connecting to database on port 3306";
    }
};

class MySQLQuery : public Query
{
public:
    void Execute(const std::string &sql) const override
    {
        std::cout << "MySQL: Executing [" << sql << "]";
    }
};

// PostgreSQL Implementation
class PostgreSQLConnection : public Connection
{
public:
    void Connect() const override
    {
        std::cout << "PostgreSQL: Connecting to database on port 5432";
    }
};

class PostgreSQLQuery : public Query
{
public:
    void Execute(const std::string &sql) const override
    {
        std::cout << "PostgreSQL: Executing [" << sql << "]";
    }
};

// Abstract Database Factory
class DatabaseFactory
{
public:
    virtual std::unique_ptr<Connection> CreateConnection() const = 0;
    virtual std::unique_ptr<Query> CreateQuery() const = 0;
    virtual ~DatabaseFactory() = default;
};

// Concrete Database Factories
class MySQLFactory : public DatabaseFactory
{
public:
    std::unique_ptr<Connection> CreateConnection() const override
    {
        return std::make_unique<MySQLConnection>();
    }

    std::unique_ptr<Query> CreateQuery() const override
    {
        return std::make_unique<MySQLQuery>();
    }
};

class PostgreSQLFactory : public DatabaseFactory
{
public:
    std::unique_ptr<Connection> CreateConnection() const override
    {
        return std::make_unique<PostgreSQLConnection>();
    }

    std::unique_ptr<Query> CreateQuery() const override
    {
        return std::make_unique<PostgreSQLQuery>();
    }
};

// Database Manager using Abstract Factory
class DatabaseManager
{
private:
    std::unique_ptr<Connection> conn;
    std::unique_ptr<Query> query;

public:
    DatabaseManager(const DatabaseFactory &factory)
        : conn{factory.CreateConnection()}, query{factory.CreateQuery()}
    {
    }

    void Setup() const
    {
        std::cout << "Setting up database:\n";
        std::cout << "  ";
        conn->Connect();
        std::cout << "\n";
    }

    void ExecuteQuery(const std::string &sql) const
    {
        std::cout << "  ";
        query->Execute(sql);
        std::cout << "\n";
    }
};
