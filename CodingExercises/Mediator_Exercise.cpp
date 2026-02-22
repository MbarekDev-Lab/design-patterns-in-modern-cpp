#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Participant;

class Mediator
{
public:
    vector<Participant *> participants;
    void broadcast(Participant *origin, int value);

    int get_participant_count() const
    {
        return participants.size();
    }

    void clear()
    {
        participants.clear();
    }
};

struct Participant
{
    int value = 0;
    Mediator &mediator;

    Participant(Mediator &mediator) : mediator(mediator)
    {
        mediator.participants.push_back(this);
    }

    void say(int val)
    {
        mediator.broadcast(this, val);
    }

    void receive(int val)
    {
        value += val;
    }

    int get_value() const { return value; }
    void set_value(int val) { value = val; }
};

inline void Mediator::broadcast(Participant *origin, int value)
{
    for (auto participant : participants)
    {
        if (participant != origin)
        {
            participant->receive(value);
        }
    }
}

// ============================================================================
// TEST FRAMEWORK
// ============================================================================

int g_test_count = 0;
int g_passed = 0;
int g_assertion_count = 0;

#define TEST(name)                                                 \
    {                                                              \
        g_test_count++;                                            \
        cout << "\nTest " << g_test_count << ": " << name << endl; \
    }
#define ASSERT_EQ_INT(actual, expected)                                                           \
    {                                                                                             \
        g_assertion_count++;                                                                      \
        if ((actual) == (expected))                                                               \
        {                                                                                         \
            cout << "  ✓ ASSERT_EQ(" << (actual) << ") passed" << endl;                           \
        }                                                                                         \
        else                                                                                      \
        {                                                                                         \
            cout << "  ✗ ASSERT_EQ(" << (actual) << ") FAILED - expected " << (expected) << endl; \
            return;                                                                               \
        }                                                                                         \
    }
#define TEST_END()  \
    {               \
        g_passed++; \
    }

// ============================================================================
// CODING EXERCISE TESTS
// ============================================================================

void test_single_participant()
{
    TEST("Single Participant: Initial value should be 0");

    Mediator mediator;
    Participant p1{mediator};

    ASSERT_EQ_INT(p1.get_value(), 0);
    TEST_END();
}

void test_two_participants_initial()
{
    TEST("Two Participants: Both start with value 0");

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    ASSERT_EQ_INT(p1.get_value(), 0);
    ASSERT_EQ_INT(p2.get_value(), 0);
    TEST_END();
}

void test_two_participants_first_says_3()
{
    TEST("Two Participants: P1 says 3, P2 receives +3");

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(3);

    ASSERT_EQ_INT(p1.get_value(), 0); // Sender doesn't receive
    ASSERT_EQ_INT(p2.get_value(), 3); // Receiver gets +3
    TEST_END();
}

void test_example_from_spec()
{
    TEST("Example from spec: Two participants with multiple broadcasts");

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    // Initial: both at 0
    ASSERT_EQ_INT(p1.get_value(), 0);
    ASSERT_EQ_INT(p2.get_value(), 0);

    // P1 says 3
    p1.say(3);
    ASSERT_EQ_INT(p1.get_value(), 0);
    ASSERT_EQ_INT(p2.get_value(), 3);

    // P2 says 2
    p2.say(2);
    ASSERT_EQ_INT(p1.get_value(), 2);
    ASSERT_EQ_INT(p2.get_value(), 3);
    TEST_END();
}

void test_three_participants()
{
    TEST("Three Participants: Each broadcasts affects others");

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};
    Participant p3{mediator};

    // P1 says 1
    p1.say(1);
    ASSERT_EQ_INT(p1.get_value(), 0);
    ASSERT_EQ_INT(p2.get_value(), 1);
    ASSERT_EQ_INT(p3.get_value(), 1);

    // P2 says 2
    p2.say(2);
    ASSERT_EQ_INT(p1.get_value(), 2);
    ASSERT_EQ_INT(p2.get_value(), 1);
    ASSERT_EQ_INT(p3.get_value(), 3);

    // P3 says 4
    p3.say(4);
    ASSERT_EQ_INT(p1.get_value(), 6);
    ASSERT_EQ_INT(p2.get_value(), 5);
    ASSERT_EQ_INT(p3.get_value(), 3);
    TEST_END();
}

void test_sender_not_in_recipients()
{
    TEST("Broadcast: Sender doesn't receive their own value");

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};
    Participant p3{mediator};

    p1.say(10);

    ASSERT_EQ_INT(p1.get_value(), 0);  // P1 unchanged
    ASSERT_EQ_INT(p2.get_value(), 10); // P2 receives
    ASSERT_EQ_INT(p3.get_value(), 10); // P3 receives
    TEST_END();
}

void test_single_participant_says()
{
    TEST("Single Participant: Broadcast has no effect (no others)");

    Mediator mediator;
    Participant p1{mediator};

    p1.say(100);

    ASSERT_EQ_INT(p1.get_value(), 0); // Still 0, no one else to receive
    TEST_END();
}

