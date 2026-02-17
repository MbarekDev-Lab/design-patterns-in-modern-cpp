#include <iostream>
#include <creational/multiton.h>
#include <memory>
#include <vector>

using namespace std;
using namespace multiton_solution;

// ============================================================================
// EXAMPLE CLASSES
// ============================================================================

// Example 1: Database by Importance
class Database : public Multiton<Database, string>
{
public:
    Database(const string &name) : name_(name), connection_id_(++instance_count_)
    {
        cout << "Database [" << name << "] created (ID: " << connection_id_ << ")\n";
    }

    void query(const string &sql) const
    {
        cout << "  [" << name_ << "] Executing: " << sql << "\n";
    }

    string get_name() const { return name_; }

private:
    string name_;
    int connection_id_;
    static int instance_count_;
};
int Database::instance_count_ = 0;

// Example 2: Logger by Severity
enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class SeverityLogger : public EnumMultiton<SeverityLogger, LogLevel>
{
public:
    void log(const string &message) const
    {
        cout << "  [LOG] " << message << "\n";
    }

private:
    static int instance_count_;
};
int SeverityLogger::instance_count_ = 0;

// Example 3: Cache Manager
class CacheManager : public Multiton<CacheManager, string>
{
public:
    CacheManager(const string &type) : cache_type_(type), size_(0)
    {
        cout << "CacheManager [" << type << "] initialized\n";
    }

    void store(const string &key, const string &value)
    {
        cache_[key] = value;
        size_++;
        cout << "  [" << cache_type_ << "] Cached: " << key << " = " << value << "\n";
    }

    string retrieve(const string &key) const
    {
        auto it = cache_.find(key);
        if (it != cache_.end())
        {
            cout << "  [" << cache_type_ << "] Retrieved: " << key << " = " << it->second << "\n";
            return it->second;
        }
        cout << "  [" << cache_type_ << "] Cache miss: " << key << "\n";
        return "";
    }

    int size() const { return size_; }

private:
    string cache_type_;
    map<string, string> cache_;
    int size_;
};

// Example 4: Connection Pool
class ConnectionPool : public Multiton<ConnectionPool, string>
{
public:
    ConnectionPool(const string &name) : pool_name_(name), active_connections_(0)
    {
        cout << "ConnectionPool [" << name << "] created\n";
    }

    void acquire_connection()
    {
        active_connections_++;
        cout << "  [" << pool_name_ << "] Connection acquired (active: " << active_connections_ << ")\n";
    }

    void release_connection()
    {
        if (active_connections_ > 0)
            active_connections_--;
        cout << "  [" << pool_name_ << "] Connection released (active: " << active_connections_ << ")\n";
    }

    int get_active_count() const { return active_connections_; }

private:
    string pool_name_;
    int active_connections_;
};

// Example 5: Configuration by Profile
enum class Profile
{
    DEVELOPMENT,
    STAGING,
    PRODUCTION
};

class Configuration : public EnumMultiton<Configuration, Profile>
{
public:
    void set_value(const string &key, const string &value)
    {
        config_[key] = value;
        cout << "  Config set: " << key << " = " << value << "\n";
    }

    string get_value(const string &key) const
    {
        auto it = config_.find(key);
        return it != config_.end() ? it->second : "NOT_FOUND";
    }

private:
    map<string, string> config_;
};

// Example 6: Session Manager by User
class SessionManager : public Multiton<SessionManager, string>
{
public:
    SessionManager(const string &user) : user_(user), session_num_(++session_count_)
    {
        cout << "SessionManager for user [" << user << "] created (Session #" << session_num_ << ")\n";
    }

    void store_data(const string &key, const string &value)
    {
        session_data_[key] = value;
        cout << "  [" << user_ << "] Session data: " << key << " = " << value << "\n";
    }

    string get_data(const string &key) const
    {
        auto it = session_data_.find(key);
        return it != session_data_.end() ? it->second : "NOT_FOUND";
    }

private:
    string user_;
    int session_num_;
    map<string, string> session_data_;
    static int session_count_;
};
int SessionManager::session_count_ = 0;

// ============================================================================
// MAIN EXAMPLES
// ============================================================================

