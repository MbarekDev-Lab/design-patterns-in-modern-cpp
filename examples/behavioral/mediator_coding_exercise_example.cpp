#include <iostream>
#include "../../include/behavioral/mediator_coding_exercise.h"

using namespace std;

void example_basic_broadcast()
{
     cout << "\n=== Example 1: Basic Two-Participant Broadcast ===\n";

     Mediator mediator;
     Participant p1{mediator};
     Participant p2{mediator};

     cout << "Initial values:\n";
     cout << "P1: " << p1.get_value() << "\n";
     cout << "P2: " << p2.get_value() << "\n";

     cout << "\nP1 broadcasts 3:\n";
     p1.say(3);
     cout << "P1: " << p1.get_value() << "\n";
     cout << "P2: " << p2.get_value() << "\n";

     cout << "\nP2 broadcasts 2:\n";
     p2.say(2);
     cout << "P1: " << p1.get_value() << "\n";
     cout << "P2: " << p2.get_value() << "\n";
}

void example_three_participants()
{
     cout << "\n=== Example 2: Three Participants ===\n";

     Mediator mediator;
     Participant p1{mediator};
     Participant p2{mediator};
     Participant p3{mediator};

     cout << "Initial: P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << "\n";

     cout << "\nP1 broadcasts 5:\n";
     p1.say(5);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << "\n";

     cout << "\nP2 broadcasts 3:\n";
     p2.say(3);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << "\n";

     cout << "\nP3 broadcasts 2:\n";
     p3.say(2);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << "\n";
}

void example_multiple_broadcasts()
{
     cout << "\n=== Example 3: Multiple Consecutive Broadcasts ===\n";

     Mediator mediator;
     Participant p1{mediator};
     Participant p2{mediator};

     cout << "Initial: P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";

     cout << "\nP1 broadcasts 10:\n";
     p1.say(10);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";

     cout << "\nP1 broadcasts again 5:\n";
     p1.say(5);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";

     cout << "\nP2 broadcasts 7:\n";
     p2.say(7);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";
}

void example_chain_reactions()
{
     cout << "\n=== Example 4: Chain Reactions ===\n";

     Mediator mediator;
     Participant p1{mediator};
     Participant p2{mediator};
     Participant p3{mediator};
     Participant p4{mediator};

     cout << "Initial: P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << ", P4=" << p4.get_value() << "\n";

     cout << "\nP1 broadcasts 1:\n";
     p1.say(1);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << ", P4=" << p4.get_value() << "\n";

     cout << "\nP2 broadcasts 1:\n";
     p2.say(1);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << ", P4=" << p4.get_value() << "\n";

     cout << "\nP3 broadcasts 1:\n";
     p3.say(1);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << ", P4=" << p4.get_value() << "\n";

     cout << "\nP4 broadcasts 1:\n";
     p4.say(1);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << ", P4=" << p4.get_value() << "\n";
}

void example_zero_broadcast()
{
     cout << "\n=== Example 5: Zero Broadcast ===\n";

     Mediator mediator;
     Participant p1{mediator};
     Participant p2{mediator};

     p1.say(5);
     cout << "After P1 broadcasts 5: P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";

     cout << "\nP2 broadcasts 0:\n";
     p2.say(0);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";
}

void example_negative_values()
{
     cout << "\n=== Example 6: Negative Value Broadcast ===\n";

     Mediator mediator;
     Participant p1{mediator};
     Participant p2{mediator};
     Participant p3{mediator};

     p1.say(10);
     cout << "After P1 broadcasts 10: P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << "\n";

     cout << "\nP2 broadcasts -3:\n";
     p2.say(-3);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << "\n";

     cout << "\nP3 broadcasts -5:\n";
     p3.say(-5);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value()
          << ", P3=" << p3.get_value() << "\n";
}

void example_large_values()
{
     cout << "\n=== Example 7: Large Values ===\n";

     Mediator mediator;
     Participant p1{mediator};
     Participant p2{mediator};

     cout << "Initial: P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";

     cout << "\nP1 broadcasts 1000000:\n";
     p1.say(1000000);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";

     cout << "\nP2 broadcasts 500000:\n";
     p2.say(500000);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";
}

void example_dynamic_participant_addition()
{
     cout << "\n=== Example 8: Dynamically Adding Participants ===\n";

     Mediator mediator;
     Participant p1{mediator};
     Participant p2{mediator};

     cout << "Two participants, P1 broadcasts 5:\n";
     p1.say(5);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";

     cout << "\nAdding P3...\n";
     Participant p3{mediator};
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << ", P3=" << p3.get_value() << "\n";

     cout << "\nP1 broadcasts 3 (with 3 participants):\n";
     p1.say(3);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << ", P3=" << p3.get_value() << "\n";
}

void example_single_participant()
{
     cout << "\n=== Example 9: Single Participant ===\n";

     Mediator mediator;
     Participant p1{mediator};

     cout << "Single participant, initial value: " << p1.get_value() << "\n";

     cout << "\nP1 broadcasts 5:\n";
     p1.say(5);
     cout << "P1 value (should still be 0): " << p1.get_value() << "\n";
}

void example_manual_value_setting()
{
     cout << "\n=== Example 10: Manual Value Setting ===\n";

     Mediator mediator;
     Participant p1{mediator};
     Participant p2{mediator};

     cout << "Setting P1 to 10, P2 to 20:\n";
     p1.set_value(10);
     p2.set_value(20);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";

     cout << "\nP1 broadcasts 5:\n";
     p1.say(5);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";

     cout << "\nP2 broadcasts 3:\n";
     p2.say(3);
     cout << "P1=" << p1.get_value() << ", P2=" << p2.get_value() << "\n";
}

int main()
{
     cout << "========================================\n";
     cout << "  MEDIATOR CODING EXERCISE\n";
     cout << "  Participant Value Broadcasting\n";
     cout << "========================================\n";

     example_basic_broadcast();
     example_three_participants();
     example_multiple_broadcasts();
     example_chain_reactions();
     example_zero_broadcast();
     example_negative_values();
     example_large_values();
     example_dynamic_participant_addition();
     example_single_participant();
     example_manual_value_setting();

     cout << "\n========================================\n";

     return 0;
}
