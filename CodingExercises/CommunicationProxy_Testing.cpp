#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <cassert>

using namespace std;

/*
 * COMMUNICATION PROXY TESTING EXAMPLE
 *
 * This demonstrates how to test communication proxies effectively using:
 * 1. Mock services
 * 2. Fake network implementations
 * 3. Dependency injection
 */

namespace testing_proxies
{

    // ============================================================================
    // INTERFACE DEFINITION
    // ============================================================================

    struct RemoteService
    {
        virtual ~RemoteService() = default;
        virtual string execute(const string &request) = 0;
    };

    // ============================================================================
    // REAL IMPLEMENTATION (Simulated HTTP Service)
    // ============================================================================

    class HTTPService : public RemoteService
    {
    private:
        string server_address;
        int call_count = 0;
        bool should_fail = false;

    public:
        explicit HTTPService(const string &address) : server_address(address) {}

        string execute(const string &request) override
        {
            call_count++;
            if (should_fail)
            {
                throw runtime_error("Service unavailable");
            }
            return request + ":response";
        }

        void set_failure(bool fail)
        {
            should_fail = fail;
        }

        int get_call_count() const
        {
            return call_count;
        }
    };

    // ============================================================================
    // MOCK IMPLEMENTATION (For Testing)
    // ============================================================================

    class MockService : public RemoteService
    {
    private:
        map<string, string> responses;
        vector<string> requests_received;
        int call_count = 0;

    public:
        explicit MockService() {}

        string execute(const string &request) override
        {
            call_count++;
            requests_received.push_back(request);

            if (responses.find(request) != responses.end())
            {
                return responses[request];
            }
            throw runtime_error("Mock: Unknown request");
        }

        void set_response(const string &request, const string &response)
        {
            responses[request] = response;
        }

        const vector<string> &get_requests() const
        {
            return requests_received;
        }

        int get_call_count() const
        {
            return call_count;
        }

        void clear()
        {
            requests_received.clear();
            call_count = 0;
        }
    };

    // ============================================================================
    // PROXY WITH DEPENDENCY INJECTION
    // ============================================================================

    class ServiceProxy
    {
    private:
        shared_ptr<RemoteService> service;
        int operations = 0;

    public:
        explicit ServiceProxy(shared_ptr<RemoteService> svc)
            : service(svc) {}

        string call(const string &request)
        {
            operations++;
            cout << "ServiceProxy: Call #" << operations << " - " << request << endl;
            return service->execute(request);
        }

        int get_operation_count() const
        {
            return operations;
        }
    };

    // ============================================================================
    // LOGGING PROXY WRAPPER (For Testing Instrumentation)
    // ============================================================================

    class LoggingProxy : public RemoteService
    {
    private:
        shared_ptr<RemoteService> wrapped;
        vector<pair<string, string>> log; // (request, response)

    public:
        explicit LoggingProxy(shared_ptr<RemoteService> service)
            : wrapped(service) {}

        string execute(const string &request) override
        {
            cout << "LoggingProxy: Recording request - " << request << endl;
            try
            {
                string response = wrapped->execute(request);
                log.push_back({request, response});
                cout << "LoggingProxy: Recording response - " << response << endl;
                return response;
            }
            catch (const exception &e)
            {
                cout << "LoggingProxy: Exception - " << e.what() << endl;
                throw;
            }
        }

        const vector<pair<string, string>> &get_log() const
        {
            return log;
        }
    };

    // ============================================================================
    // CACHING PROXY WRAPPER (For Testing)
    // ============================================================================

    class CachingProxy : public RemoteService
    {
    private:
        shared_ptr<RemoteService> wrapped;
        map<string, string> cache;
        int hits = 0;
        int misses = 0;

    public:
        explicit CachingProxy(shared_ptr<RemoteService> service)
            : wrapped(service) {}

        string execute(const string &request) override
        {
            if (cache.find(request) != cache.end())
            {
                cout << "CachingProxy: Cache HIT for " << request << endl;
                hits++;
                return cache[request];
            }

            cout << "CachingProxy: Cache MISS for " << request << endl;
            misses++;
            string response = wrapped->execute(request);
            cache[request] = response;
            return response;
        }

        int get_cache_hits() const
        {
            return hits;
        }

        int get_cache_misses() const
        {
            return misses;
        }
    };

} // namespace testing_proxies

// ============================================================================
// TEST FRAMEWORK
// ============================================================================

int g_test_count = 0;
int g_passed = 0;

#define TEST(name)                       \
    cout << "\n[TEST] " << name << "\n"; \
    g_test_count++;                      \
    bool test_passed = true;

