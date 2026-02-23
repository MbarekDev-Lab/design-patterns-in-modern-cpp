/*
 * Observer Pattern - Rat Swarm Exercise
 *
 * Problem: Imagine a game where one or more rats can attack a player. Each
 * individual rat has a base attack value of 1. However, rats attack as a swarm,
 * so each rat's ACTUAL attack value is equal to the total number of rats in play.
 *
 * Requirements:
 * - A rat enters play through the constructor
 * - A rat leaves play (dies) via its destructor
 * - At any point, all rats' attack values must be consistent (= total rat count)
 * - Implement using the Observer pattern
 *
 * This is a prime example of the Observer pattern where:
 * - Game is the Subject (observable)
 * - Rat is the Observer that gets notified of changes
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

// Forward declaration
struct Rat;

struct IRat
{
    virtual ~IRat() = default;
    virtual int getAttack() const = 0;
};

struct Game
{
    vector<Rat *> rats;

    void addRat(Rat *rat);
    void removeRat(Rat *rat);
    void updateAllRats();

    int getRatCount() const
    {
        return static_cast<int>(rats.size());
    }
};

struct Rat : IRat
{
    Game &game;
    int attack{1};

    Rat(Game &game);
    ~Rat();

    int getAttack() const override
    {
        return attack;
    }
};

// Implementation of Game methods (defined after Rat is declared)
inline void Game::addRat(Rat *rat)
{
    rats.push_back(rat);
    updateAllRats();
}

inline void Game::removeRat(Rat *rat)
{
    auto it = find(rats.begin(), rats.end(), rat);
    if (it != rats.end())
        rats.erase(it);
    updateAllRats();
}

inline void Game::updateAllRats()
{
    int count = static_cast<int>(rats.size());
    for (auto rat : rats)
        rat->attack = count;
}

// Implementation of Rat methods
inline Rat::Rat(Game &game) : game(game)
{
    game.addRat(this);
}

inline Rat::~Rat()
{
    game.removeRat(this);
}
// TEST SUITE
// ============================================================================

void test_single_rat()
{
    Game game;

    Rat rat1(game);
    assert(rat1.attack == 1);
    assert(rat1.getAttack() == 1);
    assert(game.getRatCount() == 1);
}

void test_rat_death()
{
    Game game;
    assert(game.getRatCount() == 0);

    {
        Rat rat1(game);
        assert(game.getRatCount() == 1);
        assert(rat1.attack == 1);
    } // rat1 dies here

    assert(game.getRatCount() == 0);
}

void test_two_rats_synchronization()
{
    Game game;

    Rat rat1(game);
    assert(rat1.attack == 1);
    assert(game.getRatCount() == 1);

    Rat rat2(game);
    assert(rat1.attack == 2); // rat1's attack updated!
    assert(rat2.attack == 2);
    assert(game.getRatCount() == 2);
}

void test_three_rats_swarm()
{
    Game game;

    Rat rat1(game);
    Rat rat2(game);
    Rat rat3(game);

    assert(rat1.attack == 3);
    assert(rat2.attack == 3);
    assert(rat3.attack == 3);
    assert(game.getRatCount() == 3);
}

void test_selective_rat_death()
{
    Game game;

    Rat rat1(game);
    Rat rat2(game);
    Rat rat3(game);

    assert(rat1.attack == 3);
    assert(rat2.attack == 3);
    assert(rat3.attack == 3);

    // rat2 dies
    {
        Rat temp = rat2;
    } // This won't work - we need pointers or different scope
}

void test_dynamic_swarm_growth_and_shrinkage()
{
    Game game;

    {
        Rat rat1(game);
        assert(rat1.attack == 1);

        {
            Rat rat2(game);
            assert(rat1.attack == 2);
            assert(rat2.attack == 2);

            {
                Rat rat3(game);
                assert(rat1.attack == 3);
                assert(rat2.attack == 3);
                assert(rat3.attack == 3);
                assert(game.getRatCount() == 3);
            } // rat3 dies

            assert(rat1.attack == 2);
            assert(rat2.attack == 2);
            assert(game.getRatCount() == 2);
        } // rat2 dies

        assert(rat1.attack == 1);
        assert(game.getRatCount() == 1);
    } // rat1 dies

    assert(game.getRatCount() == 0);
}

void test_large_swarm()
{
    Game game;

    vector<Rat *> rats;
    for (int i = 0; i < 10; ++i)
    {
        rats.push_back(new Rat(game));
    }

    // Every rat should have attack value of 10
    for (auto rat : rats)
    {
        assert(rat->attack == 10);
        assert(rat->getAttack() == 10);
    }

    assert(game.getRatCount() == 10);

    // Clean up
    for (auto rat : rats)
        delete rat;

    assert(game.getRatCount() == 0);
}

void test_observer_pattern_consistency()
{
    Game game;

    // Create rats one by one and verify consistency
    Rat rat1(game);
    assert(rat1.attack == 1);
    assert(game.getRatCount() == 1);

    Rat rat2(game);
    assert(rat1.attack == 2); // Observer pattern in action!
    assert(rat2.attack == 2);
    assert(game.getRatCount() == 2);

    Rat rat3(game);
    assert(rat1.attack == 3);
    assert(rat2.attack == 3);
    assert(rat3.attack == 3);
    assert(game.getRatCount() == 3);

    // All rats see consistent state
    for (int i = 0; i < 3; ++i)
    {
        // Verify we can call virtual method
        assert(rat1.getAttack() == 3);
        assert(rat2.getAttack() == 3);
        assert(rat3.getAttack() == 3);
    }
}

void test_reference_validity()
{
    Game game;

    Rat rat1(game);
    Rat rat2(game);

    // Verify we're always referencing the same game
    assert(&rat1.game == &rat2.game);
    assert(&rat1.game == &game);
}

void test_multiple_sequential_generations()
{
    Game game;

    // First generation
    {
        Rat r1(game);
        Rat r2(game);
        assert(r1.attack == 2);
        assert(r2.attack == 2);
    }

    assert(game.getRatCount() == 0);

    // Second generation
    {
        Rat r1(game);
        Rat r2(game);
        Rat r3(game);
        assert(r1.attack == 3);
        assert(r2.attack == 3);
        assert(r3.attack == 3);
    }

    assert(game.getRatCount() == 0);
}

int main()
{
    try
    {
        cout << "Running Observer Pattern - Rat Swarm Tests\n"
             << endl;

        cout << "Test 1: Single rat..." << flush;
        test_single_rat();
        cout << " ✓" << endl;

        cout << "Test 2: Rat death..." << flush;
        test_rat_death();
        cout << " ✓" << endl;

        cout << "Test 3: Two rats synchronization..." << flush;
        test_two_rats_synchronization();
        cout << " ✓" << endl;

        cout << "Test 4: Three rats swarm..." << flush;
        test_three_rats_swarm();
        cout << " ✓" << endl;

        cout << "Test 5: Dynamic growth and shrinkage..." << flush;
        test_dynamic_swarm_growth_and_shrinkage();
        cout << " ✓" << endl;

        cout << "Test 6: Large swarm (10 rats)..." << flush;
        test_large_swarm();
        cout << " ✓" << endl;

        cout << "Test 7: Observer pattern consistency..." << flush;
        test_observer_pattern_consistency();
        cout << " ✓" << endl;

        cout << "Test 8: Reference validity..." << flush;
        test_reference_validity();
        cout << " ✓" << endl;

        cout << "Test 9: Multiple sequential generations..." << flush;
        test_multiple_sequential_generations();
        cout << " ✓" << endl;

        cout << "\n✅ All 9 tests passed!\n"
             << endl;
        cout << "Key concepts demonstrated:" << endl;
        cout << "  • Observer Pattern: Game notifies all Rats of changes" << endl;
        cout << "  • Automatic registration/unregistration via constructor/destructor" << endl;
        cout << "  • Consistent state across all observers at any time" << endl;
        cout << "  • Virtual methods for polymorphic behavior" << endl;
        cout << "  • Forward declarations for cyclic dependencies" << endl;
        cout << "  • RAII (Resource Acquisition Is Initialization) principle" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Test failed with exception: " << e.what() << endl;
        return 1;
    }
}
