#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

using namespace std;

// ============================================================================
// TEST FRAMEWORK
// ============================================================================

int g_test_count = 0;
int g_passed = 0;
int g_assertion_count = 0;

void TEST(const string &name)
{
    g_test_count++;
    cout << "\nTest " << g_test_count << ": " << name << "\n";
}

void ASSERT_EQ_INT(int actual, int expected)
{
    g_assertion_count++;
    if (actual == expected)
    {
        cout << "  ✓ ASSERT_EQ(" << actual << ") passed\n";
    }
    else
    {
        cout << "  ✗ ASSERT_EQ failed: expected " << expected
             << ", got " << actual << "\n";
        throw runtime_error("Assertion failed");
    }
}

void TEST_END()
{
    g_passed++;
}

// ============================================================================
// GOBLIN GAME IMPLEMENTATION
// ============================================================================

struct Creature;

struct Game
{
    vector<Creature *> creatures;
};

struct Creature
{
protected:
    Game &game;
    int base_attack, base_defense;

public:
    Creature(Game &game, int base_attack, int base_defense)
        : game(game), base_attack(base_attack), base_defense(base_defense) {}

    virtual int get_attack() = 0;
    virtual int get_defense() = 0;
    virtual ~Creature() = default;

    int get_base_attack() const { return base_attack; }
    int get_base_defense() const { return base_defense; }
};

// Forward declarations
class Goblin;
class GoblinKing;

// Goblin declaration with method declarations
class Goblin : public Creature
{
public:
    Goblin(Game &game, int base_attack, int base_defense)
        : Creature(game, base_attack, base_defense) {}

    explicit Goblin(Game &game)
        : Creature(game, 1, 1) {}

    int get_attack() override;
    int get_defense() override;

    virtual string get_name() const { return "Goblin"; }
};

// GoblinKing declaration with method declarations
class GoblinKing : public Goblin
{
public:
    explicit GoblinKing(Game &game)
        : Goblin(game, 3, 3) {}

    int get_attack() override;
    int get_defense() override;

    string get_name() const override { return "GoblinKing"; }
};

// Implementation of Goblin methods
inline int Goblin::get_attack()
{
    int attack = base_attack;
    // Get +1 attack for each GoblinKing in play (excluding self)
    for (auto c : game.creatures)
    {
        if (c != this)
        {
            if (dynamic_cast<GoblinKing *>(c))
            {
                attack += 1;
            }
        }
    }
    return attack;
}

inline int Goblin::get_defense()
{
    int defense = base_defense;
    // Get +1 defense for each other Goblin (including GoblinKing)
    for (auto c : game.creatures)
    {
        if (c != this)
        {
            if (dynamic_cast<Goblin *>(c))
            {
                defense += 1;
            }
        }
    }
    return defense;
}

// Implementation of GoblinKing methods
inline int GoblinKing::get_attack()
{
    // GoblinKing gets +1 from other GoblinKings
    int attack = base_attack;
    for (auto c : game.creatures)
    {
        if (c != this)
        {
            if (dynamic_cast<GoblinKing *>(c))
            {
                attack += 1;
            }
        }
    }
    return attack;
}

inline int GoblinKing::get_defense()
{
    int defense = base_defense;
    // Get +1 defense for each other Goblin (including other GoblinKings)
    for (auto c : game.creatures)
    {
        if (c != this)
        {
            if (dynamic_cast<Goblin *>(c))
            {
                defense += 1;
            }
        }
    }
    return defense;
}

// ============================================================================
// TEST CASES
// ============================================================================

void test_single_goblin()
{
    TEST("Single Goblin: 1/1");
    Game game;
    Goblin goblin(game);
    game.creatures.push_back(&goblin);

    ASSERT_EQ_INT(goblin.get_attack(), 1);
    ASSERT_EQ_INT(goblin.get_defense(), 1);
    TEST_END();
}

void test_single_goblin_king()
{
    TEST("Single GoblinKing: 3/3");
    Game game;
    GoblinKing king(game);
    game.creatures.push_back(&king);

    ASSERT_EQ_INT(king.get_attack(), 3);
    ASSERT_EQ_INT(king.get_defense(), 3);
    TEST_END();
}

void test_two_goblins_no_king()
{
    TEST("Two Goblins (no King): 1/2 each");
    Game game;
    Goblin goblin1(game);
    Goblin goblin2(game);
    game.creatures.push_back(&goblin1);
    game.creatures.push_back(&goblin2);

    // Each goblin: 1 base attack, 0 from king (no king)
    // Each goblin: 1 base defense + 1 from the other goblin
    ASSERT_EQ_INT(goblin1.get_attack(), 1);
    ASSERT_EQ_INT(goblin1.get_defense(), 2);
    ASSERT_EQ_INT(goblin2.get_attack(), 1);
    ASSERT_EQ_INT(goblin2.get_defense(), 2);
    TEST_END();
}

