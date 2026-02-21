#include <iostream>
#include "../include/behavioral/goblin_kingdom.h"

int main()
{
    cout << "=== GOBLIN KINGDOM EXAMPLE ===" << endl;
    cout << endl;

    // Create a game world
    Game game;

    // Create some goblins
    cout << "--- Initial Setup ---" << endl;
    Goblin g1(game);
    Goblin g2(game);
    GoblinKing gk(game);

    game.creatures.push_back(&g1);
    game.creatures.push_back(&g2);
    game.creatures.push_back(&gk);

    cout << "Created: Goblin g1, Goblin g2, GoblinKing gk" << endl;
    cout << endl;

    // Display stats
    cout << "--- Creature Stats (with bonuses from nearby creatures) ---" << endl;
    cout << "Goblin g1:" << endl;
    cout << "  - Base Attack: 1, Actual Attack: " << g1.get_attack() << " (bonus from GoblinKing)" << endl;
    cout << "  - Base Defense: 1, Actual Defense: " << g1.get_defense() << " (bonus from g2 and gk)" << endl;
    cout << endl;

    cout << "Goblin g2:" << endl;
    cout << "  - Base Attack: 1, Actual Attack: " << g2.get_attack() << " (bonus from GoblinKing)" << endl;
    cout << "  - Base Defense: 1, Actual Defense: " << g2.get_defense() << " (bonus from g1 and gk)" << endl;
    cout << endl;

    cout << "GoblinKing gk:" << endl;
    cout << "  - Base Attack: 3, Actual Attack: " << gk.get_attack() << " (bonus only from other GoblinKings)" << endl;
    cout << "  - Base Defense: 3, Actual Defense: " << gk.get_defense() << " (bonus from g1 and g2)" << endl;
    cout << endl;

    // Add another GoblinKing
    cout << "--- After Adding Another GoblinKing ---" << endl;
    GoblinKing gk2(game);
    game.creatures.push_back(&gk2);

    cout << "Added GoblinKing gk2" << endl;
    cout << endl;

    cout << "GoblinKing gk:" << endl;
    cout << "  - Actual Attack: " << gk.get_attack() << " (bonus from gk2)" << endl;
    cout << "  - Actual Defense: " << gk.get_defense() << " (bonus from g1, g2, and gk2)" << endl;
    cout << endl;

    cout << "GoblinKing gk2:" << endl;
    cout << "  - Actual Attack: " << gk2.get_attack() << " (bonus from gk)" << endl;
    cout << "  - Actual Defense: " << gk2.get_defense() << " (bonus from g1, g2, and gk)" << endl;

    return 0;
}
