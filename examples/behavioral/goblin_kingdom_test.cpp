#include <iostream>
#include <cassert>
#include "../include/behavioral/goblin_kingdom.h"

int main()
{
    cout << "=== GOBLIN KINGDOM TEST ===" << endl;
    cout << endl;

    // === TEST 1: Single Goblin ===
    {
        cout << "TEST 1: Single Goblin (no bonuses)" << endl;
        Game game;
        Goblin g(game);
        game.creatures.push_back(&g);

        assert(g.get_attack() == 1);
        assert(g.get_defense() == 1);
        cout << "  ✓ Single goblin has base stats (1, 1)" << endl;
        cout << endl;
    }

    // === TEST 2: Two Goblins (defense bonus) ===
    {
        cout << "TEST 2: Two Goblins (each gets defense bonus from the other)" << endl;
        Game game;
        Goblin g1(game), g2(game);
        game.creatures.push_back(&g1);
        game.creatures.push_back(&g2);

        assert(g1.get_attack() == 1);  // No GoblinKing nearby
        assert(g1.get_defense() == 2); // +1 from g2
        assert(g2.get_attack() == 1);  // No GoblinKing nearby
        assert(g2.get_defense() == 2); // +1 from g1

        cout << "  ✓ g1 stats: attack=" << g1.get_attack() << ", defense=" << g1.get_defense() << endl;
        cout << "  ✓ g2 stats: attack=" << g2.get_attack() << ", defense=" << g2.get_defense() << endl;
        cout << endl;
    }

    // === TEST 3: Goblin with GoblinKing ===
    {
        cout << "TEST 3: Goblin with GoblinKing (attack and defense bonuses)" << endl;
        Game game;
        Goblin g(game);
        GoblinKing gk(game);
        game.creatures.push_back(&g);
        game.creatures.push_back(&gk);

        assert(g.get_attack() == 2);   // +1 from GoblinKing
        assert(g.get_defense() == 2);  // +1 from GoblinKing (GoblinKing is a Goblin)
        assert(gk.get_attack() == 3);  // No other GoblinKings
        assert(gk.get_defense() == 4); // +1 from regular goblin

        cout << "  ✓ Goblin stats: attack=" << g.get_attack() << ", defense=" << g.get_defense() << endl;
        cout << "  ✓ GoblinKing stats: attack=" << gk.get_attack() << ", defense=" << gk.get_defense() << endl;
        cout << endl;
    }

    // === TEST 4: Multiple Goblins with GoblinKing ===
    {
        cout << "TEST 4: Multiple Goblins with GoblinKing" << endl;
        Game game;
        Goblin g1(game), g2(game);
        GoblinKing gk(game);
        game.creatures.push_back(&g1);
        game.creatures.push_back(&g2);
        game.creatures.push_back(&gk);

        assert(g1.get_attack() == 2);  // +1 from GoblinKing
        assert(g1.get_defense() == 3); // +1 from g2, +1 from GoblinKing
        assert(g2.get_attack() == 2);  // +1 from GoblinKing
        assert(g2.get_defense() == 3); // +1 from g1, +1 from GoblinKing
        assert(gk.get_attack() == 3);  // No other GoblinKings
        assert(gk.get_defense() == 5); // +1 from g1, +1 from g2

        cout << "  ✓ g1 stats: attack=" << g1.get_attack() << ", defense=" << g1.get_defense() << endl;
        cout << "  ✓ g2 stats: attack=" << g2.get_attack() << ", defense=" << g2.get_defense() << endl;
        cout << "  ✓ GoblinKing stats: attack=" << gk.get_attack() << ", defense=" << gk.get_defense() << endl;
        cout << endl;
    }

    // === TEST 5: Two GoblinKings ===
    {
        cout << "TEST 5: Two GoblinKings (attack bonus from each other)" << endl;
        Game game;
        GoblinKing gk1(game), gk2(game);
        game.creatures.push_back(&gk1);
        game.creatures.push_back(&gk2);

        assert(gk1.get_attack() == 4);  // +1 from gk2
        assert(gk1.get_defense() == 4); // +1 from gk2
        assert(gk2.get_attack() == 4);  // +1 from gk1
        assert(gk2.get_defense() == 4); // +1 from gk1

        cout << "  ✓ gk1 stats: attack=" << gk1.get_attack() << ", defense=" << gk1.get_defense() << endl;
        cout << "  ✓ gk2 stats: attack=" << gk2.get_attack() << ", defense=" << gk2.get_defense() << endl;
        cout << endl;
    }

    // === TEST 6: Complex Scenario ===
    {
        cout << "TEST 6: Complex Scenario (2 Goblins, 2 GoblinKings)" << endl;
        Game game;
        Goblin g1(game), g2(game);
        GoblinKing gk1(game), gk2(game);
        game.creatures.push_back(&g1);
        game.creatures.push_back(&g2);
        game.creatures.push_back(&gk1);
        game.creatures.push_back(&gk2);

        assert(g1.get_attack() == 3);   // +1 from gk1, +1 from gk2
        assert(g1.get_defense() == 4);  // +1 from g2, +1 from gk1, +1 from gk2
        assert(gk1.get_attack() == 4);  // +1 from gk2
        assert(gk1.get_defense() == 5); // +1 from g1, +1 from g2, +1 from gk2

        cout << "  ✓ g1 stats: attack=" << g1.get_attack() << ", defense=" << g1.get_defense() << endl;
        cout << "  ✓ gk1 stats: attack=" << gk1.get_attack() << ", defense=" << gk1.get_defense() << endl;
        cout << endl;
    }

    cout << "=== ALL TESTS PASSED ===" << endl;
    return 0;
}
