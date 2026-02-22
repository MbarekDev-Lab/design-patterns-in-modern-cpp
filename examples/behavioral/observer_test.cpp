#include <iostream>
#include <cassert>
#include "../../include/behavioral/observer.h"

using namespace std;

void test_observer_subscription()
{
    cout << "Test: Observer subscription... ";

    Game game;
    Coach coach{"Coach", game};

    assert(game.observer_count() == 1);
    assert(game.has_observer(&coach));

    cout << "PASS\n";
}

void test_observer_unsubscription()
{
    cout << "Test: Observer unsubscription... ";

    Game game;
    Coach coach{"Coach", game};

    assert(game.observer_count() == 1);

    game.unsubscribe(&coach);
    assert(game.observer_count() == 0);
    assert(!game.has_observer(&coach));

    cout << "PASS\n";
}

void test_multiple_observers()
{
    cout << "Test: Multiple observers... ";

    Game game;
    Coach coach{"Coach", game};
    Commentator commentator{"Commentator", game};
    StatisticsCollector stats{game};

    assert(game.observer_count() == 3);

    cout << "PASS\n";
}

void test_player_score_notification()
{
    cout << "Test: Player score notification... ";

    Game game;
    StatisticsCollector stats{game};

    game.player_scored("Player1", 1);
    assert(stats.get_total_goals() == 1);

    game.player_scored("Player2", 1);
    assert(stats.get_total_goals() == 2);

    cout << "PASS\n";
}

void test_card_notification()
{
    cout << "Test: Card notification... ";

    Game game;
    StatisticsCollector stats{game};

    game.give_card("Player1", 'Y');
    assert(stats.get_yellow_cards() == 1);
    assert(stats.get_total_cards() == 1);

    game.give_card("Player2", 'R');
    assert(stats.get_red_cards() == 1);
    assert(stats.get_total_cards() == 2);

    cout << "PASS\n";
}

void test_coach_celebration_threshold()
{
    cout << "Test: Coach celebration threshold... ";

    Game game;
    Coach coach{"Coach", game};

    game.player_scored("Player1", 1);
    assert(coach.get_celebrations() == 1);

    game.player_scored("Player1", 2);
    assert(coach.get_celebrations() == 2);

    game.player_scored("Player1", 3);
    assert(coach.get_celebrations() == 2); // No celebration for 3rd goal

    game.player_scored("Player1", 4);
    assert(coach.get_celebrations() == 2); // Still no celebration

    cout << "PASS\n";
}

void test_commentator_comment_count()
{
    cout << "Test: Commentator comment count... ";

    Game game;
    Commentator commentator{"Commentator", game};

    assert(commentator.get_comment_count() == 0);

    game.player_scored("Player1", 1);
    assert(commentator.get_comment_count() == 1);

    game.give_card("Player2", 'Y');
    assert(commentator.get_comment_count() == 2);

    game.start_game("Team A", "Team B");
    assert(commentator.get_comment_count() == 3);

    cout << "PASS\n";
}

void test_game_start_event()
{
    cout << "Test: Game start event... ";

    Game game;

    // Custom observer to capture event
    bool event_received = false;
    class GameStartObserver : public Observer
    {
    public:
        bool *flag;
        GameStartObserver(bool *f) : flag(f) {}
        void on_event(EventData *event) override
        {
            auto *gs = dynamic_cast<GameStartedData *>(event);
            if (gs)
                *flag = true;
        }
    };

    GameStartObserver observer{&event_received};
    game.subscribe(&observer);

    game.start_game("Team A", "Team B");
    assert(event_received);

    cout << "PASS\n";
}

void test_game_end_event()
{
    cout << "Test: Game end event... ";

    Game game;
    bool event_received = false;

    class GameEndObserver : public Observer
    {
    public:
        bool *flag;
        GameEndObserver(bool *f) : flag(f) {}
        void on_event(EventData *event) override
        {
            auto *ge = dynamic_cast<GameEndedData *>(event);
            if (ge && ge->team_a_score == 2 && ge->team_b_score == 1)
                *flag = true;
        }
    };

    GameEndObserver observer{&event_received};
    game.subscribe(&observer);

    game.end_game("Team A", 2, 1);
    assert(event_received);

    cout << "PASS\n";
}

