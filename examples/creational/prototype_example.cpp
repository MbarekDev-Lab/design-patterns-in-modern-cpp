#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
#include "../include/creational/prototype.h"

using namespace std;

// ============================================================================
// EXAMPLE 1: VIOLATION - Manual Object Creation and Copying
// ============================================================================

void example_1_violation()
{
    cout << "\n========== EXAMPLE 1: VIOLATION - Manual Creation ==========\n";

    // Problem: Creating similar objects without a prototype is tedious and error-prone
    auto addr1 = new AddressBad{"123 East Dr", "London", 0};
    ContactBad john{"John Doe", addr1};

    auto addr2 = new AddressBad{"123 East Dr", "London", 0}; // Duplicate!
    ContactBad jane{"Jane Doe", addr2};

    auto addr3 = new AddressBad{"123 East Dr", "London", 0}; // Another duplicate!
    ContactBad bob{"Bob Smith", addr3};

    cout << john << "\n";
    cout << jane << "\n";
    cout << bob << "\n";

    cout << "\nProblem: Code repetition, memory inefficiency, error-prone\n";
}

// ============================================================================
// EXAMPLE 2: SOLUTION 1 - Simple Prototype Cloning with Copy Constructor
// ============================================================================

void example_2_simple_prototype()
{
    cout << "\n========== EXAMPLE 2: Simple Prototype Cloning ==========\n";

    // Create a prototype
    Contact employee{"Unknown", new Address{"123 East Dr", "London", 0}};

    // Clone the prototype for multiple employees
    Contact john = employee;
    john.name = "John Doe";
    john.address->suite = 123;

    Contact jane = employee;
    jane.name = "Jane Doe";
    jane.address->suite = 124;

    Contact bob = employee;
    bob.name = "Bob Smith";
    bob.address->suite = 125;

    cout << john << "\n";
    cout << jane << "\n";
    cout << bob << "\n";

    cout << "\nBenefit: Single prototype, cloned for variations\n";
}

// ============================================================================
// EXAMPLE 3: Factory with Prototype Templates
// ============================================================================

void example_3_factory_prototypes()
{
    cout << "\n========== EXAMPLE 3: Factory with Prototypes ==========\n";

    // Use static prototypes for different office locations
    auto john = EmployeeFactory::NewMainOfficeEmployee("John Doe", 123);
    auto jane = EmployeeFactory::NewAuxOfficeEmployee("Jane Doe", 125);

    cout << "Main Office Employee: " << *john << "\n";
    cout << "Aux Office Employee: " << *jane << "\n";

    cout << "\nBenefit: Factory encapsulates prototype logic\n";
}

// ============================================================================
// EXAMPLE 4: Creating Multiple Employees with Factory
// ============================================================================

void example_4_bulk_creation()
{
    cout << "\n========== EXAMPLE 4: Bulk Employee Creation ==========\n";

    vector<unique_ptr<Contact>> employees;

    // Create main office employees
    employees.push_back(EmployeeFactory::NewMainOfficeEmployee("Alice Johnson", 101));
    employees.push_back(EmployeeFactory::NewMainOfficeEmployee("Bob Smith", 102));
    employees.push_back(EmployeeFactory::NewMainOfficeEmployee("Charlie Brown", 103));

    cout << "Main Office Team:\n";
    for (const auto &emp : employees)
    {
        cout << "  - " << *emp << "\n";
    }

    cout << "\nAuxiliary Office:\n";
    employees.push_back(EmployeeFactory::NewAuxOfficeEmployee("Diana Prince", 201));
    employees.push_back(EmployeeFactory::NewAuxOfficeEmployee("Eve Wilson", 202));

    for (size_t i = 3; i < employees.size(); ++i)
    {
        cout << "  - " << *employees[i] << "\n";
    }

    cout << "\nBenefit: Efficient creation of multiple similar objects\n";
}

// ============================================================================
// EXAMPLE 5: Prototype Registry
// ============================================================================

