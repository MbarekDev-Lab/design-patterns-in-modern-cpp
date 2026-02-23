#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

using namespace std;

// ============================================================================
// OBSERVER PATTERN - STOCK PRICE MONITORING
// ============================================================================
// Problem:
// - Multiple observers need to be notified when subject state changes
// - Observers are decoupled from subject/observable
// - Need to support subscription/unsubscription
// - Multiple observers should receive notifications independently
//
// Solution: Observer Pattern
// - Observer: Interface for receiving notifications
// - Observable: Manages observers, sends notifications
// - Observers subscribe to get notified of field changes
// ============================================================================

// Forward declaration
template <typename T>
struct Observer;

// ============================================================================
// OBSERVER - Interface that receives notifications
// ============================================================================
template <typename T>
struct Observer
{
    virtual ~Observer() = default;
    virtual void field_changed(T &source, const string &field_name) = 0;
};

// ============================================================================
// OBSERVABLE - Manages observers and sends notifications
// ============================================================================
template <typename T>
struct Observable
{
    vector<Observer<T> *> observers;

    // Notify all observers of a field change
    void notify(T &source, const string &field_name)
    {
        for (auto observer : observers)
        {
            if (observer)
                observer->field_changed(source, field_name);
        }
    }

    // Add observer to subscription list
    void subscribe(Observer<T> &observer)
    {
        observers.push_back(&observer);
    }

    // Remove observer from subscription list
    void unsubscribe(Observer<T> &observer)
    {
        auto it = find(observers.begin(), observers.end(), &observer);
        if (it != observers.end())
        {
            observers.erase(it);
        }
    }

    // Get number of observers
    int get_observer_count() const
    {
        return observers.size();
    }
};

// ============================================================================
// STOCK - Observable class representing a stock with price and volume
// ============================================================================
struct Stock : Observable<Stock>
{
    string symbol;
    double price = 0.0;
    int volume = 0;

    explicit Stock(const string &symbol)
        : symbol(symbol)
    {
    }

    void set_price(double new_price)
    {
        if (price != new_price)
        {
            price = new_price;
            notify(*this, "price");
        }
    }

    void set_volume(int new_volume)
    {
        if (volume != new_volume)
        {
            volume = new_volume;
            notify(*this, "volume");
        }
    }

    double get_price() const { return price; }
    int get_volume() const { return volume; }
    string get_symbol() const { return symbol; }
};

// ============================================================================
// CONCRETE OBSERVERS
// ============================================================================

// Observer 1: Portfolio Manager - tracks price changes
struct PortfolioManager : Observer<Stock>
{
    int call_count = 0;

    void field_changed(Stock &source, const string &field_name) override
    {
        if (field_name == "price")
        {
            call_count++;
            cout << "Portfolio: Stock " << source.get_symbol()
                 << " price changed to $" << source.get_price() << "\n";
        }
    }
};

// Observer 2: Risk Manager - monitors large volume changes
struct RiskManager : Observer<Stock>
{
    int call_count = 0;

    void field_changed(Stock &source, const string &field_name) override
    {
        if (field_name == "volume")
        {
            call_count++;
            if (source.get_volume() > 1000000)
            {
                cout << "Risk Alert: Unusual volume for " << source.get_symbol()
                     << " - " << source.get_volume() << " shares!\n";
            }
        }
    }
};

// Observer 3: Trading System - executes trades on price changes
struct TradingSystem : Observer<Stock>
{
    int call_count = 0;

    void field_changed(Stock &source, const string &field_name) override
    {
        if (field_name == "price")
        {
            call_count++;
            if (source.get_price() > 150.0)
            {
                cout << "Trading: Selling " << source.get_symbol()
                     << " at $" << source.get_price() << "\n";
            }
            else if (source.get_price() < 50.0)
            {
                cout << "Trading: Buying " << source.get_symbol()
                     << " at $" << source.get_price() << "\n";
            }
        }
    }
};

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
#define ASSERT_EQ_INT(actual, expected)                                    \
    {                                                                      \
        g_assertion_count++;                                               \
        if ((actual) == (expected))                                        \
        {                                                                  \
            cout << "  ✓ ASSERT_EQ(" << (actual) << ") passed" << endl;    \
        }                                                                  \
        else                                                               \
        {                                                                  \
            cout << "  ✗ ASSERT_EQ(" << (actual) << ") FAILED - expected " \
                 << (expected) << endl;                                    \
            return;                                                        \
        }                                                                  \
    }
