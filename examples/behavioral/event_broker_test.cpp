#include <iostream>
#include <cassert>
#include "../../include/behavioral/event_broker.h"

using namespace std;

void test_basic_event_publishing()
{
    cout << "Test: Basic event publishing... ";

    EventBroker broker;
    int score_count = 0;

    broker.subscribe<PlayerScoredData>([&score_count](const PlayerScoredData *event)
                                       { score_count++; });

    Player player{"Alice", broker};
    player.score();

    assert(score_count == 1);
    assert(player.get_goals() == 1);

    cout << "PASS\n";
}

void test_multiple_scores()
{
    cout << "Test: Multiple score events... ";

    EventBroker broker;
    int score_count = 0;

    broker.subscribe<PlayerScoredData>([&score_count](const PlayerScoredData *event)
                                       { score_count++; });

    Player player{"Bob", broker};
    player.score();
    player.score();
    player.score();

    assert(score_count == 3);
    assert(player.get_goals() == 3);

    cout << "PASS\n";
}

void test_multiple_publishers()
{
    cout << "Test: Multiple publishers... ";

    EventBroker broker;
    int score_count = 0;

    broker.subscribe<PlayerScoredData>([&score_count](const PlayerScoredData *event)
                                       { score_count++; });

    Player p1{"Alice", broker};
    Player p2{"Bob", broker};

    p1.score();
    p2.score();
    p1.score();

    assert(score_count == 3);

    cout << "PASS\n";
}

void test_multiple_subscribers()
{
    cout << "Test: Multiple subscribers... ";

    EventBroker broker;
    int count1 = 0;
    int count2 = 0;

    broker.subscribe<PlayerScoredData>([&count1](const PlayerScoredData *event)
                                       { count1++; });

    broker.subscribe<PlayerScoredData>([&count2](const PlayerScoredData *event)
                                       { count2++; });

    Player player{"Alice", broker};
    player.score();

    assert(count1 == 1);
    assert(count2 == 1);

    cout << "PASS\n";
}

void test_selective_event_handling()
{
    cout << "Test: Selective event handling... ";

    EventBroker broker;
    int score_count = 0;
    int injury_count = 0;

    broker.subscribe<PlayerScoredData>([&score_count](const PlayerScoredData *event)
                                       { score_count++; });

    broker.subscribe<PlayerInjuredData>([&injury_count](const PlayerInjuredData *event)
                                        { injury_count++; });

    Player player{"Alice", broker};
    player.score();
    player.score();
    player.get_injured("ankle");

    assert(score_count == 2);
    assert(injury_count == 1);

    cout << "PASS\n";
}

void test_coach_celebration_threshold()
{
    cout << "Test: Coach celebration threshold... ";

    EventBroker broker;
    Coach coach{"Coach", broker};

    Player player{"Alice", broker};
    player.score();
    assert(coach.get_celebrations() == 1);

    player.score();
    assert(coach.get_celebrations() == 2);

    player.score();
    assert(coach.get_celebrations() == 2); // No celebration for 3rd goal

    cout << "PASS\n";
}

void test_statistics_collector()
{
    cout << "Test: Statistics collector aggregation... ";

    EventBroker broker;
    StatisticsCollector stats{broker};

    Player p1{"Alice", broker};
    Player p2{"Bob", broker};

    p1.score();
    p2.score();
    p1.score();

    assert(stats.get_total_goals() == 3);

    cout << "PASS\n";
}

void test_independent_brokers()
{
    cout << "Test: Independent event brokers... ";

    EventBroker broker1;
    EventBroker broker2;

    int count1 = 0;
    int count2 = 0;

    broker1.subscribe<PlayerScoredData>([&count1](const PlayerScoredData *event)
                                        { count1++; });

    broker2.subscribe<PlayerScoredData>([&count2](const PlayerScoredData *event)
                                        { count2++; });

    Player p1{"Alice", broker1};
    Player p2{"Bob", broker2};

    p1.score();
    p2.score();

    assert(count1 == 1);
    assert(count2 == 1);

    cout << "PASS\n";
}

