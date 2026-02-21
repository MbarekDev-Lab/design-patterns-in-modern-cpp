#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cassert>

using namespace std;

// ============================================================================
// TEST FRAMEWORK
// ============================================================================

int g_test_count = 0;
int g_passed = 0;
int g_assertion_count = 0;

void TEST(const string &name)
{
    g_test_count++;
    cout << "\nTest " << g_test_count << ": " << name << "\n";
}

void ASSERT_EQ(const string &actual, const string &expected)
{
    g_assertion_count++;
    if (actual == expected)
    {
        cout << "  ✓ ASSERT_EQ passed\n";
    }
    else
    {
        cout << "  ✗ ASSERT_EQ failed: expected '" << expected
             << "', got '" << actual << "'\n";
        throw runtime_error("Assertion failed");
    }
}

void ASSERT_EQ_INT(int actual, int expected)
{
    g_assertion_count++;
    if (actual == expected)
    {
        cout << "  ✓ ASSERT_EQ(" << actual << ") passed\n";
    }
    else
    {
        cout << "  ✗ ASSERT_EQ failed: expected " << expected
             << ", got " << actual << "\n";
        throw runtime_error("Assertion failed");
    }
}

void ASSERT_TRUE(bool condition)
{
    g_assertion_count++;
    if (condition)
    {
        cout << "  ✓ ASSERT_TRUE passed\n";
    }
    else
    {
        cout << "  ✗ ASSERT_TRUE failed\n";
        throw runtime_error("Assertion failed");
    }
}

void ASSERT_FALSE(bool condition)
{
    g_assertion_count++;
    if (!condition)
    {
        cout << "  ✓ ASSERT_FALSE passed\n";
    }
    else
    {
        cout << "  ✗ ASSERT_FALSE failed\n";
        throw runtime_error("Assertion failed");
    }
}

void TEST_END()
{
    g_passed++;
}

// ============================================================================
// CHAIN OF RESPONSIBILITY IMPLEMENTATIONS
// ============================================================================

namespace chain_tests
{

    // ----- EXAMPLE 1: CREATURE MODIFIERS -----

    struct Creature
    {
        string name;
        int attack, defense;

        Creature(const string &name, int attack, int defense)
            : name(name), attack(attack), defense(defense) {}
    };

    class CreatureModifier
    {
    protected:
        Creature &creature;
        CreatureModifier *next = nullptr;

    public:
        explicit CreatureModifier(Creature &creature) : creature(creature) {}
        virtual ~CreatureModifier() = default;

        void add(CreatureModifier *cm)
        {
            if (next)
            {
                next->add(cm);
            }
            else
            {
                next = cm;
            }
        }

        virtual void handle()
        {
            if (next)
                next->handle();
        }
    };

    class NoBonusesModifier : public CreatureModifier
    {
    public:
        explicit NoBonusesModifier(Creature &creature)
            : CreatureModifier(creature) {}

        void handle() override
        {
            // Terminal: stops the chain
        }
    };

    class DoubleAttackModifier : public CreatureModifier
    {
    public:
        explicit DoubleAttackModifier(Creature &creature)
            : CreatureModifier(creature) {}

        void handle() override
        {
            creature.attack *= 2;
            CreatureModifier::handle();
        }
    };

    class IncreaseDefenseModifier : public CreatureModifier
    {
    public:
        explicit IncreaseDefenseModifier(Creature &creature)
            : CreatureModifier(creature) {}

        void handle() override
        {
            if (creature.attack <= 2)
            {
                creature.defense += 1;
            }
            CreatureModifier::handle();
        }
    };

    // ----- EXAMPLE 2: LOGGING CHAIN -----

    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    class Logger
    {
    protected:
        Logger *next = nullptr;
        LogLevel handle_level;
        vector<string> messages;

    public:
        explicit Logger(LogLevel level) : handle_level(level) {}
        virtual ~Logger() = default;

        void add_next(Logger *logger)
        {
            next = logger;
        }

        void log(LogLevel level, const string &message)
        {
            if (level >= handle_level)
            {
                write(message);
            }
            if (next)
            {
                next->log(level, message);
            }
        }

        const vector<string> &get_messages() const { return messages; }

    protected:
        virtual void write(const string &message)
        {
            messages.push_back(message);
        }
    };

    class ConsoleLogger : public Logger
    {
    public:
        ConsoleLogger(LogLevel level) : Logger(level) {}

    protected:
        void write(const string &message) override
        {
            Logger::write(message);
        }
    };

