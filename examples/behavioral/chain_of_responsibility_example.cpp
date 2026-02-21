#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>

using namespace std;

/*
 * CHAIN OF RESPONSIBILITY PATTERN EXAMPLES
 *
 * A behavioral design pattern that passes requests along a chain of handlers.
 * Each handler decides whether to process the request or pass it to the next handler.
 *
 * Uses:
 * - Event handling (GUI frameworks)
 * - Logging systems with multiple levels
 * - Request validation chains
 * - Command processing
 * - Game modifier stacking
 */

// ============================================================================
// EXAMPLE 1: CREATURE MODIFIERS (From your example)
// ============================================================================

namespace creature_modifiers
{

    struct Creature
    {
        string name;
        int attack, defense;

        Creature(const string &name, int attack, int defense)
            : name(name), attack(attack), defense(defense) {}

        friend ostream &operator<<(ostream &os, const Creature &obj)
        {
            return os << "name: " << obj.name
                      << " attack: " << obj.attack
                      << " defense: " << obj.defense;
        }
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
            cout << "  [NoBonusesModifier] No bonuses allowed!\n";
        }
    };

    class DoubleAttackModifier : public CreatureModifier
    {
    public:
        explicit DoubleAttackModifier(Creature &creature)
            : CreatureModifier(creature) {}

        void handle() override
        {
            cout << "  [DoubleAttackModifier] Attack: " << creature.attack
                 << " -> " << (creature.attack * 2) << "\n";
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
                cout << "  [IncreaseDefenseModifier] Defense: " << creature.defense
                     << " -> " << (creature.defense + 1) << "\n";
                creature.defense += 1;
            }
            CreatureModifier::handle();
        }
    };

} // namespace creature_modifiers

// ============================================================================
// EXAMPLE 2: LOGGING CHAIN (Multiple log levels)
// ============================================================================

namespace logging_chain
{

    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    class Logger
    {
    protected:
        Logger *next_logger = nullptr;
        LogLevel handle_level;

    public:
        explicit Logger(LogLevel level) : handle_level(level) {}
        virtual ~Logger() = default;

        void add_next(Logger *logger)
        {
            next_logger = logger;
        }

        void log(LogLevel level, const string &message)
        {
            if (level >= handle_level)
            {
                write(message);
            }
            if (next_logger)
            {
                next_logger->log(level, message);
            }
        }

    protected:
        virtual void write(const string &message) = 0;
    };

    class ConsoleLogger : public Logger
    {
    public:
        ConsoleLogger(LogLevel level) : Logger(level) {}

    protected:
        void write(const string &message) override
        {
            cout << "  [CONSOLE] " << message << "\n";
        }
    };

    class FileLogger : public Logger
    {
    private:
        vector<string> file_contents;

    public:
        FileLogger(LogLevel level) : Logger(level) {}

    protected:
        void write(const string &message) override
        {
            file_contents.push_back(message);
            cout << "  [FILE] " << message << " (stored)\n";
        }

    public:
        const vector<string> &get_contents() const { return file_contents; }
    };

    class EmailLogger : public Logger
    {
    private:
        vector<string> emails_sent;

    public:
        EmailLogger(LogLevel level) : Logger(level) {}

    protected:
        void write(const string &message) override
        {
            if (!message.empty())
            {
                emails_sent.push_back(message);
                cout << "  [EMAIL] Alert sent: " << message << "\n";
            }
        }

    public:
        size_t get_email_count() const { return emails_sent.size(); }
    };

} // namespace logging_chain

// ============================================================================
// EXAMPLE 3: REQUEST VALIDATION CHAIN
// ============================================================================

