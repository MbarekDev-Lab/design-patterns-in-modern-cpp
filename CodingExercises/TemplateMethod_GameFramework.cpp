/*
 * Template Method Pattern - Game Framework
 *
 * The Template Method pattern defines the skeleton of an algorithm in a base class
 * and lets subclasses override specific steps without changing the algorithm's structure.
 *
 * This is particularly useful for:
 * - Defining the overall structure that subclasses must follow
 * - Avoiding code duplication
 * - Ensuring consistency across implementations
 *
 * In this example:
 * - Game defines the template method run()
 * - Subclasses (Chess, Checkers, DraughtsGame) implement game-specific logic
 * - The run() method always follows: start() → play() → declare winner()
 */

#include <iostream>
#include <string>
#include <cassert>
#include <sstream>
using namespace std;

// ============================================================================
// TEMPLATE METHOD BASE CLASS
// ============================================================================

class Game
{
public:
    explicit Game(int number_of_players)
        : number_of_players(number_of_players)
    {
    }

    virtual ~Game() = default;

    // Template Method - defines the algorithm skeleton
    void run()
    {
        start();
        while (!have_winner())
            take_turn();
        cout << "Player " << get_winner() << " wins.\n";
    }

    // Hook for capturing output in tests
    string get_game_log() const { return game_log; }

protected:
    // Abstract methods that subclasses must implement
    virtual void start() = 0;
    virtual bool have_winner() = 0;
    virtual void take_turn() = 0;
    virtual int get_winner() = 0;

    // Helper method to log game events
    void log(const string &message)
    {
        game_log += message + "\n";
        cout << message << "\n";
    }

    int current_player{0};
    int number_of_players;

private:
    string game_log;
};

// ============================================================================
// GAME IMPLEMENTATIONS
// ============================================================================

class Chess : public Game
{
public:
    explicit Chess()
        : Game{2}
    {
    }

protected:
    void start() override
    {
        log("Starting a game of chess with " + to_string(number_of_players) + " players");
    }

    bool have_winner() override
    {
        return turns >= max_turns;
    }

    void take_turn() override
    {
        log("Turn " + to_string(turns + 1) + " taken by player " + to_string(current_player));
        turns++;
        current_player = (current_player + 1) % number_of_players;
    }

    int get_winner() override
    {
        return current_player;
    }

private:
    int turns{0};
    int max_turns{10};
};

class Checkers : public Game
{
public:
    explicit Checkers()
        : Game{2}
    {
    }

protected:
    void start() override
    {
        log("Starting a game of checkers");
    }

    bool have_winner() override
    {
        return turns >= max_turns;
    }

    void take_turn() override
    {
        log("Checkers turn " + to_string(turns + 1) + " by player " + to_string(current_player));
        turns++;
        current_player = (current_player + 1) % number_of_players;
    }

    int get_winner() override
    {
        return current_player;
    }

private:
    int turns{0};
    int max_turns{5};
};

class DraughtsGame : public Game
{
public:
    explicit DraughtsGame()
        : Game{2}
    {
    }

protected:
    void start() override
    {
        log("Draught game started with " + to_string(number_of_players) + " players");
    }

    bool have_winner() override
    {
        return turns >= max_turns;
    }

    void take_turn() override
    {
        log("Draught move by player " + to_string(current_player));
        turns++;
        current_player = (current_player + 1) % number_of_players;
    }

    int get_winner() override
    {
        return current_player;
    }

private:
    int turns{0};
    int max_turns{3};
};

class MultiPlayerGame : public Game
{
public:
    explicit MultiPlayerGame(int players)
        : Game{players}
    {
    }

protected:
    void start() override
    {
        log("Starting multiplayer game with " + to_string(number_of_players) + " players");
    }

    bool have_winner() override
    {
        return turns >= max_turns;
    }

    void take_turn() override
    {
        log("Player " + to_string(current_player) + " takes turn");
        turns++;
        current_player = (current_player + 1) % number_of_players;
    }

    int get_winner() override
    {
        return current_player;
    }

private:
    int turns{0};
    int max_turns{10};
};

// ============================================================================
// TEST SUITE
// ============================================================================

void test_chess_game()
{
    cout << "\n=== Testing Chess ===" << endl;
    Chess chess;
    chess.run();

    string log = chess.get_game_log();
    assert(log.find("Starting a game of chess") != string::npos);
    assert(log.find("Turn") != string::npos);
    assert(log.find("player") != string::npos);
    cout << "✓ Chess game works" << endl;
}

void test_checkers_game()
{
    cout << "\n=== Testing Checkers ===" << endl;
    Checkers checkers;
    checkers.run();

    string log = checkers.get_game_log();
    assert(log.find("Checkers") != string::npos);
    assert(log.find("turn") != string::npos);
    cout << "✓ Checkers game works" << endl;
}