    class FileLogger : public Logger
    {
    public:
        FileLogger(LogLevel level) : Logger(level) {}

    protected:
        void write(const string &message) override
        {
            Logger::write(message);
        }
    };

    // ----- EXAMPLE 3: REQUEST VALIDATION -----

    struct Request
    {
        string username;
        string password;
        string email;
        int age;

        Request(const string &u, const string &p, const string &e, int a)
            : username(u), password(p), email(e), age(a) {}
    };

    class RequestValidator
    {
    protected:
        RequestValidator *next = nullptr;

    public:
        virtual ~RequestValidator() = default;

        void set_next(RequestValidator *validator)
        {
            next = validator;
        }

        virtual bool validate(const Request &req)
        {
            if (next)
            {
                return next->validate(req);
            }
            return true;
        }
    };

    class UsernameValidator : public RequestValidator
    {
    public:
        bool validate(const Request &req) override
        {
            if (req.username.length() < 3)
            {
                return false;
            }
            return RequestValidator::validate(req);
        }
    };

    class PasswordValidator : public RequestValidator
    {
    public:
        bool validate(const Request &req) override
        {
            if (req.password.length() < 8)
            {
                return false;
            }
            if (req.password.find_first_of("0123456789") == string::npos)
            {
                return false;
            }
            return RequestValidator::validate(req);
        }
    };

    class EmailValidator : public RequestValidator
    {
    public:
        bool validate(const Request &req) override
        {
            if (req.email.find('@') == string::npos)
            {
                return false;
            }
            return RequestValidator::validate(req);
        }
    };

    class AgeValidator : public RequestValidator
    {
    public:
        bool validate(const Request &req) override
        {
            if (req.age < 18)
            {
                return false;
            }
            return RequestValidator::validate(req);
        }
    };

    // ----- EXAMPLE 4: EVENT HANDLING -----

    struct Event
    {
        string type;
        string data;

        Event(const string &t, const string &d = "") : type(t), data(d) {}
    };

    class EventHandler
    {
    protected:
        EventHandler *next = nullptr;
        string last_handled;

    public:
        virtual ~EventHandler() = default;

        void set_next(EventHandler *handler)
        {
            next = handler;
        }

        virtual void handle(const Event &event)
        {
            if (next)
            {
                next->handle(event);
            }
        }

        string get_last_handled() const { return last_handled; }

    protected:
        void mark_handled(const string &type)
        {
            last_handled = type;
        }
    };

    class MouseEventHandler : public EventHandler
    {
    public:
        void handle(const Event &event) override
        {
            if (event.type == "mouse_click")
            {
                mark_handled("mouse");
                return;
            }
            EventHandler::handle(event);
        }
    };

    class KeyboardEventHandler : public EventHandler
    {
    public:
        void handle(const Event &event) override
        {
            if (event.type == "key_press")
            {
                mark_handled("keyboard");
                return;
            }
            EventHandler::handle(event);
        }
    };

    class DefaultEventHandler : public EventHandler
    {
    public:
        void handle(const Event &event) override
        {
            mark_handled("unknown");
            EventHandler::handle(event);
        }
    };

} // namespace chain_tests

using namespace chain_tests;

// ============================================================================
// TEST CASES - CREATURE MODIFIERS
// ============================================================================

void test_creature_single_doubleattack()
{
    TEST("Creature: Single DoubleAttackModifier");
    Creature goblin("Goblin", 1, 1);
    CreatureModifier root(goblin);
    DoubleAttackModifier da(goblin);
    root.add(&da);
    root.handle();
    ASSERT_EQ_INT(goblin.attack, 2);
    TEST_END();
}

void test_creature_double_doubleattack()
{
    TEST("Creature: Two DoubleAttackModifiers stack");
    Creature goblin("Goblin", 1, 1);
    CreatureModifier root(goblin);
    DoubleAttackModifier da1(goblin);
    DoubleAttackModifier da2(goblin);
    root.add(&da1);
    root.add(&da2);
    root.handle();
    ASSERT_EQ_INT(goblin.attack, 4);
    TEST_END();
}

void test_creature_aumentdefense_when_low_attack()
{
    TEST("Creature: IncreaseDefense when attack <= 2");
    Creature goblin("Goblin", 1, 1);
    CreatureModifier root(goblin);
    IncreaseDefenseModifier id(goblin);
    root.add(&id);
    root.handle();
    ASSERT_EQ_INT(goblin.defense, 2);
    TEST_END();
}