void test_referee_infraction_tracking()
{
    cout << "Test: Referee infraction tracking... ";

    Game game;
    Referee referee{game};

    assert(referee.get_infractions() == 0);

    game.give_card("Player1", 'Y');
    assert(referee.get_infractions() == 1);

    game.give_card("Player2", 'R');
    assert(referee.get_infractions() == 2);

    game.player_scored("Player3", 1); // Scoring is not an infraction
    assert(referee.get_infractions() == 2);

    cout << "PASS\n";
}

void test_statistics_collector_aggregation()
{
    cout << "Test: Statistics collector aggregation... ";

    Game game;
    StatisticsCollector stats{game};

    game.player_scored("P1", 1);
    game.player_scored("P2", 1);
    game.player_scored("P3", 1);
    game.give_card("P4", 'Y');
    game.give_card("P5", 'Y');
    game.give_card("P6", 'R');

    assert(stats.get_total_goals() == 3);
    assert(stats.get_yellow_cards() == 2);
    assert(stats.get_red_cards() == 1);
    assert(stats.get_total_cards() == 3);

    cout << "PASS\n";
}

void test_unsubscribe_stops_notifications()
{
    cout << "Test: Unsubscribe stops notifications... ";

    Game game;
    StatisticsCollector stats{game};

    game.player_scored("P1", 1);
    assert(stats.get_total_goals() == 1);

    game.unsubscribe(&stats);

    game.player_scored("P2", 1);
    assert(stats.get_total_goals() == 1); // Still 1, no notification received

    cout << "PASS\n";
}

void test_resubscribe_resumes_notifications()
{
    cout << "Test: Resubscribe resumes notifications... ";

    Game game;
    StatisticsCollector stats{game};

    game.player_scored("P1", 1);
    assert(stats.get_total_goals() == 1);

    game.unsubscribe(&stats);
    game.player_scored("P2", 1);
    assert(stats.get_total_goals() == 1);

    game.subscribe(&stats);
    game.player_scored("P3", 1);
    assert(stats.get_total_goals() == 2);

    cout << "PASS\n";
}

void test_player_class_with_observers()
{
    cout << "Test: Player class with observers... ";

    Game game;
    Player player{"Alice", game};
    StatisticsCollector stats{game};

    player.score();
    assert(player.get_goals() == 1);

    player.score();
    assert(player.get_goals() == 2);

    assert(stats.get_total_goals() == 2);

    cout << "PASS\n";
}

void test_player_receives_card()
{
    cout << "Test: Player receives card... ";

    Game game;
    Player player{"Bob", game};
    StatisticsCollector stats{game};

    player.receive_card('Y');
    assert(stats.get_yellow_cards() == 1);

    player.receive_card('R');
    assert(stats.get_red_cards() == 1);

    cout << "PASS\n";
}

void test_multiple_independent_games()
{
    cout << "Test: Multiple independent games... ";

    Game game1;
    Game game2;

    StatisticsCollector stats1{game1};
    StatisticsCollector stats2{game2};

    game1.player_scored("P1", 1);
    game1.player_scored("P2", 1);
    game2.player_scored("P3", 1);

    assert(stats1.get_total_goals() == 2);
    assert(stats2.get_total_goals() == 1);

    cout << "PASS\n";
}

void test_observer_count_after_destructor()
{
    cout << "Test: Observer count after destructor... ";

    Game game;

    {
        Coach coach{"Coach", game};
        assert(game.observer_count() == 1);
    } // Coach destructor removes observer

    assert(game.observer_count() == 0);

    cout << "PASS\n";
}

void test_coach_warning_on_red_card()
{
    cout << "Test: Coach warning on red card... ";

    Game game;
    Coach coach{"Coach", game};

    assert(coach.get_warnings() == 0);

    game.give_card("Player1", 'Y'); // Yellow card, no warning
    assert(coach.get_warnings() == 0);

    game.give_card("Player2", 'R'); // Red card, warning issued
    assert(coach.get_warnings() == 1);

    cout << "PASS\n";
}

void test_no_duplicate_observers()
{
    cout << "Test: No duplicate observers... ";

    Game game;
    Coach coach{"Coach", game};

    assert(game.observer_count() == 1);

    // Try to subscribe same observer again
    game.subscribe(&coach);
    assert(game.observer_count() == 1); // Still 1, no duplicate

    cout << "PASS\n";
}

void test_unsubscribe_null_observer()
{
    cout << "Test: Unsubscribe null observer... ";

    Game game;
    Coach coach{"Coach", game};

    assert(game.observer_count() == 1);

    game.unsubscribe(nullptr); // Should not crash
    assert(game.observer_count() == 1);

    cout << "PASS\n";
}

