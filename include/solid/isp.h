#ifndef ISP_H
#define ISP_H

#include <iostream>
#include <string>
#include <vector>

/**
 * Interface Segregation Principle (ISP)
 *
 * "Clients should not be forced to depend on interfaces they do not use."
 *
 * In other words: Create many client-specific interfaces rather than one
 * general-purpose interface.
 *
 * This example shows:
 * 1. VIOLATION: Fat interface IMachine forces all implementers to support print, fax, scan
 * 2. SOLUTION: Segregate into focused interfaces: IPrinter, IScanner, IFax
 */

namespace isp
{

    // Dummy document class
    struct Document
    {
        std::string name;
        explicit Document(const std::string &name) : name(name) {}
    };

    // ============ VIOLATION: Fat Interface ============
    /**
     * ❌ VIOLATION: IMachine is a "fat interface"
     * Forces implementers to support all operations
     * Problems:
     * 1. Printer must implement scan() and fax() even if not needed
     * 2. Scanner must implement print() and fax() even if not needed
     * 3. Clients that only need printing depend on scanning functionality
     * 4. Changes to any operation affect all implementers
     */
    class IMachineBad
    {
    public:
        virtual ~IMachineBad() = default;
        virtual void print(const Document &doc) = 0;
        virtual void fax(const Document &doc) = 0;
        virtual void scan(const Document &doc) = 0;
    };

    /**
     * ❌ VIOLATION: SimplePrinter must implement scan() and fax()
     * Even though it's just a printer!
     */
    class SimplePrinterBad : public IMachineBad
    {
    public:
        void print(const Document &doc) override
        {
            std::cout << "  Printing: " << doc.name << "\n";
        }

        void fax(const Document &doc) override
        {
            throw std::logic_error("SimplePrinter does not support faxing!");
        }

        void scan(const Document &doc) override
        {
            throw std::logic_error("SimplePrinter does not support scanning!");
        }
    };

    /**
     * ❌ VIOLATION: SimpleScannerBad must implement print() and fax()
     * Even though it's just a scanner!
     */
    class SimpleScannerBad : public IMachineBad
    {
    public:
        void print(const Document &doc) override
        {
            throw std::logic_error("SimpleScanner does not support printing!");
        }

        void fax(const Document &doc) override
        {
            throw std::logic_error("SimpleScanner does not support faxing!");
        }

        void scan(const Document &doc) override
        {
            std::cout << "  Scanning: " << doc.name << "\n";
        }
    };

    // ============ SOLUTION: Segregated Interfaces ============

    /**
     * ✅ SOLUTION: Small, focused interfaces
     * Each interface has ONE reason to change
     */

    class IPrinter
    {
    public:
        virtual ~IPrinter() = default;
        virtual void print(const Document &doc) = 0;
    };

    class IScanner
    {
    public:
        virtual ~IScanner() = default;
        virtual void scan(const Document &doc) = 0;
    };

    class IFax
    {
    public:
        virtual ~IFax() = default;
        virtual void fax(const Document &doc) = 0;
    };

    /**
     * ✅ CORRECT: SimplePrinter only implements printing
     * No need for scan() or fax()
     * Clean, focused responsibility
     */
    class Printer : public IPrinter
    {
    public:
        void print(const Document &doc) override
        {
            std::cout << "  Printing: " << doc.name << "\n";
        }
    };

    /**
     * ✅ CORRECT: SimpleScanner only implements scanning
     * No need for print() or fax()
     * Clean, focused responsibility
     */
    class Scanner : public IScanner
    {
    public:
        void scan(const Document &doc) override
        {
            std::cout << "  Scanning: " << doc.name << "\n";
        }
    };

    /**
     * ✅ CORRECT: Fax machine only implements faxing
     * Specific responsibility
     */
    class FaxMachine : public IFax
    {
    public:
        void fax(const Document &doc) override
        {
            std::cout << "  Faxing: " << doc.name << "\n";
        }
    };

    /**
     * ✅ CORRECT: Compose focused interfaces to create a multi-function printer
     * Uses composition instead of forcing one big interface
     * IMachine composes the interfaces it needs
     */
    class IMachine : public IPrinter, public IScanner, public IFax
    {
    public:
        virtual ~IMachine() = default;
    };

    /**
     * ✅ CORRECT: MultiFunctionPrinter uses composition
     * Delegates to specific printers/scanners/faxes
     * No need to implement unneeded operations
     */
    class MultiFunctionPrinter : public IMachine
    {
    private:
        IPrinter &printer;
        IScanner &scanner;
        IFax &fax_machine;

    public:
        MultiFunctionPrinter(IPrinter &printer, IScanner &scanner, IFax &fax_machine)
            : printer(printer), scanner(scanner), fax_machine(fax_machine) {}

        void print(const Document &doc) override
        {
            printer.print(doc);
        }

        void scan(const Document &doc) override
        {
            scanner.scan(doc);
        }

        void fax(const Document &doc) override
        {
            fax_machine.fax(doc);
        }
    };

} // namespace isp

#endif // ISP_H
