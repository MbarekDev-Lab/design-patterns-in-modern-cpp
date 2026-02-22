#include <iostream>
#include "../../include/behavioral/observer.h"

using namespace std;

void example_basic_score_notification()
{
    cout << "\n=== Example 1: Basic Score Notification ===\n";

    Game game;
    Coach coach{"John", game};

    game.player_scored("Sam", 1);
    game.player_scored("Sam", 2);
    game.player_scored("Sam", 3); // Coach won't celebrate this one
}

void example_multiple_observers()
{
    cout << "\n=== Example 2: Multiple Observers ===\n";

    Game game;
    Coach coach{"Alice", game};
    Commentator commentator{"Bob", game};

    game.player_scored("Emma", 1);
    game.player_scored("Emma", 2);
}

void example_statistics_collection()
{
    cout << "\n=== Example 3: Statistics Collection ===\n";

    Game game;
    StatisticsCollector stats{game};

    game.player_scored("Player1", 1);
    game.player_scored("Player2", 1);
    game.player_scored("Player1", 2);

    game.give_card("Player3", 'Y');
    game.give_card("Player4", 'R');

    cout << "\nStatistics:\n";
    cout << "Total goals: " << stats.get_total_goals() << "\n";
    cout << "Yellow cards: " << stats.get_yellow_cards() << "\n";
    cout << "Red cards: " << stats.get_red_cards() << "\n";
    cout << "Total cards: " << stats.get_total_cards() << "\n";
}

void example_game_lifecycle()
{
    cout << "\n=== Example 4: Complete Game Lifecycle ===\n";

    Game game;
    Coach coach{"Manager", game};
    Commentator commentator{"Announcer", game};
    Referee referee{game};

    game.start_game("Team A", "Team B");

    cout << "\n--- Scoring sequence ---\n";
    game.player_scored("Alice", 1);
    game.player_scored("Bob", 1);
    game.player_scored("Alice", 2);

    cout << "\n--- Cards ---\n";
    game.give_card("Charlie", 'Y');

    cout << "\n--- Game ends ---\n";
    game.end_game("Team A", 2, 1);
}

void example_observer_subscription()
{
    cout << "\n=== Example 5: Dynamic Observer Subscription ===\n";

    Game game;
    Coach coach1{"Coach A", game};

    cout << "Number of observers: " << game.observer_count() << "\n";
    cout << "Coach1 subscribed: " << game.has_observer(&coach1) << "\n";

    // Unsubscribe
    game.unsubscribe(&coach1);
    cout << "After unsubscribe - Number of observers: " << game.observer_count() << "\n";

    game.player_scored("Sam", 1); // Coach1 won't receive this

    // Resubscribe
    game.subscribe(&coach1);
    cout << "After resubscribe - Number of observers: " << game.observer_count() << "\n";
    game.player_scored("Sam", 2); // Coach1 will receive this
}

void example_multiple_coaches()
{
    cout << "\n=== Example 6: Multiple Coaches Monitoring ===\n";

    Game game;
    Coach coach_a{"Coach A (optimistic)", game};
    Coach coach_b{"Coach B (pragmatic)", game};

    cout << "Coach A celebrations: " << coach_a.get_celebrations() << "\n";
    cout << "Coach B celebrations: " << coach_b.get_celebrations() << "\n";

    game.player_scored("Player1", 1);
    game.player_scored("Player1", 2);
    game.player_scored("Player1", 3);

    cout << "\nAfter scoring:\n";
    cout << "Coach A celebrated: " << coach_a.get_celebrations() << " times\n";
    cout << "Coach B celebrated: " << coach_b.get_celebrations() << " times\n";
}

void example_multiple_games()
{
    cout << "\n=== Example 7: Multiple Independent Games ===\n";

    Game game1;
    Game game2;

    Coach coach1{"Coach 1", game1};
    Coach coach2{"Coach 2", game2};
    Commentator commentator1{"Commentator 1", game1};
    Commentator commentator2{"Commentator 2", game2};

    cout << "--- Game 1 Activity ---\n";
    game1.player_scored("Alice", 1);

    cout << "\n--- Game 2 Activity ---\n";
    game2.player_scored("Bob", 1);

    cout << "\nCommentator stats:\n";
    cout << "Commentator 1 made " << commentator1.get_comment_count() << " comments\n";
    cout << "Commentator 2 made " << commentator2.get_comment_count() << " comments\n";
}

void example_card_events()
{
    cout << "\n=== Example 8: Card Event Handling ===\n";

    Game game;
    Coach coach{"Coach", game};
    Referee referee{game};

    game.give_card("Player1", 'Y');
    game.give_card("Player2", 'Y');
    game.give_card("Player3", 'R');

    cout << "\nReferee recorded " << referee.get_infractions() << " infractions\n";
    cout << "Coach gave " << coach.get_warnings() << " warnings\n";
}

void example_selective_observer_filtering()
{
    cout << "\n=== Example 9: Selective Event Processing ===\n";

    // Custom observer that only cares about player 'Alice'
    class AliceFan : public Observer
    {
    public:
        void on_event(EventData *event) override
        {
            auto *scored = dynamic_cast<PlayerScoredData *>(event);
            if (scored && scored->player_name == "Alice")
            {
                cout << "AliceFan: WOW! Alice scored! That's "
                     << scored->goals_scored_so_far << " goals!\n";
            }
        }
    };

    Game game;
    AliceFan alice_fan;
    game.subscribe(&alice_fan);

    game.player_scored("Alice", 1);
    game.player_scored("Bob", 1);
    game.player_scored("Alice", 2);
}

void example_observer_count_tracking()
{
    cout << "\n=== Example 10: Observer Count Tracking ===\n";

    Game game;

    cout << "Initial observers: " << game.observer_count() << "\n";

    Coach coach1{"Coach 1", game};
    cout << "After adding coach1: " << game.observer_count() << "\n";

    Coach coach2{"Coach 2", game};
    cout << "After adding coach2: " << game.observer_count() << "\n";

    Commentator commentator{"Commentator", game};
    cout << "After adding commentator: " << game.observer_count() << "\n";

    StatisticsCollector stats{game};
    cout << "After adding stats collector: " << game.observer_count() << "\n";
}

int main()
{
    cout << "========================================\n";
    cout << "  OBSERVER PATTERN EXAMPLES\n";
    cout << "  Event-Driven Architecture\n";
    cout << "========================================\n";

    example_basic_score_notification();
    example_multiple_observers();
    example_statistics_collection();
    example_game_lifecycle();
    example_observer_subscription();
    example_multiple_coaches();
    example_multiple_games();
    example_card_events();
    example_selective_observer_filtering();
    example_observer_count_tracking();

    cout << "\n========================================\n";

    return 0;
}
