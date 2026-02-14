#include "creational/singleton.h"
#include <iostream>

int main()
{
    std::cout << "=== Singleton Pattern Example ===\n\n";

    // Get the singleton instance
    Logger &logger1 = Logger::getInstance();
    logger1.log("First message");

    // Get the instance again - it's the same object!
    Logger &logger2 = Logger::getInstance();
    logger2.log("Second message");

    // Verify they are the same instance
    std::cout << "\nAddress of logger1: " << &logger1 << "\n";
    std::cout << "Address of logger2: " << &logger2 << "\n";
    std::cout << "Are they the same? " << (&logger1 == &logger2 ? "YES" : "NO") << "\n";

    return 0;
}
