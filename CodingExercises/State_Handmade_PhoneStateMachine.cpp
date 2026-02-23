/*
 * State Pattern - Handmade Phone State Machine
 *
 * This example demonstrates a manual implementation of a state machine
 * for a telephone system. Unlike the classic approach with state objects,
 * this uses enums and a transition table.
 *
 * States: OffHook, Connecting, Connected, OnHold, OnHook
 * Triggers: CallDialed, HungUp, CallConnected, PlacedOnHold, etc.
 *
 * This approach is useful for:
 * - Simple state machines
 * - Performance-critical systems
 * - Embedded systems with limited memory
 */

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cassert>
using namespace std;

enum class State
{
    OffHook,
    Connecting,
    Connected,
    OnHold,
    OnHook
};

inline ostream &operator<<(ostream &os, const State &s)
{
    switch (s)
    {
    case State::OffHook:
        os << "off the hook";
        break;
    case State::Connecting:
        os << "connecting";
        break;
    case State::Connected:
        os << "connected";
        break;
    case State::OnHold:
        os << "on hold";
        break;
    case State::OnHook:
        os << "on the hook";
        break;
    }
    return os;
}

enum class Trigger
{
    CallDialed,
    HungUp,
    CallConnected,
    PlacedOnHold,
    TakenOffHold,
    LeftMessage,
    StopUsingPhone
};

inline ostream &operator<<(ostream &os, const Trigger &t)
{
    switch (t)
    {
    case Trigger::CallDialed:
        os << "call dialed";
        break;
    case Trigger::HungUp:
        os << "hung up";
        break;
    case Trigger::CallConnected:
        os << "call connected";
        break;
    case Trigger::PlacedOnHold:
        os << "placed on hold";
        break;
    case Trigger::TakenOffHold:
        os << "taken off hold";
        break;
    case Trigger::LeftMessage:
        os << "left message";
        break;
    case Trigger::StopUsingPhone:
        os << "putting phone on hook";
        break;
    default:
        break;
    }
    return os;
}

class PhoneStateMachine
{
private:
    map<State, vector<pair<Trigger, State>>> rules;
    State currentState{State::OffHook};

public:
    PhoneStateMachine()
    {
        // Define transition rules
        rules[State::OffHook] = {
            {Trigger::CallDialed, State::Connecting},
            {Trigger::StopUsingPhone, State::OnHook}};

        rules[State::Connecting] = {
            {Trigger::HungUp, State::OffHook},
            {Trigger::CallConnected, State::Connected}};

        rules[State::Connected] = {
            {Trigger::LeftMessage, State::OffHook},
            {Trigger::HungUp, State::OffHook},
            {Trigger::PlacedOnHold, State::OnHold}};

        rules[State::OnHold] = {
            {Trigger::TakenOffHold, State::Connected},
            {Trigger::HungUp, State::OffHook}};
    }

    State getCurrentState() const { return currentState; }

    bool canTransition(Trigger trigger) const
    {
        auto it = rules.find(currentState);
        if (it == rules.end())
            return false;

        for (const auto &[t, _] : it->second)
        {
            if (t == trigger)
                return true;
        }
        return false;
    }

    bool processEvent(Trigger trigger)
    {
        auto it = rules.find(currentState);
        if (it == rules.end())
            return false;

        for (const auto &[t, nextState] : it->second)
        {
            if (t == trigger)
            {
                cout << "Transitioning from " << currentState << " to " << nextState
                     << " (triggered by " << trigger << ")" << endl;
                currentState = nextState;
                return true;
            }
        }

        cout << "No valid transition from " << currentState
             << " on event " << trigger << endl;
        return false;
    }
};

// ============================================================================
// TEST SUITE
// ============================================================================

void test_initial_state()
{
    PhoneStateMachine phone;
    assert(phone.getCurrentState() == State::OffHook);
    cout << "✓ Phone starts in OffHook state" << endl;
}

void test_dial_call()
{
    PhoneStateMachine phone;
    bool success = phone.processEvent(Trigger::CallDialed);
    assert(success);
    assert(phone.getCurrentState() == State::Connecting);
    cout << "✓ Can dial call from OffHook" << endl;
}

void test_call_connected()
{
    PhoneStateMachine phone;
    phone.processEvent(Trigger::CallDialed);
    bool success = phone.processEvent(Trigger::CallConnected);
    assert(success);
    assert(phone.getCurrentState() == State::Connected);
    cout << "✓ Call can connect after dialing" << endl;
}

