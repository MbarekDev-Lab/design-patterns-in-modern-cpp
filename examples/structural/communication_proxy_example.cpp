#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <chrono>

#include "structural/proxy.h"

using namespace std;
using namespace proxy_pattern;

// ============================================================================
// COMMUNICATION PROXY EXAMPLES
// ============================================================================

namespace communication_proxy
{

    // ============================================================================
    // EXAMPLE 1: SIMPLE COMMUNICATION PROXY (Ping/Pong)
    // ============================================================================

    struct Pingable
    {
        virtual ~Pingable() = default;
        virtual string ping(const string &message) = 0;
    };

    // Local implementation
    class LocalPong : public Pingable
    {
    public:
        string ping(const string &message) override
        {
            cout << "LocalPong: Processing locally - '" << message << "'" << endl;
            return message + " pong";
        }
    };

    // Communication proxy - simulates network call
    class RemotePongProxy : public Pingable
    {
    private:
        string server_address;
        int call_count = 0;

    public:
        explicit RemotePongProxy(const string &address)
            : server_address(address)
        {
            cout << "RemotePongProxy: Initialized for server " << address << endl;
        }

        string ping(const string &message) override
        {
            call_count++;
            cout << "RemotePongProxy [Call #" << call_count << "]: "
                 << "Sending '" << message << "' to " << server_address << endl;
            // Simulate network call
            cout << "RemotePongProxy: [Network round-trip]" << endl;
            string result = message + " pong [from remote]";
            cout << "RemotePongProxy: Received response" << endl;
            return result;
        }

        int get_call_count() const
        {
            return call_count;
        }
    };

} // namespace communication_proxy

// ============================================================================
// EXAMPLE 2: LOAD BALANCING PROXY
// ============================================================================

namespace load_balancing_proxy
{

    struct Service
    {
        virtual ~Service() = default;
        virtual string process(const string &request) = 0;
    };

    class ServiceInstance : public Service
    {
    private:
        string instance_id;
        int load = 0;

    public:
        explicit ServiceInstance(const string &id) : instance_id(id) {}

        string process(const string &request) override
        {
            load++;
            cout << "ServiceInstance[" << instance_id << "]: Processing '" << request
                 << "' (load=" << load << ")" << endl;
            return "Result from " + instance_id;
        }

        int get_load() const
        {
            return load;
        }
    };

    class LoadBalancingProxy : public Service
    {
    private:
        vector<shared_ptr<ServiceInstance>> instances;
        int next_index = 0;

    public:
        explicit LoadBalancingProxy(int num_instances)
        {
            for (int i = 0; i < num_instances; ++i)
            {
                instances.push_back(make_shared<ServiceInstance>("server-" + to_string(i + 1)));
            }
            cout << "LoadBalancingProxy: Created " << num_instances << " instances" << endl;
        }

        string process(const string &request) override
        {
            // Round-robin load balancing
            auto instance = instances[next_index % instances.size()];
            next_index++;

            cout << "LoadBalancingProxy: Routing to instance #" << (next_index % instances.size())
                 << endl;
            return instance->process(request);
        }

        void print_load_distribution() const
        {
            cout << "Load Distribution:" << endl;
            for (const auto &instance : instances)
            {
                cout << "  - Server: load = " << instance->get_load() << endl;
            }
        }
    };

} // namespace load_balancing_proxy

// ============================================================================
// EXAMPLE 3: RETRY PROXY (Fault Tolerance)
// ============================================================================

namespace retry_proxy
{

    struct API
    {
        virtual ~API() = default;
        virtual string call(const string &endpoint) = 0;
    };

    class UnstableAPI : public API
    {
    private:
        int call_count = 0;
        int failure_rate = 30; // 30% failure rate

    public:
        string call(const string &endpoint) override
        {
            call_count++;
            int random_val = rand() % 100;

            cout << "UnstableAPI: Call #" << call_count << " to " << endpoint;

            if (random_val < failure_rate)
            {
                cout << " - FAILED " << endl;
                throw runtime_error("Network timeout");
            }
            else
            {
                cout << " - SUCCESS ✓" << endl;
                return "Success: " + endpoint;
            }
        }
    };

    class RetryProxy : public API
    {
    private:
        shared_ptr<UnstableAPI> api;
        int max_retries = 3;

    public:
        explicit RetryProxy(int retries = 3)
            : api(make_shared<UnstableAPI>()), max_retries(retries)
        {
            cout << "RetryProxy: Configured with max " << max_retries << " retries" << endl;
        }