void test_subscribe_null_observer()
{
    cout << "Test: Subscribe null observer... ";

    Game game;

    game.subscribe(nullptr); // Should not crash
    assert(game.observer_count() == 0);

    cout << "PASS\n";
}

void test_multiple_coaches_independent()
{
    cout << "Test: Multiple coaches independent... ";

    Game game;
    Coach coach1{"Coach1", game};
    Coach coach2{"Coach2", game};

    game.player_scored("P1", 1);
    assert(coach1.get_celebrations() == 1);
    assert(coach2.get_celebrations() == 1);

    game.player_scored("P1", 2);
    assert(coach1.get_celebrations() == 2);
    assert(coach2.get_celebrations() == 2);

    game.player_scored("P1", 3); // No celebration
    assert(coach1.get_celebrations() == 2);
    assert(coach2.get_celebrations() == 2);

    cout << "PASS\n";
}

void test_event_data_polymorphism()
{
    cout << "Test: Event data polymorphism... ";

    int event_count = 0;

    class CountingObserver : public Observer
    {
    public:
        int *count;
        CountingObserver(int *c) : count(c) {}
        void on_event(EventData *event) override
        {
            (*count)++;
        }
    };

    Game game;
    CountingObserver counter{&event_count};
    game.subscribe(&counter);

    game.start_game("A", "B");
    assert(event_count == 1);

    game.player_scored("P", 1);
    assert(event_count == 2);

    game.give_card("P", 'Y');
    assert(event_count == 3);

    game.end_game("A", 1, 0);
    assert(event_count == 4);

    cout << "PASS\n";
}

void test_game_lifecycle_with_observers()
{
    cout << "Test: Game lifecycle with observers... ";

    Game game;
    StatisticsCollector stats{game};

    game.start_game("Team A", "Team B");
    game.player_scored("P1", 1);
    game.player_scored("P2", 1);
    game.give_card("P3", 'Y');
    game.end_game("Team A", 2, 1);

    assert(stats.get_total_goals() == 2);
    assert(stats.get_yellow_cards() == 1);

    cout << "PASS\n";
}

void test_complex_scenario()
{
    cout << "Test: Complex scenario with multiple observers... ";

    Game game;
    Coach coach{"Coach", game};
    Commentator commentator{"Commentator", game};
    StatisticsCollector stats{game};
    Referee referee{game};

    assert(game.observer_count() == 4);

    game.start_game("Home", "Away");
    game.player_scored("P1", 1);
    game.player_scored("P2", 1);
    game.give_card("P3", 'Y');
    game.give_card("P4", 'R');
    game.player_scored("P1", 2);
    game.end_game("Home", 3, 1);

    assert(stats.get_total_goals() == 3);
    assert(stats.get_yellow_cards() == 1);
    assert(stats.get_red_cards() == 1);
    assert(coach.get_celebrations() == 3);        // P1's 1st, P2's 1st, P1's 2nd (all < 3)
    assert(commentator.get_comment_count() == 7); // 7 events total
    assert(referee.get_infractions() == 2);

    cout << "PASS\n";
}

int main()
{
    cout << "========================================\n";
    cout << "  OBSERVER PATTERN TEST SUITE\n";
    cout << "========================================\n\n";

    test_observer_subscription();
    test_observer_unsubscription();
    test_multiple_observers();
    test_player_score_notification();
    test_card_notification();
    test_coach_celebration_threshold();
    test_commentator_comment_count();
    test_game_start_event();
    test_game_end_event();
    test_referee_infraction_tracking();
    test_statistics_collector_aggregation();
    test_unsubscribe_stops_notifications();
    test_resubscribe_resumes_notifications();
    test_player_class_with_observers();
    test_player_receives_card();
    test_multiple_independent_games();
    test_observer_count_after_destructor();
    test_coach_warning_on_red_card();
    test_no_duplicate_observers();
    test_unsubscribe_null_observer();
    test_subscribe_null_observer();
    test_multiple_coaches_independent();
    test_event_data_polymorphism();
    test_game_lifecycle_with_observers();
    test_complex_scenario();

    cout << "\n========================================\n";
    cout << "  ALL TESTS PASSED! (25 tests)\n";
    cout << "========================================\n";

    return 0;
}