void test_place_on_hold()
{
    PhoneStateMachine phone;
    phone.processEvent(Trigger::CallDialed);
    phone.processEvent(Trigger::CallConnected);
    bool success = phone.processEvent(Trigger::PlacedOnHold);
    assert(success);
    assert(phone.getCurrentState() == State::OnHold);
    cout << "✓ Can place connected call on hold" << endl;
}

void test_take_off_hold()
{
    PhoneStateMachine phone;
    phone.processEvent(Trigger::CallDialed);
    phone.processEvent(Trigger::CallConnected);
    phone.processEvent(Trigger::PlacedOnHold);
    bool success = phone.processEvent(Trigger::TakenOffHold);
    assert(success);
    assert(phone.getCurrentState() == State::Connected);
    cout << "✓ Can take call off hold" << endl;
}

void test_hangup_from_connected()
{
    PhoneStateMachine phone;
    phone.processEvent(Trigger::CallDialed);
    phone.processEvent(Trigger::CallConnected);
    bool success = phone.processEvent(Trigger::HungUp);
    assert(success);
    assert(phone.getCurrentState() == State::OffHook);
    cout << "✓ Can hangup from connected state" << endl;
}

void test_hangup_from_hold()
{
    PhoneStateMachine phone;
    phone.processEvent(Trigger::CallDialed);
    phone.processEvent(Trigger::CallConnected);
    phone.processEvent(Trigger::PlacedOnHold);
    bool success = phone.processEvent(Trigger::HungUp);
    assert(success);
    assert(phone.getCurrentState() == State::OffHook);
    cout << "✓ Can hangup from on hold state" << endl;
}

void test_stop_using_phone()
{
    PhoneStateMachine phone;
    bool success = phone.processEvent(Trigger::StopUsingPhone);
    assert(success);
    assert(phone.getCurrentState() == State::OnHook);
    cout << "✓ Can stop using phone from OffHook" << endl;
}

void test_invalid_transition()
{
    PhoneStateMachine phone;
    // From OffHook, cannot place on hold
    bool success = phone.processEvent(Trigger::PlacedOnHold);
    assert(!success);
    assert(phone.getCurrentState() == State::OffHook);
    cout << "✓ Invalid transition is prevented" << endl;
}

void test_can_transition_check()
{
    PhoneStateMachine phone;

    // From OffHook
    assert(phone.canTransition(Trigger::CallDialed));
    assert(phone.canTransition(Trigger::StopUsingPhone));
    assert(!phone.canTransition(Trigger::CallConnected));

    phone.processEvent(Trigger::CallDialed);

    // From Connecting
    assert(phone.canTransition(Trigger::HungUp));
    assert(phone.canTransition(Trigger::CallConnected));
    assert(!phone.canTransition(Trigger::PlacedOnHold));

    cout << "✓ Transition checking works correctly" << endl;
}

int main()
{
    cout << "State Pattern - Handmade Phone State Machine\n"
         << endl;

    try
    {
        cout << "Running tests...\n"
             << endl;

        test_initial_state();
        test_dial_call();
        test_call_connected();
        test_place_on_hold();
        test_take_off_hold();
        test_hangup_from_connected();
        test_hangup_from_hold();
        test_stop_using_phone();
        test_invalid_transition();
        test_can_transition_check();

        cout << "\n✅ All 10 tests passed!" << endl;
        cout << "\nState Machine Visualization:" << endl;
        cout << "  OffHook ----CallDialed----> Connecting" << endl;
        cout << "    |                            |" << endl;
        cout << "    |                            v" << endl;
        cout << "    |<---HungUp------- CallConnected" << endl;
        cout << "    |                      |" << endl;
        cout << "    |<--LeftMessage--      v" << endl;
        cout << "    |                  OnHold" << endl;
        cout << "    |                      |" << endl;
        cout << "    |<------HungUp----     v" << endl;
        cout << "    |<-TakenOffHold---Connected" << endl;
        cout << "    v" << endl;
        cout << "  OnHook" << endl;
        cout << "\nKey concepts:" << endl;
        cout << "  • Transition table defines valid state transitions" << endl;
        cout << "  • States and triggers are enums (lightweight)" << endl;
        cout << "  • Invalid transitions are rejected" << endl;
        cout << "  • Easy to visualize and understand" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
}