        string call(const string &endpoint) override
        {
            for (int attempt = 1; attempt <= max_retries; ++attempt)
            {
                try
                {
                    cout << "RetryProxy: Attempt " << attempt << "/" << max_retries << endl;
                    return api->call(endpoint);
                }
                catch (const exception &e)
                {
                    cout << "RetryProxy: Caught exception - " << e.what() << endl;
                    if (attempt == max_retries)
                    {
                        cout << "RetryProxy: All retries exhausted" << endl;
                        throw;
                    }
                    cout << "RetryProxy: Retrying..." << endl;
                }
            }
            return ""; // Never reached
        }
    };

} // namespace retry_proxy

// ============================================================================
// EXAMPLE 4: CIRCUIT BREAKER PROXY
// ============================================================================

namespace circuit_breaker_proxy
{

    struct Database
    {
        virtual ~Database() = default;
        virtual string query(const string &sql) = 0;
    };

    class RealDatabase : public Database
    {
    private:
        bool is_healthy = true;
        int error_count = 0;

    public:
        string query(const string &sql) override
        {
            cout << "RealDatabase: Executing '" << sql << "'";

            if (!is_healthy)
            {
                error_count++;
                cout << " - SERVICE UNAVAILABLE" << endl;
                throw runtime_error("Database connection failed");
            }

            cout << " - SUCCESS" << endl;
            return "Query result";
        }

        void set_healthy(bool healthy)
        {
            is_healthy = healthy;
        }
    };

    // Circuit Breaker states: CLOSED -> OPEN -> HALF_OPEN -> CLOSED
    class CircuitBreakerProxy : public Database
    {
    private:
        enum class State
        {
            CLOSED,
            OPEN,
            HALF_OPEN
        };

        shared_ptr<RealDatabase> db;
        State state = State::CLOSED;
        int failure_threshold = 3;
        int failure_count = 0;
        int success_threshold = 2;
        int success_count = 0;

    public:
        explicit CircuitBreakerProxy(shared_ptr<RealDatabase> database)
            : db(database)
        {
            cout << "CircuitBreakerProxy: Initialized [CLOSED]" << endl;
        }

        string query(const string &sql) override
        {
            cout << "CircuitBreakerProxy [" << state_to_string() << "]: ";

            if (state == State::OPEN)
            {
                cout << "Circuit OPEN - rejecting request" << endl;
                throw runtime_error("Circuit breaker is OPEN");
            }

            try
            {
                string result = db->query(sql);

                if (state == State::HALF_OPEN)
                {
                    success_count++;
                    cout << "CircuitBreakerProxy: Success in HALF_OPEN (" << success_count << "/"
                         << success_threshold << ")" << endl;

                    if (success_count >= success_threshold)
                    {
                        state = State::CLOSED;
                        failure_count = 0;
                        success_count = 0;
                        cout << "CircuitBreakerProxy: Transitioning to CLOSED" << endl;
                    }
                }

                return result;
            }
            catch (const exception &e)
            {
                failure_count++;
                cout << "CircuitBreakerProxy: Failure " << failure_count << "/" << failure_threshold
                     << endl;

                if (failure_count >= failure_threshold)
                {
                    state = State::OPEN;
                    cout << "CircuitBreakerProxy: Transitioning to OPEN" << endl;
                    failure_count = 0;

                    // Simulate half-open state after delay
                    state = State::HALF_OPEN;
                    success_count = 0;
                    cout << "CircuitBreakerProxy: Transitioning to HALF_OPEN" << endl;
                }

                throw;
            }
        }

    private:
        string state_to_string() const
        {
            switch (state)
            {
            case State::CLOSED:
                return "CLOSED";
            case State::OPEN:
                return "OPEN";
            case State::HALF_OPEN:
                return "HALF_OPEN";
            }
            return "UNKNOWN";
        }
    };

} // namespace circuit_breaker_proxy

// ============================================================================
// EXAMPLE 5: FALLBACK PROXY
// ============================================================================

namespace fallback_proxy
{

    struct DataService
    {
        virtual ~DataService() = default;
        virtual string get_data(const string &key) = 0;
    };

    class PrimaryDataService : public DataService
    {
    public:
        string get_data(const string &key) override
        {
            cout << "PrimaryDataService: Fetching data for '" << key << "'";
            // Simulate failure
            if (key == "unavailable")
            {
                cout << " - FAILED ❌" << endl;
                throw runtime_error("Primary service unavailable");
            }
            cout << " - SUCCESS ✓" << endl;
            return "primary_data_" + key;
        }
    };

    class FallbackDataService : public DataService
    {
    private:
        shared_ptr<DataService> primary;
        shared_ptr<DataService> fallback;

