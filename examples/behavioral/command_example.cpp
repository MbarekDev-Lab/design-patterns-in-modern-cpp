#include <iostream>
#include "../include/behavioral/command.h"

int main()
{
    cout << "=== COMMAND PATTERN: Bank Account Example ===" << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 1: Single Commands
    // ========================================================================
    cout << "--- EXAMPLE 1: Individual Commands ---" << endl;
    BankAccount account;
    cout << "Initial balance: " << account.balance << endl;
    cout << endl;

    BankAccountCommand cmd1(account, BankAccountCommand::deposit, 100);
    cout << "Execute: Deposit 100" << endl;
    cmd1.call();
    cout << "Current balance: " << account.balance << endl;
    cout << endl;

    BankAccountCommand cmd2(account, BankAccountCommand::withdraw, 50);
    cout << "Execute: Withdraw 50" << endl;
    cmd2.call();
    cout << "Current balance: " << account.balance << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 2: Undo Single Commands
    // ========================================================================
    cout << "--- EXAMPLE 2: Undo Individual Commands ---" << endl;
    cout << "Undo: Withdraw 50" << endl;
    cmd2.undo();
    cout << "Current balance: " << account.balance << endl;
    cout << endl;

    cout << "Undo: Deposit 100" << endl;
    cmd1.undo();
    cout << "Current balance: " << account.balance << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 3: Composite Commands (Batch Operations)
    // ========================================================================
    cout << "--- EXAMPLE 3: Composite Commands (Batch) ---" << endl;
    account.balance = 0; // Reset
    cout << "Reset balance to: " << account.balance << endl;
    cout << endl;

    cout << "Execute composite command: [Deposit 100, Withdraw 200]" << endl;
    CompositeBankAccountCommand batch1{
        BankAccountCommand{account, BankAccountCommand::deposit, 100},
        BankAccountCommand{account, BankAccountCommand::withdraw, 200}};
    batch1.call();
    cout << "Final balance: " << account.balance << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 4: Undo Composite Commands
    // ========================================================================
    cout << "--- EXAMPLE 4: Undo Composite Commands ---" << endl;
    cout << "Undo composite command (in reverse order)" << endl;
    batch1.undo();
    cout << "After undo balance: " << account.balance << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 5: Multiple Batches
    // ========================================================================
    cout << "--- EXAMPLE 5: Multiple Batches ---" << endl;
    account.balance = 0; // Reset
    cout << "Reset balance to: " << account.balance << endl;
    cout << endl;

    cout << "Execute Batch 1: [Deposit 500]" << endl;
    CompositeBankAccountCommand batch2{
        BankAccountCommand{account, BankAccountCommand::deposit, 500}};
    batch2.call();
    cout << "Balance after batch 1: " << account.balance << endl;
    cout << endl;

    cout << "Execute Batch 2: [Withdraw 100, Withdraw 200]" << endl;
    CompositeBankAccountCommand batch3{
        BankAccountCommand{account, BankAccountCommand::withdraw, 100},
        BankAccountCommand{account, BankAccountCommand::withdraw, 200}};
    batch3.call();
    cout << "Balance after batch 2: " << account.balance << endl;
    cout << endl;

    cout << "Undo Batch 2" << endl;
    batch3.undo();
    cout << "Balance after undo batch 2: " << account.balance << endl;
    cout << endl;

    cout << "Undo Batch 1" << endl;
    batch2.undo();
    cout << "Balance after undo batch 1: " << account.balance << endl;

    return 0;
}
