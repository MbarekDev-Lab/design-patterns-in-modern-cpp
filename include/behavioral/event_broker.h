#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <algorithm>
#include <typeinfo>
#include <any>

using namespace std;

// ============================================================================
// EventData - Base class for all events
// ============================================================================
struct EventData
{
    virtual ~EventData() = default;
    virtual void print() const = 0;
};

// ============================================================================
// Event Types
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

struct PlayerInjuredData : EventData
{
    string player_name;
    string injury_type;

    PlayerInjuredData(const string &name, const string &injury)
        : player_name(name), injury_type(injury)
    {
    }

    void print() const override
    {
        cout << player_name << " is injured: " << injury_type << "\n";
    }
};

// ============================================================================
// EventBroker - Central event bus for decoupled communication
// ============================================================================
class EventBroker
{
private:
    struct SubscriptionBase
    {
        virtual ~SubscriptionBase() = default;
        virtual void publish(const EventData *event) = 0;
    };

    template <typename EventType>
    struct Subscription : SubscriptionBase
    {
        using CallbackType = function<void(const EventType *)>;
        vector<CallbackType> callbacks;

        void publish(const EventData *event) override
        {
            if (auto *typed_event = dynamic_cast<const EventType *>(event))
            {
                for (auto &callback : callbacks)
                {
                    callback(typed_event);
                }
            }
        }

        void subscribe(CallbackType callback)
        {
            callbacks.push_back(callback);
        }

        size_t subscriber_count() const
        {
            return callbacks.size();
        }
    };

    map<string, shared_ptr<SubscriptionBase>> subscriptions;

    // Get or create subscription for an event type
    template <typename EventType>
    shared_ptr<Subscription<EventType>> get_subscription()
    {
        string type_name = typeid(EventType).name();
        auto it = subscriptions.find(type_name);

        if (it == subscriptions.end())
        {
            auto subscription = make_shared<Subscription<EventType>>();
            subscriptions[type_name] = subscription;
            return subscription;
        }

        return static_pointer_cast<Subscription<EventType>>(it->second);
    }

public:
    // Subscribe to a specific event type
    template <typename EventType>
    void subscribe(function<void(const EventType *)> callback)
    {
        auto subscription = get_subscription<EventType>();
        subscription->subscribe(callback);
    }

    // Publish an event to all subscribers
    void publish(const EventData *event)
    {
        if (!event)
            return;

        string type_name = typeid(*event).name();
        auto it = subscriptions.find(type_name);

        if (it != subscriptions.end())
        {
            it->second->publish(event);
        }
    }

    // Get subscriber count for all event types
    size_t total_subscriptions() const
    {
        size_t total = 0;
        for (const auto &subscription : subscriptions)
        {
            if (auto *sub = dynamic_cast<Subscription<PlayerScoredData> *>(subscription.second.get()))
                total += sub->subscriber_count();
            // Could add more event types here if needed
        }
        return total;
    }

    // Clear all subscriptions
    void clear()
    {
        subscriptions.clear();
    }

    // Get number of event types with subscribers
    size_t event_type_count() const
    {
        return subscriptions.size();
    }
};

// ============================================================================
// Player - Publishes score events
// ============================================================================
class Player
{
private:
    string name;
    int goals_scored = 0;
    EventBroker &broker;

public:
    Player(const string &name, EventBroker &broker)
        : name(name), broker(broker)
    {
    }

    const string &get_name() const { return name; }
    int get_goals() const { return goals_scored; }

    void score()
    {
        goals_scored++;
        PlayerScoredData event{name, goals_scored};
        broker.publish(&event);
    }

    void get_injured(const string &injury_type)
    {
        PlayerInjuredData event{name, injury_type};
        broker.publish(&event);
    }
};

// ============================================================================
// Coach - Subscribes to score events
// ============================================================================
class Coach
{
private:
    string name;
    int celebrations_given = 0;
    int injury_reports = 0;

public:
    Coach(const string &name, EventBroker &broker)
        : name(name)
    {
        // Subscribe to score events
        broker.subscribe<PlayerScoredData>([this, &name](const PlayerScoredData *event)
                                           {
            if (event->goals_scored_so_far < 3)
            {
                cout << "Coach " << this->name << " says: well done, " 
                     << event->player_name << "!\n";
                celebrations_given++;
            } });

        // Subscribe to injury events
        broker.subscribe<PlayerInjuredData>([this, &name](const PlayerInjuredData *event)
                                            {
            cout << "Coach " << this->name << " is concerned: " 
                 << event->player_name << " has a " << event->injury_type << "\n";
            injury_reports++; });
    }

    int get_celebrations() const { return celebrations_given; }
    int get_injury_reports() const { return injury_reports; }
};

