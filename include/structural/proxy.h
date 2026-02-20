#ifndef PROXY_PATTERN_H
#define PROXY_PATTERN_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <map>

using namespace std;

/*
 * PROXY PATTERN
 *
 * INTENT:
 * Provide a surrogate or placeholder for another object to control access to it.
 *
 * KEY CONCEPTS:
 * - Proxy: Controls access to the real subject
 * - Real Subject: The actual object being proxied
 * - Common Interface: Both proxy and real subject implement same interface
 *
 * TYPES OF PROXIES:
 * 1. Virtual Proxy: Defers expensive object creation (lazy loading)
 * 2. Protection Proxy: Controls access based on permissions
 * 3. Logging Proxy: Logs method calls for debugging
 * 4. Caching Proxy: Caches expensive results
 * 5. Remote Proxy: Represents a remote object
 * 6. Smart Pointer Proxy: Manages memory automatically
 * 7. Synchronization Proxy: Threads safe access
 *
 * BENEFITS:
 * - Lazy initialization of expensive objects
 * - Access control and authorization
 * - Logging and monitoring
 * - Caching expensive computations
 * - Remote object transparency
 * - Change implementation without changing client code
 *
 * TRADE-OFFS:
 * - Additional layer of indirection
 * - Slightly slower access
 * - More classes to maintain
 */

namespace proxy_pattern
{

    // ============================================================================
    // EXAMPLE 1: VIRTUAL PROXY (Lazy Loading)
    // ============================================================================

    namespace virtual_proxy
    {

        // Interface for image operations
        struct Image
        {
            virtual ~Image() = default;
            virtual void draw() = 0;
            virtual string get_filename() const = 0;
        };

        // Real image - expensive to load
        class BitmapImage : public Image
        {
        private:
            string filename;
            bool loaded = false;

        public:
            explicit BitmapImage(const string &fname) : filename(fname)
            {
                cout << "RealImage::RealImage() Loading image from disk: " << filename << endl;
            }

            void draw() override
            {
                cout << "BitmapImage::draw() Displaying image: " << filename << endl;
            }

            string get_filename() const override
            {
                return filename;
            }
        };

        // Proxy - delays loading until actually needed
        class ImageProxy : public Image
        {
        private:
            string filename;
            mutable shared_ptr<BitmapImage> real_image;

            void load() const
            {
                if (!real_image)
                {
                    cout << "ImageProxy::load() Creating real image on first access" << endl;
                    real_image = make_shared<BitmapImage>(filename);
                }
            }

        public:
            explicit ImageProxy(const string &fname) : filename(fname)
            {
                cout << "ImageProxy created for: " << filename << " (NOT loaded yet)" << endl;
            }

            void draw() override
            {
                load(); // Lazy load on first access
                real_image->draw();
            }

            string get_filename() const override
            {
                return filename;
            }
        };

    } // namespace virtual_proxy

    // ============================================================================
    // EXAMPLE 2: PROTECTION PROXY (Access Control)
    // ============================================================================

    namespace protection_proxy
    {

        // Interface for sensitive operations
        struct Database
        {
            virtual ~Database() = default;
            virtual void read(const string &query) = 0;
            virtual void write(const string &query) = 0;
            virtual void delete_data(const string &query) = 0;
        };

        // Real database implementation
        class RealDatabase : public Database
        {
        public:
            void read(const string &query) override
            {
                cout << "RealDatabase: Reading with query: " << query << endl;
            }

            void write(const string &query) override
            {
                cout << "RealDatabase: Writing with query: " << query << endl;
            }

            void delete_data(const string &query) override
            {
                cout << "RealDatabase: Deleting with query: " << query << endl;
            }
        };

        // Proxy with access control
        class DatabaseProxy : public Database
        {
        private:
            shared_ptr<RealDatabase> real_db;
            string user_role;

        public:
            explicit DatabaseProxy(const string &role, shared_ptr<RealDatabase> db)
                : real_db(db), user_role(role)
            {
                cout << "DatabaseProxy created for user role: " << user_role << endl;
            }

