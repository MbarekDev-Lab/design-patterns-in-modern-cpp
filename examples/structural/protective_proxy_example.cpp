#include <iostream>
#include <string>
#include <memory>
#include <cassert>

#include "structural/proxy.h"

using namespace std;

/*
 * PROTECTIVE PROXY PATTERN EXAMPLES
 *
 * A Protective Proxy controls access to an object based on:
 * - User permissions
 * - Age restrictions
 * - State conditions
 * - Business rules
 * - Resource availability
 */

namespace protective_proxy
{

    // ============================================================================
    // EXAMPLE 1: AGE-BASED ACCESS CONTROL (From your example)
    // ============================================================================

    namespace age_restricted
    {

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

        // Protective proxy - controls access based on age
        class ResponsiblePerson
        {
        private:
            Person person;

        public:
            explicit ResponsiblePerson(const Person &person) : person(person) {}

            int get_age() const { return person.age; }
            void set_age(int value) { person.age = value; }

            // Protected method: only if 18+
            string drink() const
            {
                if (person.get_age() >= 18)
                {
                    return person.drink();
                }
                else
                {
                    return "too young";
                }
            }

            // Protected method: only if 16+
            string drive() const
            {
                if (person.get_age() >= 16)
                {
                    return person.drive();
                }
                else
                {
                    return "too young";
                }
            }

            // Always blocked
            string drink_and_drive() const
            {
                return "dead"; // Never allowed
            }
        };

    } // namespace age_restricted

    // ============================================================================
    // EXAMPLE 2: PERMISSION-BASED ACCESS CONTROL
    // ============================================================================

    namespace permission_based
    {

        enum class Permission
        {
            READ,
            WRITE,
            DELETE,
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
                if (is_deleted)
                    return "[DELETED]";
                return content;
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

        // Protective proxy with permission checking
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
                    cout << "Write permission denied" << endl;
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
                    cout << "Delete permission denied" << endl;
                    return false;
                }
            }

            bool is_deleted() const { return doc.is_deleted_doc(); }
        };

    } // namespace permission_based

    // ============================================================================
    // EXAMPLE 3: STATE-BASED ACCESS CONTROL
    // ============================================================================

    namespace state_based
    {

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

        // Protective proxy - controls access based on account status
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
                    cout << "Account is " << (account.get_status() == AccountStatus::SUSPENDED ? "suspended" : "frozen") << endl;
                    return false;
                }
                account.withdraw(amount);
                return true;
            }

            bool deposit(double amount)
            {
                if (account.get_status() == AccountStatus::FROZEN)
                {
                    cout << "Account is frozen - no transactions allowed" << endl;
                    return false;
                }
                account.deposit(amount);
                return true;
            }

            double get_balance() const { return account.get_balance(); }
            AccountStatus get_status() const { return account.get_status(); }
            void set_status(AccountStatus s) { account.set_status(s); }
        };

    } // namespace state_based

    // ============================================================================
    // EXAMPLE 4: RESOURCE-BASED ACCESS CONTROL
    // ============================================================================

    namespace resource_based
    {

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
                cout << "SecretFile: Access #" << access_count << endl;
                return content;
            }

            int get_access_count() const { return access_count; }
        };

        // Protective proxy - controls access based on resource quota
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
                    cout << "LimitedAccessFile: Access limit exceeded ("
                         << current_accesses << "/" << max_accesses << ")" << endl;
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

    } // namespace resource_based

    // ============================================================================
    // EXAMPLE 5: VALIDATION PROXY
    // ============================================================================

    namespace validation_proxy
    {

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

        // Protective proxy - validates input before delegating
        class ValidatedAccount
        {
        private:
            Account account;

        public:
            explicit ValidatedAccount(const Account &acc) : account(acc) {}

            bool change_password(const string &old_pass, const string &new_pass)
            {
                // Validate inputs
                if (old_pass.length() < 6)
                {
                    cout << "ValidatedAccount: Old password too short" << endl;
                    return false;
                }
                if (new_pass.length() < 6)
                {
                    cout << "ValidatedAccount: New password too short" << endl;
                    return false;
                }
                if (old_pass == new_pass)
                {
                    cout << "ValidatedAccount: New password same as old" << endl;
                    return false;
                }

                // Perform the change
                return account.change_password(old_pass, new_pass);
            }

            string get_username() const { return account.get_username(); }
        };

    } // namespace validation_proxy

} // namespace protective_proxy

// ============================================================================
// EXAMPLES
// ============================================================================

void example1_age_based_access()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 1: AGE-BASED ACCESS CONTROL\n";
    cout << string(70, '=') << "\n\n";

    using namespace protective_proxy::age_restricted;

    Person p(10);
    ResponsiblePerson rp(p);

    cout << "Person age: " << rp.get_age() << "\n";
    cout << "Can drink? " << rp.drink() << "\n";
    cout << "Can drive? " << rp.drive() << "\n";
    cout << "Can drink and drive? " << rp.drink_and_drive() << "\n\n";

    rp.set_age(18);
    cout << "After setting age to 18:\n";
    cout << "Can drink? " << rp.drink() << "\n";
    cout << "Can drive? " << rp.drive() << "\n\n";

    cout << "BENEFIT: Age-based business rules enforced by proxy\n";
}