void test_multiple_broadcasts_from_one()
{
    TEST("Multiple broadcasts from same participant");

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(5);
    p1.say(3);
    p1.say(2);

    ASSERT_EQ_INT(p1.get_value(), 0);  // P1 never receives
    ASSERT_EQ_INT(p2.get_value(), 10); // P2 gets 5+3+2
    TEST_END();
}

void test_zero_value_broadcast()
{
    TEST("Broadcasting zero value");

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(0);

    ASSERT_EQ_INT(p1.get_value(), 0);
    ASSERT_EQ_INT(p2.get_value(), 0); // +0 = no change
    TEST_END();
}

void test_negative_value_broadcast()
{
    TEST("Broadcasting negative values");

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};

    p1.say(10);
    ASSERT_EQ_INT(p2.get_value(), 10);

    p1.say(-3);
    ASSERT_EQ_INT(p2.get_value(), 7); // 10 + (-3)
    TEST_END();
}

void test_large_group()
{
    TEST("Large group: 5 participants with multiple broadcasts");

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};
    Participant p3{mediator};
    Participant p4{mediator};
    Participant p5{mediator};

    p1.say(1); // p2-p5 get +1 each (now 1,1,1,1)
    ASSERT_EQ_INT(p2.get_value(), 1);
    ASSERT_EQ_INT(p3.get_value(), 1);
    ASSERT_EQ_INT(p4.get_value(), 1);
    ASSERT_EQ_INT(p5.get_value(), 1);

    p3.say(2); // p1,p2,p4,p5 get +2
    ASSERT_EQ_INT(p1.get_value(), 2);
    ASSERT_EQ_INT(p2.get_value(), 3);
    ASSERT_EQ_INT(p3.get_value(), 1);
    ASSERT_EQ_INT(p4.get_value(), 3);
    ASSERT_EQ_INT(p5.get_value(), 3);
    TEST_END();
}

void test_round_robin_broadcasts()
{
    TEST("Round-robin: Each participant broadcasts to all others");

    Mediator mediator;
    Participant p1{mediator};
    Participant p2{mediator};
    Participant p3{mediator};

    // First round: each says 1
    p1.say(1);
    p2.say(1);
    p3.say(1);

    // After p1.say(1): p2=1, p3=1
    // After p2.say(1): p1=1, p3+=1=2
    // After p3.say(1): p1+=1=2, p2+=1=2

    ASSERT_EQ_INT(p1.get_value(), 2);
    ASSERT_EQ_INT(p2.get_value(), 2);
    ASSERT_EQ_INT(p3.get_value(), 2);
    TEST_END();
}

void test_mediator_count()
{
    TEST("Mediator tracks participant count");

    Mediator mediator;
    ASSERT_EQ_INT(mediator.get_participant_count(), 0);

    Participant p1{mediator};
    ASSERT_EQ_INT(mediator.get_participant_count(), 1);

    Participant p2{mediator};
    ASSERT_EQ_INT(mediator.get_participant_count(), 2);

    Participant p3{mediator};
    ASSERT_EQ_INT(mediator.get_participant_count(), 3);
    TEST_END();
}

void test_all_participants_receive_except_sender()
{
    TEST("Verify all participants except sender receive broadcast");

    Mediator mediator;
    Participant participants[4] = {
        Participant{mediator},
        Participant{mediator},
        Participant{mediator},
        Participant{mediator}};

    // Participant 1 broadcasts 5
    participants[1].say(5);

    ASSERT_EQ_INT(participants[0].get_value(), 5); // Receives
    ASSERT_EQ_INT(participants[1].get_value(), 0); // Sender doesn't receive
    ASSERT_EQ_INT(participants[2].get_value(), 5); // Receives
    ASSERT_EQ_INT(participants[3].get_value(), 5); // Receives
    TEST_END();
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main()
{
    cout << "\n======================================================================\n";
    cout << "MEDIATOR CODING EXERCISE - PARTICIPANT BROADCAST\n";
    cout << "======================================================================\n";

    test_single_participant();
    test_two_participants_initial();
    test_two_participants_first_says_3();
    test_example_from_spec();
    test_three_participants();
    test_sender_not_in_recipients();
    test_single_participant_says();
    test_multiple_broadcasts_from_one();
    test_zero_value_broadcast();
    test_negative_value_broadcast();
    test_large_group();
    test_round_robin_broadcasts();
    test_mediator_count();
    test_all_participants_receive_except_sender();

    cout << "\n======================================================================\n";
    cout << "TEST SUMMARY\n";
    cout << "======================================================================\n";
    cout << "Tests Run:     " << g_test_count << "\n";
    cout << "Tests Passed:  " << g_passed << "\n";
    cout << "Tests Failed:  " << (g_test_count - g_passed) << "\n";
    cout << "Assertions:    " << g_assertion_count << "\n";
    cout << "Status:        " << (g_passed == g_test_count ? "✅ ALL TESTS PASSED" : "❌ SOME TESTS FAILED")
         << " (" << g_passed << "/" << g_test_count << ")\n";
    cout << "======================================================================\n\n";

    return g_passed == g_test_count ? 0 : 1;
}
