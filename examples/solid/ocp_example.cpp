#include "solid/ocp.h"
#include <iostream>

using namespace ocp;

int main()
{
    std::cout << "=== Open/Closed Principle (OCP) ===\n\n";

    // Create products
    Product apple{"Apple", Color::green, Size::small};
    Product tree{"Tree", Color::green, Size::large};
    Product house{"House", Color::blue, Size::large};

    const std::vector<Product *> all{&apple, &tree, &house};

    std::cout << "All products:\n";
    for (const auto &p : all)
    {
        std::cout << "  - " << p->name << "\n";
    }
    std::cout << "\n";

    // ============ VIOLATION: ProductFilterBad ============
    std::cout << "VIOLATION - ProductFilterBad (open for modification, closed for extension):\n";
    {
        ProductFilterBad bf;

        // To add each filter, we had to MODIFY the class
        auto green_things = bf.by_color(all, Color::green);
        std::cout << "Green products: ";
        for (const auto &x : green_things)
        {
            std::cout << x->name << " ";
        }
        std::cout << "\n";

        // Adding by_size_and_color required MODIFYING the class ❌
        auto big_things = bf.by_size(all, Size::large);
        std::cout << "Large products: ";
        for (const auto &x : big_things)
        {
            std::cout << x->name << " ";
        }
        std::cout << "\n";

        std::cout << "Problem: Each new filter type requires modifying ProductFilterBad\n\n";
    }

    // ============ SOLUTION: Specification Pattern ============
    std::cout << "SOLUTION - Specification Pattern (open for extension, closed for modification):\n";
    {
        BetterFilter bf;

        // Single filter
        ColorSpecification green(Color::green);
        auto green_things = bf.filter(all, green);
        std::cout << "Green products: ";
        for (const auto &x : green_things)
        {
            std::cout << x->name << " ";
        }
        std::cout << "\n";

        // Combining specifications using &&
        SizeSpecification large(Size::large);
        auto spec = green && large;
        auto green_and_large = bf.filter(all, spec);
        std::cout << "Green AND large products: ";
        for (const auto &x : green_and_large)
        {
            std::cout << x->name << " ";
        }
        std::cout << "\n";

        std::cout << "\nBenefit: New specifications can be added WITHOUT modifying BetterFilter\n";
        std::cout << "  - Add new Specification subclass\n";
        std::cout << "  - BetterFilter works with it immediately\n";
        std::cout << "  - No need to change existing code\n";
    }

    std::cout << "\n=== Key Insights ===\n";
    std::cout << "OCP VIOLATION (ProductFilterBad):\n";
    std::cout << "  - CLOSED for extension: Hard to add new filters without modifying class\n";
    std::cout << "  - OPEN for modification: Must edit the class for each new filter\n\n";

    std::cout << "OCP COMPLIANCE (Specification Pattern):\n";
    std::cout << "  - OPEN for extension: Easy to add new Specification types\n";
    std::cout << "  - CLOSED for modification: BetterFilter code never changes\n";

    return 0;
}