namespace validation_chain
{

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
            cout << "  [UsernameValidator] Checking username...\n";
            if (req.username.length() < 3)
            {
                cout << "    ✗ Username too short\n";
                return false;
            }
            cout << "    ✓ Username valid\n";
            return RequestValidator::validate(req);
        }
    };

    class PasswordValidator : public RequestValidator
    {
    public:
        bool validate(const Request &req) override
        {
            cout << "  [PasswordValidator] Checking password...\n";
            if (req.password.length() < 8)
            {
                cout << "    ✗ Password too short\n";
                return false;
            }
            if (req.password.find_first_of("0123456789") == string::npos)
            {
                cout << "    ✗ Password must contain numbers\n";
                return false;
            }
            cout << "    ✓ Password valid\n";
            return RequestValidator::validate(req);
        }
    };

    class EmailValidator : public RequestValidator
    {
    public:
        bool validate(const Request &req) override
        {
            cout << "  [EmailValidator] Checking email...\n";
            if (req.email.find('@') == string::npos)
            {
                cout << "    ✗ Invalid email format\n";
                return false;
            }
            cout << "    ✓ Email valid\n";
            return RequestValidator::validate(req);
        }
    };

    class AgeValidator : public RequestValidator
    {
    public:
        bool validate(const Request &req) override
        {
            cout << "  [AgeValidator] Checking age...\n";
            if (req.age < 18)
            {
                cout << "    ✗ Must be 18 or older\n";
                return false;
            }
            cout << "    ✓ Age valid\n";
            return RequestValidator::validate(req);
        }
    };

} // namespace validation_chain

// ============================================================================
// EXAMPLE 4: EVENT HANDLER CHAIN (GUI-like)
// ============================================================================

namespace event_chain
{

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
    };

    class MouseEventHandler : public EventHandler
    {
    public:
        void handle(const Event &event) override
        {
            if (event.type == "mouse_click")
            {
                cout << "  [MouseEventHandler] Processing click at " << event.data << "\n";
                return; // Consume event
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
                cout << "  [KeyboardEventHandler] Processing key: " << event.data << "\n";
                return; // Consume event
            }
            EventHandler::handle(event);
        }
    };

    class ScrollEventHandler : public EventHandler
    {
    public:
        void handle(const Event &event) override
        {
            if (event.type == "scroll")
            {
                cout << "  [ScrollEventHandler] Processing scroll: " << event.data << "\n";
                return; // Consume event
            }
            EventHandler::handle(event);
        }
    };

    class DefaultEventHandler : public EventHandler
    {
    public:
        void handle(const Event &event) override
        {
            cout << "  [DefaultEventHandler] Unknown event type: " << event.type << "\n";
            EventHandler::handle(event);
        }
    };

} // namespace event_chain

// ============================================================================
// EXAMPLE 5: COMMAND PROCESSING CHAIN
// ============================================================================

namespace command_chain
{

    struct Command
    {
        string operation;
        int value;

        Command(const string &op, int v = 0) : operation(op), value(v) {}
    };

    class CommandHandler
    {
    protected:
        CommandHandler *next = nullptr;

    public:
        virtual ~CommandHandler() = default;

        virtual void set_next(CommandHandler *handler)
        {
            next = handler;
        }

        virtual bool execute(const Command &cmd)
        {
            if (next)
            {
                return next->execute(cmd);
            }
            return false;
        }
    };

    class MathCommandHandler : public CommandHandler
    {
    private:
        int result = 0;

    public:
        bool execute(const Command &cmd) override
        {
            if (cmd.operation == "add")
            {
                result += cmd.value;
                cout << "  [MathCommandHandler] ADD: result = " << result << "\n";
                return true;
            }
            if (cmd.operation == "multiply")
            {
                result *= cmd.value;
                cout << "  [MathCommandHandler] MULTIPLY: result = " << result << "\n";
                return true;
            }
            return CommandHandler::execute(cmd);
        }

        int get_result() const { return result; }
    };

    class StringCommandHandler : public CommandHandler
    {
    private:
        string result;

    public:
        bool execute(const Command &cmd) override
        {
            if (cmd.operation == "append")
            {
                result += to_string(cmd.value);
                cout << "  [StringCommandHandler] APPEND: result = '" << result << "'\n";
                return true;
            }
            if (cmd.operation == "clear")
            {
                result.clear();
                cout << "  [StringCommandHandler] CLEAR: result = ''\n";
                return true;
            }
            return CommandHandler::execute(cmd);
        }