void test_game_event_publishing()
{
    cout << "Test: Game event publishing... ";

    EventBroker broker;
    int game_start_count = 0;
    int game_end_count = 0;

    broker.subscribe<GameStartedData>([&game_start_count](const GameStartedData *event)
                                      { game_start_count++; });

    broker.subscribe<GameEndedData>([&game_end_count](const GameEndedData *event)
                                    { game_end_count++; });

    Game game{broker};

    assert(game_start_count == 0);
    assert(game_end_count == 0);

    game.start("Team A", "Team B");
    assert(game_start_count == 1);

    game.end("Team A", 2, 1);
    assert(game_end_count == 1);

    cout << "PASS\n";
}

void test_card_events()
{
    cout << "Test: Card event publishing... ";

    EventBroker broker;
    StatisticsCollector stats{broker};

    Game game{broker};

    game.give_card("Player1", 'Y');
    assert(stats.get_yellow_cards() == 1);
    assert(stats.get_total_cards() == 1);

    game.give_card("Player2", 'R');
    assert(stats.get_red_cards() == 1);
    assert(stats.get_total_cards() == 2);

    cout << "PASS\n";
}

void test_injury_events()
{
    cout << "Test: Injury event publishing... ";

    EventBroker broker;
    StatisticsCollector stats{broker};

    Player player{"Alice", broker};

    assert(stats.get_injuries() == 0);

    player.get_injured("hamstring");
    assert(stats.get_injuries() == 1);

    player.get_injured("knee");
    assert(stats.get_injuries() == 2);

    cout << "PASS\n";
}

void test_coach_injury_concern()
{
    cout << "Test: Coach injury concern tracking... ";

    EventBroker broker;
    Coach coach{"Coach", broker};

    Player player{"Alice", broker};

    assert(coach.get_injury_reports() == 0);

    player.get_injured("ankle");
    assert(coach.get_injury_reports() == 1);

    cout << "PASS\n";
}

void test_commentator_event_tracking()
{
    cout << "Test: Commentator event tracking... ";

    EventBroker broker;
    Commentator commentator{"Commentator", broker};
    Game game{broker};

    // Commentator should track all event types
    assert(commentator.get_comment_count() == 0);

    game.start("Home", "Away");
    assert(commentator.get_comment_count() == 1);

    game.end("Home", 1, 0);
    assert(commentator.get_comment_count() == 2);

    cout << "PASS\n";
}

void test_injury_reporter()
{
    cout << "Test: Injury reporter selective subscription... ";

    EventBroker broker;
    InjuryReporter reporter{broker};

    Player player{"Alice", broker};

    assert(reporter.get_injury_count() == 0);

    player.score();
    player.score();
    assert(reporter.get_injury_count() == 0); // No injuries yet

    player.get_injured("back");
    assert(reporter.get_injury_count() == 1);

    cout << "PASS\n";
}

void test_referee_infraction_tracking()
{
    cout << "Test: Referee infraction tracking... ";

    EventBroker broker;
    Referee referee{broker};

    Game game{broker};

    assert(referee.get_infractions() == 0);
    assert(referee.get_injuries_noted() == 0);

    game.give_card("Player1", 'Y');
    assert(referee.get_infractions() == 1);
    assert(referee.get_injuries_noted() == 0);

    game.give_card("Player2", 'R');
    assert(referee.get_infractions() == 2);

    cout << "PASS\n";
}

void test_complex_multi_event_flow()
{
    cout << "Test: Complex multi-event flow... ";

    EventBroker broker;
    Coach coach{"Coach", broker};
    StatisticsCollector stats{broker};
    Referee referee{broker};
    Game game{broker};

    Player alice{"Alice", broker};
    Player bob{"Bob", broker};

    game.start("Team A", "Team B");
    alice.score();
    bob.score();
    alice.score();
    game.give_card("Charlie", 'Y');
    alice.get_injured("hamstring");
    game.end("Team A", 2, 1);

    assert(coach.get_celebrations() == 3); // Alice 1st, Bob 1st, Alice 2nd
    assert(stats.get_total_goals() == 3);
    assert(stats.get_total_cards() == 1);
    assert(stats.get_injuries() == 1);
    assert(referee.get_infractions() == 1);
    assert(referee.get_injuries_noted() == 1);

    cout << "PASS\n";
}

