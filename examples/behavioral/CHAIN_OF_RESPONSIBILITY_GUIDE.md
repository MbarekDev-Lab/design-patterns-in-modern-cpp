# Chain of Responsibility Pattern

## Overview

The **Chain of Responsibility** is a behavioral design pattern that passes requests along a chain of handlers. Each handler decides whether to process the request or pass it to the next handler in the chain.

The pattern allows you to:

- **Create chains** of handlers dynamically
- **Decouple** senders from receivers
- **Process requests** through multiple handlers
- **Stop processing** at any point in the chain

## Motivation

In real-world applications, you often need to process requests through multiple handlers:

1. **Event Systems**: GUI frameworks pass events through handler chains
2. **Logging**: Messages flow through loggers of different levels
3. **Approval Workflows**: Requests pass through approval chains (manager → director → VP)
4. **Request Validation**: Each validator checks one aspect, passes to next
5. **Game Modifiers**: Game effects stack and modify entities

Without Chain of Responsibility, you'd have:

- Complex conditional logic (if-else chains)
- Tight coupling between handlers
- Difficulty adding/removing handlers
- Mixed responsibilities in one handler

## Structure

```
Client
  |
  v
┌──────────────────────────────┐
│ ConcreteHandler_1            │
│  - Can handle request?       │
│  - YES: process & return OR  │
│  - NO:  pass to next         │
└──────────────────────────────┘
         |
         | (delegates if needed)
         v
┌──────────────────────────────┐
│ ConcreteHandler_2            │
│  - Can handle request?       │
│  - YES: process & return OR  │
│  - NO:  pass to next         │
└──────────────────────────────┘
         |
         | (delegates if needed)
         v
┌──────────────────────────────┐
│ ConcreteHandler_N            │
│  - Process or return         │
└──────────────────────────────┘
```

## Implementation Patterns

### 1. Creature Modifiers (From Your Example)

Game effects that stack and modify creature stats:

```cpp
class Creature {
    string name;
    int attack, defense;
};

class CreatureModifier {  // Handler
protected:
    Creature& creature;
    CreatureModifier* next = nullptr;
public:
    explicit CreatureModifier(Creature& creature)
        : creature(creature) {}

    void add(CreatureModifier* cm) {
        if (next)
            next->add(cm);
        else
            next = cm;
    }

    virtual void handle() {
        if (next) next->handle();
    }
};

class DoubleAttackModifier : public CreatureModifier {
public:
    void handle() override {
        creature.attack *= 2;
        CreatureModifier::handle();  // Pass to next
    }
};

class NoBonusesModifier : public CreatureModifier {
public:
    void handle() override {
        // Terminal handler - no delegates
    }
};

// Usage:
Creature goblin("Goblin", 1, 1);
CreatureModifier root(goblin);
DoubleAttackModifier r1(goblin);
DoubleAttackModifier r2(goblin);
root.add(&r1);
root.add(&r2);
root.handle();  // goblin.attack = 4
```

**Key Points:**

- Terminal handlers (NoBonusesModifier) stop the chain
- Handlers may apply changes before delegating
- Chain is built with `add()` method

---

### 2. Logging Chain

Handlers process messages based on log level:

```cpp
enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

class Logger {
protected:
    Logger* next = nullptr;
    LogLevel handle_level;
public:
    Logger(LogLevel level) : handle_level(level) {}

    void add_next(Logger* logger) { next = logger; }

    void log(LogLevel level, const string& message) {
        if (level >= handle_level) {
            write(message);
        }
        if (next) {
            next->log(level, message);
        }
    }
};

class ConsoleLogger : public Logger {
protected:
    void write(const string& msg) override {
        cout << "[CONSOLE] " << msg << "\n";
    }
};

class FileLogger : public Logger {
protected:
    void write(const string& msg) override {
        file_contents.push_back(msg);
    }
};

// Usage:
ConsoleLogger console(LogLevel::INFO);
FileLogger file(LogLevel::WARNING);
console.add_next(&file);

console.log(LogLevel::INFO, "Starting app");      // Console only
console.log(LogLevel::WARNING, "Low memory");     // Console + File
console.log(LogLevel::ERROR, "Critical issue");   // Console + File
```

**Key Points:**

- All handlers in chain process the message
- Each handler checks if it should write
- Handlers are independent loggers

---

### 3. Request Validation Chain

Each validator checks one aspect, stops at first failure:

```cpp
struct Request {
    string username, password, email;
    int age;
};

class RequestValidator {
protected:
    RequestValidator* next = nullptr;
public:
    void set_next(RequestValidator* validator) { next = validator; }

    virtual bool validate(const Request& req) {
        if (next) {
            return next->validate(req);
        }
        return true;
    }
};

class UsernameValidator : public RequestValidator {
public:
    bool validate(const Request& req) override {
        if (req.username.length() < 3)
            return false;  // STOP - validation failed
        return RequestValidator::validate(req);
    }
};

class PasswordValidator : public RequestValidator {
public:
    bool validate(const Request& req) override {
        if (req.password.length() < 8)
            return false;  // STOP - validation failed
        return RequestValidator::validate(req);
    }
};

// Usage:
UsernameValidator username;
PasswordValidator password;
username.set_next(&password);

Request req("john_doe", "SecurePass123", "john@example.com", 25);
if (username.validate(req)) {
    // All validations passed
}
```

**Key Points:**

- Stops at first failure
- Early exit prevents unnecessary processing
- Clean separation of validation rules

---

### 4. GUI Event Handling

Events routed to appropriate handler:

```cpp
struct Event {
    string type;
    string data;
};

class EventHandler {
protected:
    EventHandler* next = nullptr;
public:
    void set_next(EventHandler* handler) { next = handler; }

    virtual void handle(const Event& event) {
        if (next) next->handle(event);
    }
};

class MouseEventHandler : public EventHandler {
public:
    void handle(const Event& event) override {
        if (event.type == "mouse_click") {
            cout << "Processing click at " << event.data << "\n";
            return;  // Consume event
        }
        EventHandler::handle(event);
    }
};

class KeyboardEventHandler : public EventHandler {
public:
    void handle(const Event& event) override {
        if (event.type == "key_press") {
            cout << "Processing key: " << event.data << "\n";
            return;  // Consume event
        }
        EventHandler::handle(event);
    }
};

class DefaultEventHandler : public EventHandler {
public:
    void handle(const Event& event) override {
        cout << "Unknown event: " << event.type << "\n";
    }
};

// Usage (typical GUI framework):
MouseEventHandler mouse;
KeyboardEventHandler keyboard;
DefaultEventHandler default_handler;

mouse.set_next(&keyboard);
keyboard.set_next(&default_handler);

mouse.handle(Event("mouse_click", "300,150"));
mouse.handle(Event("key_press", "Enter"));
mouse.handle(Event("unknown"));
```

**Key Points:**

- First handler to match consumes event (returns)
- Unhandled events pass to next handler
- Default handler as fallback

---

### 5. Command Processing Chain

Multiple handlers can process same command:

```cpp
struct Command {
    string operation;
    int value;
};

class CommandHandler {
protected:
    CommandHandler* next = nullptr;
public:
    virtual void set_next(CommandHandler* handler) { next = handler; }

    virtual void execute(const Command& cmd) {
        if (next) next->execute(cmd);
    }
};

class MathCommandHandler : public CommandHandler {
private:
    int result = 0;
public:
    void execute(const Command& cmd) override {
        if (cmd.operation == "add") {
            result += cmd.value;
            return;
        }
        if (cmd.operation == "multiply") {
            result *= cmd.value;
            return;
        }
        CommandHandler::execute(cmd);
    }
};

class StringCommandHandler : public CommandHandler {
private:
    string result;
public:
    void execute(const Command& cmd) override {
        if (cmd.operation == "append") {
            result += to_string(cmd.value);
            return;
        }
        CommandHandler::execute(cmd);
    }
};

class LogCommandHandler : public CommandHandler {
public:
    void execute(const Command& cmd) override {
        cout << "LOG: " << cmd.operation << "\n";
        CommandHandler::execute(cmd);
    }
};
```

**Key Points:**

- Different handlers specialize in different commands
- Some handlers always delegate (LogCommandHandler)
- Others handle and return (MathCommandHandler)

---

## Two Approaches

### Approach 1: Always Call Super (Continue Chain)

```cpp
void handle() override {
    // Do work
    CreatureModifier::handle();  // Always delegate
}
```

Useful for: Logging, chaining effects, cumulative processing

### Approach 2: Conditionally Delegate

```cpp
void validate() override {
    if (!is_valid()) {
        return false;  // STOP HERE
    }
    return RequestValidator::validate(req);  // Continue
}
```

Useful for: Validation, early exit, event handling

---

## Key Benefits

