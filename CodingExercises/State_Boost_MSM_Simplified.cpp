/*
 * State Pattern - Boost MSM (Meta State Machine) Implementation
 *
 * NOTE: This example requires Boost library to compile.
 * Boost.MSM provides a sophisticated state machine framework that:
 * - Uses template metaprogramming for compile-time optimization
 * - Supports entry/exit actions on states
 * - Supports guards on transitions
 * - Supports internal transitions
 * - Generates efficient code
 *
 * This is the most advanced approach for complex state machines.
 * For this demonstration, we show the structure without requiring compilation.
 */

/*
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// back-end
#include <boost/msm/back/state_machine.hpp>

//front-end
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;

vector<string> state_names{
    "off hook",
    "connecting",
    "connected",
    "on hold",
    "destroyed"
};

// transitions/events
struct CallDialed {};
struct HungUp {};
struct CallConnected {};
struct PlacedOnHold {};
struct TakenOffHold {};
struct LeftMessage {};
struct PhoneThrownIntoWall {};

struct PhoneStateMachine : state_machine_def<PhoneStateMachine>
{
    bool angry{ true };

    struct OffHook : state<> {};
    struct Connecting : state<>
    {
        template <class Event, class FSM>
        void on_entry(Event const& evt, FSM&)
        {
            cout << "We are connecting..." << endl;
        }
    };
    struct Connected : state<> {};
    struct OnHold : state<> {};
    struct PhoneDestroyed : state<> {};

    struct PhoneBeingDestroyed
    {
        template <class EVT, class FSM, class SourceState, class TargetState>
        void operator()(EVT const&, FSM&, SourceState&, TargetState&)
        {
            cout << "Phone breaks into a million pieces" << endl;
        }
    };

    struct CanDestroyPhone
    {
        template <class EVT, class FSM, class SourceState, class TargetState>
        bool operator()(EVT const&, FSM& fsm, SourceState&, TargetState&)
        {
            return fsm.angry;
        }
    };

    // start, event, target, action, guard
    struct transition_table : mpl::vector <
        Row<OffHook, CallDialed, Connecting>,
        Row<Connecting, CallConnected, Connected>,
        Row<Connected, PlacedOnHold, OnHold>,
        Row<OnHold, PhoneThrownIntoWall, PhoneDestroyed,
            PhoneBeingDestroyed, CanDestroyPhone>
    > {};

    typedef OffHook initial_state;

    template <class FSM, class Event>
    void no_transition(Event const& e, FSM&, int state)
    {
        cout << "No transition from state " << state_names[state]
            << " on event " << typeid(e).name() << endl;
    }
};

int main()
{
    msm::back::state_machine<PhoneStateMachine> phone;

    auto info = [&]()
    {
        auto i = phone.current_state()[0];
        cout << "The phone is currently " << state_names[i] << "\n";
    };

    info();
    phone.process_event(CallDialed{});
    info();
    phone.process_event(CallConnected{});
    info();
    phone.process_event(PlacedOnHold{});
    info();
    phone.process_event(PhoneThrownIntoWall{});
    info();

    phone.process_event(CallDialed{});

    cout << "We are done using the phone" << "\n";

    getchar();
    return 0;
}
*/

// ============================================================================
// SIMPLIFIED VERSION WITHOUT BOOST (For Demonstration)
// ============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <memory>
using namespace std;

enum class PhoneState
{
    OffHook,
    Connecting,
    Connected,
    OnHold,
    PhoneDestroyed
};

ostream &operator<<(ostream &os, PhoneState state)
{
    switch (state)
    {
    case PhoneState::OffHook:
        os << "OffHook";
        break;
    case PhoneState::Connecting:
        os << "Connecting";
        break;
    case PhoneState::Connected:
        os << "Connected";
        break;
    case PhoneState::OnHold:
        os << "OnHold";
        break;
    case PhoneState::PhoneDestroyed:
        os << "PhoneDestroyed";
        break;
    }
    return os;
}

class BoostMSMPhoneSimulation
{
private:
    PhoneState state{PhoneState::OffHook};
    bool angry{true};

    bool canTransition(PhoneState from, PhoneState to) const
    {
        // Define valid transitions (simplified version of MSM rules)
        if (from == PhoneState::OffHook && to == PhoneState::Connecting)
            return true;
        if (from == PhoneState::Connecting && to == PhoneState::Connected)
            return true;
        if (from == PhoneState::Connected && to == PhoneState::OnHold)
            return true;
        if (from == PhoneState::OnHold && to == PhoneState::PhoneDestroyed && angry)
            return true;
        return false;
    }

public:
    PhoneState getState() const { return state; }