void test_event_type_count()
{
    cout << "Test: Event type count... ";

    EventBroker broker;

    assert(broker.event_type_count() == 0);

    broker.subscribe<PlayerScoredData>([](const PlayerScoredData *e) {});
    assert(broker.event_type_count() == 1);

    broker.subscribe<CardGivenData>([](const CardGivenData *e) {});
    assert(broker.event_type_count() == 2);

    broker.subscribe<PlayerInjuredData>([](const PlayerInjuredData *e) {});
    assert(broker.event_type_count() == 3);

    cout << "PASS\n";
}

void test_broker_clear()
{
    cout << "Test: Broker clear functionality... ";

    EventBroker broker;

    broker.subscribe<PlayerScoredData>([](const PlayerScoredData *e) {});
    broker.subscribe<CardGivenData>([](const CardGivenData *e) {});

    assert(broker.event_type_count() == 2);

    broker.clear();
    assert(broker.event_type_count() == 0);

    cout << "PASS\n";
}

void test_player_attributes()
{
    cout << "Test: Player attributes tracking... ";

    EventBroker broker;

    Player player{"Sam", broker};

    assert(player.get_name() == "Sam");
    assert(player.get_goals() == 0);

    player.score();
    assert(player.get_goals() == 1);

    player.score();
    assert(player.get_goals() == 2);

    cout << "PASS\n";
}

void test_multiple_coaches_independent()
{
    cout << "Test: Multiple coaches independent... ";

    EventBroker broker;
    Coach coach1{"Coach A", broker};
    Coach coach2{"Coach B", broker};

    Player player{"Player", broker};

    player.score();
    assert(coach1.get_celebrations() == 1);
    assert(coach2.get_celebrations() == 1);

    player.score();
    assert(coach1.get_celebrations() == 2);
    assert(coach2.get_celebrations() == 2);

    player.score();
    assert(coach1.get_celebrations() == 2); // No celebration
    assert(coach2.get_celebrations() == 2); // No celebration

    cout << "PASS\n";
}

void test_game_start_event_data()
{
    cout << "Test: Game start event data capture... ";

    EventBroker broker;

    string captured_team_a = "";
    string captured_team_b = "";

    broker.subscribe<GameStartedData>([&](const GameStartedData *event)
                                      {
        captured_team_a = event->team_a;
        captured_team_b = event->team_b; });

    Game game{broker};
    game.start("Home", "Away");

    assert(captured_team_a == "Home");
    assert(captured_team_b == "Away");

    cout << "PASS\n";
}

void test_game_end_event_data()
{
    cout << "Test: Game end event data capture... ";

    EventBroker broker;

    string winner = "";
    int score_a = -1;
    int score_b = -1;

    broker.subscribe<GameEndedData>([&](const GameEndedData *event)
                                    {
        winner = event->winner;
        score_a = event->team_a_score;
        score_b = event->team_b_score; });

    Game game{broker};
    game.end("Team A", 3, 1);

    assert(winner == "Team A");
    assert(score_a == 3);
    assert(score_b == 1);

    cout << "PASS\n";
}

void test_player_scored_event_data()
{
    cout << "Test: Player scored event data capture... ";

    EventBroker broker;

    string player_name = "";
    int goals = -1;

    broker.subscribe<PlayerScoredData>([&](const PlayerScoredData *event)
                                       {
        player_name = event->player_name;
        goals = event->goals_scored_so_far; });

    Player player{"Alice", broker};
    player.score();
    player.score();

    assert(player_name == "Alice");
    assert(goals == 2);

    cout << "PASS\n";
}

int main()
{
    cout << "========================================\n";
    cout << "  EVENT BROKER PATTERN TEST SUITE\n";
    cout << "========================================\n\n";

    test_basic_event_publishing();
    test_multiple_scores();
    test_multiple_publishers();
    test_multiple_subscribers();
    test_selective_event_handling();
    test_coach_celebration_threshold();
    test_statistics_collector();
    test_independent_brokers();
    test_game_event_publishing();
    test_card_events();
    test_injury_events();
    test_coach_injury_concern();
    test_commentator_event_tracking();
    test_injury_reporter();
    test_referee_infraction_tracking();
    test_complex_multi_event_flow();
    test_event_type_count();
    test_broker_clear();
    test_player_attributes();
    test_multiple_coaches_independent();
    test_game_start_event_data();
    test_game_end_event_data();
    test_player_scored_event_data();

    cout << "\n========================================\n";
    cout << "  ALL TESTS PASSED! (24 tests)\n";
    cout << "========================================\n";

    return 0;
}
