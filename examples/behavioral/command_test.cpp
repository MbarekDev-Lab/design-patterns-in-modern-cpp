#include <iostream>
#include <cassert>
#include "../include/behavioral/command.h"

int main()
{
    cout << "=== COMMAND PATTERN TEST ===" << endl;
    cout << endl;

    // === TEST 1: Single Deposit Command ===
    {
        cout << "TEST 1: Single Deposit Command" << endl;
        BankAccount acc;
        assert(acc.balance == 0);

        BankAccountCommand deposit_cmd(acc, BankAccountCommand::deposit, 100);
        deposit_cmd.call();

        assert(acc.balance == 100);
        cout << "  ✓ Deposit command executed: balance = " << acc.balance << endl;

        deposit_cmd.undo();
        assert(acc.balance == 0);
        cout << "  ✓ Deposit command undone: balance = " << acc.balance << endl;
        cout << endl;
    }

    // === TEST 2: Single Withdraw Command ===
    {
        cout << "TEST 2: Single Withdraw Command" << endl;
        BankAccount acc;
        acc.balance = 200;

        BankAccountCommand withdraw_cmd(acc, BankAccountCommand::withdraw, 50);
        withdraw_cmd.call();

        assert(acc.balance == 150);
        cout << "  ✓ Withdraw command executed: balance = " << acc.balance << endl;

        withdraw_cmd.undo();
        assert(acc.balance == 200);
        cout << "  ✓ Withdraw command undone: balance = " << acc.balance << endl;
        cout << endl;
    }

    // === TEST 3: Sequence of Commands ===
    {
        cout << "TEST 3: Sequence of Operations" << endl;
        BankAccount acc;
        assert(acc.balance == 0);

        BankAccountCommand cmd1(acc, BankAccountCommand::deposit, 100);
        BankAccountCommand cmd2(acc, BankAccountCommand::withdraw, 30);
        BankAccountCommand cmd3(acc, BankAccountCommand::deposit, 50);

        cmd1.call();
        assert(acc.balance == 100);
        cout << "  ✓ After deposit 100: balance = " << acc.balance << endl;

        cmd2.call();
        assert(acc.balance == 70);
        cout << "  ✓ After withdraw 30: balance = " << acc.balance << endl;

        cmd3.call();
        assert(acc.balance == 120);
        cout << "  ✓ After deposit 50: balance = " << acc.balance << endl;

        cmd3.undo();
        assert(acc.balance == 70);
        cout << "  ✓ Undo deposit 50: balance = " << acc.balance << endl;

        cmd2.undo();
        assert(acc.balance == 100);
        cout << "  ✓ Undo withdraw 30: balance = " << acc.balance << endl;

        cmd1.undo();
        assert(acc.balance == 0);
        cout << "  ✓ Undo deposit 100: balance = " << acc.balance << endl;
        cout << endl;
    }

    // === TEST 4: Composite Commands (Batch) ===
    {
        cout << "TEST 4: Composite Command (Batch Execution)" << endl;
        BankAccount acc;
        assert(acc.balance == 0);

        CompositeBankAccountCommand batch{
            BankAccountCommand{acc, BankAccountCommand::deposit, 100},
            BankAccountCommand{acc, BankAccountCommand::withdraw, 30},
            BankAccountCommand{acc, BankAccountCommand::deposit, 50}};

        batch.call();
        assert(acc.balance == 120);
        cout << "  ✓ Batch executed: balance = " << acc.balance << endl;
        cout << "    (100 - 30 + 50 = 120)" << endl;

        batch.undo();
        assert(acc.balance == 0);
        cout << "  ✓ Batch undone (reverse order): balance = " << acc.balance << endl;
        cout << endl;
    }

    // === TEST 5: Overdraft Limit ===
    {
        cout << "TEST 5: Overdraft Limit Protection" << endl;
        BankAccount acc;
        acc.balance = 100;

        // Try to withdraw more than allowed (balance - amount < -500)
        BankAccountCommand bad_withdraw(acc, BankAccountCommand::withdraw, 700);
        bad_withdraw.call();

        // Should still be 100 (withdraw failed)
        assert(acc.balance == 100);
        cout << "  ✓ Withdraw blocked: balance = " << acc.balance << endl;

        // Valid withdraw to test overdraft limit
        BankAccountCommand valid_withdraw(acc, BankAccountCommand::withdraw, 600);
        valid_withdraw.call();
        assert(acc.balance == -500);
        cout << "  ✓ Withdraw at limit: balance = " << acc.balance << endl;
        cout << endl;
    }

    // === TEST 6: Nested Composite Commands ===
    {
        cout << "TEST 6: Multiple Batches (Sequential)" << endl;
        BankAccount acc;
        assert(acc.balance == 0);

        CompositeBankAccountCommand batch1{
            BankAccountCommand{acc, BankAccountCommand::deposit, 200}};

        CompositeBankAccountCommand batch2{
            BankAccountCommand{acc, BankAccountCommand::withdraw, 50},
            BankAccountCommand{acc, BankAccountCommand::withdraw, 50}};

        batch1.call();
        assert(acc.balance == 200);
        cout << "  ✓ Batch 1 executed: balance = " << acc.balance << endl;

        batch2.call();
        assert(acc.balance == 100);
        cout << "  ✓ Batch 2 executed: balance = " << acc.balance << endl;

        batch2.undo();
        assert(acc.balance == 200);
        cout << "  ✓ Batch 2 undone: balance = " << acc.balance << endl;

        batch1.undo();
        assert(acc.balance == 0);
        cout << "  ✓ Batch 1 undone: balance = " << acc.balance << endl;
        cout << endl;
    }

    cout << "=== ALL TESTS PASSED ===" << endl;
    return 0;
}