void example_5_prototype_registry()
{
    cout << "\n========== EXAMPLE 5: Prototype Registry ==========\n";

    PrototypeRegistry registry;

    // Register different office prototypes
    Contact mainOffice{"Unknown", new Address{"123 East Dr", "London", 0}};
    Contact auxOffice{"Unknown", new Address{"123B East Dr", "London", 0}};
    Contact teleOffice{"Unknown", new Address{"456 Remote St", "NewYork", 0}};

    registry.register_prototype("main", mainOffice);
    registry.register_prototype("aux", auxOffice);
    registry.register_prototype("remote", teleOffice);

    cout << "Registered Prototypes:\n";
    registry.list_prototypes();

    cout << "\nCreating employees from prototypes:\n";
    auto emp1 = registry.create("main", "Frank Miller", 110);
    auto emp2 = registry.create("aux", "Grace Lee", 210);
    auto emp3 = registry.create("remote", "Henry Adams", 1);

    cout << "  - " << *emp1 << "\n";
    cout << "  - " << *emp2 << "\n";
    cout << "  - " << *emp3 << "\n";

    cout << "\nBenefit: Flexible prototype management with registry\n";
}

// ============================================================================
// EXAMPLE 6: Deep Copy Verification
// ============================================================================

void example_6_deep_copy()
{
    cout << "\n========== EXAMPLE 6: Deep Copy Verification ==========\n";

    Contact original{"Original", new Address{"Original St", "Original City", 100}};
    Contact cloned{original};

    cout << "Original: " << original << "\n";
    cout << "Cloned: " << cloned << "\n";

    // Modify the clone - should NOT affect original
    cloned.name = "Modified Clone";
    cloned.address->street = "New Street";
    cloned.address->suite = 999;

    cout << "\nAfter modifying clone:\n";
    cout << "Original: " << original << "\n";
    cout << "Cloned: " << cloned << "\n";

    cout << "\nBenefit: Deep copy ensures independence\n";
}

// ============================================================================
// EXAMPLE 7: Cloneable Interface with Polymorphism
// ============================================================================

void example_7_cloneable_interface()
{
    cout << "\n========== EXAMPLE 7: Cloneable Interface ==========\n";

    vector<unique_ptr<Cloneable>> prototypes;

    auto proto1 = make_unique<FlexibleContact>("Main Proto", new Address{"123 Main St", "NYC", 0});
    auto proto2 = make_unique<FlexibleContact>("Remote Proto", new Address{"456 Remote Ave", "LA", 0});

    prototypes.push_back(make_unique<FlexibleContact>(*proto1));
    prototypes.push_back(make_unique<FlexibleContact>(*proto2));

    // Clone prototypes
    auto cloned1 = prototypes[0]->clone();
    auto cloned2 = prototypes[1]->clone();

    if (auto flex1 = dynamic_cast<FlexibleContact *>(cloned1.get()))
    {
        flex1->name = "Cloned Employee 1";
        flex1->address->suite = 101;
        cout << "Cloned 1: " << *flex1 << "\n";
    }

    if (auto flex2 = dynamic_cast<FlexibleContact *>(cloned2.get()))
    {
        flex2->name = "Cloned Employee 2";
        flex2->address->suite = 201;
        cout << "Cloned 2: " << *flex2 << "\n";
    }

    cout << "\nBenefit: Polymorphic cloning through virtual interface\n";
}

// ============================================================================
// EXAMPLE 8: Performance - Prototype vs Manual Creation
// ============================================================================

