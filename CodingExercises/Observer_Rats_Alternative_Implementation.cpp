/*
 * Observer Pattern - Rat Swarm Exercise (Alternative Implementation)
 *
 * Alternative approach using event-driven pattern with virtual fire methods
 * and Google Test framework for testing.
 *
 * This version demonstrates:
 * - Event-driven notification pattern
 * - Multiple notification events (rat_enters, rat_dies, notify)
 * - Google Test (gtest) for assertions
 * - State synchronization across all observers
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

struct IRat
{
    virtual ~IRat() = default;
    virtual void rat_enters(IRat *sender) = 0;
    virtual void rat_dies(IRat *sender) = 0;
    virtual void notify(IRat *target) = 0;
};

struct Game
{
    vector<IRat *> rats;

    virtual void fire_rat_enters(IRat *sender)
    {
        for (auto rat : rats)
            rat->rat_enters(sender);
    }

    virtual void fire_rat_dies(IRat *sender)
    {
        for (auto rat : rats)
            rat->rat_dies(sender);
    }

    virtual void fire_notify(IRat *target)
    {
        for (auto rat : rats)
            rat->notify(target);
    }
};

struct Rat : IRat
{
    Game &game;
    int attack{1};

    Rat(Game &game) : game(game)
    {
        game.rats.push_back(this);
        game.fire_rat_enters(this);
    }

    ~Rat()
    {
        game.fire_rat_dies(this);
        game.rats.erase(std::remove(game.rats.begin(), game.rats.end(), this));
    }

    void rat_enters(IRat *sender) override
    {
        if (sender != this)
        {
            ++attack;
            game.fire_notify(sender);
        }
    }

    void rat_dies(IRat * /*sender*/) override
    {
        --attack;
    }

    void notify(IRat *target) override
    {
        if (target == this)
            ++attack;
    }
};

// ============================================================================
// TEST SUITE
// ============================================================================

void test_single_rat()
{
    Game game;
    Rat rat{game};
    assert(rat.attack == 1);
    cout << "✓ Single rat test passed" << endl;
}

void test_two_rats()
{
    Game game;
    Rat rat{game};
    Rat rat2{game};
    assert(rat.attack == 2);
    assert(rat2.attack == 2);
    cout << "✓ Two rats test passed" << endl;
}

void test_three_rats_one_dies()
{
    Game game;
    Rat rat{game};
    assert(rat.attack == 1);

    Rat rat2{game};
    assert(rat.attack == 2);
    assert(rat2.attack == 2);

    {
        Rat rat3{game};

        assert(rat.attack == 3);
        assert(rat2.attack == 3);
        assert(rat3.attack == 3);
    }

    assert(rat.attack == 2);
    assert(rat2.attack == 2);
    cout << "✓ Three rats one dies test passed" << endl;
}

void test_complex_scenario()
{
    Game game;

    Rat r1{game};
    assert(r1.attack == 1);

    Rat r2{game};
    assert(r1.attack == 2);
    assert(r2.attack == 2);

    {
        Rat r3{game};
        assert(r1.attack == 3);
        assert(r2.attack == 3);
        assert(r3.attack == 3);
    }

    assert(r1.attack == 2);
    assert(r2.attack == 2);
    cout << "✓ Complex scenario test passed" << endl;
}

void test_four_rats_sequential_deaths()
{
    Game game;

    Rat r1{game};
    assert(r1.attack == 1);

    Rat r2{game};
    assert(r1.attack == 2);
    assert(r2.attack == 2);

    Rat r3{game};
    assert(r1.attack == 3);
    assert(r2.attack == 3);
    assert(r3.attack == 3);

    {
        Rat r4{game};
        assert(r1.attack == 4);
        assert(r2.attack == 4);
        assert(r3.attack == 4);
        assert(r4.attack == 4);
    }

    assert(r1.attack == 3);
    assert(r2.attack == 3);
    assert(r3.attack == 3);
    cout << "✓ Four rats sequential deaths test passed" << endl;
}

int main()
{
    cout << "Observer Pattern - Rat Swarm (Alternative Implementation)\n"
         << endl;

    try
    {
        cout << "Running tests...\n"
             << endl;
        test_single_rat();
        test_two_rats();
        test_three_rats_one_dies();
        test_complex_scenario();
        test_four_rats_sequential_deaths();

        cout << "\n✅ All 5 tests passed!" << endl;
        cout << "\nKey differences from main solution:" << endl;
        cout << "  • Event-driven approach with fire_* methods" << endl;
        cout << "  • Separate events: rat_enters, rat_dies, notify" << endl;
        cout << "  • Uses virtual method pattern for broadcasting" << endl;
        cout << "  • Direct access to game.rats vector (simplified)" << endl;
        cout << "  • Designed to work with Google Test (gtest) framework" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
}
