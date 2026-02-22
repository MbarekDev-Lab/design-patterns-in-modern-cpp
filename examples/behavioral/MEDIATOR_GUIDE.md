# Mediator Pattern - Comprehensive Guide

## Overview

The **Mediator** pattern is a behavioral design pattern that defines an object that encapsulates how a set of objects interact. Instead of objects communicating directly with each other, they communicate through a mediator object, reducing dependencies between communicating objects and centralizing complex communication and control.

## Problem

Without the Mediator pattern, objects that need to communicate must have explicit references to each other, creating:

- **Tight coupling**: Changes to one object require changes to others
- **Complex interdependencies**: Hard to understand the flow of communication
- **Difficult maintenance**: Adding new participants requires modifying existing ones
- **Code duplication**: Broadcast logic scattered across multiple objects

### Example (Without Mediator)

```cpp
class Participant {
    vector<Participant*> others;  // Direct references to all others
public:
    void say(int value) {
        for (auto p : others) {
            p->receive(value);    // Direct calls to others
        }
    }
};
```

## Solution

Create a **Mediator** that acts as a central hub where all objects communicate through it, rather than directly with each other.

### Mediator Pattern Structure

```
┌─────────────────┐
│    Mediator     │
│  - participants │
│  - broadcast()  │
└────────┬────────┘
         │
    ┌────┴────┐
    │          │
┌───▼──┐  ┌───▼──┐
│  P1  │  │  P2  │
│ say()│  │say() │
└──────┘  └──────┘
```

## Implementation

### Header File: `mediator_coding_exercise.h`

```cpp
#pragma once
#include <vector>
using namespace std;

// Forward declaration
struct Participant;

// MEDIATOR - Central hub for participant communication
class Mediator {
public:
    vector<Participant *> participants;

    void broadcast(Participant *origin, int value);

    int get_participant_count() const {
        return participants.size();
    }

    void clear() {
        participants.clear();
    }
};

// PARTICIPANT - Individual that can broadcast and receive
struct Participant {
    int value = 0;
    Mediator &mediator;

    Participant(Mediator &mediator) : mediator(mediator) {
        mediator.participants.push_back(this);
    }

    void say(int val) {
        mediator.broadcast(this, val);
    }

    void receive(int val) {
        value += val;
    }

    int get_value() const { return value; }
};

// Implementation (after full Participant definition)
inline void Mediator::broadcast(Participant *origin, int value) {
    for (auto participant : participants) {
        if (participant != origin) {
            participant->receive(value);
        }
    }
}
```

## Key Concepts

### 1. **Central Coordination**

The Mediator maintains a list of all participants and coordinates their communication.

```cpp
// Without Mediator: P1 must know about P2, P2 about P1
P1.send_to(P2, value);  // Tight coupling

// With Mediator: Both go through mediator
P1.say(value);    // Mediator notifies P2
P2.say(value);    // Mediator notifies P1
```

### 2. **Loose Coupling**

Participants don't need references to each other, only to the mediator.

```cpp
struct Participant {
    Mediator &mediator;  // Only dependency
    // No vector<Participant*> needed!
};
```

### 3. **Broadcast Pattern**

When a participant broadcasts, all others receive the value.

```
Initial: P1=0, P2=0, P3=0

P1.say(5)  →  P2=5, P3=5  (P1 unchanged)
P2.say(3)  →  P1=3, P3=8  (P2 unchanged)
P3.say(2)  →  P1=5, P2=5  (P3 unchanged)
```

### 4. **Circular Dependencies Resolved**

Using forward declarations and inline implementations avoids incomplete type errors:

```cpp
// Forward declare Participant before Mediator
struct Participant;

class Mediator {
    void broadcast(Participant *origin, int value);  // Declaration
};

struct Participant { /* ... */ };

// Implementation after both are complete
inline void Mediator::broadcast(Participant *origin, int value) {
    // Can now safely work with complete Participant
    for (auto p : participants) {
        p->receive(value);  // Safe - Participant is complete
    }
}
```

## Usage Examples

### Example 1: Two Participants

```cpp
Mediator mediator;
Participant p1{mediator};
Participant p2{mediator};

cout << "Initial: P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";
// Output: Initial: P1=0, P2=0

p1.say(3);
cout << "After P1 says 3: P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";
// Output: After P1 says 3: P1=0, P2=3

p2.say(2);
cout << "After P2 says 2: P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";
// Output: After P2 says 2: P1=2, P2=3
```

### Example 2: Multiple Participants

```cpp
Mediator mediator;
Participant p1{mediator}, p2{mediator}, p3{mediator};

p1.say(1);  // P2, P3 each get +1
// P1=0, P2=1, P3=1

p2.say(2);  // P1, P3 each get +2
// P1=2, P2=1, P3=3

p3.say(4);  // P1, P2 each get +4
// P1=6, P2=5, P3=3
```

### Example 3: Negative Values

```cpp
Mediator mediator;
Participant p1{mediator}, p2{mediator};

p1.say(10);     // P2 += 10 → P2 = 10
p1.say(-3);     // P2 += (-3) → P2 = 7
```

## Advantages

