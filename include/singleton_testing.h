#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <memory>
// #include <boost/lexical_cast.hpp>
// using namespace boost;

using namespace std;

// ============================================================================
// SINGLETON PATTERN - Testing & Dependency Injection
// ============================================================================
// This demonstrates the problem with Singletons: they are hard to test
// because they are tightly coupled. The solution is to use an interface
// and dependency injection instead.
// ============================================================================

// ============================================================================
// DATABASE INTERFACE - The Abstraction
// ============================================================================

class Database
{
public:
    virtual ~Database() = default;
    virtual int get_population(const string &name) = 0;
};

// ============================================================================
// VIOLATION: SINGLETON DATABASE - Tight Coupling Problem
// ============================================================================

class SingletonDatabase : public Database
{
private:
    static int instance_count;

    SingletonDatabase()
    {
        cout << "Initializing singleton database from file..." << endl;
        instance_count++;
        // In real scenario, this would read from capitals.txt
        /*ifstream ifs("capitals.txt");
        if (!ifs)
        {
            cerr << "Failed to open capitals.txt, using hardcoded data\n";
            string s, s2;

            while (getline(ifs, s))
            {
                getline(ifs, s2);
                int pop = lexical_cast<int>(s2);
                capitals[s] = pop;
            }
        }*/

        // For demo, we'll use hardcoded data
        capitals["Tokyo"] = 37400068;
        capitals["Delhi"] = 32941000;
        capitals["Shanghai"] = 27058000;
        capitals["Sao Paulo"] = 22043028;
        capitals["Mexico City"] = 17400000;
        capitals["Seoul"] = 17500000;
    }

    map<string, int> capitals;

public:
    // Delete copy/assignment to enforce singleton
    SingletonDatabase(const SingletonDatabase &) = delete;
    SingletonDatabase &operator=(const SingletonDatabase &) = delete;

    // Single instance accessor
    static SingletonDatabase &get()
    {
        static SingletonDatabase db;
        return db;
    }

    int get_population(const string &name) override
    {
        return capitals[name];
    }

    // Property for testing
    static int get_instance_count()
    {
        return instance_count;
    }

    // For testing purposes - show database state
    void show_all() const
    {
        cout << "Database contents:\n";
        for (const auto &pair : capitals)
        {
            cout << "  " << pair.first << ": " << pair.second << "\n";
        }
    }
};

// ============================================================================
// DUMMY DATABASE - Mock for Testing
// ============================================================================

class DummyDatabase : public Database
{
private:
    map<string, int> capitals;

public:
    DummyDatabase()
    {
        capitals["alpha"] = 1;
        capitals["beta"] = 2;
        capitals["gamma"] = 3;
    }

    int get_population(const string &name) override
    {
        return capitals[name];
    }

    void show_all() const
    {
        cout << "Dummy database contents:\n";
        for (const auto &pair : capitals)
        {
            cout << "  " << pair.first << ": " << pair.second << "\n";
        }
    }
};

// ============================================================================
// SOLUTION: CONFIGURABLE DATABASE - Dependency Injection
// ============================================================================

class ConfigurableDatabase : public Database
{
private:
    map<string, int> capitals;

public:
    ConfigurableDatabase()
    {
        cout << "Initializing configurable database with custom data..." << endl;
        capitals["London"] = 9002488;
        capitals["Paris"] = 2161000;
        capitals["Berlin"] = 3645000;
    }

    int get_population(const string &name) override
    {
        return capitals[name];
    }

    void show_all() const
    {
        cout << "Configurable database contents:\n";
        for (const auto &pair : capitals)
        {
            cout << "  " << pair.first << ": " << pair.second << "\n";
        }
    }
};

// ============================================================================
// PROBLEM: SINGLETON RECORD FINDER - Tightly Coupled
// ============================================================================
// This is BAD because:
// - Cannot test with mock data
// - Requires actual database file to exist
// - Hard to test in isolation
// - Cannot swap implementations

class SingletonRecordFinder
{
public:
    int total_population(const vector<string> &names)
    {
        int result = 0;
        for (const auto &name : names)
        {
            result += SingletonDatabase::get().get_population(name);
        }
        return result;
    }
};

// ============================================================================
// SOLUTION: CONFIGURABLE RECORD FINDER - Loosely Coupled
// ============================================================================
// This is GOOD because:
// - Works with any Database implementation
// - Can be tested with mock data
// - Easy to test in isolation
// - Can swap implementations at runtime
// - Follows Dependency Inversion Principle

class ConfigurableRecordFinder
{
private:
    Database &db;

public:
    explicit ConfigurableRecordFinder(Database &db)
        : db(db)
    {
    }

    int total_population(const vector<string> &names) const
    {
        int result = 0;
        for (const auto &name : names)
        {
            result += db.get_population(name);
        }
        return result;
    }
};

// ============================================================================
// ADVANCED: DEPENDENCY INJECTOR - Factory Pattern
// ============================================================================

class ServiceLocator
{
private:
    static unique_ptr<Database> instance;

public:
    static void set_database(unique_ptr<Database> db)
    {
        instance = std::move(db);
    }

    static Database &get_database()
    {
        if (!instance)
        {
            instance = make_unique<DummyDatabase>();
        }
        return *instance;
    }

    static void reset()
    {
        instance = nullptr;
    }
};

// Static member definition
int SingletonDatabase::instance_count = 0;
unique_ptr<Database> ServiceLocator::instance = nullptr;
