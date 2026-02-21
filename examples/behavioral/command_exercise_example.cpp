#include <iostream>
#include "../include/behavioral/command_exercise.h"

int main()
{
    cout << "=== COMMAND PATTERN CODING EXERCISE ===" << endl;
    cout << endl;

    cout << "--- Example 1: Deposit ---" << endl;
    Account acc;
    cout << "Initial balance: " << acc.balance << endl;

    Command deposit_cmd{Command::deposit, 100};
    acc.process(deposit_cmd);
    cout << "Deposit 100" << endl;
    cout << "Success: " << (deposit_cmd.success ? "true" : "false") << endl;
    cout << "Balance: " << acc.balance << endl;
    cout << endl;

    cout << "--- Example 2: Successful Withdrawal ---" << endl;
    Command withdraw_cmd{Command::withdraw, 50};
    acc.process(withdraw_cmd);
    cout << "Withdraw 50" << endl;
    cout << "Success: " << (withdraw_cmd.success ? "true" : "false") << endl;
    cout << "Balance: " << acc.balance << endl;
    cout << endl;

    cout << "--- Example 3: Failed Withdrawal (Insufficient Balance) ---" << endl;
    Command bad_withdraw{Command::withdraw, 100};
    acc.process(bad_withdraw);
    cout << "Try to withdraw 100 (balance is only 50)" << endl;
    cout << "Success: " << (bad_withdraw.success ? "true" : "false") << endl;
    cout << "Balance: " << acc.balance << " (unchanged)" << endl;
    cout << endl;

    cout << "--- Example 4: Multiple Operations ---" << endl;
    acc.balance = 0; // Reset
    cout << "Reset balance to 0" << endl;
    cout << endl;

    Command ops[] = {
        {Command::deposit, 1000},
        {Command::withdraw, 500},
        {Command::withdraw, 300},
        {Command::withdraw, 300}, // Will succeed (exactly the amount)
        {Command::withdraw, 200}  // Will fail (insufficient balance)
    };

    for (int i = 0; i < 5; ++i)
    {
        acc.process(ops[i]);
        cout << (ops[i].action == Command::deposit ? "Deposit " : "Withdraw ")
             << ops[i].amount << " - "
             << (ops[i].success ? "SUCCESS" : "FAILED")
             << " - Balance: " << acc.balance << endl;
    }

    return 0;
}