// ============================================================================
// StatisticsCollector - Subscribes to all events for aggregation
// ============================================================================
class StatisticsCollector
{
private:
    int total_goals = 0;
    int total_cards = 0;
    int yellow_cards = 0;
    int red_cards = 0;
    int injuries = 0;

public:
    StatisticsCollector(EventBroker &broker)
    {
        broker.subscribe<PlayerScoredData>([this](const PlayerScoredData *event)
                                           { total_goals++; });

        broker.subscribe<CardGivenData>([this](const CardGivenData *event)
                                        {
            total_cards++;
            if (event->card_type == 'Y')
                yellow_cards++;
            else if (event->card_type == 'R')
                red_cards++; });

        broker.subscribe<PlayerInjuredData>([this](const PlayerInjuredData *event)
                                            { injuries++; });
    }

    int get_total_goals() const { return total_goals; }
    int get_total_cards() const { return total_cards; }
    int get_yellow_cards() const { return yellow_cards; }
    int get_red_cards() const { return red_cards; }
    int get_injuries() const { return injuries; }
};

// ============================================================================
// Commentator - Narrates events
// ============================================================================
class Commentator
{
private:
    string name;
    int comments_made = 0;

public:
    Commentator(const string &name, EventBroker &broker)
        : name(name)
    {
        broker.subscribe<PlayerScoredData>([this](const PlayerScoredData *event)
                                           {
            cout << "[" << this->name << "] " << event->player_name << " has scored! (their " 
                 << event->goals_scored_so_far << " goal)\n";
            comments_made++; });

        broker.subscribe<GameStartedData>([this](const GameStartedData *event)
                                          {
            cout << "[" << this->name << "] Game started: " << event->team_a 
                 << " vs " << event->team_b << "\n";
            comments_made++; });

        broker.subscribe<GameEndedData>([this](const GameEndedData *event)
                                        {
            cout << "[" << this->name << "] Game ended! " << event->winner << " wins " 
                 << event->team_a_score << "-" << event->team_b_score << "\n";
            comments_made++; });

        broker.subscribe<CardGivenData>([this](const CardGivenData *event)
                                        {
            cout << "[" << this->name << "] " << event->player_name << " received a " 
                 << (event->card_type == 'Y' ? "yellow" : "red") << " card\n";
            comments_made++; });

        broker.subscribe<PlayerInjuredData>([this](const PlayerInjuredData *event)
                                            {
            cout << "[" << this->name << "] " << event->player_name << " is injured: " 
                 << event->injury_type << "\n";
            comments_made++; });
    }

    int get_comment_count() const { return comments_made; }
};

// ============================================================================
// Referee - Enforces rules
// ============================================================================
class Referee
{
private:
    int infractions_recorded = 0;
    int injuries_noted = 0;

public:
    Referee(EventBroker &broker)
    {
        broker.subscribe<CardGivenData>([this](const CardGivenData *event)
                                        {
            cout << "Referee records: " << event->player_name 
                 << " - " << (event->card_type == 'Y' ? "Yellow" : "Red") << " card\n";
            infractions_recorded++; });

        broker.subscribe<PlayerInjuredData>([this](const PlayerInjuredData *event)
                                            {
            cout << "Referee notes: " << event->player_name 
                 << " is injured and may be substituted\n";
            injuries_noted++; });
    }

    int get_infractions() const { return infractions_recorded; }
    int get_injuries_noted() const { return injuries_noted; }
};

// ============================================================================
// Game - Coordinates and publishes game events
// ============================================================================
class Game
{
private:
    EventBroker &broker;

public:
    Game(EventBroker &broker)
        : broker(broker)
    {
    }

    void start(const string &team_a, const string &team_b)
    {
        GameStartedData event{team_a, team_b};
        broker.publish(&event);
    }

    void end(const string &winner, int a_score, int b_score)
    {
        GameEndedData event{winner, a_score, b_score};
        broker.publish(&event);
    }

    void give_card(const string &player_name, char card_type)
    {
        CardGivenData event{player_name, card_type};
        broker.publish(&event);
    }
};

// ============================================================================
// Utility function to demonstrate selective subscription
// ============================================================================
class InjuryReporter
{
private:
    int injuries_reported = 0;

public:
    InjuryReporter(EventBroker &broker)
    {
        // Only subscribes to injury events
        broker.subscribe<PlayerInjuredData>([this](const PlayerInjuredData *event)
                                            {
            cout << "INJURY ALERT: " << event->player_name 
                 << " - " << event->injury_type << "\n";
            injuries_reported++; });
    }

    int get_injury_count() const { return injuries_reported; }
};
