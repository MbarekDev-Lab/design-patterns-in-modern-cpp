/*
 * Template Method Pattern - Card Game Combat Exercise
 *
 * The Template Method pattern defines the skeleton of an algorithm in a base class
 * and lets subclasses override specific steps without changing the algorithm's structure.
 *
 * In this card game example:
 * - CardGame::combat() is the template method (defines the algorithm skeleton)
 * - hit() is the abstract method (subclasses implement damage mechanics)
 * - TemporaryCardDamageGame: Health restores if creature survives the hit
 * - PermanentCardDamageGame: Health damage persists permanently
 */

#include <iostream>
#include <vector>
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

    // Template Method: Defines the combat algorithm skeleton
    // Two creatures exchange simultaneous attacks in a single round
    // Returns: index of survivor, or -1 if no clear winner
    int combat(int creature1, int creature2)
    {
        Creature &first = creatures[creature1];
        Creature &second = creatures[creature2];

        // Both creatures attack simultaneously
        hit(first, second);
        hit(second, first);

        // Determine winner based on survival
        bool first_alive = first.health > 0;
        bool second_alive = second.health > 0;

        // If both alive (neither dead) or both dead -> no clear winner
        if (first_alive == second_alive)
            return -1;

        // Return index of the survivor
        return first_alive ? creature1 : creature2;
    }

    // Abstract method: subclasses implement how damage is applied
    virtual void hit(Creature &attacker, Creature &other) = 0;
};

struct TemporaryCardDamageGame : CardGame
{
    TemporaryCardDamageGame(const vector<Creature> &creatures) : CardGame(creatures) {}

    // Temporary damage: health restores if creature survives the hit
    // (Like Magic: the Gathering - damage only matters if lethal)
    void hit(Creature &attacker, Creature &other) override
    {
        auto old_health = other.health;
        other.health -= attacker.attack;
        // If creature survives, restore health to original
        if (other.health > 0)
            other.health = old_health;
    }
};

struct PermanentCardDamageGame : CardGame
{
    PermanentCardDamageGame(const vector<Creature> &creatures) : CardGame(creatures) {}

    // Permanent damage: health reduction persists
    // (Like Hearthstone - damage accumulates over time)
    void hit(Creature &attacker, Creature &other) override
    {
        other.health -= attacker.attack;
    }
};

// ============================================================================
// GOOGLE TEST SUITE
// ============================================================================

#include "gtest/gtest.h"

namespace
{

    class Evaluate : public ::testing::Test
    {
    };

    TEST_F(Evaluate, ImpasseTest)
    {
        // 1/2 vs 1/2 with temporary damage - no winner possible
        // Round 1: C1 attacks (damage dealt but health restored),
        //          C2 attacks (damage dealt but health restored)
        // Both survive -> draw (-1)
        Creature c1{1, 2};
        Creature c2{1, 2};
        TemporaryCardDamageGame game({c1, c2});
        ASSERT_EQ(-1, game.combat(0, 1));
        ASSERT_EQ(-1, game.combat(0, 1));
    }

    TEST_F(Evaluate, TemporaryMurderTest)
    {
        // 1/1 vs 2/2 with temporary damage
        // Round 1: C1 (1/1) attacks C2 (health goes 2->1, restored to 2)
        //          C2 (2/2) attacks C1 (health goes 1->-1, dies permanently)
        // C2 survives -> creature 1 (index 1) wins
        Creature c1{1, 1};
        Creature c2{2, 2};
        TemporaryCardDamageGame game({c1, c2});
        ASSERT_EQ(1, game.combat(0, 1));
    }

    TEST_F(Evaluate, DoubleMurderTest)
    {
        // 2/2 vs 2/2 with temporary damage
        // Round 1: C1 (2/2) attacks C2 (health goes 2->0, dies - can't restore)
        //          C2 (2/2) attacks C1 (health goes 2->0, dies - can't restore)
        // Both dead simultaneously -> no winner (-1)
        Creature c1{2, 2};
        Creature c2{2, 2};
        TemporaryCardDamageGame game({c1, c2});
        ASSERT_EQ(-1, game.combat(0, 1))
            << "The expectation here is that two 2/2 creatures kill each other";
    }

    TEST_F(Evaluate, PermanentDamageDeathTest)
    {
        // Test permanent damage accumulation over multiple combats
        Creature c1{1, 2};
        Creature c2{1, 3};
        PermanentCardDamageGame game({c1, c2});

        // First combat: 1/2 vs 1/3 (full health)
        // C1 attacks C2 (health: 3->2), C2 attacks C1 (health: 2->1)
        // Neither dies -> no winner
        ASSERT_EQ(-1, game.combat(0, 1))
            << "1/2 vs 1/3 should have no winner after first round of combat";

        // After first combat, creatures are now 1/1 vs 1/2 (damaged)
        // Second combat: C1 (1/1) attacks C2 (health: 2->1, survives)
        //                C2 (1/2) attacks C1 (health: 1->0, dies)
        // C2 survives -> creature 1 wins
        ASSERT_EQ(1, game.combat(0, 1))
            << "1/1 vs 1/2 here, so winner should be = 1";
    }

} // namespace

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
