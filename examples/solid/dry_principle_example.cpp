#include <iostream>
#include "../include/solid/dry_principle.h"

int main()
{
    Creature orc;
    orc.set_strength(16);
    orc.set_agility(11);
    orc.set_intelligence(9);

    std::cout << "The orc has an average stat of "
              << orc.average()
              << " and a maximum stat of "
              << orc.max()
              << std::endl;

    return 0;
}
