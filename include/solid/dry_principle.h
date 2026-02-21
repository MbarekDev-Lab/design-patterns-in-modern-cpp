#pragma once
#include <array>
#include <numeric>
#include <algorithm>

class Creature
{
    enum Abilities
    {
        str,
        agl,
        intl,
        count
    };
    std::array<int, count> abilities;

public:
    int get_strength() const { return abilities[str]; }
    void set_strength(int value) { abilities[str] = value; }

    int get_agility() const { return abilities[agl]; }
    void set_agility(int value) { abilities[agl] = value; }

    int get_intelligence() const { return abilities[intl]; }
    void set_intelligence(int value) { abilities[intl] = value; }

    int sum() const
    {
        return std::accumulate(abilities.begin(), abilities.end(), 0);
    }

    double average() const
    {
        return sum() / static_cast<double>(count);
    }

    int max() const
    {
        return *std::max_element(abilities.begin(), abilities.end());
    }
};
