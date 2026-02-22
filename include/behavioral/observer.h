#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>

using namespace std;

// ============================================================================
// EventData - Base class for all event types
// ============================================================================
struct EventData
{
    virtual ~EventData() = default;
    virtual void print() const = 0;
};

// ============================================================================
// Observer - Abstract observer interface
// ============================================================================
struct Observer
{
    virtual ~Observer() = default;
    virtual void on_event(EventData *event) = 0;
};

// ============================================================================
// Observable - Subject that manages observers
// ============================================================================
class Observable
{
private:
    vector<Observer *> observers;

public:
    // Subscribe an observer to this observable
    void subscribe(Observer *observer)
    {
        if (observer && find(observers.begin(), observers.end(), observer) == observers.end())
        {
            observers.push_back(observer);
        }
    }

    // Unsubscribe an observer from this observable
    void unsubscribe(Observer *observer)
    {
        auto it = find(observers.begin(), observers.end(), observer);
        if (it != observers.end())
        {
            observers.erase(it);
        }
    }

    // Notify all observers of an event
    void notify(EventData *event)
    {
        for (auto observer : observers)
        {
            observer->on_event(event);
        }
    }

    // Get number of observers
    size_t observer_count() const
    {
        return observers.size();
    }

    // Check if an observer is subscribed
    bool has_observer(Observer *observer) const
    {
        return find(observers.begin(), observers.end(), observer) != observers.end();
    }
};

// ============================================================================
// Event Types for Game/Sports scenario
// ============================================================================
struct PlayerScoredData : EventData
{
    string player_name;
    int goals_scored_so_far;

    PlayerScoredData(const string &name, int goals)
        : player_name(name), goals_scored_so_far(goals)
    {
    }

    void print() const override
    {
        cout << player_name << " has scored! (their "
             << goals_scored_so_far << " goal)" << "\n";
    }
};

struct GameStartedData : EventData
{
    string team_a;
    string team_b;

    GameStartedData(const string &a, const string &b)
        : team_a(a), team_b(b)
    {
    }

    void print() const override
    {
        cout << "Game started: " << team_a << " vs " << team_b << "\n";
    }
};

struct GameEndedData : EventData
{
    string winner;
    int team_a_score;
    int team_b_score;

    GameEndedData(const string &w, int a, int b)
        : winner(w), team_a_score(a), team_b_score(b)
    {
    }

    void print() const override
    {
        cout << "Game ended! " << winner << " wins "
             << team_a_score << "-" << team_b_score << "\n";
    }
};

struct CardGivenData : EventData
{
    string player_name;
    char card_type; // 'Y' for yellow, 'R' for red

    CardGivenData(const string &name, char card)
        : player_name(name), card_type(card)
    {
    }

    void print() const override
    {
        cout << player_name << " received a "
             << (card_type == 'Y' ? "yellow" : "red") << " card\n";
    }
};

// ============================================================================
// Game - Observable that triggers events
// ============================================================================
class Game : public Observable
{
public:
    void start_game(const string &team_a, const string &team_b)
    {
        GameStartedData event{team_a, team_b};
        notify(&event);
    }

    void end_game(const string &winner, int a_score, int b_score)
    {
        GameEndedData event{winner, a_score, b_score};
        notify(&event);
    }

    void player_scored(const string &player_name, int goals_so_far)
    {
        PlayerScoredData event{player_name, goals_so_far};
        notify(&event);
    }

    void give_card(const string &player_name, char card_type)
    {
        CardGivenData event{player_name, card_type};
        notify(&event);
    }
};

// ============================================================================
// Player - Observable in game context
// ============================================================================
class Player : public Observable
{
private:
    string name;
    int goals_scored = 0;
    Game &game;

public:
    Player(const string &name, Game &game)
        : name(name), game(game)
    {
    }

    const string &get_name() const { return name; }
    int get_goals() const { return goals_scored; }

