#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// ============================================================================
// BankAccount - Target object for commands to operate on
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

    void withdraw(int amount)
    {
        if (balance - amount >= overdraft_limit)
        {
            balance -= amount;
            cout << "Withdrew " << amount << ", balance now " << balance << "\n";
        }
        else
        {
            cout << "Cannot withdraw " << amount << ", would exceed overdraft limit\n";
        }
    }
};

// ============================================================================
// Command - Abstract Interface
// ============================================================================
// The Command pattern encapsulates requests as objects.
// Each command can be executed (call) and undone (undo).
// ============================================================================
struct Command
{
    virtual ~Command() = default;
    virtual void call() const = 0;
    virtual void undo() const = 0;
};

// ============================================================================
// BankAccountCommand - Concrete Command
// ============================================================================
// Encapsulates a deposit or withdraw operation on a BankAccount.
// Stores all the information needed to perform and undo the operation.
// ============================================================================
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
    }

    void call() const override
    {
        switch (action)
        {
        case deposit:
            account.deposit(amount);
            break;
        case withdraw:
            account.withdraw(amount);
            break;
        default:
            break;
        }
    }

    void undo() const override
    {
        switch (action)
        {
        case withdraw:
            account.deposit(amount); // Undo withdraw by depositing
            break;
        case deposit:
            account.withdraw(amount); // Undo deposit by withdrawing
            break;
        default:
            break;
        }
    }
};

// ============================================================================
// CompositeBankAccountCommand - Composite Command
// ============================================================================
// Groups multiple commands together.
// Calling execute() runs all commands in sequence.
// Calling undo() undoes all commands in reverse order.
// This combines the Command pattern with the Composite pattern.
// ============================================================================
struct CompositeBankAccountCommand : vector<BankAccountCommand>, Command
{
    CompositeBankAccountCommand(const initializer_list<value_type> &items)
        : vector<BankAccountCommand>(items)
    {
    }

    void call() const override
    {
        for (auto &cmd : *this)
            cmd.call();
    }

    void undo() const override
    {
        // Undo in reverse order to properly undo all operations
        for (auto it = rbegin(); it != rend(); ++it)
            it->undo();
    }
};
