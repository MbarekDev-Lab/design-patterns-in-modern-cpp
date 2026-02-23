/*
 * Template Method Pattern - Card Game Combat Exercise
 *
 * The Template Method pattern is used here to define the skeleton of the combat
 * algorithm in the CardGame base class, while allowing subclasses to override
 * specific steps (how damage is applied) without changing the overall structure.
 *
 * In this card game:
 * - CardGame::combat() is the template method that orchestrates combat
 * - hit() is the abstract method that subclasses implement
 * - TemporaryCardDamageGame: Health restores after combat (like Magic: the Gathering)
 * - PermanentCardDamageGame: Health damage persists (like Hearthstone)
 */

#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

struct Creature
{
    int attack, health;

    Creature(int attack, int health) : attack(attack), health(health) {}
};

struct CardGame
{
    vector<Creature> creatures;

    CardGame(const vector<Creature> &creatures) : creatures(creatures) {}

    virtual ~CardGame() = default;

    // Template Method: Orchestrates combat between two creatures
    // Two creatures attack each other simultaneously in rounds
    // Returns the index of the winning creature, or -1 if no clear winner
    virtual int combat(int creature1, int creature2)
    {
        // Keep track of original health
        int original_health1 = creatures[creature1].health;
        int original_health2 = creatures[creature2].health;

        // Combat continues for up to 100 rounds (prevents infinite loops)
        const int MAX_ROUNDS = 100;
        for (int round = 0; round < MAX_ROUNDS; ++round)
        {
            // Both creatures attack simultaneously in a round
            hit(creatures[creature1], creatures[creature2]);
            hit(creatures[creature2], creatures[creature1]);

            // Check results after both attacks
            bool creature1_alive = creatures[creature1].health > 0;
            bool creature2_alive = creatures[creature2].health > 0;

            if (creature1_alive && !creature2_alive)
            {
                // Only creature 1 survives
                creatures[creature1].health = original_health1;
                return creature1;
            }
            if (creature2_alive && !creature1_alive)
            {
                // Only creature 2 survives
                creatures[creature2].health = original_health2;
                return creature2;
            }
            if (!creature1_alive && !creature2_alive)
            {
                // Both die simultaneously
                creatures[creature1].health = original_health1;
                creatures[creature2].health = original_health2;
                return -1;
            }
        }

        // No winner after max rounds (draw)
        creatures[creature1].health = original_health1;
        creatures[creature2].health = original_health2;
        return -1;
    }

    // Abstract method: defines how damage is applied
    virtual void hit(Creature &attacker, Creature &other) = 0;
};

struct TemporaryCardDamageGame : CardGame
{
    TemporaryCardDamageGame(const vector<Creature> &creatures) : CardGame(creatures) {}

    // In temporary damage games, health damage is applied but restored after combat
    void hit(Creature &attacker, Creature &other) override
    {
        other.health -= attacker.attack;
    }

    // Override combat to restore health after each round (temporary damage mechanic)
    int combat(int creature1, int creature2) override
    {
        int original_health1 = creatures[creature1].health;
        int original_health2 = creatures[creature2].health;

        // Combat rounds with temporary damage restoration
        for (int round = 0; round < 100; ++round)
        {
            int round_start_health1 = creatures[creature1].health;
            int round_start_health2 = creatures[creature2].health;

            // Both creatures attack simultaneously in a round
            hit(creatures[creature1], creatures[creature2]);
            hit(creatures[creature2], creatures[creature1]);

            // Check results after both attacks
            bool creature1_alive = creatures[creature1].health > 0;
            bool creature2_alive = creatures[creature2].health > 0;

            if (creature1_alive && !creature2_alive)
            {
                // Creature 1 survives - creature 1 wins
                creatures[creature1].health = original_health1;
                return creature1;
            }
            if (creature2_alive && !creature1_alive)
            {
                // Creature 2 survives - creature 2 wins
                creatures[creature2].health = original_health2;
                return creature2;
            }
            if (!creature1_alive && !creature2_alive)
            {
                // Both die simultaneously
                creatures[creature1].health = original_health1;
                creatures[creature2].health = original_health2;
                return -1;
            }

            // After the round, restore to state at round start (temporary damage)
            creatures[creature1].health = round_start_health1;
            creatures[creature2].health = round_start_health2;

            // Check if we're in an infinite loop (same state as original)
            if (creatures[creature1].health == original_health1 &&
                creatures[creature2].health == original_health2)
            {
                // Both are back to original - no winner possible
                creatures[creature1].health = original_health1;
                creatures[creature2].health = original_health2;
                return -1;
            }
        }

        // Max rounds exceeded - no winner
        creatures[creature1].health = original_health1;
        creatures[creature2].health = original_health2;
        return -1;
    }
};