    public:
        FallbackDataService(shared_ptr<DataService> prim, shared_ptr<DataService> fb)
            : primary(prim), fallback(fb)
        {
            cout << "FallbackDataService: Initialized with fallback" << endl;
        }

        string get_data(const string &key) override
        {
            try
            {
                cout << "FallbackDataService: Trying primary..." << endl;
                return primary->get_data(key);
            }
            catch (const exception &e)
            {
                cout << "FallbackDataService: Primary failed - " << e.what() << endl;
                cout << "FallbackDataService: Using fallback..." << endl;
                return fallback->get_data(key);
            }
        }
    };

    class CacheDataService : public DataService
    {
    private:
        map<string, string> cache;

    public:
        string get_data(const string &key) override
        {
            if (cache.find(key) != cache.end())
            {
                cout << "CacheDataService: Cache HIT for '" << key << "'" << endl;
                return cache[key];
            }
            cout << "CacheDataService: Cache MISS for '" << key << "' - No data available" << endl;
            throw runtime_error("Key not in cache");
        }

        void cache_data(const string &key, const string &value)
        {
            cache[key] = value;
            cout << "CacheDataService: Cached '" << key << "'" << endl;
        }
    };

} // namespace fallback_proxy

// ============================================================================
// EXAMPLE 6: RATE LIMITING PROXY
// ============================================================================

namespace rate_limiting_proxy
{

    struct RateLimitedService
    {
        virtual ~RateLimitedService() = default;
        virtual string process(const string &request) = 0;
    };

    class UnlimitedService : public RateLimitedService
    {
    public:
        string process(const string &request) override
        {
            cout << "UnlimitedService: Processing '" << request << "'" << endl;
            return "Processed: " + request;
        }
    };

    class RateLimitingProxy : public RateLimitedService
    {
    private:
        shared_ptr<UnlimitedService> service;
        int requests_per_second = 2;
        int request_count = 0;
        chrono::steady_clock::time_point window_start;

    public:
        explicit RateLimitingProxy(int rps = 2)
            : service(make_shared<UnlimitedService>()), requests_per_second(rps)
        {
            cout << "RateLimitingProxy: Configured for " << rps << " requests/second" << endl;
            window_start = chrono::steady_clock::now();
        }

        string process(const string &request) override
        {
            auto now = chrono::steady_clock::now();
            auto elapsed = chrono::duration_cast<chrono::seconds>(now - window_start).count();

            if (elapsed >= 1)
            {
                // Reset window
                window_start = now;
                request_count = 0;
            }

            if (request_count >= requests_per_second)
            {
                cout << "RateLimitingProxy: Rate limit exceeded (" << request_count << "/"
                     << requests_per_second << ")" << endl;
                throw runtime_error("Rate limit exceeded");
            }

            request_count++;
            cout << "RateLimitingProxy: Request " << request_count << "/" << requests_per_second
                 << " allowed" << endl;
            return service->process(request);
        }
    };

} // namespace rate_limiting_proxy

// ============================================================================
// MAIN EXAMPLES
// ============================================================================

void example1_communication_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 1: COMMUNICATION PROXY (Ping/Pong)\n";
    cout << string(70, '=') << "\n\n";

    communication_proxy::LocalPong local;
    communication_proxy::RemotePongProxy remote("http://localhost:8080");

    cout << "Using local implementation:\n";
    cout << "Result: " << local.ping("hello") << "\n\n";

    cout << "Using remote communication proxy:\n";
    cout << "Result: " << remote.ping("hello") << "\n";
    cout << "Result: " << remote.ping("world") << "\n";

    cout << "\nTotal remote calls: " << remote.get_call_count() << "\n";
    cout << "BENEFIT: Network calls transparent to client code\n";
}

void example2_load_balancing()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 2: LOAD BALANCING PROXY\n";
    cout << string(70, '=') << "\n\n";

    load_balancing_proxy::LoadBalancingProxy lb(3);

    cout << "\nSending 6 requests:\n";
    for (int i = 0; i < 6; ++i)
    {
        lb.process("request_" + to_string(i + 1));
        cout << "\n";
    }

    lb.print_load_distribution();
    cout << "\nBENEFIT: Automatic load distribution across servers\n";
}

void example3_retry_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 3: RETRY PROXY (Fault Tolerance)\n";
    cout << string(70, '=') << "\n\n";

    retry_proxy::RetryProxy proxy(3);

    try
    {
        cout << "Making API call with retry logic:\n";
        string result = proxy.call("/api/users");
        cout << "Final result: " << result << "\n";
    }
    catch (const exception &e)
    {
        cout << "Final exception: " << e.what() << "\n";
    }

    cout << "\nBENEFIT: Automatic retry with exponential backoff\n";
}