#define ASSERT_EQ_DOUBLE(actual, expected)                                 \
    {                                                                      \
        g_assertion_count++;                                               \
        if (abs((actual) - (expected)) < 0.001)                            \
        {                                                                  \
            cout << "  ✓ ASSERT_EQ(" << (actual) << ") passed" << endl;    \
        }                                                                  \
        else                                                               \
        {                                                                  \
            cout << "  ✗ ASSERT_EQ(" << (actual) << ") FAILED - expected " \
                 << (expected) << endl;                                    \
            return;                                                        \
        }                                                                  \
    }
#define TEST_END()  \
    {               \
        g_passed++; \
    }

// ============================================================================
// OBSERVER PATTERN TESTS
// ============================================================================

void test_observable_initialization()
{
    TEST("Observable initializes with no observers");

    Stock stock("AAPL");
    ASSERT_EQ_INT(stock.get_observer_count(), 0);
    TEST_END();
}

void test_observer_subscription()
{
    TEST("Observer can subscribe to observable");

    Stock stock("AAPL");
    PortfolioManager pm;

    stock.subscribe(pm);
    ASSERT_EQ_INT(stock.get_observer_count(), 1);
    TEST_END();
}

void test_multiple_observer_subscriptions()
{
    TEST("Multiple observers can subscribe");

    Stock stock("AAPL");
    PortfolioManager pm;
    RiskManager rm;
    TradingSystem ts;

    stock.subscribe(pm);
    stock.subscribe(rm);
    stock.subscribe(ts);

    ASSERT_EQ_INT(stock.get_observer_count(), 3);
    TEST_END();
}

void test_observer_unsubscription()
{
    TEST("Observer can unsubscribe from observable");

    Stock stock("AAPL");
    PortfolioManager pm;

    stock.subscribe(pm);
    ASSERT_EQ_INT(stock.get_observer_count(), 1);

    stock.unsubscribe(pm);
    ASSERT_EQ_INT(stock.get_observer_count(), 0);
    TEST_END();
}

void test_price_change_notifications()
{
    TEST("Price change notifies all observers");

    Stock stock("AAPL");
    PortfolioManager pm;
    RiskManager rm;
    TradingSystem ts;

    stock.subscribe(pm);
    stock.subscribe(rm);
    stock.subscribe(ts);

    // Change price - should notify PM and TS (not RM, only cares about volume)
    stock.set_price(100.0);

    ASSERT_EQ_INT(pm.call_count, 1);
    ASSERT_EQ_INT(rm.call_count, 0);
    ASSERT_EQ_INT(ts.call_count, 1);
    TEST_END();
}

void test_volume_change_notifications()
{
    TEST("Volume change notifies relevant observers");

    Stock stock("AAPL");
    PortfolioManager pm;
    RiskManager rm;
    TradingSystem ts;

    stock.subscribe(pm);
    stock.subscribe(rm);
    stock.subscribe(ts);

    // Change volume - should notify RM only
    stock.set_volume(2000000);

    ASSERT_EQ_INT(pm.call_count, 0);
    ASSERT_EQ_INT(rm.call_count, 1);
    ASSERT_EQ_INT(ts.call_count, 0);
    TEST_END();
}

void test_multiple_price_changes()
{
    TEST("Multiple price changes send multiple notifications");

    Stock stock("AAPL");
    PortfolioManager pm;

    stock.subscribe(pm);

    stock.set_price(100.0);
    ASSERT_EQ_INT(pm.call_count, 1);

    stock.set_price(105.0);
    ASSERT_EQ_INT(pm.call_count, 2);

    stock.set_price(110.0);
    ASSERT_EQ_INT(pm.call_count, 3);
    TEST_END();
}

void test_same_price_no_notification()
{
    TEST("Setting same price doesn't notify observers");

    Stock stock("AAPL");
    PortfolioManager pm;

    stock.subscribe(pm);

    stock.set_price(100.0);
    ASSERT_EQ_INT(pm.call_count, 1);

    // Set same price again
    stock.set_price(100.0);
    ASSERT_EQ_INT(pm.call_count, 1); // No additional notification
    TEST_END();
}