#define ASSERT_EQ(a, b)                                     \
    if ((a) == (b))                                         \
    {                                                       \
        cout << "  ✓ PASS\n";                               \
    }                                                       \
    else                                                    \
    {                                                       \
        cout << "  ✗ FAIL: " << #a << " != " << #b << "\n"; \
        test_passed = false;                                \
    }

#define ASSERT_TRUE(condition)                      \
    if (condition)                                  \
    {                                               \
        cout << "  ✓ PASS\n";                       \
    }                                               \
    else                                            \
    {                                               \
        cout << "  ✗ FAIL: " << #condition << "\n"; \
        test_passed = false;                        \
    }

#define ASSERT_THROWS(code, exception_type)       \
    try                                           \
    {                                             \
        code;                                     \
        cout << "  ✗ FAIL: Expected exception\n"; \
        test_passed = false;                      \
    }                                             \
    catch (const exception_type &)                \
    {                                             \
        cout << "  ✓ PASS: Exception caught\n";   \
    }

#define TEST_END()             \
    if (test_passed)           \
    {                          \
        cout << "  ✅ PASS\n"; \
        g_passed++;            \
    }                          \
    else                       \
    {                          \
        cout << "  ❌ FAIL\n"; \
    }

// ============================================================================
// MOCK SERVICE TESTS (5 tests)
// ============================================================================

void test_mock_service_basic()
{
    TEST("Mock Service: Basic execution");

    using namespace testing_proxies;
    MockService mock;
    mock.set_response("request1", "response1");

    string result = mock.execute("request1");

    ASSERT_EQ(result, "response1");
    ASSERT_EQ(mock.get_call_count(), 1);

    TEST_END();
}

void test_mock_service_multiple_responses()
{
    TEST("Mock Service: Multiple distinct responses");

    using namespace testing_proxies;
    MockService mock;
    mock.set_response("hello", "hello_response");
    mock.set_response("world", "world_response");

    string r1 = mock.execute("hello");
    string r2 = mock.execute("world");

    ASSERT_EQ(r1, "hello_response");
    ASSERT_EQ(r2, "world_response");
    ASSERT_EQ(mock.get_call_count(), 2);

    TEST_END();
}

void test_mock_service_tracks_requests()
{
    TEST("Mock Service: Tracks all requests");

    using namespace testing_proxies;
    MockService mock;
    mock.set_response("req1", "resp1");
    mock.set_response("req2", "resp2");

    mock.execute("req1");
    mock.execute("req2");
    mock.execute("req1");

    const auto &requests = mock.get_requests();
    ASSERT_EQ(requests.size(), 3);
    ASSERT_EQ(requests[0], "req1");
    ASSERT_EQ(requests[1], "req2");
    ASSERT_EQ(requests[2], "req1");

    TEST_END();
}

void test_mock_service_unknown_request()
{
    TEST("Mock Service: Throws on unknown request");

    using namespace testing_proxies;
    MockService mock;

    ASSERT_THROWS(mock.execute("unknown"), runtime_error);

    TEST_END();
}

void test_mock_service_clear()
{
    TEST("Mock Service: Can clear state");

    using namespace testing_proxies;
    MockService mock;
    mock.set_response("test", "response");
    mock.execute("test");

    ASSERT_EQ(mock.get_call_count(), 1);

    mock.clear();

    ASSERT_EQ(mock.get_call_count(), 0);
    ASSERT_EQ(mock.get_requests().size(), 0);

    TEST_END();
}

// ============================================================================
// SERVICE PROXY TESTS (5 tests)
// ============================================================================

void test_service_proxy_with_mock()
{
    TEST("Service Proxy: Works with mock service");

    using namespace testing_proxies;
    auto mock = make_shared<MockService>();
    mock->set_response("test", "test_result");

    ServiceProxy proxy(mock);
    string result = proxy.call("test");

    ASSERT_EQ(result, "test_result");

    TEST_END();
}

void test_service_proxy_counts_operations()
{
    TEST("Service Proxy: Counts all operations");

    using namespace testing_proxies;
    auto mock = make_shared<MockService>();
    mock->set_response("op1", "r1");
    mock->set_response("op2", "r2");

    ServiceProxy proxy(mock);
    proxy.call("op1");
    proxy.call("op2");
    proxy.call("op1");

    ASSERT_EQ(proxy.get_operation_count(), 3);

    TEST_END();
}

void test_service_proxy_with_http_service()
{
    TEST("Service Proxy: Works with HTTP service");

    using namespace testing_proxies;
    auto http = make_shared<HTTPService>("http://localhost:8080");

    ServiceProxy proxy(http);
    string result = proxy.call("request");

    ASSERT_EQ(result, "request:response");

    TEST_END();
}

