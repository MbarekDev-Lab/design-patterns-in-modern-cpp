#include <iostream>
#include <cassert>
#include "../include/behavioral/command_advanced.h"

int main()
{
    cout << "=== COMMAND PATTERN: Advanced Tests ===" << endl;
    cout << endl;

    // === TEST 1: Simple Command Pattern ===
    {
        cout << "TEST 1: Simple Command Pattern (Data-Driven)" << endl;
        SimpleAccount acc;
        assert(acc.balance == 0);

        SimpleCommand deposit_cmd{SimpleCommand::deposit, 100};
        acc.process(deposit_cmd);
        assert(acc.balance == 100);
        assert(deposit_cmd.success == true);
        cout << "  ✓ Deposit 100: balance = " << acc.balance << endl;

        SimpleCommand withdraw_cmd{SimpleCommand::withdraw, 50};
        acc.process(withdraw_cmd);
        assert(acc.balance == 50);
        assert(withdraw_cmd.success == true);
        cout << "  ✓ Withdraw 50: balance = " << acc.balance << endl;

        SimpleCommand bad_withdraw{SimpleCommand::withdraw, 100};
        acc.process(bad_withdraw);
        assert(acc.balance == 50); // Unchanged
        assert(bad_withdraw.success == false);
        cout << "  ✓ Withdraw 100 (failed): balance = " << acc.balance << endl;
        cout << endl;
    }

    // === TEST 2: Basic Command with Undo ===
    {
        cout << "TEST 2: Basic Command with Undo" << endl;
        BankAccount ba;
        assert(ba.balance == 0);

        BankAccountCommand deposit_cmd(ba, BankAccountCommand::deposit, 200);
        deposit_cmd.call();
        assert(ba.balance == 200);
        cout << "  ✓ Deposit 200: balance = " << ba.balance << endl;

        deposit_cmd.undo();
        assert(ba.balance == 0);
        cout << "  ✓ Undo deposit: balance = " << ba.balance << endl;
        cout << endl;
    }

    // === TEST 3: Withdraw with Undo ===
    {
        cout << "TEST 3: Withdraw with Undo" << endl;
        BankAccount ba;
        ba.deposit(300);
        assert(ba.balance == 300);

        BankAccountCommand withdraw_cmd(ba, BankAccountCommand::withdraw, 100);
        withdraw_cmd.call();
        assert(ba.balance == 200);
        cout << "  ✓ Withdraw 100: balance = " << ba.balance << endl;

        withdraw_cmd.undo();
        assert(ba.balance == 300);
        cout << "  ✓ Undo withdraw: balance = " << ba.balance << endl;
        cout << endl;
    }

    // === TEST 4: Failed Withdraw Cannot Be Undone ===
    {
        cout << "TEST 4: Failed Withdraw Cannot Be Undone" << endl;
        BankAccount ba;
        ba.deposit(100);
        assert(ba.balance == 100);

        BankAccountCommand bad_withdraw(ba, BankAccountCommand::withdraw, 500);
        bad_withdraw.call();
        assert(ba.balance == 100); // Failed, balance unchanged
        assert(bad_withdraw.succeeded == false);
        cout << "  ✓ Failed withdraw: balance = " << ba.balance << endl;

        bad_withdraw.undo();
        assert(ba.balance == 100); // Undo does nothing since succeeded=false
        cout << "  ✓ Undo on failed command does nothing: balance = " << ba.balance << endl;
        cout << endl;
    }

    // === TEST 5: Composite Commands ===
    {
        cout << "TEST 5: Composite Commands" << endl;
        BankAccount ba;
        assert(ba.balance == 0);

        CompositeBankAccountCommand composite{
            BankAccountCommand{ba, BankAccountCommand::deposit, 500},
            BankAccountCommand{ba, BankAccountCommand::withdraw, 200},
            BankAccountCommand{ba, BankAccountCommand::withdraw, 100}};

        composite.call();
        assert(ba.balance == 200);
        cout << "  ✓ Composite executed: balance = " << ba.balance << endl;
        cout << "    (500 - 200 - 100 = 200)" << endl;

        composite.undo();
        assert(ba.balance == 0);
        cout << "  ✓ Composite undone: balance = " << ba.balance << endl;
        cout << endl;
    }

    // === TEST 6: Dependent Composite - All Succeed ===
    {
        cout << "TEST 6: Dependent Composite - All Commands Succeed" << endl;
        BankAccount ba;
        ba.deposit(500);

        DependentCompositeCommand dependent{
            BankAccountCommand{ba, BankAccountCommand::deposit, 200},
            BankAccountCommand{ba, BankAccountCommand::withdraw, 300}};

        dependent.call();
        assert(ba.balance == 400);
        assert(dependent.succeeded == true);
        cout << "  ✓ All commands succeeded: balance = " << ba.balance << endl;
        cout << "  ✓ Overall success: true" << endl;
        cout << endl;
    }

    // === TEST 7: Dependent Composite - Second Fails ===
    {
        cout << "TEST 7: Dependent Composite - Second Command Fails" << endl;
        BankAccount ba;
        ba.deposit(100);

        DependentCompositeCommand dependent{
            BankAccountCommand{ba, BankAccountCommand::deposit, 200},
            BankAccountCommand{ba, BankAccountCommand::withdraw, 500}};

        dependent.call();
        // First succeeds: balance becomes 300
        // Second fails: balance stays 300, but command marked as failed
        assert(ba.balance == 300);
        assert(dependent.succeeded == false);
        cout << "  ✓ First command succeeded, second failed: balance = " << ba.balance << endl;
        cout << "  ✓ Overall success: false" << endl;
        cout << endl;
    }

    // === TEST 8: Dependent Composite - First Fails ===
    {
        cout << "TEST 8: Dependent Composite - First Command Fails" << endl;
        BankAccount ba;
        ba.deposit(100);

        DependentCompositeCommand dependent{
            BankAccountCommand{ba, BankAccountCommand::withdraw, 500}, // Will fail
            BankAccountCommand{ba, BankAccountCommand::deposit, 100}   // Won't execute
        };

        dependent.call();
        // First fails: balance unchanged
        // Second doesn't execute (marked as failed)
        assert(ba.balance == 100);
        assert(dependent.succeeded == false);
        cout << "  ✓ First command failed, second skipped: balance = " << ba.balance << endl;
        cout << "  ✓ Overall success: false" << endl;
        cout << endl;
    }

    // === TEST 9: Money Transfer - Success ===
    {
        cout << "TEST 9: Money Transfer - Successful" << endl;
        BankAccount from, to;
        from.deposit(1000);

        assert(from.balance == 1000);
        assert(to.balance == 0);

        MoneyTransferCommand transfer(from, to, 300);
        transfer.call();

        assert(from.balance == 700);
        assert(to.balance == 300);
        assert(transfer.succeeded == true);
        cout << "  ✓ Transfer 300: From = " << from.balance << ", To = " << to.balance << endl;
        cout << "  ✓ Transfer succeeded: true" << endl;
        cout << endl;
    }

    // === TEST 10: Money Transfer - Insufficient Funds ===
    {
        cout << "TEST 10: Money Transfer - Insufficient Funds" << endl;
        BankAccount from, to;
        from.deposit(100);

        assert(from.balance == 100);
        assert(to.balance == 0);

        MoneyTransferCommand transfer(from, to, 500); // More than available
        transfer.call();

        // Withdraw fails, so deposit doesn't execute
        assert(from.balance == 100); // Unchanged
        assert(to.balance == 0);     // Unchanged
        assert(transfer.succeeded == false);
        cout << "  ✓ Transfer blocked: From = " << from.balance << ", To = " << to.balance << endl;
        cout << "  ✓ Transfer succeeded: false" << endl;
        cout << endl;
    }

    // === TEST 11: Money Transfer - Undo ===
    {
        cout << "TEST 11: Money Transfer - Undo" << endl;
        BankAccount from, to;
        from.deposit(500);

        MoneyTransferCommand transfer(from, to, 200);
        transfer.call();
        assert(from.balance == 300);
        assert(to.balance == 200);
        cout << "  ✓ Transfer 200: From = " << from.balance << ", To = " << to.balance << endl;

        transfer.undo();
        assert(from.balance == 500);
        assert(to.balance == 0);
        cout << "  ✓ Undo transfer: From = " << from.balance << ", To = " << to.balance << endl;
        cout << endl;
    }

    // === TEST 12: Multiple Transfers ===
    {
        cout << "TEST 12: Multiple Sequential Transfers" << endl;
        BankAccount acc1, acc2, acc3;
        acc1.deposit(1000);

        MoneyTransferCommand t1(acc1, acc2, 300);
        t1.call();
        assert(acc1.balance == 700);
        assert(acc2.balance == 300);
        cout << "  ✓ Transfer 1: acc1 = " << acc1.balance << ", acc2 = " << acc2.balance << endl;

        MoneyTransferCommand t2(acc2, acc3, 200);
        t2.call();
        assert(acc2.balance == 100);
        assert(acc3.balance == 200);
        cout << "  ✓ Transfer 2: acc2 = " << acc2.balance << ", acc3 = " << acc3.balance << endl;

        t2.undo();
        assert(acc2.balance == 300);
        assert(acc3.balance == 0);
        cout << "  ✓ Undo transfer 2: acc2 = " << acc2.balance << ", acc3 = " << acc3.balance << endl;

        t1.undo();
        assert(acc1.balance == 1000);
        assert(acc2.balance == 0);
        cout << "  ✓ Undo transfer 1: acc1 = " << acc1.balance << ", acc2 = " << acc2.balance << endl;
        cout << endl;
    }

    cout << "=== ALL TESTS PASSED ===" << endl;
    return 0;
}