1. **Loose Coupling**: Senders don't know about handlers
2. **Single Responsibility**: Each handler has one job
3. **Dynamic Chains**: Add/remove handlers at runtime
4. **Flexible Order**: Reorder handlers to change behavior
5. **Easy to Extend**: Add new handler types easily

## Key Trade-offs

1. **Complexity**: More classes to manage
2. **Performance**: Chain traversal adds overhead
3. **Debugging**: Request path harder to trace
4. **Guarantees**: Request might not be handled
5. **Null Safety**: Must handle unhandled requests

## Variations & Extensions

### Weak Pointer Chains

Prevent dangling pointers:

```cpp
class CreatureModifier {
private:
    weak_ptr<CreatureModifier> next;
public:
    void add(shared_ptr<CreatureModifier> cm) {
        if (auto n = next.lock()) {
            n->add(cm);
        } else {
            next = cm;
        }
    }
};
```

### Event Bubbling

Events bubble up through hierarchy:

```cpp
void handle(const Event& event, bool bubble = true) {
    if (process(event)) {
        return;  // Consumed
    }
    if (bubble && next) {
        next->handle(event, bubble);
    }
}
```

### Handler Priority

Process by priority:

```cpp
struct HandlerEntry {
    int priority;
    Handler* handler;
};

class PriorityChain {
    vector<HandlerEntry> handlers;  // Sorted by priority
};
```

### Async Handling

Non-blocking handler chains:

```cpp
future<bool> handle_async(const Request& req) {
    if (can_handle()) {
        return async(launch::async, [this]() {
            return process();
        });
    }
    if (next) {
        return next->handle_async(req);
    }
}
```

---

## Comparison with Other Patterns

| Pattern                     | Purpose                         | When to Use                       |
| --------------------------- | ------------------------------- | --------------------------------- |
| **Chain of Responsibility** | Route requests through handlers | Multiple handlers, dynamic chains |
| Strategy                    | Select algorithm                | Static choice, one handler        |
| Command                     | Encapsulate requests            | Action history, undo/redo         |
| Iterator                    | Traverse collection             | Sequence access                   |
| Decorator                   | Add responsibilities            | Transparent wrapping              |

---

## Real-World Examples

### 1. Express.js Middleware

```cpp
// Express-like middleware chain
vector<function<void(Request&, Response&)>> middlewares;
middlewares.push_back(authentication);
middlewares.push_back(logging);
middlewares.push_back(route_handler);

for (auto& middleware : middlewares) {
    middleware(req, res);
}
```

### 2. Java Exception Handling

```cpp
try {
    // code
} catch (SpecificException& e) {
    // handler 1
} catch (GeneralException& e) {
    // handler 2
} catch (...) {
    // handler N (default)
}
```

### 3. Web Server Request Pipeline

```
Proxy → Authentication → Logging → Rate Limiting → Handler
```

### 4. Game Event System

```
Input → UI → Combat System → Physics → Rendering
```

---

## Implementation Checklist

- [x] Create Handler base class with `next` pointer
- [x] Implement `set_next()` to build chains
- [x] Create ConcreteHandler classes
- [x] Override `handle()` to process or delegate
- [x] Decide: always delegate or conditional?
- [x] Handle unhandled requests gracefully
- [x] Test chain processing order
- [x] Test handler elimination
- [x] Consider performance of chain traversal

## Testing Considerations

1. **Chain Processing**: Does request traverse correctly?
2. **Handler Selection**: Does correct handler process request?
3. **Early Exit**: Does terminator stop chain?
4. **Unhandled**: What happens if no handler accepts?
5. **Order Matters**: Does handler order affect results?
6. **Shared State**: Do handlers interfere with each other?

## Common Mistakes to Avoid

1. **Forgetting to delegate**: Handler doesn't call `next->handle()`
2. **Circular chains**: Chain refers back to itself
3. **Null pointer dereference**: Accessing uninitialzed `next`
4. **Assuming handling**: Request might not be handled anywhere
5. **Performance**: Long chains with expensive operations
6. **Memory leaks**: Improper ownership (use smart pointers!)

## Summary

The Chain of Responsibility pattern is essential for building flexible, extensible systems where requests flow through multiple handlers. It's particularly valuable for:

- **Event Systems**: GUI frameworks, game engines
- **Logging**: Multi-level logging with different outputs
- **Validation**: Breaking validation into steps
- **Middleware**: Web servers, API gateways
- **Game Modifiers**: Stacking effects and bonuses

The pattern shines when you need to compose behavior dynamically and decouple request sources from handlers while maintaining the ability to add, remove, or reorder handlers at runtime.