void example2_permission_based_access()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 2: PERMISSION-BASED ACCESS CONTROL\n";
    cout << string(70, '=') << "\n\n";

    using namespace protective_proxy::permission_based;

    Document doc("Secret data");

    cout << "User with READ permission:\n";
    ProtectedDocument reader(doc, Permission::READ);
    cout << "  Read: " << reader.read() << "\n";
    cout << "  Write: " << (reader.write("new data") ? "success" : "denied") << "\n\n";

    cout << "User with WRITE permission:\n";
    ProtectedDocument writer(doc, Permission::WRITE);
    cout << "  Read: " << writer.read() << "\n";
    cout << "  Write: " << (writer.write("updated") ? "success" : "denied") << "\n\n";

    cout << "User with ADMIN permission:\n";
    ProtectedDocument admin(doc, Permission::ADMIN);
    cout << "  Delete: " << (admin.delete_document() ? "success" : "denied") << "\n\n";

    cout << "BENEFIT: Fine-grained access control based on permissions\n";
}

void example3_state_based_access()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 3: STATE-BASED ACCESS CONTROL\n";
    cout << string(70, '=') << "\n\n";

    using namespace protective_proxy::state_based;

    BankAccount account(1000);
    ProtectedBankAccount protected_account(account);

    cout << "Account ACTIVE - balance: " << protected_account.get_balance() << "\n";
    cout << "Withdraw $100: " << (protected_account.withdraw(100) ? "success" : "failed") << "\n";
    cout << "New balance: " << protected_account.get_balance() << "\n\n";

    protected_account.set_status(AccountStatus::SUSPENDED);
    cout << "Account SUSPENDED:\n";
    cout << "Withdraw $100: " << (protected_account.withdraw(100) ? "success" : "failed") << "\n";
    cout << "Deposit $100: " << (protected_account.deposit(100) ? "success" : "failed") << "\n\n";

    protected_account.set_status(AccountStatus::FROZEN);
    cout << "Account FROZEN:\n";
    cout << "Deposit $100: " << (protected_account.deposit(100) ? "success" : "failed") << "\n\n";

    cout << "BENEFIT: Access control based on object state\n";
}

void example4_resource_based_access()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 4: RESOURCE-BASED ACCESS CONTROL (Quota)\n";
    cout << string(70, '=') << "\n\n";

    using namespace protective_proxy::resource_based;

    SecretFile file("Confidential data");
    LimitedAccessFile limited(file, 3);

    cout << "Accessing file (limit: 3 accesses):\n";
    for (int i = 0; i < 5; ++i)
    {
        cout << "Access " << (i + 1) << ": " << limited.read() << "\n";
        cout << "Remaining accesses: " << limited.get_remaining_accesses() << "\n\n";
    }

    cout << "BENEFIT: Resource quotas enforced transparently\n";
}

void example5_validation_proxy()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "EXAMPLE 5: VALIDATION PROXY\n";
    cout << string(70, '=') << "\n\n";

    using namespace protective_proxy::validation_proxy;

    Account account("user123", "password1");
    ValidatedAccount validated(account);

    cout << "User: " << validated.get_username() << "\n\n";

    cout << "Attempt 1: Old password too short\n";
    cout << "Result: " << (validated.change_password("pass", "newpass123") ? "success" : "failed") << "\n\n";

    cout << "Attempt 2: New password too short\n";
    cout << "Result: " << (validated.change_password("password1", "pass") ? "success" : "failed") << "\n\n";

    cout << "Attempt 3: New password same as old\n";
    cout << "Result: " << (validated.change_password("password1", "password1") ? "success" : "failed") << "\n\n";

    cout << "Attempt 4: Valid password change\n";
    cout << "Result: " << (validated.change_password("password1", "newpassword123") ? "success" : "failed") << "\n\n";

    cout << "BENEFIT: Input validation before operation\n";
}

void example_comparison()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "PROTECTIVE PROXY PATTERNS - COMPARISON\n";
    cout << string(70, '=') << "\n\n";

    cout << "1. AGE-BASED ACCESS:\n";
    cout << "   - Controls based on numerical threshold\n";
    cout << "   - Example: Legal age requirements\n\n";

    cout << "2. PERMISSION-BASED ACCESS:\n";
    cout << "   - Controls based on user role/permission\n";
    cout << "   - Example: READ, WRITE, ADMIN permissions\n\n";

    cout << "3. STATE-BASED ACCESS:\n";
    cout << "   - Controls based on object state\n";
    cout << "   - Example: Account ACTIVE/SUSPENDED/FROZEN\n\n";

    cout << "4. RESOURCE-BASED ACCESS:\n";
    cout << "   - Controls based on quota/resource availability\n";
    cout << "   - Example: Access limits, rate limits\n\n";

    cout << "5. VALIDATION PROXY:\n";
    cout << "   - Validates inputs before operation\n";
    cout << "   - Example: Password strength, format validation\n\n";

    cout << "KEY CHARACTERISTICS OF PROTECTIVE PROXIES:\n";
    cout << "✓ Same interface as original object\n";
    cout << "✓ Enforces business rules transparently\n";
    cout << "✓ Prevents invalid operations\n";
    cout << "✓ Returns meaningful error messages\n";
    cout << "✓ Decouples rules from object logic\n";
}

int main()
{
    cout << "\n"
         << string(70, '*') << "\n";
    cout << "PROTECTIVE PROXY PATTERN - EXAMPLES\n";
    cout << string(70, '*') << "\n";

    example1_age_based_access();
    example2_permission_based_access();
    example3_state_based_access();
    example4_resource_based_access();
    example5_validation_proxy();
    example_comparison();

    cout << "\n"
         << string(70, '*') << "\n";
    cout << "END OF EXAMPLES\n";
    cout << string(70, '*') << "\n\n";

    return 0;
}
