#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <memory>

using namespace std;

// ============================================================================
// TEST FRAMEWORK
// ============================================================================

int g_test_count = 0;
int g_passed = 0;
int g_assertion_count = 0;

void TEST(const string &name)
{
    g_test_count++;
    cout << "\nTest " << g_test_count << ": " << name << "\n";
}

void ASSERT_EQ(const string &actual, const string &expected)
{
    g_assertion_count++;
    if (actual == expected)
    {
        cout << "  ✓ ASSERT_EQ passed\n";
    }
    else
    {
        cout << "  ✗ ASSERT_EQ failed: expected '" << expected
             << "', got '" << actual << "'\n";
        throw runtime_error("Assertion failed");
    }
}

void ASSERT_EQ_INT(int actual, int expected)
{
    g_assertion_count++;
    if (actual == expected)
    {
        cout << "  ✓ ASSERT_EQ(" << actual << ") passed\n";
    }
    else
    {
        cout << "  ✗ ASSERT_EQ failed: expected " << expected
             << ", got " << actual << "\n";
        throw runtime_error("Assertion failed");
    }
}

void ASSERT_TRUE(bool condition)
{
    g_assertion_count++;
    if (condition)
    {
        cout << "  ✓ ASSERT_TRUE passed\n";
    }
    else
    {
        cout << "  ✗ ASSERT_TRUE failed\n";
        throw runtime_error("Assertion failed");
    }
}

void ASSERT_FALSE(bool condition)
{
    g_assertion_count++;
    if (!condition)
    {
        cout << "  ✓ ASSERT_FALSE passed\n";
    }
    else
    {
        cout << "  ✗ ASSERT_FALSE failed\n";
        throw runtime_error("Assertion failed");
    }
}

void TEST_END()
{
    try
    {
        // Test passed
        g_passed++;
    }
    catch (...)
    {
        // Test failed, already reported
    }
}

// ============================================================================
// PROTECTIVE PROXY PATTERN - IMPLEMENTATIONS
// ============================================================================

namespace protective_proxy_tests
{

    // ----- EXAMPLE 1: AGE-BASED ACCESS CONTROL -----

    class Person
    {
        friend class ResponsiblePerson;
        int age;

    public:
        Person(int age) : age(age) {}

        int get_age() const { return age; }
        void set_age(int age) { this->age = age; }

        string drink() const { return "drinking"; }
        string drive() const { return "driving"; }
        string drink_and_drive() const { return "driving while drunk"; }
    };

    class ResponsiblePerson
    {
    private:
        Person person;

    public:
        explicit ResponsiblePerson(const Person &person) : person(person) {}

        int get_age() const { return person.age; }
        void set_age(int value) { person.age = value; }

        string drink() const
        {
            return (person.get_age() >= 18) ? person.drink() : "too young";
        }

        string drive() const
        {
            return (person.get_age() >= 16) ? person.drive() : "too young";
        }

        string drink_and_drive() const
        {
            return "dead"; // Never allowed
        }
    };

    // ----- EXAMPLE 2: PERMISSION-BASED ACCESS CONTROL -----

    enum class Permission
    {
        READ,
        WRITE,
        DELETE_PERM,
        ADMIN
    };

    class Document
    {
    private:
        string content;
        bool is_deleted = false;

    public:
        explicit Document(const string &text) : content(text) {}

        string read() const
        {
            return is_deleted ? "[DELETED]" : content;
        }

        void write(const string &text)
        {
            content = text;
        }

        void delete_document()
        {
            is_deleted = true;
        }

        bool is_deleted_doc() const { return is_deleted; }
    };

    class ProtectedDocument
    {
    private:
        Document doc;
        Permission user_permission;

    public:
        ProtectedDocument(const Document &document, Permission perm)
            : doc(document), user_permission(perm) {}

        string read() const
        {
            if (user_permission == Permission::READ ||
                user_permission == Permission::WRITE ||
                user_permission == Permission::ADMIN)
            {
                return doc.read();
            }
            else
            {
                return "[ACCESS DENIED]";
            }
        }

        bool write(const string &text)
        {
            if (user_permission == Permission::WRITE ||
                user_permission == Permission::ADMIN)
            {
                doc.write(text);
                return true;
            }
            else
            {
                return false;
            }
        }

        bool delete_document()
        {
            if (user_permission == Permission::ADMIN)
            {
                doc.delete_document();
                return true;
            }
            else
            {
                return false;
            }
        }

        bool is_deleted() const { return doc.is_deleted_doc(); }
    };