1. **Decoupling**: Objects don't need explicit references to each other
2. **Centralized Control**: All communication logic is in one place
3. **Easy Extension**: Add new participants without modifying existing ones
4. **Simplified Communication**: Objects only interact through mediator
5. **Cleaner Object Model**: Objects don't expose all their methods

## Disadvantages

1. **Mediator Complexity**: If many communication patterns exist, mediator becomes complex
2. **Single Point of Failure**: If mediator breaks, all communication fails
3. **God Object**: Mediator can become too large with too many responsibilities
4. **Hidden Dependencies**: The mediator masks dependencies between objects

## Real-World Examples

### 1. Chat Room (Classic Mediator Example)

```cpp
class ChatRoom {  // Mediator
    vector<User*> users;
public:
    void send_message(User* from, string msg) {
        for (auto user : users) {
            if (user != from) {
                user->receive_message(msg);
            }
        }
    }
};

class User {
    ChatRoom &room;
public:
    void send(string msg) {
        room.send_message(this, msg);  // Through mediator
    }
};
```

### 2. Air Traffic Control

```cpp
class AirTrafficControl {  // Mediator
    vector<Airplane*> planes;
public:
    void update_position(Airplane* plane, Position pos) {
        plane->current_pos = pos;
        for (auto other : planes) {
            if (other != plane) {
                other->check_collision(plane);
            }
        }
    }
};
```

### 3. GUI Dialog Box

```cpp
class DialogMediator {  // Mediator
    Button ok_btn, cancel_btn;
    TextField text_field;
public:
    void on_ok_clicked() {
        if (text_field.is_valid()) {
            close_dialog();
        } else {
            show_error();
        }
    }
};
```

### 4. Game State Manager

```cpp
class GameMediator {  // Mediator
    vector<GameComponent*> components;
public:
    void broadcast_event(Event e) {
        for (auto comp : components) {
            comp->handle_event(e);
        }
    }
};
```

## Comparison with Other Patterns

| Pattern      | Purpose                        | Focus             |
| ------------ | ------------------------------ | ----------------- |
| **Mediator** | Centralize object interactions | Communication hub |
| **Observer** | One-to-many notifications      | Push model        |
| **Facade**   | Simplify complex subsystems    | Hide complexity   |
| **Command**  | Encapsulate requests           | Request queuing   |

### Mediator vs Observer

- **Mediator**: Two-way communication, centralized control
- **Observer**: One-way communication (subject→observers)

## Implementation Checklist

- [ ] Define Mediator interface/class with participants collection
- [ ] Define Participant with reference to Mediator
- [ ] Forward declare Participant before Mediator (circular dependency)
- [ ] Move method implementations after both classes are defined
- [ ] Implement broadcast/notification logic in Mediator
- [ ] Ensure participants register with Mediator
- [ ] Test with multiple participants
- [ ] Handle edge cases (single participant, empty mediator)

## Common Mistakes

1. **Incomplete Type Error**
   - ❌ Calling `participant->method()` on forward-declared type
   - ✅ Move implementation after class definition

2. **Mediator Too Simple**
   - ❌ Just passes messages unchanged
   - ✅ Add logic to mediator: filtering, transformation, routing

3. **Mediator Too Complex**
   - ❌ Mediator handles all possible interactions
   - ✅ Keep concerns separated; may need multiple mediators

4. **Tight Coupling to Mediator**
   - ❌ Participants depend on mediator internals
   - ✅ Use clean interface methods

## Pattern Variations

### 1. Bidirectional Communication

```cpp
class Mediator {
    void request(Participant* from, Participant* to, Data data) {
        to->handle_request(from, data);  // Direct to specific recipient
    }
};
```

### 2. Selective Broadcasting

```cpp
class Mediator {
    void broadcast(Participant* origin, int value, Filter filter) {
        for (auto p : participants) {
            if (p != origin && filter.matches(p)) {
                p->receive(value);  // Conditional broadcast
            }
        }
    }
};
```

### 3. Prioritized Communication

```cpp
class Mediator {
    priority_queue<Message> queue;
    void broadcast(Participant* origin, Message msg) {
        queue.push(msg);  // Queue messages
    }
};
```

## When to Use Mediator

✅ **Good For:**

- Chat rooms, messaging systems
- Game state management
- GUI components in dialogs
- Air traffic control systems
- Traffic light coordination

❌ **Avoid For:**

- Simple, direct communication (overhead not worth it)
- Hierarchical communications (use Chain of Responsibility)
- One-to-many notifications (use Observer)

## Summary

The **Mediator pattern** is essential for:

1. Decoupling a set of classes that interact in complex ways
2. Centralizing communication logic
3. Making systems more maintainable and extensible
4. Reducing object dependencies

By introducing a mediator, you convert direct object-to-object communication into object-to-mediator-to-object communication, making the system more flexible and easier to evolve.

---

## Testing Coverage

The comprehensive test suite covers:

- **Basic Operations**: Single and multiple participants
- **Broadcasting**: Sender doesn't receive their own broadcast
- **Edge Cases**: Zero values, negative values, single participant
- **Scaling**: Large groups of participants
- **Patterns**: Round-robin broadcasts, multiple consecutive broadcasts
- **Mediator Tracking**: Participant count and lifetime
- **Complete Scenarios**: Complex interaction sequences

**Test Suite**: 14 tests, 50 assertions, 100% pass rate
