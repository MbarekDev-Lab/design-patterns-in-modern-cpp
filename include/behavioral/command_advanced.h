#pragma once
#include <vector>
#include <iostream>

using namespace std;

// ============================================================================
// SIMPLE VERSION: Data-Driven Command Pattern
// ============================================================================
// This is the simplest form - Command is just data, Account processes it
// ============================================================================
struct SimpleCommand
{
    enum Action
    {
        deposit,
        withdraw
    } action;
    int amount{0};
    bool success{false};
};

struct SimpleAccount
{
    int balance{0};

    void process(SimpleCommand &cmd)
    {
        switch (cmd.action)
        {
        case SimpleCommand::deposit:
            balance += cmd.amount;
            cmd.success = true;
            break;
        case SimpleCommand::withdraw:
            cmd.success = (balance >= cmd.amount);
            if (cmd.success)
                balance -= cmd.amount;
            break;
        }
    }
};

// ============================================================================
// INTERMEDIATE VERSION: Object-Oriented Command Pattern
// ============================================================================
// Commands are objects with virtual methods for call() and undo()
// ============================================================================
struct BankAccount
{
    int balance = 0;
    int overdraft_limit = -500;

    void deposit(int amount)
    {
        balance += amount;
        cout << "Deposited " << amount << ", balance now " << balance << "\n";
    }

    bool withdraw(int amount)
    {
        if (balance - amount >= overdraft_limit)
        {
            balance -= amount;
            cout << "Withdrew " << amount << ", balance now " << balance << "\n";
            return true;
        }
        return false;
    }
};

struct Command
{
    bool succeeded;
    virtual ~Command() = default;
    virtual void call() = 0;
    virtual void undo() = 0;
}; 

struct BankAccountCommand : Command
{
    BankAccount &account;
    enum Action
    {
        deposit,
        withdraw
    } action;
    int amount;

    BankAccountCommand(BankAccount &account, const Action action, const int amount)
        : account(account), action(action), amount(amount)
    {
        succeeded = false;
    }

    void call() override
    {
        switch (action)
        {
        case deposit:
            account.deposit(amount);
            succeeded = true;
            break;
        case withdraw:
            succeeded = account.withdraw(amount);
            break;
        }
    }

    void undo() override
    {
        if (!succeeded)
            return;

        switch (action)
        {
        case withdraw:
            account.deposit(amount);
            break;
        case deposit:
            account.withdraw(amount);
            break;
        }
    }
};

// ============================================================================
// INTERMEDIATE VERSION: Composite Commands
// ============================================================================
// Groups multiple commands and executes them together
// ============================================================================
struct CompositeBankAccountCommand : vector<BankAccountCommand>, Command
{
    CompositeBankAccountCommand(const initializer_list<value_type> &items)
        : vector<BankAccountCommand>(items)
    {
        succeeded = false;
    }

    void call() override
    {
        for (auto &cmd : *this)
            cmd.call();
        succeeded = true;
    }

    void undo() override
    {
        // Undo in reverse order!
        for (auto it = rbegin(); it != rend(); ++it)
            it->undo();
    }
};

// ============================================================================
// ADVANCED VERSION: Dependent Composite Commands
// ============================================================================
// Only executes subsequent commands if previous ones succeed (atomic behavior)
// ============================================================================
struct DependentCompositeCommand : CompositeBankAccountCommand
{
    explicit DependentCompositeCommand(const initializer_list<value_type> &items)
        : CompositeBankAccountCommand{items}
    {
    }

    void call() override
    {
        bool ok = true;
        for (auto &cmd : *this)
        {
            if (ok)
            {
                cmd.call();
                ok = cmd.succeeded;
            }
            else
            {
                cmd.succeeded = false;
            }
        }
        succeeded = ok;
    }
};

// ============================================================================
// ADVANCED VERSION: Money Transfer Command
// ============================================================================
// Atomic operation: withdraw from one account and deposit to another
// Uses DependentCompositeCommand to ensure both operations succeed or neither
// ============================================================================
struct MoneyTransferCommand : DependentCompositeCommand
{
    MoneyTransferCommand(BankAccount &from, BankAccount &to, int amount)
        : DependentCompositeCommand{
              BankAccountCommand{from, BankAccountCommand::withdraw, amount},
              BankAccountCommand{to, BankAccountCommand::deposit, amount}}
    {
    }
};