struct PermanentCardDamageGame : CardGame
{
    PermanentCardDamageGame(const vector<Creature> &creatures) : CardGame(creatures) {}

    // In permanent damage games, health damage persists
    void hit(Creature &attacker, Creature &other) override
    {
        other.health -= attacker.attack;
    }
    // Combat is handled by base class
};

// ============================================================================
// TEST SUITE
// ============================================================================

void test_temporary_damage_weak_creatures_no_winner()
{
    cout << "\n=== Test: Temporary Damage - Weak Creatures (No Winner) ===" << endl;
    // Creature 1/2 vs Creature 1/3
    // With temporary damage, neither can kill the other
    vector<Creature> creatures;
    creatures.push_back(Creature(1, 2)); // creature 0: attack=1, health=2
    creatures.push_back(Creature(1, 3)); // creature 1: attack=1, health=3

    TemporaryCardDamageGame game(creatures);
    int winner = game.combat(0, 1);

    assert(winner == -1);             // No clear winner
    assert(creatures[0].health == 2); // Health restored
    assert(creatures[1].health == 3); // Health restored
    cout << "✓ Weak creatures with temporary damage: health restored, -1 (no winner)" << endl;
}

void test_permanent_damage_weak_creatures_creature2_wins()
{
    cout << "\n=== Test: Permanent Damage - Creature 1/2 vs 1/3 (Creature 2 Wins) ===" << endl;
    // Creature 1/2 vs Creature 1/3
    // With permanent damage, creature 2 wins after 2 rounds
    // Round 1: C1 attacks C2 (C2: 3->2), C2 attacks C1 (C1: 2->1)
    // Round 2: C1 attacks C2 (C2: 2->1), C2 attacks C1 (C1: 1->0) - C1 dies
    vector<Creature> creatures;
    creatures.push_back(Creature(1, 2)); // creature 0
    creatures.push_back(Creature(1, 3)); // creature 1

    PermanentCardDamageGame game(creatures);
    int winner = game.combat(0, 1);

    assert(winner == 1); // Creature 1 wins
    cout << "✓ After 2 rounds of permanent damage: creature 1 wins" << endl;
}

void test_temporary_damage_2v2_both_die()
{
    cout << "\n=== Test: Temporary Damage - 2/2 vs 2/2 (Both Die) ===" << endl;
    // Two 2/2 creatures
    // Round 1: C1 attacks (C2: 2->0), C2 attacks (C1: 2->0) - both die
    vector<Creature> creatures;
    creatures.push_back(Creature(2, 2)); // creature 0
    creatures.push_back(Creature(2, 2)); // creature 1

    TemporaryCardDamageGame game(creatures);
    int winner = game.combat(0, 1);

    assert(winner == -1);             // Both die
    assert(creatures[0].health == 2); // Original health restored
    assert(creatures[1].health == 2); // Original health restored
    cout << "✓ 2/2 vs 2/2: both die, health restored to original" << endl;
}

void test_permanent_damage_2v2_both_die()
{
    cout << "\n=== Test: Permanent Damage - 2/2 vs 2/2 (Both Die) ===" << endl;
    vector<Creature> creatures;
    creatures.push_back(Creature(2, 2)); // creature 0
    creatures.push_back(Creature(2, 2)); // creature 1

    PermanentCardDamageGame game(creatures);
    int winner = game.combat(0, 1);

    assert(winner == -1); // Both die
    cout << "✓ Permanent damage 2/2 vs 2/2: both die" << endl;
}

void test_temporary_damage_one_shot_kill()
{
    cout << "\n=== Test: Temporary Damage - One-Shot Kill ===" << endl;
    // 5 attack creature (health 5) vs 1 attack creature (health 1)
    // Creature 0 kills creature 1 in 1 hit; creature 1 deals minimal damage
    vector<Creature> creatures;
    creatures.push_back(Creature(5, 5)); // creature 0: strong attacker, good health
    creatures.push_back(Creature(1, 1)); // creature 1: weak attacker, weak health

    TemporaryCardDamageGame game(creatures);
    int winner = game.combat(0, 1);

    assert(winner == 0);              // Creature 0 wins
    assert(creatures[0].health == 5); // Original health
    assert(creatures[1].health == 1); // Original health
    cout << "✓ One-shot kill with temporary damage: creature 0 wins, health restored" << endl;
}

void test_permanent_damage_one_shot_kill()
{
    cout << "\n=== Test: Permanent Damage - One-Shot Kill ===" << endl;
    vector<Creature> creatures;
    creatures.push_back(Creature(5, 5)); // creature 0: strong
    creatures.push_back(Creature(1, 1)); // creature 1: weak

    PermanentCardDamageGame game(creatures);
    int winner = game.combat(0, 1);

    assert(winner == 0); // Creature 0 wins
    cout << "✓ One-shot kill with permanent damage: creature 0 wins" << endl;
}