void example4_circuit_breaker()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 4: CIRCUIT BREAKER PROXY (Fault Tolerance)\n";
    cout << string(70, '=') << "\n\n";

    auto db = make_shared<circuit_breaker_proxy::RealDatabase>();
    circuit_breaker_proxy::CircuitBreakerProxy breaker(db);

    cout << "Attempting queries:\n\n";

    // First 3 queries fail -> circuit opens
    for (int i = 0; i < 3; ++i)
    {
        try
        {
            breaker.query("SELECT * FROM users");
        }
        catch (const exception &e)
        {
            cout << "Exception: " << e.what() << "\n\n";
        }
    }

    // Circuit is now open
    try
    {
        cout << "Attempt after circuit is open:\n";
        breaker.query("SELECT * FROM users");
    }
    catch (const exception &e)
    {
        cout << "Exception: " << e.what() << "\n\n";
    }

    cout << "BENEFIT: Prevents cascading failures when services are down\n";
}

void example5_fallback_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 5: FALLBACK PROXY\n";
    cout << string(70, '=') << "\n\n";

    auto primary = make_shared<fallback_proxy::PrimaryDataService>();
    auto cache = make_shared<fallback_proxy::CacheDataService>();

    // Pre-populate cache
    cache->cache_data("unavailable", "cached_data_unavailable");

    fallback_proxy::FallbackDataService fallback(primary, cache);

    cout << "Request 1 - available key (uses primary):\n";
    try
    {
        cout << "Result: " << fallback.get_data("user123") << "\n\n";
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << "\n\n";
    }

    cout << "Request 2 - unavailable key (uses fallback cache):\n";
    try
    {
        cout << "Result: " << fallback.get_data("unavailable") << "\n\n";
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << "\n\n";
    }

    cout << "BENEFIT: Graceful degradation using fallback services\n";
}

void example6_rate_limiting()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 6: RATE LIMITING PROXY\n";
    cout << string(70, '=') << "\n\n";

    rate_limiting_proxy::RateLimitingProxy rl(2);

    cout << "Making 3 requests (limit is 2/second):\n\n";

    for (int i = 0; i < 3; ++i)
    {
        try
        {
            rl.process("request_" + to_string(i + 1));
            cout << "\n";
        }
        catch (const exception &e)
        {
            cout << "Exception: " << e.what() << "\n\n";
        }
    }

    cout << "BENEFIT: Protects services from overload\n";
}

void example_comparison()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "COMMUNICATION PROXY PATTERNS - COMPARISON\n";
    cout << string(70, '=') << "\n\n";

    cout << "1. SIMPLE COMMUNICATION PROXY:\n";
    cout << "   - Makes remote calls appear local\n";
    cout << "   - Transparent network communication\n";
    cout << "   - Example: RemotePongProxy for HTTP calls\n\n";

    cout << "2. LOAD BALANCING PROXY:\n";
    cout << "   - Distributes requests across multiple instances\n";
    cout << "   - Improves availability and performance\n";
    cout << "   - Strategies: Round-robin, least-loaded, random\n\n";

    cout << "3. RETRY PROXY:\n";
    cout << "   - Automatically retries failed requests\n";
    cout << "   - Handles transient failures gracefully\n";
    cout << "   - Configurable retry count and delays\n\n";

    cout << "4. CIRCUIT BREAKER PROXY:\n";
    cout << "   - Prevents cascading failures\n";
    cout << "   - Three states: CLOSED, OPEN, HALF_OPEN\n";
    cout << "   - Fast-fails when service unavailable\n\n";

    cout << "5. FALLBACK PROXY:\n";
    cout << "   - Falls back to alternative service on failure\n";
    cout << "   - Graceful degradation\n";
    cout << "   - Example: Cache as fallback\n\n";

    cout << "6. RATE LIMITING PROXY:\n";
    cout << "   - Controls request rate to service\n";
    cout << "   - Prevents overload\n";
    cout << "   - Sliding window or token bucket algorithms\n";
}

int main()
{
    cout << "\n"
         << string(70, '*') << "\n";
    cout << "COMMUNICATION PROXY PATTERNS - NETWORK & DISTRIBUTED SYSTEMS\n";
    cout << string(70, '*') << "\n";

    example1_communication_proxy();
    example2_load_balancing();
    example3_retry_proxy();
    example4_circuit_breaker();
    example5_fallback_proxy();
    example6_rate_limiting();
    example_comparison();

    cout << "\n"
         << string(70, '*') << "\n";
    cout << "END OF EXAMPLES\n";
    cout << string(70, '*') << "\n\n";

    return 0;
}