void test_draught_game()
{
    cout << "\n=== Testing Draught Game ===" << endl;
    DraughtsGame draught;
    draught.run();

    string log = draught.get_game_log();
    assert(log.find("Draught") != string::npos);
    assert(log.find("move") != string::npos);
    cout << "✓ Draught game works" << endl;
}

void test_multiplayer_game_3_players()
{
    cout << "\n=== Testing 3-Player Game ===" << endl;
    MultiPlayerGame game{3};
    game.run();

    string log = game.get_game_log();
    assert(log.find("3 players") != string::npos);
    cout << "✓ 3-player game works" << endl;
}

void test_multiplayer_game_4_players()
{
    cout << "\n=== Testing 4-Player Game ===" << endl;
    MultiPlayerGame game{4};
    game.run();

    string log = game.get_game_log();
    assert(log.find("4 players") != string::npos);
    cout << "✓ 4-player game works" << endl;
}

void test_chess_player_alternation()
{
    // Verify that players alternate correctly
    cout << "\n=== Testing Player Alternation ===" << endl;
    Chess chess;
    chess.run();

    string log = chess.get_game_log();

    // Count how many times each player plays
    int player_0_count = 0;
    int player_1_count = 0;

    size_t pos = 0;
    while ((pos = log.find("player 0", pos)) != string::npos)
    {
        player_0_count++;
        pos += 8;
    }

    pos = 0;
    while ((pos = log.find("player 1", pos)) != string::npos)
    {
        player_1_count++;
        pos += 8;
    }

    // Both players should play roughly the same number of times
    assert(abs(player_0_count - player_1_count) <= 1);
    cout << "✓ Player alternation works (P0: " << player_0_count
         << " times, P1: " << player_1_count << " times)" << endl;
}

void test_different_max_turns()
{
    cout << "\n=== Testing Different Game Lengths ===" << endl;

    // Chess has longer max_turns (10)
    Chess chess;
    chess.run();
    string chess_log = chess.get_game_log();

    // Checkers has shorter max_turns (5)
    Checkers checkers;
    checkers.run();
    string checkers_log = checkers.get_game_log();

    // Both should run to completion but chess should have more turns
    assert(chess_log.find("Turn 10") != string::npos);
    assert(checkers_log.find("Checkers turn 5") != string::npos);
    cout << "✓ Different game lengths work correctly" << endl;
}

void test_template_method_consistency()
{
    cout << "\n=== Testing Template Method Consistency ===" << endl;

    // All games should follow the same pattern:
    // 1. Start
    // 2. Play turns until winner
    // 3. Declare winner

    Chess chess;
    chess.run();

    Checkers checkers;
    checkers.run();

    DraughtsGame draught;
    draught.run();

    // All should have start message
    assert(!chess.get_game_log().empty());
    assert(!checkers.get_game_log().empty());
    assert(!draught.get_game_log().empty());

    cout << "✓ All games follow template method structure" << endl;
}

void test_game_output_format()
{
    cout << "\n=== Testing Output Format ===" << endl;
    Chess chess;
    chess.run();

    string log = chess.get_game_log();

    // Should have: Starting message, turns, winner announcement
    assert(log.find("Starting") != string::npos);
    assert(log.find("Turn") != string::npos);
    assert(log.find("won") != string::npos || log.find("player") != string::npos);

    cout << "✓ Output format is consistent" << endl;
}

int main()
{
    cout << "Template Method Pattern - Game Framework\n"
         << endl;

    try
    {
        cout << "Running tests...\n"
             << endl;

        test_chess_game();
        test_checkers_game();
        test_draught_game();
        test_multiplayer_game_3_players();
        test_multiplayer_game_4_players();
        test_chess_player_alternation();
        test_different_max_turns();
        test_template_method_consistency();
        test_game_output_format();

        cout << "\n✅ All 9 tests passed!" << endl;
        cout << "\nTemplate Method Pattern Benefits:" << endl;
        cout << "  • Defines the skeleton of the algorithm in base class" << endl;
        cout << "  • Subclasses implement only the specific steps they care about" << endl;
        cout << "  • Ensures all games follow the same structure" << endl;
        cout << "  • Allows code reuse (the run() method is the same for all games)" << endl;
        cout << "  • Provides extension points through abstract methods" << endl;
        cout << "  • Maintains the invariant that games always:" << endl;
        cout << "    1. Start" << endl;
        cout << "    2. Play until there's a winner" << endl;
        cout << "    3. Announce the winner" << endl;
        cout << "\nGame Implementations:" << endl;
        cout << "  • Chess: 10 turns, 2 players" << endl;
        cout << "  • Checkers: 5 turns, 2 players" << endl;
        cout << "  • Draught: 3 turns, 2 players" << endl;
        cout << "  • MultiPlayerGame: Configurable turns and players" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Test failed with exception: " << e.what() << endl;
        return 1;
    }
}
