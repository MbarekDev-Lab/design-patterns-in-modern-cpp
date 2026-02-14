#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>

/**
 * Singleton Pattern - Creational Pattern
 *
 * Intent: Ensure a class has only one instance, and provide a global point
 *         of access to it.
 *
 * Use when:
 * - There must be exactly one instance of a class
 * - It must be accessible to clients from a well-known access point
 * - The single instance should be extensible by subclassing
 */

class Logger
{
private:
    // Private constructor prevents instantiation
    Logger()
    {
        std::cout << "Logger instance created\n";
    }

    // Private copy constructor and assignment operator (C++11 style)
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

public:
    // Static method to get the single instance
    static Logger &getInstance()
    {
        static Logger instance; // Created only once (thread-safe in C++11)
        return instance;
    }

    void log(const std::string &message) const
    {
        std::cout << "[LOG] " << message << "\n";
    }
};

#endif // SINGLETON_H