void test_creature_no_defense_increase_when_high_attack()
{
    TEST("Creature: No IncreaseDefense when attack > 2");
    Creature goblin("Goblin", 3, 1);
    CreatureModifier root(goblin);
    IncreaseDefenseModifier id(goblin);
    root.add(&id);
    root.handle();
    ASSERT_EQ_INT(goblin.defense, 1);
    TEST_END();
}

void test_creature_no_bonuses_modifier_terminates()
{
    TEST("Creature: NoBonusesModifier terminates chain");
    Creature goblin("Goblin", 1, 1);
    CreatureModifier root(goblin);
    DoubleAttackModifier da(goblin);
    NoBonusesModifier nb(goblin);
    root.add(&nb);
    root.add(&da);
    root.handle();
    // Attack should NOT be doubled because NoBonusesModifier terminates chain
    ASSERT_EQ_INT(goblin.attack, 1);
    TEST_END();
}

void test_creature_mixed_modifiers()
{
    TEST("Creature: Mixed modifiers in sequence");
    Creature goblin("Goblin", 1, 1);
    CreatureModifier root(goblin);
    DoubleAttackModifier da(goblin);
    IncreaseDefenseModifier id(goblin);
    root.add(&da);
    root.add(&id);
    root.handle();
    ASSERT_EQ_INT(goblin.attack, 2);
    ASSERT_EQ_INT(goblin.defense, 2);
    TEST_END();
}

// ============================================================================
// TEST CASES - LOGGING CHAIN
// ============================================================================

void test_logging_console_handles_info()
{
    TEST("Logging: Console logger handles INFO level");
    ConsoleLogger console(LogLevel::INFO);
    console.log(LogLevel::INFO, "Test message");
    ASSERT_EQ_INT(console.get_messages().size(), 1);
    TEST_END();
}

void test_logging_console_ignores_debug()
{
    TEST("Logging: Console logger ignores DEBUG (lower level)");
    ConsoleLogger console(LogLevel::INFO);
    console.log(LogLevel::DEBUG, "Debug message");
    ASSERT_EQ_INT(console.get_messages().size(), 0);
    TEST_END();
}

void test_logging_chain_passes_through()
{
    TEST("Logging: Chain passes message to all handlers");
    ConsoleLogger console(LogLevel::INFO);
    FileLogger file(LogLevel::WARNING);
    console.add_next(&file);

    console.log(LogLevel::WARNING, "Warning message");

    ASSERT_EQ_INT(console.get_messages().size(), 1);
    ASSERT_EQ_INT(file.get_messages().size(), 1);
    TEST_END();
}

void test_logging_chain_selective()
{
    TEST("Logging: Chain handlers selective by level");
    ConsoleLogger console(LogLevel::INFO);
    FileLogger file(LogLevel::WARNING);
    console.add_next(&file);

    console.log(LogLevel::INFO, "Info message");

    ASSERT_EQ_INT(console.get_messages().size(), 1);
    ASSERT_EQ_INT(file.get_messages().size(), 0);
    TEST_END();
}

// ============================================================================
// TEST CASES - REQUEST VALIDATION
// ============================================================================

void test_validation_all_pass()
{
    TEST("Validation: All validators pass");
    UsernameValidator username;
    PasswordValidator password;
    EmailValidator email;
    AgeValidator age;

    username.set_next(&password);
    password.set_next(&email);
    email.set_next(&age);

    Request req("john_doe", "SecurePass123", "john@example.com", 25);
    bool result = username.validate(req);
    ASSERT_TRUE(result);
    TEST_END();
}

void test_validation_username_fails()
{
    TEST("Validation: Username too short fails");
    UsernameValidator username;
    PasswordValidator password;
    username.set_next(&password);

    Request req("ab", "SecurePass123", "john@example.com", 25);
    bool result = username.validate(req);
    ASSERT_FALSE(result);
    TEST_END();
}

void test_validation_password_fails()
{
    TEST("Validation: Password without numbers fails");
    UsernameValidator username;
    PasswordValidator password;
    username.set_next(&password);

    Request req("john_doe", "NoNumbers", "john@example.com", 25);
    bool result = username.validate(req);
    ASSERT_FALSE(result);
    TEST_END();
}

void test_validation_email_fails()
{
    TEST("Validation: Email without @ fails");
    UsernameValidator username;
    PasswordValidator password;
    EmailValidator email;
    username.set_next(&password);
    password.set_next(&email);

    Request req("john_doe", "Password123", "invalidemail", 25);
    bool result = username.validate(req);
    ASSERT_FALSE(result);
    TEST_END();
}