void test_observer_unsubscribe_receives_no_notifications()
{
    TEST("Unsubscribed observer doesn't receive notifications");

    Stock stock("AAPL");
    PortfolioManager pm;

    stock.subscribe(pm);
    stock.set_price(100.0);
    ASSERT_EQ_INT(pm.call_count, 1);

    // Unsubscribe
    stock.unsubscribe(pm);

    // Change price
    stock.set_price(105.0);
    ASSERT_EQ_INT(pm.call_count, 1); // Count unchanged
    TEST_END();
}

void test_resubscribe_after_unsubscribe()
{
    TEST("Observer can resubscribe after unsubscribing");

    Stock stock("AAPL");
    PortfolioManager pm;

    stock.subscribe(pm);
    stock.set_price(100.0);
    ASSERT_EQ_INT(pm.call_count, 1);

    stock.unsubscribe(pm);
    stock.set_price(105.0);
    ASSERT_EQ_INT(pm.call_count, 1);

    stock.subscribe(pm);
    stock.set_price(110.0);
    ASSERT_EQ_INT(pm.call_count, 2);
    TEST_END();
}

void test_multiple_observers_independent_notification()
{
    TEST("Multiple observers receive notifications independently");

    Stock stock("AAPL");
    PortfolioManager pm1;
    PortfolioManager pm2;
    PortfolioManager pm3;

    stock.subscribe(pm1);
    stock.subscribe(pm2);
    stock.subscribe(pm3);

    stock.set_price(100.0);

    ASSERT_EQ_INT(pm1.call_count, 1);
    ASSERT_EQ_INT(pm2.call_count, 1);
    ASSERT_EQ_INT(pm3.call_count, 1);
    TEST_END();
}

void test_observer_selectively_unsubscribes()
{
    TEST("One observer can unsubscribe while others remain");

    Stock stock("AAPL");
    PortfolioManager pm;
    RiskManager rm;

    stock.subscribe(pm);
    stock.subscribe(rm);

    stock.unsubscribe(pm);
    ASSERT_EQ_INT(stock.get_observer_count(), 1);

    stock.set_price(100.0);
    ASSERT_EQ_INT(pm.call_count, 0); // PM didn't get notified
    TEST_END();
}

void test_stock_with_multiple_fields()
{
    TEST("Stock notifications for price and volume");

    Stock stock("GOOGL");
    PortfolioManager pm;
    stock.subscribe(pm);

    stock.set_price(2500.0);
    ASSERT_EQ_DOUBLE(stock.get_price(), 2500.0);

    stock.set_volume(5000000);
    ASSERT_EQ_INT(stock.get_volume(), 5000000);
    TEST_END();
}

void test_different_stocks_independent()
{
    TEST("Different stocks have independent observers");

    Stock aapl("AAPL");
    Stock googl("GOOGL");

    PortfolioManager pm1;
    PortfolioManager pm2;

    aapl.subscribe(pm1);
    googl.subscribe(pm2);

    aapl.set_price(100.0);
    ASSERT_EQ_INT(pm1.call_count, 1);
    ASSERT_EQ_INT(pm2.call_count, 0);

    googl.set_price(2500.0);
    ASSERT_EQ_INT(pm1.call_count, 1);
    ASSERT_EQ_INT(pm2.call_count, 1);
    TEST_END();
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main()
{
    cout << "\n======================================================================\n";
    cout << "OBSERVER PATTERN CODING EXERCISE - STOCK PRICE MONITORING\n";
    cout << "======================================================================\n";

    test_observable_initialization();
    test_observer_subscription();
    test_multiple_observer_subscriptions();
    test_observer_unsubscription();
    test_price_change_notifications();
    test_volume_change_notifications();
    test_multiple_price_changes();
    test_same_price_no_notification();
    test_observer_unsubscribe_receives_no_notifications();
    test_resubscribe_after_unsubscribe();
    test_multiple_observers_independent_notification();
    test_observer_selectively_unsubscribes();
    test_stock_with_multiple_fields();
    test_different_stocks_independent();

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
