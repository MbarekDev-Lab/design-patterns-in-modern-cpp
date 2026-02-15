#pragma once
#include <string>
#include <iostream>

// ============================================================================
// VIOLATION: Without Facets - Cumbersome Builder Pattern
// ============================================================================

class PersonBad
{
private:
    std::string street_address, post_code, city;
    std::string company_name, position;
    int annual_income = 0;

    PersonBad() {}

public:
    friend class PersonBuilderBad;
    friend std::ostream &operator<<(std::ostream &os, const PersonBad &obj)
    {
        return os
               << "Address: " << obj.street_address << ", " << obj.post_code
               << " " << obj.city
               << " | Work: " << obj.company_name << " (" << obj.position
               << ", $" << obj.annual_income << ")";
    }
};

class PersonBuilderBad
{
private:
    PersonBad person;

public:
    PersonBuilderBad &set_street(std::string street)
    {
        person.street_address = street;
        return *this;
    }

    PersonBuilderBad &set_postcode(std::string postcode)
    {
        person.post_code = postcode;
        return *this;
    }

    PersonBuilderBad &set_city(std::string city)
    {
        person.city = city;
        return *this;
    }

    PersonBuilderBad &set_company(std::string company)
    {
        person.company_name = company;
        return *this;
    }

    PersonBuilderBad &set_position(std::string position)
    {
        person.position = position;
        return *this;
    }

    PersonBuilderBad &set_income(int income)
    {
        person.annual_income = income;
        return *this;
    }

    // PROBLEM: Methods are all in one builder - no logical grouping
    // PROBLEM: Flat list of setters becomes hard to use with many properties
    // PROBLEM: No way to visualize related properties

    operator PersonBad() const
    {
        return person;
    }
};

// ============================================================================
// SOLUTION: With Facets - Elegant Separate Concern Builders
// ============================================================================

// Forward declarations
class PersonAddressBuilder;
class PersonJobBuilder;

class Person
{
private:
    // address
    std::string street_address, post_code, city;

    // employment
    std::string company_name, position;
    int annual_income = 0;

    Person() = default;

public:
    ~Person() = default;

    static class PersonBuilder create();

    Person(Person &&other) noexcept
        : street_address{std::move(other.street_address)},
          post_code{std::move(other.post_code)},
          city{std::move(other.city)},
          company_name{std::move(other.company_name)},
          position{std::move(other.position)},
          annual_income{other.annual_income}
    {
    }

    Person &operator=(Person &&other) noexcept
    {
        if (this == &other)
            return *this;
        street_address = std::move(other.street_address);
        post_code = std::move(other.post_code);
        city = std::move(other.city);
        company_name = std::move(other.company_name);
        position = std::move(other.position);
        annual_income = other.annual_income;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Person &obj)
    {
        return os
               << "Address: " << obj.street_address << ", " << obj.post_code
               << " " << obj.city
               << " | Work: " << obj.company_name << " (" << obj.position
               << ", $" << obj.annual_income << ")";
    }

    friend class PersonBuilder;
    friend class PersonAddressBuilder;
    friend class PersonJobBuilder;
};

// Base class providing access to all builder facets
class PersonBuilderBase
{
protected:
    Person &person;

    explicit PersonBuilderBase(Person &person) : person{person} {}

public:
    operator Person() const
    {
        return std::move(person);
    }

    // SOLUTION: Builder facets provide logical grouping
    // Each concern (address, job) has its own builder with domain-specific methods

    PersonAddressBuilder lives();
    PersonJobBuilder works();
};

class PersonAddressBuilder : public PersonBuilderBase
{
public:
    typedef PersonAddressBuilder Self;

    explicit PersonAddressBuilder(Person &person) : PersonBuilderBase{person} {}

    // Domain-specific methods for address
    Self &at(std::string street_address)
    {
        person.street_address = street_address;
        return *this;
    }

    Self &with_postcode(std::string post_code)
    {
        person.post_code = post_code;
        return *this;
    }

    Self &in(std::string city)
    {
        person.city = city;
        return *this;
    }
};

class PersonJobBuilder : public PersonBuilderBase
{
public:
    typedef PersonJobBuilder Self;

    explicit PersonJobBuilder(Person &person) : PersonBuilderBase{person} {}

    // Domain-specific methods for employment
    Self &at(std::string company_name)
    {
        person.company_name = company_name;
        return *this;
    }

    Self &as_a(std::string position)
    {
        person.position = position;
        return *this;
    }

    Self &earning(int annual_income)
    {
        person.annual_income = annual_income;
        return *this;
    }
};

class PersonBuilder : public PersonBuilderBase
{
private:
    Person p;

public:
    PersonBuilder() : PersonBuilderBase{p} {}
};

// Implementation of facet access methods - inline at end of header
inline PersonAddressBuilder PersonBuilderBase::lives()
{
    return PersonAddressBuilder{person};
}

inline PersonJobBuilder PersonBuilderBase::works()
{
    return PersonJobBuilder{person};
}

inline PersonBuilder Person::create()
{
    return PersonBuilder();
}