            void read(const string &query) override
            {
                // Everyone can read (guest, user, admin)
                cout << "DatabaseProxy: Access granted for READ" << endl;
                real_db->read(query);
            }

            void write(const string &query) override
            {
                if (user_role == "admin")
                {
                    cout << "DatabaseProxy: Access granted for WRITE" << endl;
                    real_db->write(query);
                }
                else
                {
                    cout << "DatabaseProxy: Access DENIED for WRITE (insufficient permissions)" << endl;
                }
            }

            void delete_data(const string &query) override
            {
                if (user_role == "admin")
                {
                    cout << "DatabaseProxy: Access granted for DELETE" << endl;
                    real_db->delete_data(query);
                }
                else
                {
                    cout << "DatabaseProxy: Access DENIED for DELETE (insufficient permissions)" << endl;
                }
            }
        };

    } // namespace protection_proxy

    // ============================================================================
    // EXAMPLE 3: LOGGING PROXY (Instrumentation)
    // ============================================================================

    namespace logging_proxy
    {

        // Calculator interface
        struct Calculator
        {
            virtual ~Calculator() = default;
            virtual int add(int a, int b) = 0;
            virtual int subtract(int a, int b) = 0;
            virtual int multiply(int a, int b) = 0;
        };

        // Real calculator
        class RealCalculator : public Calculator
        {
        public:
            int add(int a, int b) override
            {
                return a + b;
            }

            int subtract(int a, int b) override
            {
                return a - b;
            }

            int multiply(int a, int b) override
            {
                return a * b;
            }
        };

        // Logging proxy - logs all operations
        class LoggingCalculatorProxy : public Calculator
        {
        private:
            shared_ptr<RealCalculator> real_calc;
            vector<string> operation_log;

        public:
            LoggingCalculatorProxy() : real_calc(make_shared<RealCalculator>()) {}

            int add(int a, int b) override
            {
                cout << "LoggingCalculatorProxy: Calling add(" << a << ", " << b << ")" << endl;
                operation_log.push_back("add(" + to_string(a) + ", " + to_string(b) + ")");
                return real_calc->add(a, b);
            }

            int subtract(int a, int b) override
            {
                cout << "LoggingCalculatorProxy: Calling subtract(" << a << ", " << b << ")" << endl;
                operation_log.push_back("subtract(" + to_string(a) + ", " + to_string(b) + ")");
                return real_calc->subtract(a, b);
            }

            int multiply(int a, int b) override
            {
                cout << "LoggingCalculatorProxy: Calling multiply(" << a << ", " << b << ")" << endl;
                operation_log.push_back("multiply(" + to_string(a) + ", " + to_string(b) + ")");
                return real_calc->multiply(a, b);
            }

            void print_log() const
            {
                cout << "Operation Log:" << endl;
                for (const auto &op : operation_log)
                {
                    cout << "  - " << op << endl;
                }
            }

            size_t get_operation_count() const
            {
                return operation_log.size();
            }
        };

    } // namespace logging_proxy

    // ============================================================================
    // EXAMPLE 4: CACHING PROXY
    // ============================================================================

    namespace caching_proxy
    {

        // Service that performs expensive computations
        struct ComputeService
        {
            virtual ~ComputeService() = default;
            virtual int compute_factorial(int n) = 0;
        };

        class SlowComputeService : public ComputeService
        {
        public:
            int compute_factorial(int n) override
            {
                cout << "SlowComputeService: Computing factorial(" << n << ")..." << endl;
                // Simulate expensive computation
                int result = 1;
                for (int i = 1; i <= n; ++i)
                {
                    result *= i;
                }
                return result;
            }
        };

        // Caching proxy - caches results of expensive operations
        class CachingComputeProxy : public ComputeService
        {
        private:
            shared_ptr<SlowComputeService> real_service;
            mutable map<int, int> cache;

        public:
            CachingComputeProxy()
                : real_service(make_shared<SlowComputeService>()) {}

            int compute_factorial(int n) override
            {
                if (cache.find(n) != cache.end())
                {
                    cout << "CachingComputeProxy: Returning cached result for " << n << endl;
                    return cache[n];
                }

                cout << "CachingComputeProxy: Calling real service and caching result" << endl;
                int result = real_service->compute_factorial(n);
                cache[n] = result;
                return result;
            }