    // ----- EXAMPLE 3: STATE-BASED ACCESS CONTROL -----

    enum class AccountStatus
    {
        ACTIVE,
        SUSPENDED,
        FROZEN
    };

    class BankAccount
    {
    private:
        double balance;
        AccountStatus status;

    public:
        BankAccount(double initial) : balance(initial), status(AccountStatus::ACTIVE) {}

        double withdraw(double amount)
        {
            balance -= amount;
            return balance;
        }

        double deposit(double amount)
        {
            balance += amount;
            return balance;
        }

        AccountStatus get_status() const { return status; }
        void set_status(AccountStatus s) { status = s; }
        double get_balance() const { return balance; }
    };

    class ProtectedBankAccount
    {
    private:
        BankAccount account;

    public:
        explicit ProtectedBankAccount(const BankAccount &acc) : account(acc) {}

        bool withdraw(double amount)
        {
            if (account.get_status() != AccountStatus::ACTIVE)
            {
                return false;
            }
            account.withdraw(amount);
            return true;
        }

        bool deposit(double amount)
        {
            if (account.get_status() == AccountStatus::FROZEN)
            {
                return false;
            }
            account.deposit(amount);
            return true;
        }

        double get_balance() const { return account.get_balance(); }
        AccountStatus get_status() const { return account.get_status(); }
        void set_status(AccountStatus s) { account.set_status(s); }
    };

    // ----- EXAMPLE 4: RESOURCE-BASED ACCESS CONTROL -----

    class SecretFile
    {
    private:
        string content;
        int access_count = 0;

    public:
        explicit SecretFile(const string &text) : content(text) {}

        string read()
        {
            access_count++;
            return content;
        }

        int get_access_count() const { return access_count; }
    };

    class LimitedAccessFile
    {
    private:
        SecretFile file;
        int max_accesses;
        int current_accesses = 0;

    public:
        LimitedAccessFile(const SecretFile &f, int max_acc)
            : file(f), max_accesses(max_acc) {}

        string read()
        {
            if (current_accesses >= max_accesses)
            {
                return "[ACCESS LIMIT EXCEEDED]";
            }
            current_accesses++;
            return file.read();
        }

        int get_remaining_accesses() const
        {
            return max_accesses - current_accesses;
        }
    };

    // ----- EXAMPLE 5: VALIDATION PROXY -----

    class Account
    {
    private:
        string username;
        string password;

    public:
        Account(const string &user, const string &pass)
            : username(user), password(pass) {}

        string get_username() const { return username; }
        string get_password() const { return password; }

        bool change_password(const string &old_pass, const string &new_pass)
        {
            if (password != old_pass)
                return false;
            password = new_pass;
            return true;
        }
    };

    class ValidatedAccount
    {
    private:
        Account account;

    public:
        explicit ValidatedAccount(const Account &acc) : account(acc) {}

        bool change_password(const string &old_pass, const string &new_pass)
        {
            if (old_pass.length() < 6)
                return false;
            if (new_pass.length() < 6)
                return false;
            if (old_pass == new_pass)
                return false;
            return account.change_password(old_pass, new_pass);
        }

        string get_username() const { return account.get_username(); }
    };

} // namespace protective_proxy_tests

// ============================================================================
// TEST CASES
// ============================================================================

using namespace protective_proxy_tests;

// ===== AGE-BASED TESTS =====

void test_age_based_too_young_to_drink()
{
    TEST("Age-based: Child cannot drink");
    Person p(10);
    ResponsiblePerson rp(p);
    ASSERT_EQ(rp.drink(), "too young");
    g_passed++;
}

void test_age_based_too_young_to_drive()
{
    TEST("Age-based: Child cannot drive");
    Person p(10);
    ResponsiblePerson rp(p);
    ASSERT_EQ(rp.drive(), "too young");
    g_passed++;
}

void test_age_based_can_drive_at_16()
{
    TEST("Age-based: 16-year-old can drive");
    Person p(16);
    ResponsiblePerson rp(p);
    ASSERT_EQ(rp.drive(), "driving");
    g_passed++;
}

void test_age_based_can_drink_at_18()
{
    TEST("Age-based: 18-year-old can drink and drive");
    Person p(18);
    ResponsiblePerson rp(p);
    ASSERT_EQ(rp.drink(), "drinking");
    ASSERT_EQ(rp.drive(), "driving");
    g_passed++;
}

void test_age_based_never_drink_and_drive()
{
    TEST("Age-based: Can never drink and drive (regardless of age)");
    Person p(30);
    ResponsiblePerson rp(p);
    ASSERT_EQ(rp.drink_and_drive(), "dead");
    g_passed++;
}

