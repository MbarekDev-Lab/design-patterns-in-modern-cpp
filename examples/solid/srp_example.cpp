#include "solid/srp.h"
#include <iostream>

int main()
{
    std::cout << "=== Single Responsibility Principle (SRP) ===\n\n";

    // ❌ VIOLATION: Journal doing too much
    std::cout << "VIOLATION - Journal with persistence responsibility:\n";
    {
        JournalBad badJournal{"My Life"};
        badJournal.add("I ate a bug");
        badJournal.add("I cried today");
        badJournal.save("diary_bad.txt");
        std::cout << "Problem: Journal class has TWO reasons to change\n";
        std::cout << "  1. When entry logic changes\n";
        std::cout << "  2. When persistence logic changes\n\n";
    }

    // ✅ SOLUTION: Separation of concerns
    std::cout << "SOLUTION - Proper separation of concerns:\n";
    {
        Journal journal{"Dear Diary"};
        journal.add("I ate a bug");
        journal.add("I cried today");

        // Save using dedicated manager
        PersistenceManager pm;
        pm.save(journal, "diary.txt");

        std::cout << "\nBenefit: Classes have SINGLE reason to change\n";
        std::cout << "  - Journal: only entry management\n";
        std::cout << "  - PersistenceManager: only file I/O\n\n";
    }

    // Show entries
    std::cout << "Entries saved:\n";
    Journal journal{"Test"};
    journal.add("Entry 1");
    journal.add("Entry 2");
    journal.add("Entry 3");
    PersistenceManager::save(journal, "test_entries.txt");

    return 0;
}