void test_three_goblins_no_king()
{
    TEST("Three Goblins (no King): 1/3 each");
    Game game;
    Goblin goblin1(game);
    Goblin goblin2(game);
    Goblin goblin3(game);
    game.creatures.push_back(&goblin1);
    game.creatures.push_back(&goblin2);
    game.creatures.push_back(&goblin3);

    // Each goblin: 1 base attack, 0 from king
    // Each goblin: 1 base defense + 2 from other goblins
    ASSERT_EQ_INT(goblin1.get_attack(), 1);
    ASSERT_EQ_INT(goblin1.get_defense(), 3);
    ASSERT_EQ_INT(goblin2.get_attack(), 1);
    ASSERT_EQ_INT(goblin2.get_defense(), 3);
    ASSERT_EQ_INT(goblin3.get_attack(), 1);
    ASSERT_EQ_INT(goblin3.get_defense(), 3);
    TEST_END();
}

void test_one_goblin_one_king()
{
    TEST("One Goblin + One King: Goblin 2/2, King 3/4");
    Game game;
    Goblin goblin(game);
    GoblinKing king(game);
    game.creatures.push_back(&goblin);
    game.creatures.push_back(&king);

    // Goblin: 1 base + 1 from king = 2 attack
    // Goblin: 1 base + 1 from king (king is a goblin) = 2 defense
    ASSERT_EQ_INT(goblin.get_attack(), 2);
    ASSERT_EQ_INT(goblin.get_defense(), 2);

    // King: 3 base + 0 from other kings = 3 attack
    // King: 3 base + 1 from goblin = 4 defense
    ASSERT_EQ_INT(king.get_attack(), 3);
    ASSERT_EQ_INT(king.get_defense(), 4);
    TEST_END();
}

void test_three_goblins_one_king()
{
    TEST("Three Goblins + One King: Goblins 2/4, King 3/6");
    Game game;
    Goblin goblin1(game);
    Goblin goblin2(game);
    Goblin goblin3(game);
    GoblinKing king(game);
    game.creatures.push_back(&goblin1);
    game.creatures.push_back(&goblin2);
    game.creatures.push_back(&goblin3);
    game.creatures.push_back(&king);

    // Each Goblin: 1 base + 1 from king = 2 attack
    // Each Goblin: 1 base + 2 from other goblins + 1 from king = 4 defense
    ASSERT_EQ_INT(goblin1.get_attack(), 2);
    ASSERT_EQ_INT(goblin1.get_defense(), 4);
    ASSERT_EQ_INT(goblin2.get_attack(), 2);
    ASSERT_EQ_INT(goblin2.get_defense(), 4);
    ASSERT_EQ_INT(goblin3.get_attack(), 2);
    ASSERT_EQ_INT(goblin3.get_defense(), 4);

    // King: 3 base + 0 from other kings = 3 attack
    // King: 3 base + 3 from goblins = 6 defense
    ASSERT_EQ_INT(king.get_attack(), 3);
    ASSERT_EQ_INT(king.get_defense(), 6);
    TEST_END();
}

void test_two_goblins_king_then_add_more()
{
    TEST("Dynamic: Start with Goblin+King, then add more Goblins");
    Game game;
    Goblin goblin1(game);
    GoblinKing king(game);
    game.creatures.push_back(&goblin1);
    game.creatures.push_back(&king);

    // Initial state: goblin 2/2, king 3/4
    ASSERT_EQ_INT(goblin1.get_attack(), 2);
    ASSERT_EQ_INT(goblin1.get_defense(), 2);
    ASSERT_EQ_INT(king.get_attack(), 3);
    ASSERT_EQ_INT(king.get_defense(), 4);

    // Add another goblin
    Goblin goblin2(game);
    game.creatures.push_back(&goblin2);

    // Now: goblin1 should be 2/3 (1 + 1 from king + 1 from goblin2)
    // goblin2 should be 2/3 (1 + 1 from king + 1 from goblin1)
    // king should be 3/5 (3 + 2 from goblins)
    ASSERT_EQ_INT(goblin1.get_attack(), 2);
    ASSERT_EQ_INT(goblin1.get_defense(), 3);
    ASSERT_EQ_INT(goblin2.get_attack(), 2);
    ASSERT_EQ_INT(goblin2.get_defense(), 3);
    ASSERT_EQ_INT(king.get_attack(), 3);
    ASSERT_EQ_INT(king.get_defense(), 5);
    TEST_END();
}

void test_two_kings_three_goblins()
{
    TEST("Two Kings + Three Goblins: Complex interactions");
    Game game;
    Goblin goblin1(game);
    Goblin goblin2(game);
    Goblin goblin3(game);
    GoblinKing king1(game);
    GoblinKing king2(game);
    game.creatures.push_back(&goblin1);
    game.creatures.push_back(&goblin2);
    game.creatures.push_back(&goblin3);
    game.creatures.push_back(&king1);
    game.creatures.push_back(&king2);

    // Each Goblin: 1 base + 2 from kings = 3 attack
    // Each Goblin: 1 base + 2 other goblins + 2 kings = 5 defense
    ASSERT_EQ_INT(goblin1.get_attack(), 3);
    ASSERT_EQ_INT(goblin1.get_defense(), 5);
    ASSERT_EQ_INT(goblin2.get_attack(), 3);
    ASSERT_EQ_INT(goblin2.get_defense(), 5);
    ASSERT_EQ_INT(goblin3.get_attack(), 3);
    ASSERT_EQ_INT(goblin3.get_defense(), 5);

    // Each King: 3 base + 1 from other king = 4 attack
    // Each King: 3 base + 3 goblins + 1 other king = 7 defense
    ASSERT_EQ_INT(king1.get_attack(), 4);
    ASSERT_EQ_INT(king1.get_defense(), 7);
    ASSERT_EQ_INT(king2.get_attack(), 4);
    ASSERT_EQ_INT(king2.get_defense(), 7);
    TEST_END();
}

