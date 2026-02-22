#include <iostream>
#include <cassert>
#include "../include/behavioral/command_exercise.h"

int main()
{
    cout << "=== COMMAND PATTERN EXERCISE TESTS ===" << endl;
    cout << endl;

    // === TEST 1: Deposit Operation ===
    {
        cout << "TEST 1: Deposit Operation" << endl;
        Account acc;
        assert(acc.balance == 0);

        Command deposit{Command::deposit, 100};
        acc.process(deposit);

        assert(acc.balance == 100);
        assert(deposit.success == true);
        cout << "  ✓ Deposit 100: balance = " << acc.balance << ", success = true" << endl;
        cout << endl;
    }

    // === TEST 2: Multiple Deposits ===
    {
        cout << "TEST 2: Multiple Deposits" << endl;
        Account acc;

        Command d1{Command::deposit, 500};
        acc.process(d1);
        assert(acc.balance == 500);
        assert(d1.success == true);

        Command d2{Command::deposit, 250};
        acc.process(d2);
        assert(acc.balance == 750);
        assert(d2.success == true);

        cout << "  ✓ Deposit 500, then 250: balance = " << acc.balance << endl;
        cout << endl;
    }

    // === TEST 3: Successful Withdrawal ===
    {
        cout << "TEST 3: Successful Withdrawal" << endl;
        Account acc;
        acc.balance = 1000;

        Command withdraw{Command::withdraw, 300};
        acc.process(withdraw);

        assert(acc.balance == 700);
        assert(withdraw.success == true);
        cout << "  ✓ Withdraw 300 from 1000: balance = " << acc.balance << ", success = true" << endl;
        cout << endl;
    }

    // === TEST 4: Withdrawal Exceeds Balance ===
    {
        cout << "TEST 4: Withdrawal Exceeds Balance (Fails)" << endl;
        Account acc;
        acc.balance = 100;

        Command withdraw{Command::withdraw, 500};
        acc.process(withdraw);

        assert(acc.balance == 100); // Balance unchanged
        assert(withdraw.success == false);
        cout << "  ✓ Try to withdraw 500 from 100: balance = " << acc.balance << ", success = false" << endl;
        cout << endl;
    }

    // === TEST 5: Withdrawal Equals Balance ===
    {
        cout << "TEST 5: Withdrawal Equals Balance (Succeeds)" << endl;
        Account acc;
        acc.balance = 250;

        Command withdraw{Command::withdraw, 250};
        acc.process(withdraw);

        assert(acc.balance == 0);
        assert(withdraw.success == true);
        cout << "  ✓ Withdraw 250 from 250: balance = " << acc.balance << ", success = true" << endl;
        cout << endl;
    }

    // === TEST 6: Zero Amount Operations ===
    {
        cout << "TEST 6: Zero Amount Operations" << endl;
        Account acc;
        acc.balance = 100;

        Command zero_deposit{Command::deposit, 0};
        acc.process(zero_deposit);
        assert(acc.balance == 100);
        assert(zero_deposit.success == true);

        Command zero_withdraw{Command::withdraw, 0};
        acc.process(zero_withdraw);
        assert(acc.balance == 100);
        assert(zero_withdraw.success == true);

        cout << "  ✓ Deposit 0: balance = " << acc.balance << ", success = true" << endl;
        cout << "  ✓ Withdraw 0: balance = " << acc.balance << ", success = true" << endl;
        cout << endl;
    }

    // === TEST 7: Complex Sequence ===
    {
        cout << "TEST 7: Complex Sequence of Operations" << endl;
        Account acc;
        assert(acc.balance == 0);

        // Deposit 1000
        Command c1{Command::deposit, 1000};
        acc.process(c1);
        assert(acc.balance == 1000);
        assert(c1.success == true);

        // Withdraw 300
        Command c2{Command::withdraw, 300};
        acc.process(c2);
        assert(acc.balance == 700);
        assert(c2.success == true);

        // Deposit 500
        Command c3{Command::deposit, 500};
        acc.process(c3);
        assert(acc.balance == 1200);
        assert(c3.success == true);

        // Withdraw 1200 (exact amount)
        Command c4{Command::withdraw, 1200};
        acc.process(c4);
        assert(acc.balance == 0);
        assert(c4.success == true);

        // Try to withdraw 1 (insufficient)
        Command c5{Command::withdraw, 1};
        acc.process(c5);
        assert(acc.balance == 0);
        assert(c5.success == false);

        cout << "  ✓ Sequence: 1000 -> -300 -> +500 -> -1200 -> fail -1" << endl;
        cout << "  ✓ Final balance: " << acc.balance << endl;
        cout << endl;
    }

    // === TEST 8: Success Flag Behavior ===
    {
        cout << "TEST 8: Success Flag Behavior" << endl;
        Account acc;
        acc.balance = 100;

        // Reuse same command object
        Command cmd{Command::withdraw, 50};
        assert(cmd.success == false); // Initially false

        acc.process(cmd);
        assert(cmd.success == true); // Set to true after successful operation

        // Try with same amounts
        Command cmd2{Command::withdraw, 100};
        acc.process(cmd2);
        assert(cmd2.success == false); // Set to false after failed operation

        cout << "  ✓ Success flag correctly set for each operation" << endl;
        cout << endl;
    }

    // === TEST 9: Large Numbers ===
    {
        cout << "TEST 9: Large Numbers" << endl;
        Account acc;

        Command large_deposit{Command::deposit, 1000000};
        acc.process(large_deposit);
        assert(acc.balance == 1000000);
        assert(large_deposit.success == true);

        Command large_withdraw{Command::withdraw, 500000};
        acc.process(large_withdraw);
        assert(acc.balance == 500000);
        assert(large_withdraw.success == true);

        cout << "  ✓ Deposit 1,000,000: balance = " << acc.balance << endl;
        cout << "  ✓ Withdraw 500,000: balance = " << acc.balance << endl;
        cout << endl;
    }

    // === TEST 10: Withdraw from Zero Balance ===
    {
        cout << "TEST 10: Withdraw from Zero Balance" << endl;
        Account acc;
        assert(acc.balance == 0);

        Command withdraw{Command::withdraw, 1};
        acc.process(withdraw);

        assert(acc.balance == 0);
        assert(withdraw.success == false);
        cout << "  ✓ Withdraw 1 from 0: balance = " << acc.balance << ", success = false" << endl;
        cout << endl;
    }

    // === TEST 11: Command Reuse ===
    {
        cout << "TEST 11: Reusing Command Object" << endl;
        Account acc;

        Command cmd{Command::deposit, 100};

        acc.process(cmd);
        assert(acc.balance == 100);
        assert(cmd.success == true);

        // Reuse the command (now as withdrawal)
        cmd.action = Command::withdraw;
        cmd.amount = 50;
        acc.process(cmd);
        assert(acc.balance == 50);
        assert(cmd.success == true);

        cout << "  ✓ Reused command object for different operations" << endl;
        cout << "  ✓ Final balance: " << acc.balance << endl;
        cout << endl;
    }

    // === TEST 12: Boundary Condition ===
    {
        cout << "TEST 12: Boundary Condition (Balance Exactly At Zero)" << endl;
        Account acc;
        acc.balance = 100;

        Command w1{Command::withdraw, 50};
        acc.process(w1);
        assert(acc.balance == 50);
        assert(w1.success == true);

        Command w2{Command::withdraw, 50};
        acc.process(w2);
        assert(acc.balance == 0);
        assert(w2.success == true);

        Command w3{Command::withdraw, 0};
        acc.process(w3);
        assert(acc.balance == 0);
        assert(w3.success == true);

        Command w4{Command::withdraw, 1};
        acc.process(w4);
        assert(acc.balance == 0);
        assert(w4.success == false);

        cout << "  ✓ Correctly handles boundary at zero balance" << endl;
        cout << endl;
    }

    cout << "=== ALL TESTS PASSED ===" << endl;
    return 0;
}