void example_8_performance()
{
    cout << "\n========== EXAMPLE 8: Performance Comparison ==========\n";

    const int count = 10;

    // Method 1: Manual creation (SLOW)
    cout << "Method 1: Manual Creation\n";
    {
        vector<unique_ptr<Contact>> employees;
        for (int i = 0; i < count; ++i)
        {
            auto addr = new Address{"123 East Dr", "London", i};
            auto emp = make_unique<Contact>("Employee" + to_string(i), addr);
            employees.push_back(std::move(emp));
        }
        cout << "  Created " << employees.size() << " employees manually\n";
    }

    // Method 2: Prototype cloning (FAST)
    cout << "Method 2: Prototype Cloning\n";
    {
        Contact proto{"Unknown", new Address{"123 East Dr", "London", 0}};
        vector<unique_ptr<Contact>> employees;
        for (int i = 0; i < count; ++i)
        {
            auto emp = make_unique<Contact>(proto);
            emp->name = "Employee" + to_string(i);
            emp->address->suite = i;
            employees.push_back(std::move(emp));
        }
        cout << "  Created " << employees.size() << " employees via prototype\n";
    }

    // Method 3: Factory (BEST)
    cout << "Method 3: Factory with Prototypes\n";
    {
        vector<unique_ptr<Contact>> employees;
        for (int i = 0; i < count; ++i)
        {
            auto emp = EmployeeFactory::NewMainOfficeEmployee("Employee" + to_string(i), i);
            employees.push_back(std::move(emp));
        }
        cout << "  Created " << employees.size() << " employees via factory\n";
    }

    cout << "\nBenefit: Factory approach is cleanest and most efficient\n";
}

// ============================================================================
// EXAMPLE 9: Real-World HR System Scenario
// ============================================================================

void example_9_hr_system()
{
    cout << "\n========== EXAMPLE 9: HR System Scenario ==========\n";

    cout << "Scenario: New company opened offices, need to hire employees\n\n";

    // Initialize office prototypes
    Contact mainProto{"Template", new Address{"1 Corporate Plaza", "NewYork", 0}};
    Contact remoteProto{"Template", new Address{"Remote", "Worldwide", 0}};

    vector<unique_ptr<Contact>> hr_staff;
    vector<unique_ptr<Contact>> engineering_staff;

    // HR Department - Main Office
    cout << "HR Department (Main Office):\n";
    auto hr1 = make_unique<Contact>(mainProto);
    hr1->name = "Sarah Johnson";
    hr1->address->suite = 1001;
    hr_staff.push_back(std::move(hr1));

    auto hr2 = make_unique<Contact>(mainProto);
    hr2->name = "Michael Brown";
    hr2->address->suite = 1002;
    hr_staff.push_back(std::move(hr2));

    for (const auto &emp : hr_staff)
    {
        cout << "  • " << *emp << "\n";
    }

    // Engineering Department - Mixed locations
    cout << "\nEngineering Department (Main + Remote):\n";
    auto eng1 = make_unique<Contact>(mainProto);
    eng1->name = "Alex Chen";
    eng1->address->suite = 2001;
    engineering_staff.push_back(std::move(eng1));

    auto eng2 = make_unique<Contact>(remoteProto);
    eng2->name = "Diana Martinez";
    eng2->address->suite = 501;
    engineering_staff.push_back(std::move(eng2));

    auto eng3 = make_unique<Contact>(mainProto);
    eng3->name = "Robert Wilson";
    eng3->address->suite = 2002;
    engineering_staff.push_back(std::move(eng3));

    for (const auto &emp : engineering_staff)
    {
        cout << "  • " << *emp << "\n";
    }

    cout << "\nBenefit: Realistic HR system efficiently creates employees with prototypes\n";
}

// ============================================================================
// Main Entry Point
// ============================================================================

int main()
{
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║         PROTOTYPE PATTERN - Deep Copy & Cloning           ║\n";
    cout << "║  Problem: Creating similar objects is tedious and error   ║\n";
    cout << "║           prone, especially with complex object graphs    ║\n";
    cout << "║  Solution: Clone a prototype object to create variations  ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";

    example_1_violation();
    example_2_simple_prototype();
    example_3_factory_prototypes();
    example_4_bulk_creation();
    example_5_prototype_registry();
    example_6_deep_copy();
    example_7_cloneable_interface();
    example_8_performance();
    example_9_hr_system();

    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                    KEY TAKEAWAYS                           ║\n";
    cout << "├────────────────────────────────────────────────────────────┤\n";
    cout << "│ 1. Use Prototype pattern when object creation is complex   │\n";
    cout << "│ 2. Implement copy constructors for proper deep copying     │\n";
    cout << "│ 3. Factory class can encapsulate prototype logic           │\n";
    cout << "│ 4. Registry allows multiple prototypes to be managed       │\n";
    cout << "│ 5. Virtual clone() method enables polymorphic cloning      │\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    return 0;
}
