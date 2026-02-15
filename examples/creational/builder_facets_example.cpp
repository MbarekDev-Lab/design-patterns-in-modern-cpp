#include <iostream>
#include "../include/creational/builder_facets.h"

using namespace std;

int main()
{
    cout << "=== BUILDER FACETS PATTERN ===" << endl
         << endl;

    // ========================================================================
    // VIOLATION: Without Facets - Flat, Hard to Read Builder
    // ========================================================================
    cout << "--- VIOLATION: Flat Builder (Before) ---" << endl;
    cout << "Cumbersome method chaining with all properties mixed:" << endl;
    cout << "builder.set_street(...).set_postcode(...).set_city(...)" << endl;
    cout << "        .set_company(...).set_position(...).set_income(...)" << endl;
    cout << "Problem: No logical grouping, hard to understand dependencies" << endl
         << endl;

    PersonBad p1 = PersonBuilderBad()
                       .set_street("123 London Road")
                       .set_postcode("SW1 1GB")
                       .set_city("London")
                       .set_company("TechCorp")
                       .set_position("Senior Developer")
                       .set_income(120000);

    cout << "Result: " << p1 << endl
         << endl;

    // ========================================================================
    // SOLUTION 1: Simple Address and Job Builder
    // ========================================================================
    cout << "--- SOLUTION 1: Using Builder Facets ---" << endl;
    cout << "Elegant logical grouping with domain-specific methods:" << endl;
    cout << "Readable: lives().at(...).with_postcode(...).in(...)" << endl;
    cout << "Then:     works().at(...).as_a(...).earning(...)" << endl
         << endl;

    Person p2 = Person::create()
                    .lives()
                    .at("456 Oxford Street")
                    .with_postcode("W1A 1AA")
                    .in("London")
                    .works()
                    .at("DesignHub")
                    .as_a("UI Designer")
                    .earning(95000);

    cout << "Result: " << p2 << endl
         << endl;

    // ========================================================================
    // SOLUTION 2: Complex Multi-Builder Chaining
    // ========================================================================
    cout << "--- SOLUTION 2: Complex Multi-Builder Chaining ---" << endl;
    cout << "Seamless switching between facets:" << endl;

    Person p3 = Person::create()
                    .lives()
                    .at("789 Chelsea Lane")
                    .in("London")
                    .with_postcode("SW3 5QQ")
                    .works()
                    .at("FinanceFlow")
                    .earning(150000)
                    .as_a("Financial Analyst");

    cout << "Result: " << p3 << endl
         << endl;

    // ========================================================================
    // SOLUTION 3: Minimal Configuration
    // ========================================================================
    cout << "--- SOLUTION 3: Minimal Configuration (Optional Parts) ---" << endl;

    Person p4 = Person::create()
                    .lives()
                    .at("999 Baker Street")
                    .in("London")
                    .works()
                    .at("StartupXYZ")
                    .as_a("Co-Founder");

    cout << "Result: " << p4 << endl
         << endl;

    // ========================================================================
    // SOLUTION 4: Address-First then Job Builder
    // ========================================================================
    cout << "--- SOLUTION 4: Address Setup First ---" << endl;

    Person p5 = Person::create()
                    .lives()
                    .in("Manchester")
                    .at("42 Spring Gardens")
                    .with_postcode("M2 1AB")
                    .works()
                    .at("DataDriven")
                    .as_a("Data Scientist")
                    .earning(110000);

    cout << "Result: " << p5 << endl
         << endl;

    // ========================================================================
    // KEY BENEFITS DEMONSTRATED
    // ========================================================================
    cout << "--- KEY BENEFITS OF BUILDER FACETS ---" << endl;
    cout << "✓ Logical Grouping: Address methods in lives(), Job in works()" << endl;
    cout << "✓ Domain-Specific: at(), with_postcode(), in() make sense for address" << endl;
    cout << "✓ Fluent Interface: Order of calls doesn't matter, chain easily" << endl;
    cout << "✓ Readable Code: Clear intent without long flat method list" << endl;
    cout << "✓ Extensible: Add more facets (education, hobbies) without touching existing code" << endl;
    cout << "✓ Single Object: Single Person instance throughout the build process" << endl;

    return 0;
}