void test_validation_age_fails()
{
    TEST("Validation: Age < 18 fails");
    UsernameValidator username;
    PasswordValidator password;
    EmailValidator email;
    AgeValidator age;
    username.set_next(&password);
    password.set_next(&email);
    email.set_next(&age);

    Request req("john_doe", "Password123", "john@example.com", 15);
    bool result = username.validate(req);
    ASSERT_FALSE(result);
    TEST_END();
}

void test_validation_stops_at_first_failure()
{
    TEST("Validation: Stops at first validator failure");
    UsernameValidator username;
    PasswordValidator password;
    username.set_next(&password);

    Request req("ab", "NoNumbers", "john@example.com", 25);
    bool result = username.validate(req);
    ASSERT_FALSE(result);
    TEST_END();
}

// ============================================================================
// TEST CASES - EVENT HANDLING
// ============================================================================

void test_event_mouse_handled()
{
    TEST("Event: Mouse event handled by MouseEventHandler");
    MouseEventHandler mouse;
    KeyboardEventHandler keyboard;
    DefaultEventHandler default_handler;

    mouse.set_next(&keyboard);
    keyboard.set_next(&default_handler);

    mouse.handle(Event("mouse_click", "100,200"));
    ASSERT_EQ(mouse.get_last_handled(), "mouse");
    TEST_END();
}

void test_event_keyboard_handled()
{
    TEST("Event: Keyboard event handled by KeyboardEventHandler");
    MouseEventHandler mouse;
    KeyboardEventHandler keyboard;
    DefaultEventHandler default_handler;

    mouse.set_next(&keyboard);
    keyboard.set_next(&default_handler);

    mouse.handle(Event("key_press", "Enter"));
    ASSERT_EQ(keyboard.get_last_handled(), "keyboard");
    TEST_END();
}

void test_event_unknown_handled_by_default()
{
    TEST("Event: Unknown event handled by DefaultEventHandler");
    MouseEventHandler mouse;
    KeyboardEventHandler keyboard;
    DefaultEventHandler default_handler;

    mouse.set_next(&keyboard);
    keyboard.set_next(&default_handler);

    mouse.handle(Event("unknown_event"));
    ASSERT_EQ(default_handler.get_last_handled(), "unknown");
    TEST_END();
}

void test_event_single_handler()
{
    TEST("Event: Single handler chain");
    MouseEventHandler mouse;

    mouse.handle(Event("mouse_click", "50,50"));
    ASSERT_EQ(mouse.get_last_handled(), "mouse");
    TEST_END();
}

// ============================================================================
// MAIN - RUN ALL TESTS
// ============================================================================

int main()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "CHAIN OF RESPONSIBILITY PATTERN - TEST SUITE\n";
    cout << string(70, '=') << "\n";

    // Creature modifier tests
    test_creature_single_doubleattack();
    test_creature_double_doubleattack();
    test_creature_aumentdefense_when_low_attack();
    test_creature_no_defense_increase_when_high_attack();
    test_creature_no_bonuses_modifier_terminates();
    test_creature_mixed_modifiers();

    // Logging chain tests
    test_logging_console_handles_info();
    test_logging_console_ignores_debug();
    test_logging_chain_passes_through();
    test_logging_chain_selective();

    // Request validation tests
    test_validation_all_pass();
    test_validation_username_fails();
    test_validation_password_fails();
    test_validation_email_fails();
    test_validation_age_fails();
    test_validation_stops_at_first_failure();

    // Event handling tests
    test_event_mouse_handled();
    test_event_keyboard_handled();
    test_event_unknown_handled_by_default();
    test_event_single_handler();

    // Print summary
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "TEST SUMMARY\n";
    cout << string(70, '=') << "\n";
    cout << "Tests Run:     " << g_test_count << "\n";
    cout << "Tests Passed:  " << g_passed << "\n";
    cout << "Tests Failed:  " << (g_test_count - g_passed) << "\n";
    cout << "Assertions:    " << g_assertion_count << "\n";

    if (g_test_count == g_passed)
    {
        cout << "Status:        ✅ ALL TESTS PASSED (" << g_passed << "/" << g_test_count << ")\n";
    }
    else
    {
        cout << "Status:        ❌ SOME TESTS FAILED\n";
    }

    cout << string(70, '=') << "\n\n";

    return (g_test_count == g_passed) ? 0 : 1;
}