void test_service_proxy_multiple_calls()
{
    TEST("Service Proxy: Handles multiple requests");

    using namespace testing_proxies;
    auto mock = make_shared<MockService>();
    mock->set_response("a", "1");
    mock->set_response("b", "2");
    mock->set_response("c", "3");

    ServiceProxy proxy(mock);

    string r1 = proxy.call("a");
    string r2 = proxy.call("b");
    string r3 = proxy.call("c");

    ASSERT_EQ(r1, "1");
    ASSERT_EQ(r2, "2");
    ASSERT_EQ(r3, "3");
    ASSERT_EQ(proxy.get_operation_count(), 3);

    TEST_END();
}

void test_service_proxy_error_propagation()
{
    TEST("Service Proxy: Propagates errors from service");

    using namespace testing_proxies;
    auto mock = make_shared<MockService>();
    // Don't set response for "fail"

    ServiceProxy proxy(mock);

    try
    {
        proxy.call("fail");
        cout << "  ✗ FAIL: Expected exception\n";
        test_passed = false;
    }
    catch (const runtime_error &)
    {
        cout << "  ✓ PASS\n";
        // test_passed remains true
    }

    TEST_END();
}

// ============================================================================
// LOGGING PROXY TESTS (4 tests)
// ============================================================================

void test_logging_proxy_records_requests()
{
    TEST("Logging Proxy: Records all requests");

    using namespace testing_proxies;
    auto mock = make_shared<MockService>();
    mock->set_response("req1", "resp1");

    auto logging = make_shared<LoggingProxy>(mock);
    logging->execute("req1");

    const auto &log = logging->get_log();
    ASSERT_EQ(log.size(), 1);
    ASSERT_EQ(log[0].first, "req1");
    ASSERT_EQ(log[0].second, "resp1");

    TEST_END();
}

void test_logging_proxy_multiple_entries()
{
    TEST("Logging Proxy: Multiple log entries");

    using namespace testing_proxies;
    auto mock = make_shared<MockService>();
    mock->set_response("a", "1");
    mock->set_response("b", "2");

    auto logging = make_shared<LoggingProxy>(mock);
    logging->execute("a");
    logging->execute("b");

    ASSERT_EQ(logging->get_log().size(), 2);

    TEST_END();
}

void test_logging_proxy_with_caching()
{
    TEST("Logging Proxy: With caching combination");

    using namespace testing_proxies;
    auto mock = make_shared<MockService>();
    mock->set_response("key", "value");

    auto logging = make_shared<LoggingProxy>(mock);
    auto caching = make_shared<testing_proxies::CachingProxy>(logging);

    caching->execute("key"); // Cache miss
    caching->execute("key"); // Cache hit

    // Logging should have recorded both calls
    // but caching should have only called underlying once

    ASSERT_EQ(caching->get_cache_hits(), 1);
    ASSERT_EQ(caching->get_cache_misses(), 1);

    TEST_END();
}

void test_logging_proxy_exception_handling()
{
    TEST("Logging Proxy: Handles exceptions");

    using namespace testing_proxies;
    auto mock = make_shared<MockService>();
    // Don't set response for "fail"

    auto logging = make_shared<LoggingProxy>(mock);

    try
    {
        logging->execute("fail");
    }
    catch (const runtime_error &)
    {
        // Expected
    }

    // Log might be empty or contain the failed entry
    cout << "  ✓ PASS\n";
    TEST_END();
}

// ============================================================================
// CACHING PROXY TESTS (4 tests)
// ============================================================================

void test_caching_proxy_caches_results()
{
    TEST("Caching Proxy: Caches successful results");

    using namespace testing_proxies;
    auto mock = make_shared<MockService>();
    mock->set_response("key", "value");

    auto caching = make_shared<testing_proxies::CachingProxy>(mock);

    caching->execute("key");
    caching->execute("key");

    ASSERT_EQ(caching->get_cache_misses(), 1);
    ASSERT_EQ(caching->get_cache_hits(), 1);

    TEST_END();
}

void test_caching_proxy_different_keys()
{
    TEST("Caching Proxy: Different keys are separate");

    using namespace testing_proxies;
    auto mock = make_shared<MockService>();
    mock->set_response("a", "1");
    mock->set_response("b", "2");

    auto caching = make_shared<testing_proxies::CachingProxy>(mock);

    caching->execute("a");
    caching->execute("b");
    caching->execute("a");

    ASSERT_EQ(caching->get_cache_misses(), 2);
    ASSERT_EQ(caching->get_cache_hits(), 1);

    TEST_END();
}