        string get_result() const { return result; }
    };

    class LogCommandHandler : public CommandHandler
    {
    private:
        vector<string> log;

    public:
        bool execute(const Command &cmd) override
        {
            log.push_back("Command: " + cmd.operation);
            cout << "  [LogCommandHandler] Logged: " << cmd.operation << "\n";
            return CommandHandler::execute(cmd);
        }

        const vector<string> &get_log() const { return log; }
    };

} // namespace command_chain

// ============================================================================
// EXAMPLES
// ============================================================================

void example1_creature_modifiers()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 1: CREATURE MODIFIERS CHAIN\n";
    cout << string(70, '=') << "\n\n";

    using namespace creature_modifiers;

    Creature goblin("Goblin", 1, 1);
    cout << "Initial: " << goblin << "\n\n";

    CreatureModifier root(goblin);
    DoubleAttackModifier r1(goblin);
    DoubleAttackModifier r1_2(goblin);
    IncreaseDefenseModifier r2(goblin);

    root.add(&r1);
    root.add(&r1_2);
    root.add(&r2);

    cout << "Applying modifier chain:\n";
    root.handle();

    cout << "\nFinal: " << goblin << "\n";
    cout << "BENEFIT: Modifiers stacked transparently\n";
}

void example2_logging_chain()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 2: LOGGING CHAIN (Multiple Levels)\n";
    cout << string(70, '=') << "\n\n";

    using namespace logging_chain;

    auto console = make_unique<ConsoleLogger>(LogLevel::INFO);
    auto file = make_unique<FileLogger>(LogLevel::WARNING);
    auto email = make_unique<EmailLogger>(LogLevel::ERROR);

    console->add_next(file.get());
    file->add_next(email.get());

    cout << "Logging messages through chain:\n\n";

    cout << "[INFO level]\n";
    console->log(LogLevel::INFO, "Application started");

    cout << "\n[WARNING level]\n";
    console->log(LogLevel::WARNING, "Disk space low");

    cout << "\n[ERROR level]\n";
    console->log(LogLevel::ERROR, "Database connection failed");

    cout << "\nFile contents: " << file->get_contents().size() << " entries\n";
    cout << "Emails sent: " << email->get_email_count() << "\n";
    cout << "BENEFIT: Each logger handles appropriate levels\n";
}

void example3_request_validation()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 3: REQUEST VALIDATION CHAIN\n";
    cout << string(70, '=') << "\n\n";

    using namespace validation_chain;

    UsernameValidator username_validator;
    PasswordValidator password_validator;
    EmailValidator email_validator;
    AgeValidator age_validator;

    username_validator.set_next(&password_validator);
    password_validator.set_next(&email_validator);
    email_validator.set_next(&age_validator);

    cout << "Request 1 (Valid):\n";
    Request req1("john_doe", "SecurePass123", "john@example.com", 25);
    bool valid1 = username_validator.validate(req1);
    cout << "Result: " << (valid1 ? "✓ ACCEPTED\n" : "✗ REJECTED\n");

    cout << "\nRequest 2 (Invalid - short password):\n";
    Request req2("jane_smith", "Pass1", "jane@example.com", 30);
    bool valid2 = username_validator.validate(req2);
    cout << "Result: " << (valid2 ? "✓ ACCEPTED\n" : "✗ REJECTED\n");

    cout << "\nRequest 3 (Invalid - too young):\n";
    Request req3("kid_user", "StrongPass123", "kid@example.com", 15);
    bool valid3 = username_validator.validate(req3);
    cout << "Result: " << (valid3 ? "✓ ACCEPTED\n" : "✗ REJECTED\n");

    cout << "\nBENEFIT: Validation rules in separate, composable handlers\n";
}