// ===== PERMISSION-BASED TESTS =====

void test_permission_read_only()
{
    TEST("Permission-based: READ permission allows reading");
    Document doc("Secret");
    ProtectedDocument reader(doc, Permission::READ);
    ASSERT_EQ(reader.read(), "Secret");
    ASSERT_FALSE(reader.write("new"));
    g_passed++;
}

void test_permission_write_only()
{
    TEST("Permission-based: WRITE permission allows reading and writing");
    Document doc("Secret");
    ProtectedDocument writer(doc, Permission::WRITE);
    ASSERT_EQ(writer.read(), "Secret");
    ASSERT_TRUE(writer.write("updated"));
    ASSERT_FALSE(writer.delete_document());
    g_passed++;
}

void test_permission_admin()
{
    TEST("Permission-based: ADMIN permission allows all operations");
    Document doc("Secret");
    ProtectedDocument admin(doc, Permission::ADMIN);
    ASSERT_EQ(admin.read(), "Secret");
    ASSERT_TRUE(admin.write("new data"));
    ASSERT_TRUE(admin.delete_document());
    ASSERT_TRUE(admin.is_deleted());
    g_passed++;
}

void test_permission_denied()
{
    TEST("Permission-based: DELETE_PERM cannot read");
    Document doc("Secret");
    ProtectedDocument reader(doc, Permission::DELETE_PERM);
    ASSERT_EQ(reader.read(), "[ACCESS DENIED]");
    g_passed++;
}

void test_permission_write_denied_for_read_only()
{
    TEST("Permission-based: READ permission blocks write");
    Document doc("Secret");
    ProtectedDocument reader(doc, Permission::READ);
    ASSERT_FALSE(reader.write("attempt"));
    g_passed++;
}

// ===== STATE-BASED TESTS =====

void test_state_active_allows_withdraw()
{
    TEST("State-based: ACTIVE account allows withdrawal");
    BankAccount account(1000);
    ProtectedBankAccount protected_account(account);
    ASSERT_TRUE(protected_account.withdraw(100));
    ASSERT_EQ_INT(static_cast<int>(protected_account.get_balance()), 900);
    g_passed++;
}

void test_state_active_allows_deposit()
{
    TEST("State-based: ACTIVE account allows deposit");
    BankAccount account(1000);
    ProtectedBankAccount protected_account(account);
    ASSERT_TRUE(protected_account.deposit(100));
    ASSERT_EQ_INT(static_cast<int>(protected_account.get_balance()), 1100);
    g_passed++;
}

void test_state_suspended_blocks_withdraw()
{
    TEST("State-based: SUSPENDED account blocks withdrawal");
    BankAccount account(1000);
    ProtectedBankAccount protected_account(account);
    protected_account.set_status(AccountStatus::SUSPENDED);
    ASSERT_FALSE(protected_account.withdraw(100));
    g_passed++;
}

void test_state_suspended_allows_deposit()
{
    TEST("State-based: SUSPENDED account allows deposit");
    BankAccount account(1000);
    ProtectedBankAccount protected_account(account);
    protected_account.set_status(AccountStatus::SUSPENDED);
    ASSERT_TRUE(protected_account.deposit(100));
    ASSERT_EQ_INT(static_cast<int>(protected_account.get_balance()), 1100);
    g_passed++;
}

void test_state_frozen_blocks_all()
{
    TEST("State-based: FROZEN account blocks all transactions");
    BankAccount account(1000);
    ProtectedBankAccount protected_account(account);
    protected_account.set_status(AccountStatus::FROZEN);
    ASSERT_FALSE(protected_account.withdraw(100));
    ASSERT_FALSE(protected_account.deposit(100));
    g_passed++;
}

// ===== RESOURCE-BASED TESTS =====

void test_resource_quota_allows_limited_access()
{
    TEST("Resource-based: Allows access within quota");
    SecretFile file("data");
    LimitedAccessFile limited(file, 3);
    ASSERT_EQ(limited.read(), "data");
    ASSERT_EQ(limited.read(), "data");
    g_passed++;
}

void test_resource_quota_blocks_excess()
{
    TEST("Resource-based: Blocks access beyond quota");
    SecretFile file("data");
    LimitedAccessFile limited(file, 2);
    limited.read();
    limited.read();
    ASSERT_EQ(limited.read(), "[ACCESS LIMIT EXCEEDED]");
    g_passed++;
}