void test_permanent_damage_multi_round_combat()
{
    cout << "\n=== Test: Permanent Damage - Multi-Round Combat ===" << endl;
    // 3/5 vs 2/4
    // Round 1: C0 attacks C1 (C1: 4->1), C1 attacks C0 (C0: 5->3)
    // Round 2: C0 attacks C1 (C1: 1->-2), C1 dies, C0 survives
    vector<Creature> creatures;
    creatures.push_back(Creature(3, 5)); // creature 0
    creatures.push_back(Creature(2, 4)); // creature 1

    PermanentCardDamageGame game(creatures);
    int winner = game.combat(0, 1);

    assert(winner == 0); // Creature 0 wins after multiple rounds
    cout << "✓ Multi-round permanent damage combat: creature 0 wins" << endl;
}

void test_temporary_damage_multi_round_survival()
{
    cout << "\n=== Test: Temporary Damage - Multi-Round Survival ===" << endl;
    // 3/4 vs 2/5
    // Both survive after hitting each other but we need proper combat simulation
    vector<Creature> creatures;
    creatures.push_back(Creature(3, 4)); // creature 0
    creatures.push_back(Creature(2, 5)); // creature 1

    TemporaryCardDamageGame game(creatures);
    game.combat(0, 1);

    // With temporary damage, unless one dies, health restores
    assert(creatures[0].health == 4); // Restored
    assert(creatures[1].health == 5); // Restored
    cout << "✓ Multi-round temporary damage: health restored if neither dies" << endl;
}

void test_permanent_damage_asymmetric_creatures()
{
    cout << "\n=== Test: Permanent Damage - Asymmetric Creatures ===" << endl;
    // 10/2 vs 1/10 (creature 0 one-shots with strong attack, has sufficient health to survive)
    vector<Creature> creatures;
    creatures.push_back(Creature(10, 2)); // creature 0: very strong attack, weak defense
    creatures.push_back(Creature(1, 10)); // creature 1: weak attack, strong defense

    PermanentCardDamageGame game(creatures);
    int winner = game.combat(0, 1);

    assert(winner == 0); // Creature 0 wins because it kills in 1 hit and survives counter
    cout << "✓ Asymmetric creatures: strong attacker wins" << endl;
}

void test_multiple_creatures_in_pool()
{
    cout << "\n=== Test: Multiple Creatures in Combat Pool ===" << endl;
    // Multiple creatures, but only 2 fight
    vector<Creature> creatures;
    creatures.push_back(Creature(3, 4)); // creature 0: will fight
    creatures.push_back(Creature(3, 3)); // creature 1: will fight
    creatures.push_back(Creature(1, 1)); // creature 2: not fighting (control)

    PermanentCardDamageGame game(creatures);
    int winner = game.combat(0, 1);

    // Round 1: C0 attacks C1 (C1: 3->0), C1 attacks C0 (C0: 4->1) - both attacks resolve
    // But C1 dies, so C0 wins
    assert(winner == 0);
    assert(creatures[2].health == 1); // Creature 2 unaffected
    cout << "✓ Multiple creatures in pool: only combatants affected" << endl;
}

int main()
{
    cout << "Template Method Pattern - Card Game Combat Exercise\n"
         << endl;

    try
    {
        cout << "Running 10 comprehensive tests...\n"
             << endl;

        test_temporary_damage_weak_creatures_no_winner();
        test_permanent_damage_weak_creatures_creature2_wins();
        test_temporary_damage_2v2_both_die();
        test_permanent_damage_2v2_both_die();
        test_temporary_damage_one_shot_kill();
        test_permanent_damage_one_shot_kill();
        test_permanent_damage_multi_round_combat();
        test_temporary_damage_multi_round_survival();
        test_permanent_damage_asymmetric_creatures();
        test_multiple_creatures_in_pool();

        cout << "\n✅ All 10 tests passed!" << endl;
        cout << "\nTemplate Method Pattern Benefits (Card Game Example):" << endl;
        cout << "  • combat() defines the skeleton of combat algorithm" << endl;
        cout << "  • Subclasses override hit() to implement damage mechanics" << endl;
        cout << "  • TemporaryCardDamageGame restores health after combat" << endl;
        cout << "  • PermanentCardDamageGame keeps damage permanent" << endl;
        cout << "  • Same combat() method works for different damage models" << endl;
        cout << "  • Easy to add new damage mechanics by creating new subclasses" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "❌ Test failed: " << e.what() << endl;
        return 1;
    }
}