void example4_event_handling()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 4: GUI EVENT HANDLER CHAIN\n";
    cout << string(70, '=') << "\n\n";

    using namespace event_chain;

    MouseEventHandler mouse;
    KeyboardEventHandler keyboard;
    ScrollEventHandler scroll;
    DefaultEventHandler default_handler;

    mouse.set_next(&keyboard);
    keyboard.set_next(&scroll);
    scroll.set_next(&default_handler);

    cout << "Event 1: Mouse click\n";
    mouse.handle(Event("mouse_click", "300,150"));

    cout << "\nEvent 2: Keyboard key press\n";
    mouse.handle(Event("key_press", "Enter"));

    cout << "\nEvent 3: Scroll event\n";
    mouse.handle(Event("scroll", "down"));

    cout << "\nEvent 4: Unknown event\n";
    mouse.handle(Event("unknown_event"));

    cout << "\nBENEFIT: Events routed to appropriate handler\n";
}

void example5_command_processing()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 5: COMMAND PROCESSING CHAIN\n";
    cout << string(70, '=') << "\n\n";

    using namespace command_chain;

    auto math_handler = make_unique<MathCommandHandler>();
    auto string_handler = make_unique<StringCommandHandler>();
    auto log_handler = make_unique<LogCommandHandler>();

    math_handler->set_next(string_handler.get());
    string_handler->set_next(log_handler.get());

    cout << "Processing commands:\n\n";

    math_handler->execute(Command("add", 5));
    math_handler->execute(Command("multiply", 2));
    math_handler->execute(Command("append", 10));
    math_handler->execute(Command("add", 3));

    cout << "\nMath result: " << math_handler->get_result() << "\n";
    cout << "String result: '" << string_handler->get_result() << "'\n";
    cout << "Commands logged: " << log_handler->get_log().size() << "\n";

    cout << "\nBENEFIT: Different handlers process different commands\n";
}

void example_comparison()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "CHAIN OF RESPONSIBILITY - PATTERNS COMPARISON\n";
    cout << string(70, '=') << "\n\n";

    cout << "1. CREATURE MODIFIERS:\n";
    cout << "   - Each modifier may apply bonuses\n";
    cout << "   - Terminators (NoBonusesModifier) stop chain\n\n";

    cout << "2. LOGGING CHAIN:\n";
    cout << "   - Each logger handles certain log levels\n";
    cout << "   - Chain continues (all handlers process)\n\n";

    cout << "3. REQUEST VALIDATION:\n";
    cout << "   - Each validator checks one aspect\n";
    cout << "   - Stops immediately on first failure\n\n";

    cout << "4. EVENT HANDLING:\n";
    cout << "   - Events passed to appropriate handler\n";
    cout << "   - Handler consumes or passes along\n\n";

    cout << "5. COMMAND PROCESSING:\n";
    cout << "   - Commands routed to specialized handlers\n";
    cout << "   - Multiple handlers may process same command\n\n";

    cout << "KEY CHARACTERISTICS:\n";
    cout << "✓ Handler chain processes requests sequentially\n";
    cout << "✓ Handlers decide to process or delegate\n";
    cout << "✓ Loose coupling between handlers\n";
    cout << "✓ Handlers can be added/removed/reordered\n";
    cout << "✓ Request may not be handled at all\n";
}

int main()
{
    cout << "\n"
         << string(70, '*') << "\n";
    cout << "CHAIN OF RESPONSIBILITY PATTERN - EXAMPLES\n";
    cout << string(70, '*') << "\n";

    example1_creature_modifiers();
    example2_logging_chain();
    example3_request_validation();
    example4_event_handling();
    example5_command_processing();
    example_comparison();

    cout << "\n"
         << string(70, '*') << "\n";
    cout << "END OF EXAMPLES\n";
    cout << string(70, '*') << "\n\n";

    return 0;
}


// Make sure this appears before Goblin::get_attack()


class GoblinKing : public Goblin {
public:
    GoblinKing(Game& game) : Goblin(game, 3, 3) {}
};


int Goblin::get_attack() override {
    int attack = base_attack;

    for (auto c : game.creatures) {
        if (c != this) {
            // Make sure GoblinKing is fully defined at this point
            if (dynamic_cast<GoblinKing*>(c)) {
                attack += 1;
            }
        }
    }

    return attack;
}