    void score()
    {
        goals_scored++;
        game.player_scored(name, goals_scored);

        // Also notify direct observers
        PlayerScoredData event{name, goals_scored};
        notify(&event);
    }

    void receive_card(char card_type)
    {
        game.give_card(name, card_type);

        // Also notify direct observers
        CardGivenData event{name, card_type};
        notify(&event);
    }
};

// ============================================================================
// Coach - Observer that monitors player performance
// ============================================================================
class Coach : public Observer
{
private:
    string name;
    Game &game;
    int celebrations_given = 0;
    int warnings_given = 0;

public:
    Coach(const string &name, Game &game)
        : name(name), game(game)
    {
        game.subscribe(this);
    }

    ~Coach()
    {
        game.unsubscribe(this);
    }

    void on_event(EventData *event) override
    {
        // Celebrate if player scores < 3 goals
        auto *scored = dynamic_cast<PlayerScoredData *>(event);
        if (scored && scored->goals_scored_so_far < 3)
        {
            cout << "Coach " << name << " says: well done, "
                 << scored->player_name << "!\n";
            celebrations_given++;
        }

        // Warn after receiving red card
        auto *carded = dynamic_cast<CardGivenData *>(event);
        if (carded && carded->card_type == 'R')
        {
            cout << "Coach " << name << " warns: "
                 << carded->player_name << ", you're off the field!\n";
            warnings_given++;
        }

        // React to game start
        auto *started = dynamic_cast<GameStartedData *>(event);
        if (started)
        {
            cout << "Coach " << name << " is ready for: "
                 << started->team_a << " vs " << started->team_b << "\n";
        }
    }

    int get_celebrations() const { return celebrations_given; }
    int get_warnings() const { return warnings_given; }
};

// ============================================================================
// StatisticsCollector - Observer that tracks game statistics
// ============================================================================
class StatisticsCollector : public Observer
{
private:
    int total_goals = 0;
    int total_cards = 0;
    int yellow_cards = 0;
    int red_cards = 0;

public:
    StatisticsCollector(Game &game)
    {
        game.subscribe(this);
    }

    void on_event(EventData *event) override
    {
        auto *scored = dynamic_cast<PlayerScoredData *>(event);
        if (scored)
        {
            total_goals++;
        }

        auto *carded = dynamic_cast<CardGivenData *>(event);
        if (carded)
        {
            total_cards++;
            if (carded->card_type == 'Y')
                yellow_cards++;
            else if (carded->card_type == 'R')
                red_cards++;
        }
    }

    int get_total_goals() const { return total_goals; }
    int get_total_cards() const { return total_cards; }
    int get_yellow_cards() const { return yellow_cards; }
    int get_red_cards() const { return red_cards; }
};

// ============================================================================
// Commentator - Observer that narrates game events
// ============================================================================
class Commentator : public Observer
{
private:
    string name;
    int comments_made = 0;

public:
    Commentator(const string &name, Game &game)
        : name(name)
    {
        game.subscribe(this);
    }

    void on_event(EventData *event) override
    {
        cout << "[" << name << "] ";
        event->print();
        comments_made++;
    }

    int get_comment_count() const { return comments_made; }
};

// ============================================================================
// Referee - Observer that monitors rules enforcement
// ============================================================================
class Referee : public Observer
{
private:
    int infractions_recorded = 0;

public:
    Referee(Game &game)
    {
        game.subscribe(this);
    }

    void on_event(EventData *event) override
    {
        auto *scored = dynamic_cast<PlayerScoredData *>(event);
        if (scored)
        {
            cout << "Referee notes: Goal scored by " << scored->player_name << "\n";
        }

        auto *carded = dynamic_cast<CardGivenData *>(event);
        if (carded)
        {
            cout << "Referee records: " << carded->player_name
                 << " - " << (carded->card_type == 'Y' ? "Yellow" : "Red") << " card\n";
            infractions_recorded++;
        }
    }

    int get_infractions() const { return infractions_recorded; }
};
