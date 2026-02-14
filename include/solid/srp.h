#ifndef SRP_H
#define SRP_H

#include <string>
#include <vector>

/**
 * Single Responsibility Principle (SRP)
 *
 * A class should have one, and only one, reason to change.
 *
 * This example shows:
 * 1. VIOLATION: Journal class with multiple responsibilities
 * 2. SOLUTION: Separate concerns into different classes
 */

// ============ VIOLATION ============
class JournalBad
{
private:
    std::string title;
    std::vector<std::string> entries;

public:
    explicit JournalBad(const std::string &title) : title{title} {}

    void add(const std::string &entry);

    // ❌ VIOLATION: Journal should only manage entries, not persistence!
    void save(const std::string &filename);

    const std::vector<std::string> &getEntries() const { return entries; }
    const std::string &getTitle() const { return title; }
};

// ============ SOLUTION ============
class Journal
{
private:
    std::string title;
    std::vector<std::string> entries;
    static int entryCount;

public:
    explicit Journal(const std::string &title) : title{title} {}

    void add(const std::string &entry);

    const std::vector<std::string> &getEntries() const { return entries; }
    const std::string &getTitle() const { return title; }
};

/**
 * ✅ CORRECT: Persistence is a separate concern
 * PersistenceManager is responsible ONLY for saving/loading
 * Journal is responsible ONLY for managing entries
 */
class PersistenceManager
{
public:
    static void save(const Journal &journal, const std::string &filename);
    static void load(Journal &journal, const std::string &filename);
};

#endif // SRP_H
