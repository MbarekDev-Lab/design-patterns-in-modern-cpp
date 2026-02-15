#include <iostream>
#include "../include/creational/beverage_factory.h"

using namespace std;

int main()
{
    cout << "=== BEVERAGE FACTORY PATTERN ===" << endl
         << endl;

    // ========================================================================
    // VIOLATION: Hard-coded if/else Factory
    // ========================================================================
    cout << "--- VIOLATION: Procedural if/else Factory ---" << endl;
    cout << "Problem: Logic scattered in function, hard to extend:\n\n";

    auto tea_bad = make_drink_bad("tea", 200);
    cout << endl;

    auto coffee_bad = make_drink_bad("coffee", 150);
    cout << endl;

    auto unknown = make_drink_bad("matcha", 100); // Not handled!
    cout << "Result: nullptr (no support for new drink types)\n\n";

    // ========================================================================
    // SOLUTION 1: Registry-Based Factory with Abstract Factories
    // ========================================================================
    cout << "--- SOLUTION 1: Registry-Based Drink Factory ---" << endl;
    cout << "Clean abstraction with registry of concrete factories:\n\n";

    DrinkFactory df;
    df.list_drinks();
    cout << endl;

    cout << "Order 1: Tea (200ml)\n";
    auto tea1 = df.make_drink("tea", 200);

    cout << "Order 2: Coffee (150ml)\n";
    auto coffee1 = df.make_drink("coffee", 150);

    cout << "Order 3: Chocolate (250ml)\n";
    auto choco1 = df.make_drink("chocolate", 250);

    cout << "Order 4: Unknown drink (should fail gracefully)\n";
    auto unknown1 = df.make_drink("matcha", 100);
    cout << endl;

    // ========================================================================
    // SOLUTION 2: Lambda-Based Factory with Full Control
    // ========================================================================
    cout << "--- SOLUTION 2: Lambda-Based Drink Factory ---" << endl;
    cout << "Using lambda functions for more flexibility:\n\n";

    DrinkWithVolumeFactory lwf;

    cout << "Order 1: Tea\n";
    auto tea2 = lwf.make_drink("tea", 180);

    cout << "Order 2: Coffee\n";
    auto coffee2 = lwf.make_drink("coffee", 90);

    cout << "Order 3: Chocolate\n";
    auto choco2 = lwf.make_drink("chocolate", 200);
    cout << endl;

    // ========================================================================
    // SOLUTION 3: Dynamic Drink Registration
    // ========================================================================
    cout << "--- SOLUTION 3: Runtime Drink Registration ---" << endl;
    cout << "Register drinks dynamically at runtime:\n\n";

    // Create a custom drink type on the fly
    struct Cappuccino : HotDrink
    {
        void prepare(int volume) override
        {
            cout << "   ☕ Cappuccino: Espresso + steamed milk foam, pour " << volume << "ml\n";
        }
    };

    DrinkWithVolumeFactory dynamic_factory;

    // Register cappuccino dynamically
    dynamic_factory.add_drink("cappuccino", [](int volume)
                              {
        auto cappuccino = make_unique<Cappuccino>();
        cappuccino->prepare(volume);
        return cappuccino; });

    cout << "Order Cappuccino (180ml)\n";
    auto cappuccino = dynamic_factory.make_drink("cappuccino", 180);
    cout << endl;

    // ========================================================================
    // SOLUTION 4: Template-Based Advanced Factory
    // ========================================================================
    cout << "--- SOLUTION 4: Template-Based Advanced Factory ---" << endl;
    cout << "Register drinks using templates for type safety:\n\n";

    AdvancedDrinkFactory adf;

    // Register drinks using template method
    adf.register_drink<Tea>("tea");
    adf.register_drink<Coffee>("coffee");
    adf.register_drink<Chocolate>("chocolate");

    cout << "Registered drinks: " << adf.registered_count() << "\n\n";

    cout << "Order 1: Tea (220ml)\n";
    auto tea3 = adf.make_drink("tea", 220);

    cout << "Order 2: Coffee (120ml)\n";
    auto coffee3 = adf.make_drink("coffee", 120);

    cout << "Order 3: Chocolate (280ml)\n";
    auto choco3 = adf.make_drink("chocolate", 280);
    cout << endl;

    // ========================================================================
    // SOLUTION 5: Multiple Orders
    // ========================================================================
    cout << "--- SOLUTION 5: Batch Orders (Array of Drinks) ---" << endl;
    cout << "Create multiple drinks consistently:\n\n";

    DrinkFactory batch_factory;

    struct Order
    {
        string drink_name;
        int volume;
    };

    vector<Order> orders = {
        {"tea", 200},
        {"coffee", 150},
        {"chocolate", 250},
        {"tea", 180},
        {"coffee", 100}};

    vector<unique_ptr<HotDrink>> prepared_drinks;

    cout << "Processing batch orders:\n";
    for (int i = 0; i < orders.size(); ++i)
    {
        cout << "Order " << (i + 1) << ": " << orders[i].drink_name
             << " (" << orders[i].volume << "ml)\n";
        prepared_drinks.push_back(
            batch_factory.make_drink(orders[i].drink_name, orders[i].volume));
    }

    cout << "\nTotal drinks prepared: " << prepared_drinks.size() << endl
         << endl;

    // ========================================================================
    // KEY BENEFITS SUMMARY
    // ========================================================================
    cout << "--- KEY BENEFITS OF BEVERAGE FACTORY PATTERN ---" << endl;
    cout << "✓ Open/Closed: Easy to add new drink types without modifying existing code" << endl;
    cout << "✓ Registry Pattern: Centralized management of drink creation" << endl;
    cout << "✓ Polymorphism: Different factories can be swapped seamlessly" << endl;
    cout << "✓ Lambda Support: Flexible custom logic per drink type" << endl;
    cout << "✓ Template Support: Type-safe registration of new drinks" << endl;
    cout << "✓ Runtime Flexibility: Add drinks dynamically at runtime" << endl;
    cout << "✓ Consistency: All drinks created through factory" << endl;
    cout << "✓ Testability: Easy to mock and test with fake factories" << endl;

    return 0;
}
