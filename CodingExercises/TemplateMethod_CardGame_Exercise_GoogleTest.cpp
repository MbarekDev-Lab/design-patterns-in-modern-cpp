/*
 * Template Method Pattern - Card Game Combat Exercise
 *
 * Google Test Framework Version
 *
 * The Template Method pattern defines the skeleton of an algorithm (combat)
 * in a base class, while letting subclasses override specific steps (how damage is applied).
 *
 * Key concept:
 * - CardGame::combat() is the template method - defines the algorithm skeleton
 * - hit() is the abstract method - subclasses implement their damage mechanics
 * - TemporaryCardDamageGame: Health restores if creature survives the hit
 * - PermanentCardDamageGame: Health damage persists permanently
 */

#include <iostream>
#include <vector>
#include "gtest/gtest.h"

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
    // Two creatures exchange simultaneous attacks
    // Returns: index of survivor, or -1 if no clear winner (both die or neither dies)
    int combat(int creature1, int creature2)
    {
        Creature &first = creatures[creature1];
        Creature &second = creatures[creature2];

        // Both attack simultaneously
        hit(first, second);
        hit(second, first);

        // Determine winner
        bool first_alive = first.health > 0;
        bool second_alive = second.health > 0;

        // If both alive or both dead: no clear winner
        if (first_alive == second_alive)
            return -1;

        // Return the survivor
        return first_alive ? creature1 : creature2;
    }

    // Abstract method: subclasses implement their damage mechanics
    virtual void hit(Creature &attacker, Creature &other) = 0;
};

struct TemporaryCardDamageGame : CardGame
{
    TemporaryCardDamageGame(const vector<Creature> &creatures) : CardGame(creatures) {}

    // Temporary damage: health is restored if the creature survives
    // (Like Magic: the Gathering - damage is temporary unless lethal)
    void hit(Creature &attacker, Creature &other) override
    {
        auto old_health = other.health;
        other.health -= attacker.attack;
        // If creature survives the damage, restore health to original
        if (other.health > 0)
            other.health = old_health;
    }
};

struct PermanentCardDamageGame : CardGame
{
    PermanentCardDamageGame(const vector<Creature> &creatures) : CardGame(creatures) {}

    // Permanent damage: health reduction persists
    // (Like Hearthstone - damage is permanent and cumulative)
    void hit(Creature &attacker, Creature &other) override
    {
        other.health -= attacker.attack;
    }
};

// ============================================================================
// GOOGLE TEST SUITE
// ============================================================================

class CardGameCombat : public ::testing::Test
{
};

TEST_F(CardGameCombat, TemporaryDamageImpasse)
{
    // 1/2 vs 1/2 with temporary damage
    // Round 1: C1 attacks (health goes 2->1, then restored to 2)
    //          C2 attacks (health goes 2->1, then restored to 2)
    // Neither dies -> no winner
    Creature c1{1, 2};
    Creature c2{1, 2};
    TemporaryCardDamageGame game({c1, c2});
    ASSERT_EQ(-1, game.combat(0, 1)) << "1/2 vs 1/2: creatures too weak to kill each other";
    // Combat again with same state
    ASSERT_EQ(-1, game.combat(0, 1)) << "Second combat should also be a draw";
}

TEST_F(CardGameCombat, TemporaryDamageMurder)
{
    // 1/1 vs 2/2 with temporary damage
    // Round 1: C1 (1/1) attacks C2 (health goes 2->1, restored to 2)
    //          C2 (2/2) attacks C1 (health goes 1->-1, creature dies)
    // C2 survives -> creature 1 wins
    Creature c1{1, 1};
    Creature c2{2, 2};
    TemporaryCardDamageGame game({c1, c2});
    ASSERT_EQ(1, game.combat(0, 1)) << "Creature 2 (index 1) should kill creature 1";
}

TEST_F(CardGameCombat, TemporaryDamageDoubleMurder)
{
    // 2/2 vs 2/2 with temporary damage
    // Round 1: C1 (2/2) attacks C2 (health goes 2->0, creature dies - no restoration)
    //          C2 (2/2) attacks C1 (health goes 2->0, creature dies - no restoration)
    // Both die simultaneously -> no winner
    Creature c1{2, 2};
    Creature c2{2, 2};
    TemporaryCardDamageGame game({c1, c2});
    ASSERT_EQ(-1, game.combat(0, 1))
        << "Two 2/2 creatures kill each other simultaneously (draw)";
}

TEST_F(CardGameCombat, PermanentDamageMultipleRounds)
{
    // 1/2 vs 1/3 with permanent damage
    // Round 1: C1 attacks C2 (health: 3->2), C2 attacks C1 (health: 2->1)
    //          Neither dies -> no winner, creatures damaged
    Creature c1{1, 2};
    Creature c2{1, 3};
    PermanentCardDamageGame game({c1, c2});

    // First combat: 1/2 vs 1/3 (full health)
    ASSERT_EQ(-1, game.combat(0, 1))
        << "1/2 vs 1/3 should have no winner after first attack exchange";

    // After first combat: creatures are now 1/1 vs 1/2
    // Second combat: C1 (1/1) attacks C2 (health: 2->1, survives)
    //                C2 (1/2) attacks C1 (health: 1->0, dies)
    //                C2 survives -> creature 1 wins
    ASSERT_EQ(1, game.combat(0, 1))
        << "After damage, creatures are 1/1 vs 1/2, so creature 1 wins";
}

// ============================================================================
// Additional Tests for Completeness
// ============================================================================

TEST_F(CardGameCombat, OneHitKill)
{
    // 5/1 vs 1/5 with temporary damage
    // C1 (5/1) attacks C2 (health goes 5->0, dies)
    // C2 dies without getting return attack
    Creature attacker{5, 1};
    Creature defender{1, 5};
    TemporaryCardDamageGame game({attacker, defender});
    ASSERT_EQ(0, game.combat(0, 1)) << "Strong attack kills weak defender";
}

TEST_F(CardGameCombat, PermanentDamageAccumulation)
{
    // 2/5 vs 2/5 with permanent damage - multiple rounds needed
    // Round 1: Both take 2 damage (5->3), neither dies
    // Can directly call combat once (single round) though
    Creature c1{2, 5};
    Creature c2{2, 5};
    PermanentCardDamageGame game({c1, c2});

    // After one round: 2/3 vs 2/3
    ASSERT_EQ(-1, game.combat(0, 1)) << "Equal creatures in a single round are a draw";
}

TEST_F(CardGameCombat, TemporaryVsPermanent)
{
    // Demonstrate the difference between temporary and permanent damage
    // With 1/2 vs 1/3 and temporary damage: always a draw
    Creature temp_c1{1, 2};
    Creature temp_c2{1, 3};
    TemporaryCardDamageGame temp_game({temp_c1, temp_c2});
    ASSERT_EQ(-1, temp_game.combat(0, 1));
    ASSERT_EQ(-1, temp_game.combat(0, 1)) << "Temporary damage resets health each round";

    // With permanent damage: creatures stay damaged
    Creature perm_c1{1, 2};
    Creature perm_c2{1, 3};
    PermanentCardDamageGame perm_game({perm_c1, perm_c2});
    ASSERT_EQ(-1, perm_game.combat(0, 1)) << "First round: 1/2 vs 1/3 - no winner";
    ASSERT_EQ(1, perm_game.combat(0, 1)) << "Second round: 1/1 vs 1/2 - creature 1 wins";
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
