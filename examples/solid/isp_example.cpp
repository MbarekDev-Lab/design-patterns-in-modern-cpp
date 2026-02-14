#include "solid/isp.h"
#include <iostream>

using namespace isp;

int main()
{
    std::cout << "=== Interface Segregation Principle (ISP) ===\n\n";

    // ============ VIOLATION: Fat Interface ============
    std::cout << "VIOLATION - Fat Interface IMachineBad:\n";
    std::cout << "--------------------------------------\n";
    {
        std::cout << "SimplePrinterBad must implement print(), scan(), and fax()\n";
        std::cout << "  - But it can only print!\n";
        std::cout << "  - scan() and fax() throw exceptions\n";
        std::cout << "  - Clients of SimplePrinterBad depend on unused operations\n";

        SimplePrinterBad printer;
        Document doc{"report.txt"};

        std::cout << "Attempting to use printer:\n";
        printer.print(doc);

        std::cout << "\nProblems with fat interfaces:\n";
        std::cout << "  1. Classes implement methods they don't use\n";
        std::cout << "  2. Throwing exceptions is a code smell\n";
        std::cout << "  3. Clients depend on unnecessary methods\n";
        std::cout << "  4. Changes to fax() force printer to recompile\n";
        std::cout << "  5. Interface is NOT segregated - it's fat!\n\n";
    }

    // ============ SOLUTION: Segregated Interfaces ============
    std::cout << "SOLUTION - Segregated Interfaces:\n";
    std::cout << "--------------------------------------\n";
    {
        std::cout << "Create focused interfaces:\n";
        std::cout << "  - IPrinter: print()\n";
        std::cout << "  - IScanner: scan()\n";
        std::cout << "  - IFax: fax()\n\n";

        Printer printer;
        Scanner scanner;
        FaxMachine fax;

        Document doc{"important_doc.pdf"};

        std::cout << "Simple Printer (print only):\n";
        printer.print(doc);

        std::cout << "\nSimple Scanner (scan only):\n";
        scanner.scan(doc);

        std::cout << "\nFax Machine (fax only):\n";
        fax.fax(doc);

        std::cout << "\nBenefits:\n";
        std::cout << "  1. Each class implements only needed operations\n";
        std::cout << "  2. No exceptions for unsupported operations\n";
        std::cout << "  3. Clients depend only on what they use\n";
        std::cout << "  4. Changes to one interface don't affect others\n";
        std::cout << "  5. Easy to extend with new capabilities\n\n";
    }

    // ============ Composition with Segregated Interfaces ============
    std::cout << "Multi-Function Machine (Composition):\n";
    std::cout << "--------------------------------------\n";
    {
        Printer printer;
        Scanner scanner;
        FaxMachine fax;

        // Create a multi-function machine that composes segregated interfaces
        MultiFunctionPrinter mfp(printer, scanner, fax);

        Document doc{"multi_purpose.docx"};

        std::cout << "Multi-function machine using composed interfaces:\n";
        mfp.print(doc);
        mfp.scan(doc);
        mfp.fax(doc);

        std::cout << "\nKey Insight:\n";
        std::cout << "  MultiFunctionPrinter delegates to focused interfaces\n";
        std::cout << "  This is MUCH better than a fat IMachineBad interface\n";
        std::cout << "  We get extensibility without pollution\n\n";
    }

    // ============ Why ISP Matters ============
    std::cout << "=== Key Insights ===\n";
    std::cout << "ISP VIOLATION (Fat Interface):\n";
    std::cout << "  - One huge interface with many operations\n";
    std::cout << "  - Classes implement operations they don't use\n";
    std::cout << "  - Clients depend on unused functionality\n";
    std::cout << "  - Changes to one operation affect all implementers\n";
    std::cout << "  - Difficult to extend selectively\n\n";

    std::cout << "ISP COMPLIANCE (Segregated Interfaces):\n";
    std::cout << "  - Many small, focused interfaces\n";
    std::cout << "  - Classes only implement what they need\n";
    std::cout << "  - Clients depend only on what they use\n";
    std::cout << "  - Changes are localized to specific interfaces\n";
    std::cout << "  - Easy to extend with new capabilities\n";
    std::cout << "  - Composition allows flexible combinations\n";

    return 0;
}
