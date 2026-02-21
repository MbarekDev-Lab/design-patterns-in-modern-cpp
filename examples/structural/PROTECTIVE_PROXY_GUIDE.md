# Protective Proxy Pattern

## Overview

A **Protective Proxy** (also called an **Access Control Proxy** or **Guard Proxy**) is a structural design pattern that controls access to another object by enforcing security rules, permissions, or validity constraints before delegating operations to the real subject.

The proxy acts as a barrier that:

- **Validates** requests before passing them to the real object
- **Enforces** business rules transparently
- **Prevents** invalid operations
- **Provides** meaningful feedback when access is denied

## Motivation

In real-world applications, objects often have access restrictions based on:

- User permissions (admin, read-only, write, delete)
- Entity state (active, suspended, frozen, disabled)
- Age or eligibility requirements
- Resource quotas or limits
- Business rule compliance
- Input validation requirements

Direct access to the object would require putting all this logic inside the object itself, violating the Single Responsibility Principle. A Protective Proxy separates access control from business logic.

## Structure

```
Client
  |
  v
┌─────────────────────────────┐
│  ProtectiveProxy            │
│  ┌───────────────────────┐  │  Validates requests
│  │ - Check permissions  │  │  and enforces rules
│  │ - Validate state     │  │
│  │ - Check quotas       │  │
│  └───────────────────────┘  │
└─────────────────────────────┘
  |
  | (delegates if valid)
  v
┌─────────────────────────────┐
│  RealSubject                │
│                             │
│  Performs actual work       │
└─────────────────────────────┘
```

## Implementation Patterns

### 1. Age-Based Access Control (Protective Proxy)

Controls access based on numerical thresholds:

```cpp
class Person {
    friend class ResponsiblePerson;
    int age;
public:
    Person(int age) : age(age) {}
    int get_age() const { return age; }
    string drink() const { return "drinking"; }
    string drive() const { return "driving"; }
};

class ResponsiblePerson {  // Protective Proxy
private:
    Person person;
public:
    ResponsiblePerson(const Person& p) : person(p) {}

    string drink() const {
        return (person.get_age() >= 18)
            ? person.drink()
            : "too young";
    }

    string drive() const {
        return (person.get_age() >= 16)
            ? person.drive()
            : "too young";
    }
};
```

**Use Cases:**

- Age-based activities
- Minimum experience requirements
- Seniority-based access
- Performance threshold gating

---

### 2. Permission-Based Access Control

Controls operations based on user roles or permissions:

```cpp
enum class Permission { READ, WRITE, DELETE_PERM, ADMIN };

class Document {
    string content;
    bool is_deleted = false;
public:
    string read() const { return is_deleted ? "[DELETED]" : content; }
    void write(const string& text) { content = text; }
    void delete_document() { is_deleted = true; }
};

class ProtectedDocument {  // Protective Proxy
private:
    Document doc;
    Permission user_permission;
public:
    ProtectedDocument(const Document& d, Permission p)
        : doc(d), user_permission(p) {}

    string read() const {
        if (user_permission == Permission::READ ||
            user_permission == Permission::ADMIN) {
            return doc.read();
        }
        return "[ACCESS DENIED]";
    }

    bool write(const string& text) {
        if (user_permission == Permission::WRITE ||
            user_permission == Permission::ADMIN) {
            doc.write(text);
            return true;
        }
        return false;
    }
};
```

**Use Cases:**

- Document management systems
- File permissions
- API endpoint authorization
- Role-based access control (RBAC)

---

### 3. State-Based Access Control

Controls access based on the object's current state:

```cpp
enum class AccountStatus { ACTIVE, SUSPENDED, FROZEN };

class BankAccount {
    double balance;
    AccountStatus status;
public:
    double withdraw(double amount) { balance -= amount; return balance; }
    double deposit(double amount) { balance += amount; return balance; }
    AccountStatus get_status() const { return status; }
};

class ProtectedBankAccount {  // Protective Proxy
private:
    BankAccount account;
public:
    bool withdraw(double amount) {
        if (account.get_status() != AccountStatus::ACTIVE) {
            return false;
        }
        account.withdraw(amount);
        return true;
    }

    bool deposit(double amount) {
        if (account.get_status() == AccountStatus::FROZEN) {
            return false;
        }
        account.deposit(amount);
        return true;
    }
};
```

**Use Cases:**

- Account states (active, suspended, locked, archived)
- Workflow state transitions
- Game entity states (alive, dead, respawning)
- Service availability states

---

### 4. Resource-Based Access Control (Quota)

Controls access based on resource availability or quota:

```cpp
class SecretFile {
    string content;
    int access_count = 0;
public:
    string read() {
        access_count++;
        return content;
    }
};

class LimitedAccessFile {  // Protective Proxy
private:
    SecretFile file;
    int max_accesses;
    int current_accesses = 0;
public:
    LimitedAccessFile(const SecretFile& f, int max)
        : file(f), max_accesses(max) {}

    string read() {
        if (current_accesses >= max_accesses) {
            return "[ACCESS LIMIT EXCEEDED]";
        }
        current_accesses++;
        return file.read();
    }
};
```

**Use Cases:**

- API rate limiting
- File access quotas
- Database connection pools
- Resource consumption limits

---

### 5. Validation Proxy

Validates input before delegating operations:

```cpp
class Account {
    string username;
    string password;
public:
    Account(const string& user, const string& pass)
        : username(user), password(pass) {}

    bool change_password(const string& old_pass, const string& new_pass) {
        if (password != old_pass) return false;
        password = new_pass;
        return true;
    }
};

class ValidatedAccount {  // Protective Proxy
private:
    Account account;
public:
    bool change_password(const string& old_p, const string& new_p) {
        // Validate all inputs before delegating
        if (old_p.length() < 6) return false;
        if (new_p.length() < 6) return false;
        if (old_p == new_p) return false;

        return account.change_password(old_p, new_p);
    }
};
```

**Use Cases:**

- Input format validation
- Business rule validation
- Precondition checking
- Data sanitization

---

## Key Benefits

1. **Separation of Concerns**: Access control logic separate from business logic
2. **Transparency**: Same interface as real object (client code unchanged)
3. **Flexibility**: Easy to change access rules without modifying the real object
4. **Enforcement**: Guarantees rules are always applied
5. **Auditability**: Can log all access attempts (successful and failed)
6. **Maintainability**: Centralized access control strategy

## Key Trade-offs

1. **Additional Indirection**: One extra object in the call chain
2. **Performance**: Access checks add minimal overhead
3. **Complexity**: More classes to maintain
4. **Testing**: Need to test both valid and invalid scenarios

## Variations & Extensions

### Logging Protective Proxy

Extends protection with audit trails:

```cpp
class LoggingProtectedDocument {
private:
    ProtectedDocument doc;
    vector<string> access_log;
public:
    bool write(const string& text) {
        bool result = doc.write(text);
        access_log.push_back(result ? "WRITE ALLOWED" : "WRITE DENIED");
        return result;
    }
};
```

### Caching Protective Proxy

Combines permission checks with result caching:

```cpp
class CachingProtectedFile {
private:
    LimitedAccessFile file;
    map<string, string> cache;
public:
    string read() {
        if (cache.count("data")) {
            return cache["data"];  // Cache hit
        }
        string result = file.read();
        if (result != "[ACCESS LIMIT EXCEEDED]") {
            cache["data"] = result;
        }
        return result;
    }
};
```

### Composite Protective Proxy

Stacks multiple protection layers:

```cpp
class MultiLayerProtection {
    Document doc;
    Permission user_perm;
    int access_limit;
    int accesses = 0;
public:
    string read() {
        // Check permission layer
        if (user_perm == Permission::READ ||
            user_perm == Permission::ADMIN) {

            // Check quota layer
            if (accesses < access_limit) {
                accesses++;
                return doc.read();
            }
        }
        return "[ACCESS DENIED]";
    }
};
```

## Comparison with Other Patterns

| Pattern              | Purpose              | When to Use               |
| -------------------- | -------------------- | ------------------------- |
| **Protective Proxy** | Control access       | Enforce permissions/rules |
| Virtual Proxy        | Lazy loading         | Defer expensive creation  |
| Remote Proxy         | Network transparency | Distribute across network |
| Facade               | Simplify interface   | Hide complexity           |
| Decorator            | Add responsibilities | Extend capabilities       |

## Real-World Examples

### 1. Database Connection Pooling

```cpp
class DatabaseConnection {
    void execute(const string& query) { /* ... */ }
};

class PooledConnection {  // Protective Proxy
private:
    DatabaseConnection conn;
    bool is_available = true;
public:
    bool execute(const string& query) {
        if (!is_available) return false;
        conn.execute(query);
        return true;
    }
};
```

### 2. API Endpoint Authorization

```cpp
class ApiEndpoint {
    string process_request(const Request& r) { /* ... */ }
};

class AuthorizedEndpoint {  // Protective Proxy
private:
    ApiEndpoint endpoint;
    Permission user_perm;
public:
    string process_request(const Request& r) {
        if (!user_perm.can_access(r.resource)) {
            return "{\"error\": \"Unauthorized\"}";
        }
        return endpoint.process_request(r);
    }
};
```

### 3. Game Entity Protection

```cpp
class Player {
    void take_damage(int amount) { hp -= amount; }
    bool die() { return hp <= 0; }
};

class ProtectedPlayer {  // Protective Proxy
private:
    Player player;
    bool is_invincible = false;
public:
    void take_damage(int amount) {
        if (!is_invincible) {
            player.take_damage(amount);
        }
    }
};
```

## Implementation Checklist

- [x] Create RealSubject class with core functionality
- [x] Create Proxy class with same interface
- [x] Implement access control logic in Proxy
- [x] Proxy delegates to RealSubject only if valid
- [x] Handle invalid access gracefully
- [x] Test valid access paths
- [x] Test invalid access paths
- [x] Consider logging/auditing
- [x] Document access rules

## Testing Considerations

1. **Valid access**: Permission granted → operation succeeds
2. **Invalid access**: Permission denied → operation fails
3. **Edge cases**: Boundary conditions (age=16, age=18, etc.)
4. **Error messages**: Clear feedback on denial reasons
5. **State transitions**: Test all state changes
6. **Quota limits**: Test at limit and beyond limit

## Common Mistakes to Avoid

1. **Forgetting to check all conditions**: Incomplete permission checks
2. **Returning wrong error messages**: Generic vs. specific feedback
3. **State not updated properly**: Incorrect internal state after proxy operations
4. **Exception handling**: Not catching delegation failures
5. **Performance**: Expensive permission checks in hot paths

## Summary

The Protective Proxy Pattern is essential for implementing access control and business rule enforcement without polluting the real object with security logic. It's particularly valuable in:

- **Enterprise systems** with complex permission models
- **Financial applications** with state-based restrictions
- **Multi-user systems** requiring role-based access control
- **APIs** requiring authorization before operations
- **Resource-constrained environments** with quotas and limits

The pattern shines when you need to maintain security and business rules while keeping your core objects focused on their primary responsibility.
