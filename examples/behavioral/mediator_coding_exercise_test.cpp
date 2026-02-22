#include <iostream>
#include <cassert>
#include "../../include/behavioral/mediator_coding_exercise.h"

using namespace std;

void test_initial_values()
{
    cout << "Test: Initial values are zero... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    assert(p1.get_value() == 0);
    assert(p2.get_value() == 0);

    cout << "PASS\n";
}

void test_basic_broadcast()
{
    cout << "Test: Basic broadcast to one participant... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(3);

    assert(p1.get_value() == 0);
    assert(p2.get_value() == 3);

    cout << "PASS\n";
}

void test_second_broadcast()
{
    cout << "Test: Second broadcast from different participant... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(3);
    p2.say(2);

    assert(p1.get_value() == 2);
    assert(p2.get_value() == 3);

    cout << "PASS\n";
}

void test_three_participants()
{
    cout << "Test: Three participants... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};
    Participant p3{mediator};

    p1.say(5);
    assert(p1.get_value() == 0);
    assert(p2.get_value() == 5);
    assert(p3.get_value() == 5);

    p2.say(3);
    assert(p1.get_value() == 3);
    assert(p2.get_value() == 5);
    assert(p3.get_value() == 8);

    cout << "PASS\n";
}

void test_four_participants()
{
    cout << "Test: Four participants... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};
    Participant p3{mediator};
    Participant p4{mediator};

    p1.say(1);
    assert(p1.get_value() == 0);
    assert(p2.get_value() == 1);
    assert(p3.get_value() == 1);
    assert(p4.get_value() == 1);

    p2.say(1);
    assert(p1.get_value() == 1);
    assert(p2.get_value() == 1);
    assert(p3.get_value() == 2);
    assert(p4.get_value() == 2);

    p3.say(1);
    assert(p1.get_value() == 2);
    assert(p2.get_value() == 2);
    assert(p3.get_value() == 2);
    assert(p4.get_value() == 3);

    p4.say(1);
    assert(p1.get_value() == 3);
    assert(p2.get_value() == 3);
    assert(p3.get_value() == 3);
    assert(p4.get_value() == 3);

    cout << "PASS\n";
}

void test_multiple_broadcasts_same_participant()
{
    cout << "Test: Multiple broadcasts from same participant... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(5);
    assert(p2.get_value() == 5);

    p1.say(3);
    assert(p2.get_value() == 8);

    p1.say(2);
    assert(p2.get_value() == 10);

    cout << "PASS\n";
}

void test_zero_broadcast()
{
    cout << "Test: Zero broadcast... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(5);
    assert(p2.get_value() == 5);

    p2.say(0);
    assert(p1.get_value() == 0);
    assert(p2.get_value() == 5);

    cout << "PASS\n";
}

void test_negative_broadcast()
{
    cout << "Test: Negative value broadcast... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(10);
    assert(p2.get_value() == 10);

    p2.say(-3);
    assert(p1.get_value() == -3);

    cout << "PASS\n";
}

void test_multiple_negatives()
{
    cout << "Test: Multiple negative broadcasts... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(10);
    p2.say(-3);
    p1.say(-2);

    assert(p1.get_value() == -3);
    assert(p2.get_value() == 5);

    cout << "PASS\n";
}

void test_large_values()
{
    cout << "Test: Large values... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(1000000);
    assert(p2.get_value() == 1000000);

    p2.say(500000);
    assert(p1.get_value() == 500000);

    cout << "PASS\n";
}

void test_single_participant()
{
    cout << "Test: Single participant... ";

    Mediator mediator;
    Participant p1{mediator};

    p1.say(5);
    // Single participant broadcasts to nobody
    assert(p1.get_value() == 0);

    cout << "PASS\n";
}

void test_participant_count()
{
    cout << "Test: Participant count... ";

    Mediator mediator;

    assert(mediator.get_participant_count() == 0);

    Participant p1{mediator};
    assert(mediator.get_participant_count() == 1);

    Participant p2{mediator};
    assert(mediator.get_participant_count() == 2);

    Participant p3{mediator};
    assert(mediator.get_participant_count() == 3);

    cout << "PASS\n";
}

void test_set_value()
{
    cout << "Test: Manual value setting... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.set_value(10);
    p2.set_value(20);

    assert(p1.get_value() == 10);
    assert(p2.get_value() == 20);

    p1.say(5);
    assert(p1.get_value() == 10);
    assert(p2.get_value() == 25);

    cout << "PASS\n";
}