int main()
{
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║           MULTITON PATTERN                                ║\n";
    cout << "║  Each key maps to exactly one singleton instance          ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    // ========================================================================
    // EXAMPLE 1: Database Connections by Importance
    // ========================================================================
    {
        cout << "========== EXAMPLE 1: Databases by Importance ==========\n";
        cout << "Each importance level gets ONE database instance:\n\n";

        auto primary_db = Database::get("primary");
        auto secondary_db = Database::get("secondary");
        auto tertiary_db = Database::get("tertiary");

        cout << "\nRequesting 'primary' again:\n";
        auto primary_db2 = Database::get("primary");

        if (primary_db == primary_db2)
        {
            cout << "✓ Same instance returned for 'primary'\n";
        }

        cout << "\nExecuting queries:\n";
        primary_db->query("SELECT * FROM users");
        secondary_db->query("SELECT * FROM logs");
        tertiary_db->query("SELECT COUNT(*) FROM archive");

        cout << "\nBenefit: One instance per importance level, no duplicates!\n\n";
    }

    // ========================================================================
    // EXAMPLE 2: Logger by Severity Level
    // ========================================================================
    {
        cout << "========== EXAMPLE 2: Loggers by Severity ==========\n";
        cout << "Each severity level manages its own instance:\n\n";

        auto debug_logger = SeverityLogger::get(LogLevel::DEBUG);
        auto info_logger = SeverityLogger::get(LogLevel::INFO);
        auto warning_logger = SeverityLogger::get(LogLevel::WARNING);
        auto error_logger = SeverityLogger::get(LogLevel::ERROR);

        cout << "\nAccessing loggers:\n";
        cout << "Debug logger:";
        debug_logger->log("Debug information");

        cout << "Warning logger:";
        warning_logger->log("Warning condition detected");

        cout << "Getting ERROR logger again:\n";
        auto error_logger2 = SeverityLogger::get(LogLevel::ERROR);
        if (error_logger == error_logger2)
        {
            cout << "✓ Same ERROR logger instance returned\n";
        }

        cout << "\nTotal logger instances created: " << SeverityLogger::instance_count() << "\n";
        cout << "\nBenefit: Separate logger per severity, singleton per severity!\n\n";
    }

    // ========================================================================
    // EXAMPLE 3: Cache Managers
    // ========================================================================
    {
        cout << "========== EXAMPLE 3: Cache Managers ==========\n";
        cout << "Different cache types, one instance per type:\n\n";

        auto user_cache = CacheManager::get("user");
        auto session_cache = CacheManager::get("session");
        auto data_cache = CacheManager::get("data");

        cout << "\nStoring data in caches:\n";
        user_cache->store("id_1", "Alice");
        user_cache->store("id_2", "Bob");

        session_cache->store("token", "abc123xyz");
        session_cache->store("timeout", "3600");

        cout << "\nRetrieving from caches:\n";
        user_cache->retrieve("id_1");
        session_cache->retrieve("token");
        data_cache->retrieve("missing_key");

        cout << "\nRequest user_cache again:\n";
        auto user_cache2 = CacheManager::get("user");
        cout << "User cache size after reuse: " << user_cache2->size() << "\n";

        cout << "\nBenefit: Separate cache per type with independent data!\n\n";
    }

    // ========================================================================
    // EXAMPLE 4: Connection Pools
    // ========================================================================
    {
        cout << "========== EXAMPLE 4: Connection Pools ==========\n";
        cout << "Pool per service, maintains connection count:\n\n";

        auto web_pool = ConnectionPool::get("web");
        auto api_pool = ConnectionPool::get("api");
        auto batch_pool = ConnectionPool::get("batch");

        cout << "\nAcquiring connections from web pool:\n";
        web_pool->acquire_connection();
        web_pool->acquire_connection();

        cout << "\nAcquiring connections from api pool:\n";
        api_pool->acquire_connection();

        cout << "\nReleasing from web pool:\n";
        web_pool->release_connection();

        cout << "\nGetting same web pool again:\n";
        auto web_pool2 = ConnectionPool::get("web");
        cout << "Web pool active connections: " << web_pool2->get_active_count() << "\n";

        cout << "\nBenefit: Independent connection counts per service!\n\n";
    }

    // ========================================================================
    // EXAMPLE 5: Configuration by Profile
    // ========================================================================
    {
        cout << "========== EXAMPLE 5: Configuration by Profile ==========\n";
        cout << "Each environment has its own configuration:\n\n";

        auto dev_config = Configuration::get(Profile::DEVELOPMENT);
        auto prod_config = Configuration::get(Profile::PRODUCTION);

        cout << "\nSetting Development configuration:\n";
        dev_config->set_value("db_host", "localhost");
        dev_config->set_value("db_port", "5432");
        dev_config->set_value("debug", "true");

        cout << "\nSetting Production configuration:\n";
        prod_config->set_value("db_host", "prod-db.company.com");
        prod_config->set_value("db_port", "5433");
        prod_config->set_value("debug", "false");

        cout << "\nReading values:\n";
        cout << "  Dev DB host: " << dev_config->get_value("db_host") << "\n";
        cout << "  Prod DB host: " << prod_config->get_value("db_host") << "\n";

        cout << "\nGetting Development config again:\n";
        auto dev_config2 = Configuration::get(Profile::DEVELOPMENT);
        cout << "  Dev debug flag: " << dev_config2->get_value("debug") << "\n";

        cout << "\nBenefit: Isolated configuration per environment!\n\n";
    }

    // ========================================================================
    // EXAMPLE 6: Session Managers
    // ========================================================================
    {
        cout << "========== EXAMPLE 6: Session Managers ==========\n";
        cout << "One session per user, maintains user data:\n\n";

        auto alice_session = SessionManager::get("alice");
        auto bob_session = SessionManager::get("bob");
        auto charlie_session = SessionManager::get("charlie");

        cout << "\nStoring session data:\n";
        alice_session->store_data("cart_items", "3");
        alice_session->store_data("preference", "dark_mode");

        bob_session->store_data("cart_items", "1");
        bob_session->store_data("preference", "light_mode");

        cout << "\nRetrieving session data:\n";
        cout << "Alice cart items: " << alice_session->get_data("cart_items") << "\n";
        cout << "Bob cart items: " << bob_session->get_data("cart_items") << "\n";

        cout << "\nGetting Alice's session again:\n";
        auto alice_session2 = SessionManager::get("alice");
        cout << "Alice preference: " << alice_session2->get_data("preference") << "\n";

        cout << "\nBenefit: Per-user singleton sessions with isolated data!\n\n";
    }

    // ========================================================================
    // EXAMPLE 7: Real-world Scenario
    // ========================================================================
    {
        cout << "========== EXAMPLE 7: Real-world Multi-tenant Application ==========\n";
        cout << "Managing resources for multiple tenants:\n\n";

        // Each tenant gets exactly one database connection
        auto tenant_a_db = Database::get("tenant_a");
        auto tenant_b_db = Database::get("tenant_b");

        // Each tenant gets exactly one cache
        auto tenant_a_cache = CacheManager::get("tenant_a");
        auto tenant_b_cache = CacheManager::get("tenant_b");

        cout << "Tenant A operations:\n";
        tenant_a_db->query("SELECT * FROM tenant_a.users");
        tenant_a_cache->store("company", "ACME Corp");

        cout << "\nTenant B operations:\n";
        tenant_b_db->query("SELECT * FROM tenant_b.users");
        tenant_b_cache->store("company", "Widget Inc");

        cout << "\nTenant A operations (reusing instances):\n";
        auto tenant_a_db2 = Database::get("tenant_a");
        auto tenant_a_cache2 = CacheManager::get("tenant_a");
        cout << "Cache still has: " << tenant_a_cache2->retrieve("company") << "\n";

        cout << "\nBenefit: Multi-tenant support with automatic resource management!\n\n";
    }

    // ========================================================================
    // KEY TAKEAWAYS
    // ========================================================================
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                    KEY TAKEAWAYS                           ║\n";
    cout << "├────────────────────────────────────────────────────────────┤\n";
    cout << "│ 1. Multiton extends Singleton to manage multiple instances │\n";
    cout << "│ 2. Each key maps to exactly one singleton instance         │\n";
    cout << "│ 3. Perfect for managing pools of resources by category     │\n";
    cout << "│ 4. Useful in multi-tenant, multi-environment scenarios     │\n";
    cout << "│ 5. Provides both singleton benefits and pooling benefits   │\n";
    cout << "│ 6. Can use string or enum keys based on requirements       │\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    return 0;
}
