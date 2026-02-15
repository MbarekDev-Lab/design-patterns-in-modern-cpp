# Design Patterns in Modern C++

Course code from the Udemy course "Design Patterns in Modern C++".

This repository contains implementations of various design patterns using modern C++ (C++17 and beyond).

## Course

[Design Patterns in Modern C++](https://www.udemy.com/course/patterns-cplusplus/learn/lecture/7736680?start=15#content) on Udemy

## Patterns Covered

- [x] Creational Patterns
  - [x] Singleton - [View](include/creational/singleton.h) | [Example](examples/creational/singleton_example.cpp)
  - [ ] Factory
  - [x] Builder - [View](include/creational/builder.h) | [Example](examples/creational/builder_example.cpp)
  - [ ] Prototype
  - [ ] Abstract Factory
- [ ] Structural Patterns
  - [ ] Adapter
  - [ ] Bridge
  - [ ] Composite
  - [ ] Decorator
  - [ ] Facade
  - [ ] Flyweight
  - [ ] Proxy

- [ ] Behavioral Patterns
  - [ ] Observer
  - [ ] Strategy
  - [ ] Command
  - [ ] Chain of Responsibility
  - [ ] Iterator
  - [ ] Mediator
  - [ ] Memento
  - [ ] State
  - [ ] Template Method
  - [ ] Visitor
  - [ ] Interpreter

## SOLID Principles

- [x] Single Responsibility Principle (SRP) - [View](include/solid/srp.h) | [Example](examples/solid/srp_example.cpp)
- [x] Open/Closed Principle (OCP) - [View](include/solid/ocp.h) | [Example](examples/solid/ocp_example.cpp)
- [x] Liskov Substitution Principle (LSP) - [View](include/solid/lsp.h) | [Example](examples/solid/lsp_example.cpp)
- [x] Interface Segregation Principle (ISP) - [View](include/solid/isp.h) | [Example](examples/solid/isp_example.cpp)
- [x] Dependency Inversion Principle (DIP) - [View](include/solid/dip.h) | [Example](examples/solid/dip_example.cpp)

## Build Instructions

Prerequisites:

- C++17 compatible compiler (clang++, g++, or MSVC)

### Singleton Pattern

Using clang++ (macOS):

```bash
clang++ -std=c++17 -I./include examples/creational/singleton_example.cpp -o build/singleton_example
./build/singleton_example
```

Using g++:

```bash
g++ -std=c++17 -I./include examples/creational/singleton_example.cpp -o build/singleton_example
./build/singleton_example
```

### Builder Pattern (Creational)

Using clang++ (macOS):

```bash
clang++ -std=c++17 -I./include src/creational/builder.cpp examples/creational/builder_example.cpp -o build/builder_example
./build/builder_example
```

Using g++:

```bash
g++ -std=c++17 -I./include src/creational/builder.cpp examples/creational/builder_example.cpp -o build/builder_example
./build/builder_example
```

### Single Responsibility Principle (SRP)

Using clang++ (macOS):

```bash
clang++ -std=c++17 -I./include src/solid/srp.cpp examples/solid/srp_example.cpp -o build/srp_example
./build/srp_example
```

Using g++:

```bash
g++ -std=c++17 -I./include src/solid/srp.cpp examples/solid/srp_example.cpp -o build/srp_example
./build/srp_example
```

### Open/Closed Principle (OCP)

Using clang++ (macOS):

```bash
clang++ -std=c++17 -I./include src/solid/ocp.cpp examples/solid/ocp_example.cpp -o build/ocp_example
./build/ocp_example
```

Using g++:

```bash
g++ -std=c++17 -I./include src/solid/ocp.cpp examples/solid/ocp_example.cpp -o build/ocp_example
./build/ocp_example
```

### Liskov Substitution Principle (LSP)

Using clang++ (macOS):

```bash
clang++ -std=c++17 -I./include src/solid/lsp.cpp examples/solid/lsp_example.cpp -o build/lsp_example
./build/lsp_example
```

Using g++:

```bash
g++ -std=c++17 -I./include src/solid/lsp.cpp examples/solid/lsp_example.cpp -o build/lsp_example
./build/lsp_example
```

### Interface Segregation Principle (ISP)

Using clang++ (macOS):

```bash
clang++ -std=c++17 -I./include src/solid/isp.cpp examples/solid/isp_example.cpp -o build/isp_example
./build/isp_example
```

Using g++:

```bash
g++ -std=c++17 -I./include src/solid/isp.cpp examples/solid/isp_example.cpp -o build/isp_example
./build/isp_example
```

### Dependency Inversion Principle (DIP)

Using clang++ (macOS):

```bash
clang++ -std=c++17 -I./include src/solid/dip.cpp examples/solid/dip_example.cpp -o build/dip_example
./build/dip_example
```

Using g++:

```bash
g++ -std=c++17 -I./include src/solid/dip.cpp examples/solid/dip_example.cpp -o build/dip_example
./build/dip_example
```

## Usage

Each pattern is implemented in its own directory:

- `include/` - Header files with pattern implementations
- `examples/` - Example programs demonstrating each pattern

Run the examples directly after compilation to see the patterns in action.

## License

Personal learning project.