void test_complex_scenario()
{
    cout << "Test: Complex multi-participant scenario... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};
    Participant p3{mediator};
    Participant p4{mediator};
    Participant p5{mediator};

    p1.say(10);
    assert(p2.get_value() == 10 && p3.get_value() == 10 &&
           p4.get_value() == 10 && p5.get_value() == 10);

    p2.say(5);
    assert(p1.get_value() == 5 && p3.get_value() == 15 &&
           p4.get_value() == 15 && p5.get_value() == 15);

    p3.say(2);
    assert(p1.get_value() == 7 && p2.get_value() == 7 &&
           p4.get_value() == 17 && p5.get_value() == 17);

    cout << "PASS\n";
}

void test_sequence_example_from_problem()
{
    cout << "Test: Sequence from problem description... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    // Initial: P1=0, P2=0
    assert(p1.get_value() == 0 && p2.get_value() == 0);

    // P1 broadcasts 3
    p1.say(3);
    // P1=0, P2=3
    assert(p1.get_value() == 0 && p2.get_value() == 3);

    // P2 broadcasts 2
    p2.say(2);
    // P1=2, P2=3
    assert(p1.get_value() == 2 && p2.get_value() == 3);

    cout << "PASS\n";
}

void test_alternating_broadcasts()
{
    cout << "Test: Alternating broadcasts... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(1);
    p2.say(1);
    p1.say(1);
    p2.say(1);
    p1.say(1);

    assert(p1.get_value() == 4);
    assert(p2.get_value() == 5);

    cout << "PASS\n";
}

void test_broadcast_accumulation()
{
    cout << "Test: Broadcast accumulation... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};
    Participant p3{mediator};

    // Each participant broadcasts to accumulate
    p1.say(1); // p2=1, p3=1
    p2.say(1); // p1=1, p3=2
    p3.say(1); // p1=2, p2=2

    assert(p1.get_value() == 2);
    assert(p2.get_value() == 2);
    assert(p3.get_value() == 2);

    cout << "PASS\n";
}

void test_mixed_positive_negative()
{
    cout << "Test: Mixed positive and negative broadcasts... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};
    Participant p3{mediator};

    p1.say(10);
    p2.say(-3);
    p3.say(5);
    p1.say(-2);

    assert(p1.get_value() == 5);
    assert(p2.get_value() == 15);
    assert(p3.get_value() == 10);

    cout << "PASS\n";
}

void test_mediator_clear()
{
    cout << "Test: Mediator clear... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    assert(mediator.get_participant_count() == 2);

    mediator.clear();

    assert(mediator.get_participant_count() == 0);

    cout << "PASS\n";
}

void test_new_participants_after_broadcasts()
{
    cout << "Test: Adding new participant after broadcasts... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(5);
    assert(p2.get_value() == 5);

    Participant p3{mediator};
    assert(p3.get_value() == 0);

    p1.say(3);
    assert(p2.get_value() == 8);
    assert(p3.get_value() == 3);

    cout << "PASS\n";
}

void test_same_values_multiple_sources()
{
    cout << "Test: Same broadcast value from multiple sources... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};
    Participant p3{mediator};

    p1.say(5);
    p2.say(5);
    p3.say(5);

    // After p1.say(5): p2=5, p3=5
    // After p2.say(5): p1=5, p3=10
    // After p3.say(5): p1=10, p2=10

    assert(p1.get_value() == 10);
    assert(p2.get_value() == 10);
    assert(p3.get_value() == 10);

    cout << "PASS\n";
}

void test_cascading_effect()
{
    cout << "Test: Cascading effect... ";

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};
    Participant p3{mediator};

    // p1 broadcasts, then p2 and p3 both broadcast
    p1.say(2);
    p2.say(1);
    p3.say(1);

    assert(p1.get_value() == 2);
    assert(p2.get_value() == 2);
    assert(p3.get_value() == 2);

    cout << "PASS\n";
}

int main()
{
    cout << "========================================\n";
    cout << "  MEDIATOR CODING EXERCISE TEST SUITE\n";
    cout << "========================================\n\n";

    test_initial_values();
    test_basic_broadcast();
    test_second_broadcast();
    test_three_participants();
    test_four_participants();
    test_multiple_broadcasts_same_participant();
    test_zero_broadcast();
    test_negative_broadcast();
    test_multiple_negatives();
    test_large_values();
    test_single_participant();
    test_participant_count();
    test_set_value();
    test_complex_scenario();
    test_sequence_example_from_problem();
    test_alternating_broadcasts();
    test_broadcast_accumulation();
    test_mixed_positive_negative();
    test_mediator_clear();
    test_new_participants_after_broadcasts();
    test_same_values_multiple_sources();
    test_cascading_effect();

    cout << "\n========================================\n";
    cout << "  ALL TESTS PASSED! (23 tests)\n";
    cout << "========================================\n";

    return 0;
}
