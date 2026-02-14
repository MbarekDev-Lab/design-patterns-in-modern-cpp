#include "solid/dip.h"
#include <iostream>

using namespace dip;

int main()
{
    std::cout << "=== Dependency Inversion Principle (DIP) ===\n\n";

    // ============ VIOLATION: Direct Dependencies ============
    std::cout << "VIOLATION - Direct Dependencies on Concrete Classes:\n";
    std::cout << "--------------------------------------\n";
    {
        std::cout << "ReportBad depends directly on MySQLDatabase and ConsolePrinter\n\n";

        Document report_doc{"Quarterly Sales Report"};
        ReportBad bad_report;

        std::cout << "Generating report:\n";
        bad_report.generate_and_save(report_doc);

        std::cout << "\nProblems with this approach:\n";
        std::cout << "  1. Tightly coupled to MySQLDatabase\n";
        std::cout << "  2. Tightly coupled to ConsolePrinter\n";
        std::cout << "  3. Cannot use PostgreSQL without modifying Report\n";
        std::cout << "  4. Cannot use FilePrinter without modifying Report\n";
        std::cout << "  5. Hard to test - must use real database/printer\n";
        std::cout << "  6. High-level class depends on low-level classes\n";
        std::cout << "  7. DIP is INVERTED!\n\n";
    }

    // ============ SOLUTION: Depend on Abstractions ============
    std::cout << "SOLUTION - Depend on Abstractions (Interfaces):\n";
    std::cout << "--------------------------------------\n\n";

    // Example 1: MySQL + Console
    std::cout << "Example 1: MySQL Database + Console Printer:\n";
    {
        MySQLDatabaseImpl mysql;
        ConsolePrinterImpl console;
        Report report(mysql, console);

        Document doc{"Sales Report Q1"};
        report.generate_and_save(doc);
        std::cout << "\n";
    }

    // Example 2: PostgreSQL + Console (easy swap!)
    std::cout << "Example 2: PostgreSQL Database + Console Printer (swap database):\n";
    {
        PostgreSQLDatabaseImpl postgres;
        ConsolePrinterImpl console;
        Report report(postgres, console);

        Document doc{"Sales Report Q2"};
        report.generate_and_save(doc);
        std::cout << "\n";
    }

    // Example 3: MySQL + File (easy swap!)
    std::cout << "Example 3: MySQL Database + File Printer (swap printer):\n";
    {
        MySQLDatabaseImpl mysql;
        FilePrinterImpl file;
        Report report(mysql, file);

        Document doc{"Sales Report Q3"};
        report.generate_and_save(doc);
        std::cout << "\n";
    }

    // Example 4: PostgreSQL + File (swap both!)
    std::cout << "Example 4: PostgreSQL Database + File Printer (swap both):\n";
    {
        PostgreSQLDatabaseImpl postgres;
        FilePrinterImpl file;
        Report report(postgres, file);

        Document doc{"Sales Report Q4"};
        report.generate_and_save(doc);
        std::cout << "\n";
    }

    // ============ Testing with Mocks ============
    std::cout << "Testing with Mock Objects (Unit Test Example):\n";
    std::cout << "--------------------------------------\n";
    {
        std::cout << "Report class can be tested with mock objects:\n";
        MockDatabase mock_db;
        MockPrinter mock_printer;
        Report report(mock_db, mock_printer);

        Document test_doc{"Test Report"};
        std::cout << "Running test:\n";
        report.generate_and_save(test_doc);

        std::cout << "\nBenefit:\n";
        std::cout << "  - No real database or printer needed\n";
        std::cout << "  - Fast unit tests\n";
        std::cout << "  - Easy to verify behavior\n";
        std::cout << "  - Only possible with DIP!\n\n";
    }

    // ============ Why DIP Matters ============
    std::cout << "=== Key Insights ===\n";
    std::cout << "DIP VIOLATION (Direct Dependencies):\n";
    std::cout << "  - High-level depends on low-level concrete classes\n";
    std::cout << "  - Tightly coupled implementation\n";
    std::cout << "  - Hard to swap implementations\n";
    std::cout << "  - Difficult to unit test\n";
    std::cout << "  - Changes in low-level affect high-level\n\n";

    std::cout << "DIP COMPLIANCE (Depend on Abstractions):\n";
    std::cout << "  - High-level depends on abstractions\n";
    std::cout << "  - Low-level depends on same abstractions\n";
    std::cout << "  - Loosely coupled design\n";
    std::cout << "  - Easy to swap implementations\n";
    std::cout << "  - Easy to test with mocks\n";
    std::cout << "  - Changes isolated to abstraction layer\n";

    return 0;
}