void test_resource_remaining_count()
{
    TEST("Resource-based: Tracks remaining accesses");
    SecretFile file("data");
    LimitedAccessFile limited(file, 3);
    limited.read();
    ASSERT_EQ_INT(limited.get_remaining_accesses(), 2);
    limited.read();
    ASSERT_EQ_INT(limited.get_remaining_accesses(), 1);
    g_passed++;
}

void test_resource_zero_quota()
{
    TEST("Resource-based: Zero quota blocks immediately");
    SecretFile file("data");
    LimitedAccessFile limited(file, 0);
    ASSERT_EQ(limited.read(), "[ACCESS LIMIT EXCEEDED]");
    g_passed++;
}

void test_resource_quota_stays_at_limit()
{
    TEST("Resource-based: Remaining stays at 0 after limit");
    SecretFile file("data");
    LimitedAccessFile limited(file, 1);
    limited.read();
    ASSERT_EQ_INT(limited.get_remaining_accesses(), 0);
    limited.read();
    ASSERT_EQ_INT(limited.get_remaining_accesses(), 0);
    g_passed++;
}

// ===== VALIDATION TESTS =====

void test_validation_strong_password_allowed()
{
    TEST("Validation: Strong password accepted");
    Account account("user", "password1");
    ValidatedAccount validated(account);
    ASSERT_TRUE(validated.change_password("password1", "newpassword123"));
    g_passed++;
}

void test_validation_old_password_too_short()
{
    TEST("Validation: Old password too short rejected");
    Account account("user", "password1");
    ValidatedAccount validated(account);
    ASSERT_FALSE(validated.change_password("pass", "newpassword123"));
    g_passed++;
}

void test_validation_new_password_too_short()
{
    TEST("Validation: New password too short rejected");
    Account account("user", "password1");
    ValidatedAccount validated(account);
    ASSERT_FALSE(validated.change_password("password1", "pass"));
    g_passed++;
}

void test_validation_same_password_rejected()
{
    TEST("Validation: Same password rejected");
    Account account("user", "password1");
    ValidatedAccount validated(account);
    ASSERT_FALSE(validated.change_password("password1", "password1"));
    g_passed++;
}

void test_validation_multiple_rules()
{
    TEST("Validation: All validation rules enforce");
    Account account("user", "password1");
    ValidatedAccount validated(account);
    // Test wrong old password
    ASSERT_FALSE(validated.change_password("wrongpass", "newpassword123"));
    // Test short new password
    ASSERT_FALSE(validated.change_password("password1", "short"));
    // Test no change
    ASSERT_FALSE(validated.change_password("password1", "password1"));
    g_passed++;
}

// ============================================================================
// MAIN - RUN ALL TESTS
// ============================================================================

int main()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "PROTECTIVE PROXY PATTERN - TEST SUITE\n";
    cout << string(70, '=') << "\n";

    // Age-based tests
    test_age_based_too_young_to_drink();
    test_age_based_too_young_to_drive();
    test_age_based_can_drive_at_16();
    test_age_based_can_drink_at_18();
    test_age_based_never_drink_and_drive();

    // Permission-based tests
    test_permission_read_only();
    test_permission_admin();
    test_permission_denied();
    test_permission_write_denied_for_read_only();

    // State-based tests
    test_state_active_allows_withdraw();
    test_state_active_allows_deposit();
    test_state_suspended_blocks_withdraw();
    test_state_suspended_allows_deposit();
    test_state_frozen_blocks_all();

    // Resource-based tests
    test_resource_quota_allows_limited_access();
    test_resource_quota_blocks_excess();
    test_resource_remaining_count();
    test_resource_zero_quota();
    test_resource_quota_stays_at_limit();

    // Validation tests
    test_validation_strong_password_allowed();
    test_validation_old_password_too_short();
    test_validation_new_password_too_short();
    test_validation_same_password_rejected();
    test_validation_multiple_rules();

    // Print summary
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "TEST SUMMARY\n";
    cout << string(70, '=') << "\n";
    cout << "Tests Run:     " << g_test_count << "\n";
    cout << "Tests Passed:  " << g_passed << "\n";
    cout << "Tests Failed:  " << (g_test_count - g_passed) << "\n";
    cout << "Assertions:    " << g_assertion_count << "\n";

    if (g_test_count == g_passed)
    {
        cout << "Status:        ✅ ALL TESTS PASSED (" << g_passed << "/" << g_test_count << ")\n";
    }
    else
    {
        cout << "Status:        ❌ SOME TESTS FAILED\n";
    }

    cout << string(70, '=') << "\n\n";

    return (g_test_count == g_passed) ? 0 : 1;
}