            size_t get_cache_size() const
            {
                return cache.size();
            }
        };

    } // namespace caching_proxy

    // ============================================================================
    // EXAMPLE 5: SMART POINTER PROXY
    // ============================================================================

    namespace smart_pointer_proxy
    {

        // Resource-heavy object
        class ExpensiveResource
        {
        private:
            string resource_id;

        public:
            explicit ExpensiveResource(const string &id) : resource_id(id)
            {
                cout << "ExpensiveResource created: " << resource_id << endl;
            }

            ~ExpensiveResource()
            {
                cout << "ExpensiveResource destroyed: " << resource_id << endl;
            }

            void use()
            {
                cout << "Using resource: " << resource_id << endl;
            }

            string get_id() const
            {
                return resource_id;
            }
        };

        // Smart pointer acts as a proxy - manages lifetime automatically
        class ResourceProxy
        {
        private:
            shared_ptr<ExpensiveResource> resource;
            int request_count = 0;

        public:
            explicit ResourceProxy(const string &id)
                : resource(make_shared<ExpensiveResource>(id))
            {
                cout << "ResourceProxy created" << endl;
            }

            void use()
            {
                request_count++;
                cout << "ResourceProxy: Request #" << request_count << endl;
                resource->use();
            }

            string get_id() const
            {
                return resource->get_id();
            }

            int get_request_count() const
            {
                return request_count;
            }

            // Automatic cleanup when proxy is destroyed
            ~ResourceProxy()
            {
                cout << "ResourceProxy destroyed (automatic cleanup)" << endl;
            }
        };

    } // namespace smart_pointer_proxy

    // ============================================================================
    // EXAMPLE 6: PROPERTY PROXY
    // ============================================================================

    namespace property_proxy
    {

        // Generic property proxy with getter/setter control
        template <typename T>
        class Property
        {
        private:
            T value;
            string name;

        public:
            Property() = default;

            explicit Property(const T &initial_value, const string &prop_name = "")
                : value(initial_value), name(prop_name) {}

            // Implicit conversion to T (getter)
            operator T() const
            {
                if (!name.empty())
                {
                    cout << "Property[" << name << "]::get() = " << value << endl;
                }
                return value;
            }

            // Assignment operator (setter)
            T operator=(const T &new_value)
            {
                if (!name.empty())
                {
                    cout << "Property[" << name << "]::set(" << new_value << ")" << endl;
                }
                value = new_value;
                return value;
            }

            // Getter with logging
            T get() const
            {
                cout << "Property::get() accessed" << endl;
                return value;
            }

            // Setter with validation
            void set(const T &new_value)
            {
                cout << "Property::set() called with value: " << new_value << endl;
                value = new_value;
            }
        };

        // Class using property proxies
        class Creature
        {
        public:
            Property<int> strength{10, "strength"};
            Property<int> agility{5, "agility"};
            Property<int> health{100, "health"};

            void print_stats() const
            {
                cout << "Creature Stats: strength=" << strength << ", agility=" << agility
                     << ", health=" << health << endl;
            }
        };

    } // namespace property_proxy

    // ============================================================================
    // EXAMPLE 7: REMOTE PROXY SIMULATOR
    // ============================================================================

    namespace remote_proxy
    {

        // Interface for remote service
        struct RemoteService
        {
            virtual ~RemoteService() = default;
            virtual string get_data(const string &key) = 0;
            virtual void set_data(const string &key, const string &value) = 0;
        };

        // Local (remote proxy) implementation - simulates network calls
        class RemoteServiceProxy : public RemoteService
        {
        private:
            string server_address;
            int call_count = 0;

            string simulate_network_call(const string &operation)
            {
                call_count++;
                cout << "RemoteServiceProxy: Network call #" << call_count << " - " << operation << endl;
                // Simulate network delay
                return "remote_result";
            }

        public:
            explicit RemoteServiceProxy(const string &address)
                : server_address(address)
            {
                cout << "RemoteServiceProxy initialized for server: " << address << endl;
            }

