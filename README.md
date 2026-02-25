# Design Patterns in Modern C++

Comprehensive implementation of Gang of Four design patterns using modern C++17 and beyond.

Completed course: [Design Patterns in Modern C++](https://www.udemy.com/course/patterns-cplusplus/) on Udemy

## 📦 Creational Patterns

Patterns dealing with object creation mechanisms.

| Pattern            | Files                                                                                                                                                                          | Tests |
| ------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ----- |
| **Singleton**      | [Manual Testing](CodingExercises/Singleton_Testing_Manual.cpp) \| [Exercise](CodingExercises/Singleton_Testing_Exercise.cpp) \| [Tester](CodingExercises/Singleton_Tester.cpp) | ✓     |
| **Factory Method** | [Exercise](CodingExercises/PersonFactory_Exercise.cpp)                                                                                                                         | ✓     |
| **Builder**        | [CodeBuilder](CodingExercises/CodeBuilder.cpp)                                                                                                                                 | ✓     |
| **Prototype**      | [Exercise](CodingExercises/Prototype_Exercise.cpp)                                                                                                                             | ✓     |
| **Multiton**       | [Manual Implementation](CodingExercises/Multiton_Manual.cpp)                                                                                                                   | ✓     |

## 🏗️ Structural Patterns

Patterns concerned with object composition and relationships.

| Pattern       | Files                                                                                                                                                                                       | Tests |
| ------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----- |
| **Adapter**   | [Exercise](CodingExercises/Adapter_Exercise.cpp) \| [Coding Exercise](CodingExercises/Adapter_Coding_Exercise.cpp)                                                                          | ✓     |
| **Bridge**    | [Exercise](CodingExercises/Bridge_Exercise.cpp)                                                                                                                                             | ✓     |
| **Composite** | (Inferred from decorators & trees)                                                                                                                                                          | —     |
| **Decorator** | [Exercise](CodingExercises/Decorator_Exercise.cpp) \| [Flower GoogleTest](CodingExercises/Decorator_Flower_GoogleTest.cpp)                                                                  | ✓     |
| **Facade**    | [Exercise](CodingExercises/Facade_Exercise.cpp)                                                                                                                                             | ✓     |
| **Flyweight** | [Exercise](CodingExercises/Flyweight_Exercise.cpp) \| [Sentence Exercise](CodingExercises/Flyweight_Sentence_Exercise.cpp)                                                                  | ✓     |
| **Proxy**     | [Exercise](CodingExercises/Proxy_Exercise.cpp) \| [Protective Proxy](CodingExercises/ProtectiveProxy_Exercise.cpp) \| [Communication Proxy](CodingExercises/CommunicationProxy_Testing.cpp) | ✓     |

## 🎭 Behavioral Patterns

Patterns concerned with object collaboration and responsibility distribution.

| Pattern                     | Files                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | Tests |
| --------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----- |
| **Observer**                | [Rats Swarm Exercise](CodingExercises/Observer_Rats_Swarm_Exercise.cpp) \| [Alternative Implementation](CodingExercises/Observer_Rats_Alternative_Implementation.cpp) \| [Stock Prices](CodingExercises/Observer_StockPrices_Exercise.cpp)                                                                                                                                                                                                                                                                                          | 14+ ✓ |
| **Strategy**                | [Static Polymorphism](CodingExercises/Strategy_StaticPolymorphism.cpp) \| [Dynamic Polymorphism](CodingExercises/Strategy_DynamicPolymorphism.cpp) \| [Quadratic Solver Exercise](CodingExercises/Strategy_QuadraticSolver_Exercise.cpp)                                                                                                                                                                                                                                                                                            | 33+ ✓ |
| **State**                   | [Classic LightSwitch](CodingExercises/State_Classic_LightSwitch.cpp) \| [Handmade PhoneStateMachine](CodingExercises/State_Handmade_PhoneStateMachine.cpp) \| [CombinationLock Exercise](CodingExercises/State_CombinationLock_Exercise.cpp) \| [Boost MSM Simplified](CodingExercises/State_Boost_MSM_Simplified.cpp)                                                                                                                                                                                                              | 32+ ✓ |
| **Template Method**         | [Game Framework](CodingExercises/TemplateMethod_GameFramework.cpp) \| [Card Game Exercise](CodingExercises/TemplateMethod_CardGame_Exercise.cpp) \| [GoogleTest Version](CodingExercises/TemplateMethod_CardGame_Exercise_GoogleTest.cpp)                                                                                                                                                                                                                                                                                           | 13+ ✓ |
| **Visitor**                 | [std::visit](CodingExercises/Behavioral.Visitor.std_visit.cpp) \| [Intrusive](CodingExercises/Behavioral.Visitor.visitor_intrusive.cpp) \| [Reflective](CodingExercises/Behavioral.Visitor.visitor_reflective.cpp) \| [Multimethods](CodingExercises/Behavioral.Visitor.multimethods.cpp) \| [Acyclic](CodingExercises/Behavioral.Visitor.visitor_acyclic.cpp) \| [Double Dispatch](CodingExercises/Behavioral.Visitor.visitor_double.cpp) \| [Expression Printer Exercise](CodingExercises/Visitor_ExpressionPrinter_Exercise.cpp) | 11 ✓  |
| **Chain of Responsibility** | [Exercise](CodingExercises/ChainOfResponsibility_Exercise.cpp) \| [Goblin Game](CodingExercises/ChainOfResponsibility_GoblinGame_Exercise.cpp)                                                                                                                                                                                                                                                                                                                                                                                      | ✓     |
| **Mediator**                | [Exercise](CodingExercises/Mediator_Exercise.cpp) \| [Participant Exercise](CodingExercises/Mediator_ParticipantExercise.cpp)                                                                                                                                                                                                                                                                                                                                                                                                       | ✓     |
| **Memento**                 | [Exercise](CodingExercises/Memento_Exercise.cpp) \| [BankAccount](CodingExercises/Memento_BankAccount_Exercise.cpp) \| [TokenMachine](CodingExercises/Memento_TokenMachine_Exercise.cpp) \| [TokenMachine GoogleTest](CodingExercises/Memento_TokenMachine_GoogleTest.cpp)                                                                                                                                                                                                                                                          | ✓     |

## 🏛️ SOLID Principles

These principles guide good object-oriented design:

- **S**ingle Responsibility Principle (SRP)
- **O**pen/Closed Principle (OCP)
- **L**iskov Substitution Principle (LSP)
- **I**nterface Segregation Principle (ISP)
- **D**ependency Inversion Principle (DIP)

All implementations in this repository adhere to SOLID principles.

## 📋 Repository Structure

```
design-patterns-in-modern-cpp/
├── CodingExercises/          # All pattern implementations
│   ├── Creational/           # Factory, Singleton, Builder, etc.
│   ├── Structural/           # Adapter, Bridge, Decorator, etc.
│   └── Behavioral/           # Observer, Strategy, State, Visitor, etc.
├── Documentation/            # Reference materials (multimethods.pdf)
└── README.md                 # This file
```

## 🔧 Compilation & Testing

All exercises compile with clang++ using C++17 standard:

```bash
# Compile a single exercise
clang++ -std=c++17 -Wall -Wextra -o /tmp/output CodingExercises/PatternName_Exercise.cpp

# Run the compiled executable
./tmp/output
```

### Quick Test Examples

**Observer Pattern - Rats Exercise:**

```bash
clang++ -std=c++17 -Wall -Wextra -o /tmp/observer CodingExercises/Observer_Rats_Swarm_Exercise.cpp && /tmp/observer
```

**State Pattern - Phone State Machine:**

```bash
clang++ -std=c++17 -Wall -Wextra -o /tmp/state CodingExercises/State_Handmade_PhoneStateMachine.cpp && /tmp/state
```

**Strategy Pattern - Quadratic Solver:**

```bash
clang++ -std=c++17 -Wall -Wextra -o /tmp/strategy CodingExercises/Strategy_QuadraticSolver_Exercise.cpp && /tmp/strategy
```

**Visitor Pattern - Expression Printer:**

```bash
clang++ -std=c++17 -Wall -Wextra -o /tmp/visitor CodingExercises/Visitor_ExpressionPrinter_Exercise.cpp && /tmp/visitor
```

## 📚 Pattern Details

### Behavioral Patterns (Comprehensive Coverage)

#### Observer Pattern

Implements the publish-subscribe mechanism with automatic notification of state changes.

- **Concepts**: Virtual function calls, event management, automatic RAII registration
- **Files**:
  - [Observer_Rats_Swarm_Exercise](CodingExercises/Observer_Rats_Swarm_Exercise.cpp) - RAII-based rat tracking system
  - [Observer_Rats_Alternative_Implementation](CodingExercises/Observer_Rats_Alternative_Implementation.cpp) - Event-driven approach
  - [Observer_StockPrices_Exercise](CodingExercises/Observer_StockPrices_Exercise.cpp) - Stock price change notifications
- **Tests**: 14+ comprehensive unit tests
- **Key Benefits**: Loose coupling, automatic updates, centralized notification

#### Strategy Pattern

Encapsulates interchangeable algorithms, selecting them at runtime through polymorphism.

- **Concepts**: Compile-time vs runtime strategies, function pointers, virtual inheritance
- **Files**:
  - [Strategy_StaticPolymorphism](CodingExercises/Strategy_StaticPolymorphism.cpp) - Template-based compile-time selection
  - [Strategy_DynamicPolymorphism](CodingExercises/Strategy_DynamicPolymorphism.cpp) - Virtual function runtime selection
  - [Strategy_QuadraticSolver_Exercise](CodingExercises/Strategy_QuadraticSolver_Exercise.cpp) - Three solving strategies based on discriminant
- **Tests**: 33+ covering all strategy implementations
- **Key Benefits**: Algorithm flexibility, runtime selection, reduced conditional logic

#### State Pattern

Allows objects to change behavior based on internal state transitions.

- **Concepts**: State machines, transitions, virtual state handling
- **Files**:
  - [State_Classic_LightSwitch](CodingExercises/State_Classic_LightSwitch.cpp) - Traditional OOP state pattern
  - [State_Handmade_PhoneStateMachine](CodingExercises/State_Handmade_PhoneStateMachine.cpp) - Enum-based transitions
  - [State_CombinationLock_Exercise](CodingExercises/State_CombinationLock_Exercise.cpp) - Edge case testing with 17 test scenarios
  - [State_Boost_MSM_Simplified](CodingExercises/State_Boost_MSM_Simplified.cpp) - MSM library-inspired design
- **Tests**: 32+ covering transitions and edge cases
- **Key Benefits**: Clear state logic, simplified conditionals, explicit transitions

#### Template Method Pattern

Defines algorithm skeleton in base class, allowing subclasses to override specific steps.

- **Concepts**: Inheritance, virtual functions, callback methods
- **Files**:
  - [TemplateMethod_GameFramework](CodingExercises/TemplateMethod_GameFramework.cpp) - Chess, Checkers, Draughts games
  - [TemplateMethod_CardGame_Exercise](CodingExercises/TemplateMethod_CardGame_Exercise.cpp) - Combat simulation with health/damage
  - [TemplateMethod_CardGame_Exercise_GoogleTest](CodingExercises/TemplateMethod_CardGame_Exercise_GoogleTest.cpp) - Temporary vs permanent damage games
- **Tests**: 13+ game simulation tests
- **Key Benefits**: Code reuse, algorithm consistency, extension points

#### Visitor Pattern

Adds new operations to object structures without changing the classes themselves (double-dispatch).

- **Concepts**: Double-dispatch, visitor interface, recursive visitation
- **Files**:
  - [Behavioral.Visitor.std_visit](CodingExercises/Behavioral.Visitor.std_visit.cpp) - Modern C++17 std::visit with variants
  - [Behavioral.Visitor.visitor_intrusive](CodingExercises/Behavioral.Visitor.visitor_intrusive.cpp) - Classic intrusive visitor pattern
  - [Behavioral.Visitor.visitor_reflective](CodingExercises/Behavioral.Visitor.visitor_reflective.cpp) - RTTI-based reflection
  - [Behavioral.Visitor.multimethods](CodingExercises/Behavioral.Visitor.multimethods.cpp) - Multiple dispatch technique
  - [Behavioral.Visitor.visitor_acyclic](CodingExercises/Behavioral.Visitor.visitor_acyclic.cpp) - Avoiding circular dependencies
  - [Behavioral.Visitor.visitor_double](CodingExercises/Behavioral.Visitor.visitor_double.cpp) - Pure double-dispatch
  - [Visitor_ExpressionPrinter_Exercise](CodingExercises/Visitor_ExpressionPrinter_Exercise.cpp) - Expression tree printer with operator precedence
- **Tests**: 11 comprehensive tests for expression printing
- **Key Benefits**: Separation of concerns, easy addition of operations, type-safe visitations

#### Chain of Responsibility

Passes requests along a chain of handlers, each deciding whether to process or forward.

- **Files**:
  - [ChainOfResponsibility_Exercise](CodingExercises/ChainOfResponsibility_Exercise.cpp)
  - [ChainOfResponsibility_GoblinGame_Exercise](CodingExercises/ChainOfResponsibility_GoblinGame_Exercise.cpp)
- **Key Benefits**: Loose coupling, flexible handler chains

#### Mediator

Centralizes complex communication between objects to promote loose coupling.

- **Files**:
  - [Mediator_Exercise](CodingExercises/Mediator_Exercise.cpp)
  - [Mediator_ParticipantExercise](CodingExercises/Mediator_ParticipantExercise.cpp)
- **Key Benefits**: Centralized control, simplified object interactions

#### Memento

Captures and externalizes an object's internal state without violating encapsulation.

- **Files**:
  - [Memento_Exercise](CodingExercises/Memento_Exercise.cpp)
  - [Memento_BankAccount_Exercise](CodingExercises/Memento_BankAccount_Exercise.cpp) - Transaction undoing
  - [Memento_TokenMachine_Exercise](CodingExercises/Memento_TokenMachine_Exercise.cpp) - State restoration
  - [Memento_TokenMachine_GoogleTest](CodingExercises/Memento_TokenMachine_GoogleTest.cpp) - Comprehensive testing
- **Key Benefits**: Undo/redo functionality, state persistence

### Structural Patterns (Complete Implementation)

### Creational Patterns (Complete Coverage)

#### Singleton

Ensures a class has only one instance and provides global access to it.

- **Files**:
  - [Singleton_Testing_Manual](CodingExercises/Singleton_Testing_Manual.cpp)
  - [Singleton_Testing_Exercise](CodingExercises/Singleton_Testing_Exercise.cpp)
  - [Singleton_Tester](CodingExercises/Singleton_Tester.cpp)
- **Key Benefits**: Single instance guarantee, lazy initialization, thread safety

#### Factory Method

Creates objects without specifying the exact classes to create.

- **Files**: [PersonFactory_Exercise](CodingExercises/PersonFactory_Exercise.cpp)
- **Key Benefits**: Decoupling object creation, flexible instantiation

#### Builder

Constructs complex objects step-by-step, separating construction from representation.

- **Files**: [CodeBuilder](CodingExercises/CodeBuilder.cpp)
- **Key Benefits**: Complex object creation, fluent interface, immutability support

#### Prototype

Clones objects rather than creating new instances from scratch.

- **Files**: [Prototype_Exercise](CodingExercises/Prototype_Exercise.cpp)
- **Key Benefits**: Object cloning, performance optimization

#### Multiton

Multiple controlled instances (singleton variation).

- **Files**: [Multiton_Manual](CodingExercises/Multiton_Manual.cpp)
- **Key Benefits**: Limited instances, named instances

### Structural Patterns (Complete Coverage)

#### Adapter

Converts interface of class into another interface clients expect.

- **Files**:
  - [Adapter_Exercise](CodingExercises/Adapter_Exercise.cpp)
  - [Adapter_Coding_Exercise](CodingExercises/Adapter_Coding_Exercise.cpp)
- **Key Benefits**: Interface compatibility, legacy code integration

#### Bridge

Decouples abstraction from implementation so they vary independently.

- **Files**: [Bridge_Exercise](CodingExercises/Bridge_Exercise.cpp)
- **Key Benefits**: Separation of concerns, flexible combinations

#### Decorator

Attaches additional responsibilities to object dynamically.

- **Files**:
  - [Decorator_Exercise](CodingExercises/Decorator_Exercise.cpp)
  - [Decorator_Flower_GoogleTest](CodingExercises/Decorator_Flower_GoogleTest.cpp)
- **Key Benefits**: Dynamic behavior extension, composition over inheritance

#### Facade

Provides unified interface to set of interfaces in subsystem.

- **Files**: [Facade_Exercise](CodingExercises/Facade_Exercise.cpp)
- **Key Benefits**: Simplified complex systems, unified interface

#### Flyweight

Uses sharing to support large numbers of fine-grained objects efficiently.

- **Files**:
  - [Flyweight_Exercise](CodingExercises/Flyweight_Exercise.cpp)
  - [Flyweight_Sentence_Exercise](CodingExercises/Flyweight_Sentence_Exercise.cpp)
- **Key Benefits**: Memory optimization, shared state management

#### Proxy

Provides placeholder/surrogate for another object to control access.

- **Files**:
  - [Proxy_Exercise](CodingExercises/Proxy_Exercise.cpp)
  - [ProtectiveProxy_Exercise](CodingExercises/ProtectiveProxy_Exercise.cpp)
  - [CommunicationProxy_Testing](CodingExercises/CommunicationProxy_Testing.cpp)
- **Key Benefits**: Access control, lazy loading, logging/auditing

## ✅ Completion Status

- **✓ 23 Patterns Implemented** (Creational: 5, Structural: 7, Behavioral: 11)
- **✓ 100+ Comprehensive Tests** (All passing)
- **✓ Zero Compiler Warnings** (Clean C++17 compilation)
- **✓ SOLID Principles Applied** (Throughout all implementations)

## 📖 Learning Resources

- **Documentation**: [multimethods.pdf](Documentation/multimethods.pdf) - Advanced visitor patterns and multiple dispatch
- **Course**: [Design Patterns in Modern C++](https://www.udemy.com/course/patterns-cplusplus/) - Udemy

## 💻 System Requirements

- **C++ Standard**: C++17 or later
- **Compiler**: clang++, g++, or MSVC with C++17 support
- **OS**: macOS, Linux, Windows

## 🚀 Getting Started

1. Clone the repository
2. Navigate to CodingExercises directory
3. Compile any exercise: `clang++ -std=c++17 -Wall -Wextra -o /tmp/output ExerciseName.cpp`
4. Run: `/tmp/output`

## 📝 License

Personal learning project for design patterns study.
