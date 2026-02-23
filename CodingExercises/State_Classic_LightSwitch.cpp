/*
 * State Pattern - Classic Light Switch Example
 *
 * The State pattern allows an object to alter its behavior when its internal
 * state changes. It appears as if the object changed its class.
 *
 * This example demonstrates:
 * - Base State abstract class
 * - OnState and OffState concrete states
 * - LightSwitch context that delegates to current state
 * - State transitions via state switching
 */

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

class LightSwitch;

struct State
{
    virtual ~State() = default;

    virtual void on(LightSwitch *ls)
    {
        cout << "Light is already on\n";
    }

    virtual void off(LightSwitch *ls)
    {
        cout << "Light is already off\n";
    }
};

struct OnState : State
{
    OnState()
    {
        cout << "Light turned on\n";
    }

    void off(LightSwitch *ls) override;
};

struct OffState : State
{
    OffState()
    {
        cout << "Light turned off\n";
    }

    void on(LightSwitch *ls) override;
};

class LightSwitch
{
    State *state;
    bool is_on{false};

public:
    LightSwitch()
    {
        state = new OffState();
    }

    ~LightSwitch()
    {
        delete state;
    }

    void set_state(State *new_state)
    {
        delete state;
        this->state = new_state;
    }

    void on()
    {
        state->on(this);
        is_on = (dynamic_cast<OnState *>(state) != nullptr);
    }

    void off()
    {
        state->off(this);
        is_on = (dynamic_cast<OnState *>(state) != nullptr);
    }

    bool is_light_on() const { return is_on; }
};

void OnState::off(LightSwitch *ls)
{
    cout << "Switching light off...\n";
    ls->set_state(new OffState());
}

void OffState::on(LightSwitch *ls)
{
    cout << "Switching light on...\n";
    ls->set_state(new OnState());
}

// ============================================================================
// TEST SUITE
// ============================================================================

void test_light_starts_off()
{
    LightSwitch ls;
    assert(!ls.is_light_on());
    cout << "✓ Light starts off" << endl;
}

void test_turn_light_on()
{
    LightSwitch ls;
    ls.on();
    assert(ls.is_light_on());
    cout << "✓ Light turns on" << endl;
}

void test_turn_light_off()
{
    LightSwitch ls;
    ls.on();
    assert(ls.is_light_on());
    ls.off();
    assert(!ls.is_light_on());
    cout << "✓ Light turns off" << endl;
}

void test_double_on()
{
    cout << "\nTesting double on (should be idempotent):\n";
    LightSwitch ls;
    ls.on();
    cout << "Attempting to turn on again:\n";
    ls.on();
    assert(ls.is_light_on());
    cout << "✓ Light handles double on" << endl;
}

void test_double_off()
{
    cout << "\nTesting double off (should be idempotent):\n";
    LightSwitch ls;
    ls.off();
    cout << "Attempting to turn off again:\n";
    ls.off();
    assert(!ls.is_light_on());
    cout << "✓ Light handles double off" << endl;
}

void test_toggle_behavior()
{
    LightSwitch ls;

    // Turn on
    ls.on();
    assert(ls.is_light_on());

    // Turn off
    ls.off();
    assert(!ls.is_light_on());

    // Turn on again
    ls.on();
    assert(ls.is_light_on());

    // Turn off again
    ls.off();
    assert(!ls.is_light_on());

    cout << "✓ Light toggle works correctly" << endl;
}

int main()
{
    cout << "State Pattern - Classic Light Switch\n"
         << endl;

    try
    {
        cout << "Running tests...\n"
             << endl;

        test_light_starts_off();

        cout << "\nTesting turn on:\n";
        test_turn_light_on();

        cout << "\nTesting turn off:\n";
        test_turn_light_off();

        test_double_on();
        test_double_off();
        test_toggle_behavior();

        cout << "\n✅ All tests passed!" << endl;
        cout << "\nKey concepts:" << endl;
        cout << "  • State pattern encapsulates state-specific behavior" << endl;
        cout << "  • Each state is a separate class" << endl;
        cout << "  • Context (LightSwitch) delegates to current state" << endl;
        cout << "  • Transitions occur by changing the state object" << endl;
        cout << "  • Eliminates conditional logic based on state" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
}