            string get_data(const string &key) override
            {
                cout << "RemoteServiceProxy::get_data('" << key << "')" << endl;
                return simulate_network_call("GET " + key);
            }

            void set_data(const string &key, const string &value) override
            {
                cout << "RemoteServiceProxy::set_data('" << key << "', '" << value << "')" << endl;
                simulate_network_call("SET " + key + " = " + value);
            }

            int get_call_count() const
            {
                return call_count;
            }
        };

    } // namespace remote_proxy

    // ============================================================================
    // EXAMPLE 8: SYNCHRONIZATION PROXY (Thread-Safe)
    // ============================================================================

    namespace synchronization_proxy
    {

        // Generic proxy with synchronization
        class Counter
        {
        private:
            int value = 0;

        public:
            void increment()
            {
                value++;
                cout << "Counter incremented to: " << value << endl;
            }

            void decrement()
            {
                value--;
                cout << "Counter decremented to: " << value << endl;
            }

            int get_value() const
            {
                return value;
            }
        };

        // Synchronization proxy - ensures safe access (simplified without actual locks)
        class ThreadSafeCounterProxy
        {
        private:
            shared_ptr<Counter> counter;
            int access_count = 0;

        public:
            ThreadSafeCounterProxy()
                : counter(make_shared<Counter>()) {}

            void increment()
            {
                access_count++;
                cout << "ThreadSafeCounterProxy: Access #" << access_count << " (LOCK acquired)" << endl;
                counter->increment();
                cout << "ThreadSafeCounterProxy: (LOCK released)" << endl;
            }

            void decrement()
            {
                access_count++;
                cout << "ThreadSafeCounterProxy: Access #" << access_count << " (LOCK acquired)" << endl;
                counter->decrement();
                cout << "ThreadSafeCounterProxy: (LOCK released)" << endl;
            }

            int get_value() const
            {
                cout << "ThreadSafeCounterProxy: Reading value (read-only, no lock needed)" << endl;
                return counter->get_value();
            }

            int get_access_count() const
            {
                return access_count;
            }
        };

    } // namespace synchronization_proxy

    // ============================================================================
    // EXAMPLE 9: COMPOSITE PROXY (Multiple Proxies Stacked)
    // ============================================================================

    namespace composite_proxy
    {

        // Interface for API calls
        struct APIClient
        {
            virtual ~APIClient() = default;
            virtual string fetch_data(const string &endpoint) = 0;
        };

        // Real API client
        class RealAPIClient : public APIClient
        {
        public:
            string fetch_data(const string &endpoint) override
            {
                cout << "RealAPIClient: Making HTTP request to " << endpoint << endl;
                return "data_from_server";
            }
        };

        // Caching layer
        class CachingAPIProxy : public APIClient
        {
        private:
            shared_ptr<APIClient> next_client;
            map<string, string> cache;

        public:
            explicit CachingAPIProxy(shared_ptr<APIClient> client)
                : next_client(client) {}

            string fetch_data(const string &endpoint) override
            {
                if (cache.find(endpoint) != cache.end())
                {
                    cout << "CachingAPIProxy: Cache HIT for " << endpoint << endl;
                    return cache[endpoint];
                }

                cout << "CachingAPIProxy: Cache MISS - delegating to next layer" << endl;
                string result = next_client->fetch_data(endpoint);
                cache[endpoint] = result;
                return result;
            }
        };

        // Logging layer
        class LoggingAPIProxy : public APIClient
        {
        private:
            shared_ptr<APIClient> next_client;
            vector<string> request_log;

        public:
            explicit LoggingAPIProxy(shared_ptr<APIClient> client)
                : next_client(client) {}

            string fetch_data(const string &endpoint) override
            {
                cout << "LoggingAPIProxy: Logging request to " << endpoint << endl;
                request_log.push_back(endpoint);
                return next_client->fetch_data(endpoint);
            }

            const vector<string> &get_log() const
            {
                return request_log;
            }
        };

    } // namespace composite_proxy

} // namespace proxy_pattern

#endif // PROXY_PATTERN_H
