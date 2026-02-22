#include <iostream>
#include "../../include/behavioral/event_broker.h"

using namespace std;

void example_basic_event_publishing()
{
    cout << "\n=== Example 1: Basic Event Publishing ===\n";

    EventBroker broker;
    Coach coach{"John", broker};

    Player player{"Sam", broker};
    player.score();
    player.score();
    player.score(); // Coach won't celebrate this one
}

void example_multiple_subscribers()
{
    cout << "\n=== Example 2: Multiple Subscribers to Same Event ===\n";

    EventBroker broker;
    Coach coach{"Alice", broker};
    Commentator commentator{"Bob", broker};

    Player player{"Emma", broker};
    player.score();
    player.score();
}

void example_event_aggregation()
{
    cout << "\n=== Example 3: Event Aggregation ===\n";

    EventBroker broker;
    StatisticsCollector stats{broker};

    Player p1{"Player1", broker};
    Player p2{"Player2", broker};

    p1.score();
    p2.score();
    p1.score();

    cout << "\nStatistics:\n";
    cout << "Total goals: " << stats.get_total_goals() << "\n";
}

void example_selective_subscription()
{
    cout << "\n=== Example 4: Selective Event Subscription ===\n";

    EventBroker broker;
    InjuryReporter injury_reporter{broker};
    StatisticsCollector stats{broker};

    Player player{"Alice", broker};

    player.score();
    player.score();
    cout << "Injury reporter has " << injury_reporter.get_injury_count() << " injuries\n";

    player.get_injured("hamstring");
    cout << "Injury reporter has " << injury_reporter.get_injury_count() << " injuries\n";
}

void example_game_lifecycle()
{
    cout << "\n=== Example 5: Complete Game Lifecycle ===\n";

    EventBroker broker;
    Game game{broker};
    Coach coach{"Manager", broker};
    Commentator commentator{"Announcer", broker};
    Referee referee{broker};

    game.start("Team A", "Team B");

    Player alice{"Alice", broker};
    Player bob{"Bob", broker};

    cout << "\n--- Scoring ---\n";
    alice.score();
    bob.score();

    cout << "\n--- Card ---\n";
    game.give_card("Charlie", 'Y');

    cout << "\n--- Injury ---\n";
    alice.get_injured("ankle");

    cout << "\n--- Game ends ---\n";
    game.end("Team A", 2, 1);
}

void example_independent_events()
{
    cout << "\n=== Example 6: Independent Event Brokers ===\n";

    EventBroker broker1;
    EventBroker broker2;

    Coach coach1{"Coach 1", broker1};
    Coach coach2{"Coach 2", broker2};

    Player p1{"Alice", broker1};
    Player p2{"Bob", broker2};

    cout << "--- Broker 1 Events ---\n";
    p1.score();

    cout << "\n--- Broker 2 Events ---\n";
    p2.score();
}

void example_complex_scenario()
{
    cout << "\n=== Example 7: Complex Multi-Event Scenario ===\n";

    EventBroker broker;

    // Multiple subscribers
    Coach coach{"Coach", broker};
    Commentator commentator{"Commentator", broker};
    StatisticsCollector stats{broker};
    Referee referee{broker};
    InjuryReporter injury_reporter{broker};

    Game game{broker};
    Player alice{"Alice", broker};
    Player bob{"Bob", broker};
    Player charlie{"Charlie", broker};

    game.start("Team A", "Team B");

    cout << "\n--- First half ---\n";
    alice.score();
    bob.score();
    game.give_card("Charlie", 'Y');

    cout << "\n--- Second half ---\n";
    alice.score();

    cout << "\n--- Injury ---\n";
    bob.get_injured("knee");

    cout << "\n--- Game ends ---\n";
    game.end("Team A", 3, 1);

    cout << "\nFinal Statistics:\n";
    cout << "Goals: " << stats.get_total_goals() << "\n";
    cout << "Cards: " << stats.get_total_cards() << "\n";
    cout << "Injuries: " << stats.get_injuries() << "\n";
}

void example_event_type_isolation()
{
    cout << "\n=== Example 8: Event Type Isolation ===\n";

    EventBroker broker;

    // Subscriber that only cares about injuries
    int injury_count = 0;
    broker.subscribe<PlayerInjuredData>([&injury_count](const PlayerInjuredData *event)
                                        {
        cout << "Injury event: " << event->player_name << "\n";
        injury_count++; });

    // Subscriber that only cares about goals
    int goal_count = 0;
    broker.subscribe<PlayerScoredData>([&goal_count](const PlayerScoredData *event)
                                       {
        cout << "Goal event: " << event->player_name << "\n";
        goal_count++; });

    Player player{"Sam", broker};

    cout << "--- Scoring ---\n";
    player.score();
    cout << "Goals: " << goal_count << ", Injuries: " << injury_count << "\n";

    cout << "\n--- Injury ---\n";
    player.get_injured("muscle");
    cout << "Goals: " << goal_count << ", Injuries: " << injury_count << "\n";
}

void example_broker_event_counts()
{
    cout << "\n=== Example 9: Event Type Tracking ===\n";

    EventBroker broker;

    cout << "Initial event types: " << broker.event_type_count() << "\n";

    Coach coach{"Coach", broker};
    cout << "After coach subscription: " << broker.event_type_count() << "\n";

    Commentator commentator{"Commentator", broker};
    cout << "After commentator subscription: " << broker.event_type_count() << "\n";

    Referee referee{broker};
    cout << "After referee subscription: " << broker.event_type_count() << "\n";

    InjuryReporter reporter{broker};
    cout << "After injury reporter subscription: " << broker.event_type_count() << "\n";
}

void example_multiple_events_per_publisher()
{
    cout << "\n=== Example 10: Multiple Event Types from Single Publisher ===\n";

    EventBroker broker;
    Commentator commentator{"Commentator", broker};
    StatisticsCollector stats{broker};

    Player player{"Alice", broker};
    Game game{broker};

    game.start("Home", "Away");
    player.score();
    game.give_card("Bob", 'R');
    player.get_injured("back");
    game.end("Home", 1, 0);

    cout << "\nComments made: " << commentator.get_comment_count() << "\n";
    cout << "Goals recorded: " << stats.get_total_goals() << "\n";
    cout << "Cards recorded: " << stats.get_total_cards() << "\n";
    cout << "Injuries recorded: " << stats.get_injuries() << "\n";
}

int main()
{
    cout << "========================================\n";
    cout << "  EVENT BROKER PATTERN EXAMPLES\n";
    cout << "  Centralized Event Bus Architecture\n";
    cout << "========================================\n";

    example_basic_event_publishing();
    example_multiple_subscribers();
    example_event_aggregation();
    example_selective_subscription();
    example_game_lifecycle();
    example_independent_events();
    example_complex_scenario();
    example_event_type_isolation();
    example_broker_event_counts();
    example_multiple_events_per_publisher();

    cout << "\n========================================\n";

    return 0;
}
