#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <string>
#include <iostream>
#include <memory>
#include <functional>
#include <sstream>
#include <map>

using namespace std;

// ============================================================================
// PROTOTYPE PATTERN - Deep Copy and Object Cloning
// ============================================================================
// The Prototype pattern is used to create new objects by copying an existing
// object (prototype) rather than creating them from scratch. This is useful when
// object creation is expensive or when you need to create similar objects with
// slight variations.

// ============================================================================
// VIOLATION: Manual copying without a prototype
// ============================================================================

struct AddressBad
{
    string street;
    string city;
    int suite;

    AddressBad(const string &street, const string &city, const int suite)
        : street{street}, city{city}, suite{suite}
    {
    }

    friend ostream &operator<<(ostream &os, const AddressBad &obj)
    {
        return os
               << "street: " << obj.street
               << " city: " << obj.city
               << " suite: " << obj.suite;
    }
};

struct ContactBad
{
    string name;
    AddressBad *address;

    ContactBad(string name, AddressBad *address)
        : name{name}, address{address}
    {
    }

    ~ContactBad()
    {
        delete address;
    }

    friend ostream &operator<<(ostream &os, const ContactBad &obj)
    {
        return os
               << "name: " << obj.name
               << " works at " << *obj.address;
    }
};

// PROBLEM: Creating employees is tedious - must manually create address for each employee
// This is error-prone and violates DRY principle

// ============================================================================
// SOLUTION 1: Prototype Pattern with Simple Clone
// ============================================================================

struct Address
{
    string street;
    string city;
    int suite;

    Address(const string &street, const string &city, const int suite)
        : street{street}, city{city}, suite{suite}
    {
    }

    // Copy constructor for deep copy
    Address(const Address &other)
        : street{other.street}, city{other.city}, suite{other.suite}
    {
    }

    friend ostream &operator<<(ostream &os, const Address &obj)
    {
        return os
               << "street: " << obj.street
               << " city: " << obj.city
               << " suite: " << obj.suite;
    }
};

struct Contact
{
    string name;
    Address *address;

    Contact() : name(""), address(nullptr)
    {
    }

    Contact(string name, Address *address)
        : name{name}, address{address}
    {
    }

    // Copy constructor - deep copy with prototype cloning
    Contact(const Contact &other)
        : name{other.name}
    {
        address = new Address(*other.address); // Deep copy the address
    }

    // Assignment operator
    Contact &operator=(const Contact &other)
    {
        if (this == &other)
            return *this;

        name = other.name;

        if (address)
            delete address;

        address = new Address(*other.address);
        return *this;
    }

    ~Contact()
    {
        delete address;
    }

    friend ostream &operator<<(ostream &os, const Contact &obj)
    {
        return os
               << "name: " << obj.name
               << " works at " << *obj.address;
    }
};

// ============================================================================
// SOLUTION 2: Factory with Prototype Templates
// ============================================================================

struct EmployeeFactory
{
    static Contact main;
    static Contact aux;

    // Create employee based on main office prototype
    static unique_ptr<Contact> NewMainOfficeEmployee(string name, int suite)
    {
        return NewEmployee(name, suite, main);
    }

    // Create employee based on auxiliary office prototype
    static unique_ptr<Contact> NewAuxOfficeEmployee(string name, int suite)
    {
        return NewEmployee(name, suite, aux);
    }

private:
    // Clone the prototype and customize
    static unique_ptr<Contact> NewEmployee(string name, int suite, Contact &proto)
    {
        auto result = make_unique<Contact>(proto); // Clone prototype
        result->name = name;                       // Customize name
        result->address->suite = suite;            // Customize suite
        return result;
    }
};

// Initialize static prototypes with default values
inline Contact EmployeeFactory::main{"Unknown", new Address{"123 East Dr", "London", 0}};
inline Contact EmployeeFactory::aux{"Unknown", new Address{"123B East Dr", "London", 0}};

// ============================================================================
// SOLUTION 3: Enhanced Prototype Registry
// ============================================================================

class PrototypeRegistry
{
private:
    map<string, Contact> prototypes;

public:
    void register_prototype(const string &name, const Contact &proto)
    {
        prototypes[name] = proto;
    }

    unique_ptr<Contact> create(const string &proto_name, const string &name, int suite)
    {
        if (prototypes.find(proto_name) == prototypes.end())
            throw runtime_error("Prototype not found: " + proto_name);

        auto result = make_unique<Contact>(prototypes[proto_name]);
        result->name = name;
        result->address->suite = suite;
        return result;
    }

    void list_prototypes() const
    {
        cout << "Available prototypes:\n";
        for (const auto &p : prototypes)
        {
            cout << "  - " << p.first << ": " << p.second << "\n";
        }
    }
};

// ============================================================================
// SOLUTION 4: Cloneable Interface (Virtual Clone)
// ============================================================================

class Cloneable
{
public:
    virtual ~Cloneable() = default;
    virtual unique_ptr<Cloneable> clone() const = 0;
};

class FlexibleContact : public Cloneable
{
public:
    string name;
    Address *address;

    FlexibleContact(string name, Address *address)
        : name{name}, address{address}
    {
    }

    FlexibleContact(const FlexibleContact &other)
        : name{other.name}
    {
        address = new Address(*other.address);
    }

    FlexibleContact &operator=(const FlexibleContact &other)
    {
        if (this == &other)
            return *this;

        name = other.name;
        if (address)
            delete address;
        address = new Address(*other.address);
        return *this;
    }

    ~FlexibleContact()
    {
        delete address;
    }

    unique_ptr<Cloneable> clone() const override
    {
        return make_unique<FlexibleContact>(*this);
    }

    friend ostream &operator<<(ostream &os, const FlexibleContact &obj)
    {
        return os
               << "name: " << obj.name
               << " works at " << *obj.address;
    }
};

#endif // PROTOTYPE_H
