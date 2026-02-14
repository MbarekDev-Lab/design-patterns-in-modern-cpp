#include "solid/srp.h"
#include <fstream>
#include <iostream>

int Journal::entryCount = 1;

void Journal::add(const std::string &entry)
{
    entries.push_back(std::to_string(entryCount++) + ": " + entry);
}

void JournalBad::add(const std::string &entry)
{
    static int count = 1;
    entries.push_back(std::to_string(count++) + ": " + entry);
}

void JournalBad::save(const std::string &filename)
{
    std::ofstream ofs(filename);
    if (!ofs)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    for (const auto &entry : entries)
    {
        ofs << entry << "\n";
    }
}

void PersistenceManager::save(const Journal &journal, const std::string &filename)
{
    std::ofstream ofs(filename);
    if (!ofs)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    ofs << "=== " << journal.getTitle() << " ===\n";
    for (const auto &entry : journal.getEntries())
    {
        ofs << entry << "\n";
    }
    std::cout << "Saved to " << filename << std::endl;
}

void PersistenceManager::load(Journal &journal, const std::string &filename)
{
    std::ifstream ifs(filename);
    if (!ifs)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(ifs, line))
    {
        if (!line.empty() && line[0] != '=')
        {
            journal.add(line);
        }
    }
    std::cout << "Loaded from " << filename << std::endl;
}