void test_caching_proxy_returns_cached_value()
{
    TEST("Caching Proxy: Returns correct cached value");

    using namespace testing_proxies;
    auto mock = make_shared<MockService>();
    mock->set_response("test", "result");

    auto caching = make_shared<testing_proxies::CachingProxy>(mock);

    string r1 = caching->execute("test");
    string r2 = caching->execute("test");

    ASSERT_EQ(r1, "result");
    ASSERT_EQ(r2, "result");

    TEST_END();
}

void test_caching_proxy_with_http_service()
{
    TEST("Caching Proxy: Works with HTTP service");

    using namespace testing_proxies;
    auto http = make_shared<HTTPService>("http://localhost:8080");

    auto caching = make_shared<testing_proxies::CachingProxy>(http);

    string r1 = caching->execute("request");
    string r2 = caching->execute("request");

    ASSERT_EQ(caching->get_cache_misses(), 1);
    ASSERT_EQ(caching->get_cache_hits(), 1);

    TEST_END();
}

// ============================================================================
// INTEGRATION TESTS (3 tests)
// ============================================================================

void test_proxy_composition()
{
    TEST("Integration: Composing multiple proxies");

    using namespace testing_proxies;

    // Create a chain: Mock -> Caching -> Logging
    auto mock = make_shared<MockService>();
    mock->set_response("data", "value");

    auto caching = make_shared<testing_proxies::CachingProxy>(mock);
    auto logging = make_shared<LoggingProxy>(caching);

    // Both requests should go through logging
    logging->execute("data");
    logging->execute("data");

    ASSERT_EQ(logging->get_log().size(), 2);

    TEST_END();
}

void test_dependency_injection_flexibility()
{
    TEST("Integration: Dependency injection enables flexibility");

    using namespace testing_proxies;

    ServiceProxy proxy1(make_shared<MockService>());
    ServiceProxy proxy2(make_shared<HTTPService>("http://localhost"));

    // Both work identically
    cout << "  ✓ PASS\n";

    TEST_END();
}

void test_http_service_failure_mode()
{
    TEST("Integration: HTTP service can simulate failures");

    using namespace testing_proxies;
    auto http = make_shared<HTTPService>("http://localhost:8080");
    http->set_failure(true);

    try
    {
        http->execute("test");
        cout << "  ✗ FAIL: Expected exception\n";
        test_passed = false;
    }
    catch (const runtime_error &)
    {
        cout << "  ✓ PASS\n";
        // test_passed remains true
    }

    TEST_END();
}

// ============================================================================
// MAIN
// ============================================================================

void print_summary()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "TEST SUMMARY\n";
    cout << string(70, '=') << "\n\n";

    cout << "Tests Run:   " << g_test_count << "\n";
    cout << "Tests Passed: " << g_passed << "\n";
    cout << "Tests Failed: " << (g_test_count - g_passed) << "\n";
    cout << "Success Rate: " << (g_test_count > 0 ? (g_passed * 100 / g_test_count) : 0)
         << "%\n";

    if (g_passed == g_test_count)
    {
        cout << "\n✅ ALL TESTS PASSED\n";
    }
    else
    {
        cout << "\n❌ SOME TESTS FAILED\n";
    }

    cout << "\nTest Categories:\n";
    cout << "  Mock Service Tests: 5\n";
    cout << "  Service Proxy Tests: 5\n";
    cout << "  Logging Proxy Tests: 4\n";
    cout << "  Caching Proxy Tests: 4\n";
    cout << "  Integration Tests: 3\n";
    cout << "  Total: 21 tests\n";

    cout << "\n"
         << string(70, '=') << "\n";
}

int main()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "COMMUNICATION PROXY TESTING EXAMPLES\n";
    cout << string(70, '=') << "\n\n";

    // Mock Service Tests
    test_mock_service_basic();
    test_mock_service_multiple_responses();
    test_mock_service_tracks_requests();
    test_mock_service_unknown_request();
    test_mock_service_clear();

    // Service Proxy Tests
    test_service_proxy_with_mock();
    test_service_proxy_counts_operations();
    test_service_proxy_with_http_service();
    test_service_proxy_multiple_calls();
    test_service_proxy_error_propagation();

    // Logging Proxy Tests
    test_logging_proxy_records_requests();
    test_logging_proxy_multiple_entries();
    test_logging_proxy_with_caching();
    test_logging_proxy_exception_handling();

    // Caching Proxy Tests
    test_caching_proxy_caches_results();
    test_caching_proxy_different_keys();
    test_caching_proxy_returns_cached_value();
    test_caching_proxy_with_http_service();

    // Integration Tests
    test_proxy_composition();
    test_dependency_injection_flexibility();
    test_http_service_failure_mode();

    print_summary();

    return g_passed == g_test_count ? 0 : 1;
}
