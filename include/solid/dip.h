#ifndef DIP_H
#define DIP_H

#include <iostream>
#include <string>
#include <memory>

/**
 * Dependency Inversion Principle (DIP)
 *
 * "High-level modules should not depend on low-level modules.
 * Both should depend on abstractions."
 *
 * "Abstractions should not depend on details.
 * Details should depend on abstractions."
 *
 * This example shows:
 * 1. VIOLATION: High-level Report class depends on concrete low-level classes
 * 2. SOLUTION: Both depend on abstractions (interfaces) - dependency injection
 */

namespace dip
{

    // Dummy document class
    struct Document
    {
        std::string content;
        explicit Document(const std::string &content) : content(content) {}
    };

    // ============ VIOLATION: Direct Dependency on Concrete Classes ============

    /**
     * ❌ VIOLATION: MySQLDatabase is a concrete, low-level class
     * Tightly coupled to a specific database implementation
     * Hard to test, hard to swap implementations
     */
    class MySQLDatabase
    {
    public:
        void save(const std::string &data)
        {
            std::cout << "  Saving to MySQL: " << data << "\n";
        }
    };

    /**
     * ❌ VIOLATION: ConsolePrinter is a concrete, low-level class
     * Tightly coupled to console output
     */
    class ConsolePrinter
    {
    public:
        void print(const Document &doc)
        {
            std::cout << "  Printing to console: " << doc.content << "\n";
        }
    };

    /**
     * ❌ VIOLATION: ReportBad is HIGH-level, but depends on LOW-level concrete classes
     * Problems:
     * 1. Tightly coupled to MySQLDatabase and ConsolePrinter
     * 2. Cannot use PostgreSQL without modifying ReportBad
     * 3. Cannot use FilePrinter without modifying ReportBad
     * 4. Impossible to unit test - must use real database/printer
     * 5. DIP is INVERTED - high-level depends on low-level!
     */
    class ReportBad
    {
    private:
        MySQLDatabase database; // ❌ Direct dependency on concrete class
        ConsolePrinter printer; // ❌ Direct dependency on concrete class

    public:
        void generate_and_save(const Document &doc)
        {
            // Both of these are tightly coupled
            database.save(doc.content);
            printer.print(doc);
        }
    };

    // ============ SOLUTION: Depend on Abstractions ============

    /**
     * ✅ SOLUTION: IDatabase is an abstraction (interface)
     * Any concrete database can implement this
     */
    class IDatabase
    {
    public:
        virtual ~IDatabase() = default;
        virtual void save(const std::string &data) = 0;
    };

    /**
     * ✅ SOLUTION: IPrinter is an abstraction (interface)
     * Any concrete printer can implement this
     */
    class IPrinter
    {
    public:
        virtual ~IPrinter() = default;
        virtual void print(const Document &doc) = 0;
    };

    /**
     * ✅ CORRECT: Concrete MySQL implementation
     * Depends on the IDatabase abstraction
     */
    class MySQLDatabaseImpl : public IDatabase
    {
    public:
        void save(const std::string &data) override
        {
            std::cout << "  Saving to MySQL: " << data << "\n";
        }
    };

    /**
     * ✅ CORRECT: Alternative database implementation
     * Can be substituted without changing Report
     */
    class PostgreSQLDatabaseImpl : public IDatabase
    {
    public:
        void save(const std::string &data) override
        {
            std::cout << "  Saving to PostgreSQL: " << data << "\n";
        }
    };

    /**
     * ✅ CORRECT: Console printer implementation
     * Depends on the IPrinter abstraction
     */
    class ConsolePrinterImpl : public IPrinter
    {
    public:
        void print(const Document &doc) override
        {
            std::cout << "  Printing to console: " << doc.content << "\n";
        }
    };

    /**
     * ✅ CORRECT: File printer implementation
     * Can be substituted without changing Report
     */
    class FilePrinterImpl : public IPrinter
    {
    public:
        void print(const Document &doc) override
        {
            std::cout << "  Printing to file: " << doc.content << "\n";
        }
    };

    /**
     * ✅ CORRECT: Report is HIGH-level, depends on abstractions (not low-level)
     * Benefits:
     * 1. Loosely coupled to database and printer
     * 2. Easy to swap implementations (MySQL -> PostgreSQL)
     * 3. Easy to test with mock objects
     * 4. Dependencies injected, not created internally
     * 5. DIP is CORRECT - high-level depends on abstractions!
     */
    class Report
    {
    private:
        IDatabase &database; // ✅ Depends on abstraction
        IPrinter &printer;   // ✅ Depends on abstraction

    public:
        Report(IDatabase &database, IPrinter &printer)
            : database(database), printer(printer) {}

        void generate_and_save(const Document &doc)
        {
            database.save(doc.content);
            printer.print(doc);
        }
    };

    /**
     * ✅ MOCK for testing - implements IDatabase
     * Allows testing Report without real database
     */
    class MockDatabase : public IDatabase
    {
    public:
        void save(const std::string &data) override
        {
            std::cout << "  [MOCK] Saving to mock database: " << data << "\n";
        }
    };

    /**
     * ✅ MOCK for testing - implements IPrinter
     * Allows testing Report without real printer
     */
    class MockPrinter : public IPrinter
    {
    public:
        void print(const Document &doc) override
        {
            std::cout << "  [MOCK] Mock printer output: " << doc.content << "\n";
        }
    };

} // namespace dip

#endif // DIP_H