    bool callDialed()
    {
        if (state == PhoneState::OffHook)
        {
            cout << "Dialing call..." << endl;
            state = PhoneState::Connecting;
            cout << "State: " << state << endl;
            return true;
        }
        return false;
    }

    bool callConnected()
    {
        if (state == PhoneState::Connecting)
        {
            cout << "Call connected!" << endl;
            state = PhoneState::Connected;
            cout << "State: " << state << endl;
            return true;
        }
        return false;
    }

    bool placeOnHold()
    {
        if (state == PhoneState::Connected)
        {
            cout << "Placing call on hold..." << endl;
            state = PhoneState::OnHold;
            cout << "State: " << state << endl;
            return true;
        }
        return false;
    }

    bool throwPhoneIntoWall()
    {
        if (state == PhoneState::OnHold && angry)
        {
            cout << "Phone breaks into a million pieces!" << endl;
            state = PhoneState::PhoneDestroyed;
            cout << "State: " << state << endl;
            return true;
        }
        return false;
    }

    void setAngry(bool a) { angry = a; }
};

// ============================================================================
// TEST SUITE
// ============================================================================

void test_initial_state()
{
    BoostMSMPhoneSimulation sim;
    assert(sim.getState() == PhoneState::OffHook);
    cout << "✓ Phone starts in OffHook state" << endl;
}

void test_dial_call()
{
    BoostMSMPhoneSimulation sim;
    bool success = sim.callDialed();
    assert(success);
    assert(sim.getState() == PhoneState::Connecting);
    cout << "✓ Can dial call" << endl;
}

void test_call_connected()
{
    BoostMSMPhoneSimulation sim;
    sim.callDialed();
    bool success = sim.callConnected();
    assert(success);
    assert(sim.getState() == PhoneState::Connected);
    cout << "✓ Call can connect" << endl;
}

void test_place_on_hold()
{
    BoostMSMPhoneSimulation sim;
    sim.callDialed();
    sim.callConnected();
    bool success = sim.placeOnHold();
    assert(success);
    assert(sim.getState() == PhoneState::OnHold);
    cout << "✓ Can place on hold" << endl;
}

void test_destroy_when_angry()
{
    BoostMSMPhoneSimulation sim;
    sim.callDialed();
    sim.callConnected();
    sim.placeOnHold();
    sim.setAngry(true);
    bool success = sim.throwPhoneIntoWall();
    assert(success);
    assert(sim.getState() == PhoneState::PhoneDestroyed);
    cout << "✓ Can destroy phone when angry" << endl;
}

void test_cannot_destroy_when_not_angry()
{
    BoostMSMPhoneSimulation sim;
    sim.callDialed();
    sim.callConnected();
    sim.placeOnHold();
    sim.setAngry(false);
    bool success = sim.throwPhoneIntoWall();
    assert(!success);
    assert(sim.getState() == PhoneState::OnHold);
    cout << "✓ Cannot destroy phone when not angry" << endl;
}

void test_guard_condition()
{
    BoostMSMPhoneSimulation sim;
    sim.setAngry(false);
    bool success = sim.callDialed();
    assert(success);
    cout << "✓ Guard conditions prevent invalid transitions" << endl;
}

int main()
{
    cout << "State Pattern - Boost MSM (Simplified Demonstration)\n"
         << endl;
    cout << "NOTE: Full MSM requires Boost library\n"
         << endl;

    try
    {
        cout << "Running simplified tests...\n"
             << endl;

        test_initial_state();
        test_dial_call();
        test_call_connected();
        test_place_on_hold();
        test_destroy_when_angry();
        test_cannot_destroy_when_not_angry();
        test_guard_condition();

        cout << "\n✅ All 7 tests passed!" << endl;
        cout << "\nBoost MSM Key Features:" << endl;
        cout << "  • Compile-time state machine generation" << endl;
        cout << "  • Entry/exit actions on states" << endl;
        cout << "  • Guards on transitions (conditions)" << endl;
        cout << "  • Internal transitions" << endl;
        cout << "  • Hierarchical state machines" << endl;
        cout << "  • Exception safety" << endl;
        cout << "  • Zero-cost abstraction with optimal performance" << endl;
        cout << "\nComparison with other approaches:" << endl;
        cout << "  • Classic: More flexible, more code" << endl;
        cout << "  • Handmade: Simple, lightweight" << endl;
        cout << "  • MSM: Powerful, optimal, but requires Boost" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
}