void test_five_goblins_no_king()
{
    TEST("Five Goblins (no King): 1/5 each");
    Game game;
    vector<Goblin *> goblins;
    for (int i = 0; i < 5; ++i)
    {
        goblins.push_back(new Goblin(game));
        game.creatures.push_back(goblins.back());
    }

    // Each goblin: 1 base, 0 from king, 4 from others
    for (auto goblin : goblins)
    {
        ASSERT_EQ_INT(goblin->get_attack(), 1);
        ASSERT_EQ_INT(goblin->get_defense(), 5);
    }

    for (auto goblin : goblins)
    {
        delete goblin;
    }
    TEST_END();
}

void test_base_values_preserved()
{
    TEST("Base values unchanged");
    Game game;
    Goblin goblin(game);
    GoblinKing king(game);
    game.creatures.push_back(&goblin);
    game.creatures.push_back(&king);

    ASSERT_EQ_INT(goblin.get_base_attack(), 1);
    ASSERT_EQ_INT(goblin.get_base_defense(), 1);
    ASSERT_EQ_INT(king.get_base_attack(), 3);
    ASSERT_EQ_INT(king.get_base_defense(), 3);
    TEST_END();
}

void test_empty_game()
{
    TEST("Empty game: No creatures");
    Game game;
    // Just verify we can create a game without creatures
    ASSERT_EQ_INT(game.creatures.size(), 0);
    TEST_END();
}

void test_large_army()
{
    TEST("Large army: 10 Goblins + 2 Kings");
    Game game;
    vector<Goblin *> all_creatures;

    for (int i = 0; i < 10; ++i)
    {
        Goblin *goblin = new Goblin(game);
        all_creatures.push_back(goblin);
        game.creatures.push_back(goblin);
    }

    for (int i = 0; i < 2; ++i)
    {
        GoblinKing *king = new GoblinKing(game);
        all_creatures.push_back(king);
        game.creatures.push_back(king);
    }

    // Each Goblin: 1 base + 2 kings = 3 attack
    // Each Goblin: 1 base + 9 other goblins + 2 kings = 12 defense
    for (int i = 0; i < 10; ++i)
    {
        Goblin *goblin = dynamic_cast<Goblin *>(all_creatures[i]);
        ASSERT_EQ_INT(goblin->get_attack(), 3);
        ASSERT_EQ_INT(goblin->get_defense(), 12);
    }

    // Each King: 3 base + 1 other king = 4 attack
    // Each King: 3 base + 10 goblins + 1 other king = 14 defense
    for (int i = 10; i < 12; ++i)
    {
        GoblinKing *king = dynamic_cast<GoblinKing *>(all_creatures[i]);
        ASSERT_EQ_INT(king->get_attack(), 4);
        ASSERT_EQ_INT(king->get_defense(), 14);
    }

    for (auto creature : all_creatures)
    {
        delete creature;
    }
    TEST_END();
}

// ============================================================================
// MAIN - RUN ALL TESTS
// ============================================================================

int main()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "CHAIN OF RESPONSIBILITY CODING EXERCISE - GOBLIN GAME\n";
    cout << string(70, '=') << "\n";

    try
    {
        test_single_goblin();
        test_single_goblin_king();
        test_two_goblins_no_king();
        test_three_goblins_no_king();
        test_one_goblin_one_king();
        test_three_goblins_one_king();
        test_two_goblins_king_then_add_more();
        test_two_kings_three_goblins();
        test_five_goblins_no_king();
        test_base_values_preserved();
        test_empty_game();
        test_large_army();
    }
    catch (const runtime_error &e)
    {
        cout << "\n❌ Test failed with exception: " << e.what() << "\n";
    }

    cout << "\n"
         << string(70, '=') << "\n";
    cout << "TEST SUMMARY\n";
    cout << string(70, '=') << "\n";
    cout << "Tests Run:     " << g_test_count << "\n";
    cout << "Tests Passed:  " << g_passed << "\n";
    cout << "Tests Failed:  " << (g_test_count - g_passed) << "\n";
    cout << "Assertions:    " << g_assertion_count << "\n";

    if (g_test_count == g_passed)
    {
        cout << "Status:        ✅ ALL TESTS PASSED (" << g_passed << "/" << g_test_count << ")\n";
    }
    else
    {
        cout << "Status:        ❌ SOME TESTS FAILED\n";
    }

    cout << string(70, '=') << "\n\n";

    return (g_test_count == g_passed) ? 0 : 1;
}
