#include <iostream>
#include <cassert>
#include "../include/behavioral/command_advanced.h"

int main()
{
    cout << "=== COMMAND PATTERN: Simple Version ===" << endl;
    cout << endl;

    // === Simple Command Pattern (Data-Driven) ===
    cout << "--- Simple Command Pattern (Data-Driven) ---" << endl;
    SimpleAccount sa;
    cout << "Initial balance: " << sa.balance << endl;
    cout << endl;

    SimpleCommand cmd1{SimpleCommand::deposit, 100};
    cout << "Execute: Deposit 100" << endl;
    sa.process(cmd1);
    cout << "Success: " << (cmd1.success ? "true" : "false") << endl;
    cout << "Balance: " << sa.balance << endl;
    cout << endl;

    SimpleCommand cmd2{SimpleCommand::withdraw, 50};
    cout << "Execute: Withdraw 50" << endl;
    sa.process(cmd2);
    cout << "Success: " << (cmd2.success ? "true" : "false") << endl;
    cout << "Balance: " << sa.balance << endl;
    cout << endl;

    // Failed command
    SimpleCommand cmd3{SimpleCommand::withdraw, 100};
    cout << "Execute: Withdraw 100 (should fail)" << endl;
    sa.process(cmd3);
    cout << "Success: " << (cmd3.success ? "true" : "false") << endl;
    cout << "Balance: " << sa.balance << endl;
    cout << endl;

    // === Object-Oriented Command Pattern ===
    cout << "--- Object-Oriented Command Pattern with Undo ---" << endl;
    BankAccount ba;
    cout << "Initial balance: " << ba.balance << endl;
    cout << endl;

    BankAccountCommand oo_cmd1(ba, BankAccountCommand::deposit, 200);
    cout << "Execute: Deposit 200" << endl;
    oo_cmd1.call();
    cout << "Balance: " << ba.balance << endl;
    cout << endl;

    BankAccountCommand oo_cmd2(ba, BankAccountCommand::withdraw, 100);
    cout << "Execute: Withdraw 100" << endl;
    oo_cmd2.call();
    cout << "Balance: " << ba.balance << endl;
    cout << endl;

    cout << "Undo: Withdraw 100" << endl;
    oo_cmd2.undo();
    cout << "Balance: " << ba.balance << endl;
    cout << endl;

    // === Composite Commands ===
    cout << "--- Composite Commands ---" << endl;
    BankAccount ba2;
    cout << "Initial balance: " << ba2.balance << endl;
    cout << endl;

    cout << "Execute composite: [Deposit 500, Withdraw 200, Withdraw 100]" << endl;
    CompositeBankAccountCommand composite{
        BankAccountCommand{ba2, BankAccountCommand::deposit, 500},
        BankAccountCommand{ba2, BankAccountCommand::withdraw, 200},
        BankAccountCommand{ba2, BankAccountCommand::withdraw, 100}};
    composite.call();
    cout << "Final balance: " << ba2.balance << endl;
    cout << endl;

    cout << "Undo composite (in reverse order)" << endl;
    composite.undo();
    cout << "After undo balance: " << ba2.balance << endl;
    cout << endl;

    // === Dependent Composite Commands ===
    cout << "--- Dependent Composite Commands (Atomic Behavior) ---" << endl;
    BankAccount ba3;
    cout << "Initial balance: " << ba3.balance << endl;
    cout << endl;

    cout << "Execute dependent: [Deposit 300, Withdraw 200]" << endl;
    DependentCompositeCommand dependent1{
        BankAccountCommand{ba3, BankAccountCommand::deposit, 300},
        BankAccountCommand{ba3, BankAccountCommand::withdraw, 200}};
    dependent1.call();
    cout << "Balance: " << ba3.balance << endl;
    cout << "Overall success: " << (dependent1.succeeded ? "true" : "false") << endl;
    cout << endl;

    cout << "Execute dependent: [Deposit 100, Withdraw 500] (2nd will fail)" << endl;
    DependentCompositeCommand dependent2{
        BankAccountCommand{ba3, BankAccountCommand::deposit, 100},
        BankAccountCommand{ba3, BankAccountCommand::withdraw, 500}};
    dependent2.call();
    cout << "Balance: " << ba3.balance << "(unchanged due to 2nd command failure)" << endl;
    cout << "Overall success: " << (dependent2.succeeded ? "true" : "false") << endl;
    cout << endl;

    // === Money Transfer Command ===
    cout << "--- Money Transfer Command (Atomic Transfer) ---" << endl;
    BankAccount account_from;
    BankAccount account_to;
    account_from.deposit(1000);
    cout << "Account From initial: " << account_from.balance << endl;
    cout << "Account To initial: " << account_to.balance << endl;
    cout << endl;

    cout << "Execute: Transfer 300 from account_from to account_to" << endl;
    MoneyTransferCommand transfer1(account_from, account_to, 300);
    transfer1.call();
    cout << "Account From: " << account_from.balance << endl;
    cout << "Account To: " << account_to.balance << endl;
    cout << "Transfer success: " << (transfer1.succeeded ? "true" : "false") << endl;
    cout << endl;

    cout << "Try Transfer: 2000 from account_from to account_to (will fail)" << endl;
    MoneyTransferCommand transfer2(account_from, account_to, 2000);
    transfer2.call();
    cout << "Account From: " << account_from.balance << " (unchanged)" << endl;
    cout << "Account To: " << account_to.balance << " (unchanged)" << endl;
    cout << "Transfer success: " << (transfer2.succeeded ? "true" : "false") << endl;
    cout << endl;

    cout << "Undo successful transfer" << endl;
    transfer1.undo();
    cout << "Account From: " << account_from.balance << endl;
    cout << "Account To: " << account_to.balance << endl;

    return 0;
}
