#pragma once
#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <functional>

using namespace std;

// ============================================================================
// VIOLATION: Hard-coded if/else Factory (Procedural)
// ============================================================================

struct HotDrinkBad
{
    virtual ~HotDrinkBad() = default;
    virtual void prepare(int volume) = 0;
};

struct TeaBad : HotDrinkBad
{
    void prepare(int volume) override
    {
        cout << "Take tea bag, boil water, pour " << volume << "ml, add lemon\n";
    }
};

struct CoffeeBad : HotDrinkBad
{
    void prepare(int volume) override
    {
        cout << "Grind beans, boil water, pour " << volume << "ml, add cream\n";
    }
};

// PROBLEM: Hard-coded if/else logic scattered in functions
// PROBLEM: Difficult to add new drink types
// PROBLEM: No abstraction for drink creation
unique_ptr<HotDrinkBad> make_drink_bad(const string &type, int volume)
{
    unique_ptr<HotDrinkBad> drink;
    if (type == "tea")
    {
        drink = make_unique<TeaBad>();
    }
    else if (type == "coffee")
    {
        drink = make_unique<CoffeeBad>();
    }
    // Adding new drinks requires modifying this function!

    if (drink)
        drink->prepare(volume);

    return drink;
}

// ============================================================================
// SOLUTION: Abstract Factory Pattern with Polymorphic Factories
// ============================================================================

struct HotDrink
{
    virtual ~HotDrink() = default;
    virtual void prepare(int volume) = 0;
};

struct Tea : HotDrink
{
    void prepare(int volume) override
    {
        cout << "   🍵 Tea: Take tea bag, boil water, pour " << volume << "ml, add lemon\n";
    }
};

struct Coffee : HotDrink
{
    void prepare(int volume) override
    {
        cout << "   ☕ Coffee: Grind beans, boil water, pour " << volume << "ml, add cream\n";
    }
};

struct Chocolate : HotDrink
{
    void prepare(int volume) override
    {
        cout << "   🍫 Chocolate: Heat milk, add chocolate powder, pour " << volume << "ml\n";
    }
};

// SOLUTION: Abstract factory interface
struct HotDrinkFactory
{
    virtual ~HotDrinkFactory() = default;
    virtual unique_ptr<HotDrink> make() const = 0;
};

// Concrete factories for each drink type
struct TeaFactory : HotDrinkFactory
{
    unique_ptr<HotDrink> make() const override
    {
        return make_unique<Tea>();
    }
};

struct CoffeeFactory : HotDrinkFactory
{
    unique_ptr<HotDrink> make() const override
    {
        return make_unique<Coffee>();
    }
};

struct ChocolateFactory : HotDrinkFactory
{
    unique_ptr<HotDrink> make() const override
    {
        return make_unique<Chocolate>();
    }
};

// ============================================================================
// SOLUTION 1: Simple Registry-Based Factory
// ============================================================================

class DrinkFactory
{
private:
    map<string, unique_ptr<HotDrinkFactory>> factories;

public:
    DrinkFactory()
    {
        factories["tea"] = make_unique<TeaFactory>();
        factories["coffee"] = make_unique<CoffeeFactory>();
        factories["chocolate"] = make_unique<ChocolateFactory>();
    }

    unique_ptr<HotDrink> make_drink(const string &name, int volume = 200)
    {
        auto it = factories.find(name);
        if (it == factories.end())
        {
            cout << "   ERROR: Unknown drink type: " << name << "\n";
            return nullptr;
        }

        auto drink = it->second->make();
        if (drink)
            drink->prepare(volume);

        return drink;
    }

    void list_drinks() const
    {
        cout << "   Available drinks: ";
        for (const auto &[name, _] : factories)
        {
            cout << name << " ";
        }
        cout << "\n";
    }
};

// ============================================================================
// SOLUTION 2: Lambda-Based Factory with Full Control
// ============================================================================

class DrinkWithVolumeFactory
{
private:
    map<string, function<unique_ptr<HotDrink>(int)>> factories;

public:
    DrinkWithVolumeFactory()
    {
        // Tea factory with custom logic
        factories["tea"] = [](int volume)
        {
            auto tea = make_unique<Tea>();
            tea->prepare(volume);
            return tea;
        };

        // Coffee factory with custom logic
        factories["coffee"] = [](int volume)
        {
            auto coffee = make_unique<Coffee>();
            coffee->prepare(volume);
            return coffee;
        };

        // Chocolate factory with custom logic
        factories["chocolate"] = [](int volume)
        {
            auto chocolate = make_unique<Chocolate>();
            chocolate->prepare(volume);
            return chocolate;
        };
    }

    unique_ptr<HotDrink> make_drink(const string &name, int volume = 200)
    {
        auto it = factories.find(name);
        if (it == factories.end())
        {
            cout << "   ERROR: Unknown drink type: " << name << "\n";
            return nullptr;
        }

        return it->second(volume);
    }

    void add_drink(const string &name, function<unique_ptr<HotDrink>(int)> factory)
    {
        factories[name] = factory;
    }
};

// ============================================================================
// SOLUTION 3: Advanced Registry with Registration Method
// ============================================================================

class AdvancedDrinkFactory
{
private:
    map<string, function<unique_ptr<HotDrink>(int)>> drink_factories;

public:
    template <typename T>
    void register_drink(const string &name)
    {
        drink_factories[name] = [](int volume)
        {
            auto drink = make_unique<T>();
            drink->prepare(volume);
            return drink;
        };
    }

    unique_ptr<HotDrink> make_drink(const string &name, int volume = 200)
    {
        auto it = drink_factories.find(name);
        if (it == drink_factories.end())
        {
            cout << "   ERROR: Drink type not registered: " << name << "\n";
            return nullptr;
        }

        return it->second(volume);
    }

    int registered_count() const { return drink_factories.size(); }
};
