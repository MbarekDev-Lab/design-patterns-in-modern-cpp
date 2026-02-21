#pragma once

// ============================================================================
// COMMAND PATTERN CODING EXERCISE
// ============================================================================
// Implement the Account::process() function to process different commands.
// Rules:
// 1. success indicates whether the operation was successful
// 2. You can only withdraw if you have enough balance
// ============================================================================

struct Command
{
    enum Action
    {
        deposit,
        withdraw
    } action;
    int amount{0};
    bool success{false};
};

struct Account
{
    int balance{0};

    void process(Command &cmd)
    {
        switch (cmd.action)
        {
        case Command::deposit:
            balance += cmd.amount;
            cmd.success = true;
            break;
        case Command::withdraw:
            // Only allow withdrawal if sufficient balance
            cmd.success = (balance >= cmd.amount);
            if (cmd.success)
                balance -= cmd.amount;
            break;
        }
    }
};
