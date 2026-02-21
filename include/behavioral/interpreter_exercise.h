#pragma once
#include <map>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <iostream>

using namespace std;

// ============================================================================
// INTERPRETER PATTERN CODING EXERCISE
// ============================================================================
// Implement an expression processor for simple numeric expressions
//
// Requirements:
// 1. Support integral values (e.g., "13")
// 2. Support single-letter variables defined in variables map
// 3. Support + and - operators only (no parentheses or other operations)
// 4. Return 0 if variable not found or variable has >1 letter
// 5. Return 0 on any parsing failure
//
// Examples:
//   calculate("1+2+3")     returns 6
//   calculate("1+2+xy")    returns 0  (xy is not a valid variable)
//   calculate("10-2-x")    returns 5  (when x=3)
// ============================================================================

struct ExpressionProcessor
{
    map<char, int> variables;

    int calculate(const string &expression)
    {
        try
        {
            // Parse the expression into tokens and values
            int result = 0;
            int current_value = 0;
            char current_op = '+'; // The operation to apply to the next value
            bool parsing_number = false;

            for (size_t i = 0; i < expression.length(); ++i)
            {
                char ch = expression[i];

                if (isdigit(ch))
                {
                    // Build multi-digit number
                    current_value = current_value * 10 + (ch - '0');
                    parsing_number = true;
                }
                else if (isalpha(ch))
                {
                    // Should be a single-letter variable
                    if (parsing_number)
                    {
                        // Error: mixing number parsing with variable
                        return 0;
                    }

                    // Check if next character is also a letter (invalid multi-letter variable)
                    if (i + 1 < expression.length() && isalpha(expression[i + 1]))
                    {
                        return 0; // Multi-letter variable not allowed
                    }

                    // Look up the variable
                    if (variables.find(ch) == variables.end())
                    {
                        return 0; // Variable not found
                    }

                    current_value = variables[ch];
                    parsing_number = true;
                }
                else if (ch == '+' || ch == '-')
                {
                    if (!parsing_number)
                    {
                        return 0; // Operator without a preceding value
                    }

                    // Apply the previous operation
                    if (current_op == '+')
                        result += current_value;
                    else if (current_op == '-')
                        result -= current_value;

                    // Prepare for next value
                    current_op = ch;
                    current_value = 0;
                    parsing_number = false;
                }
                else if (ch == ' ')
                {
                    // Skip whitespace
                    continue;
                }
                else
                {
                    // Invalid character
                    return 0;
                }
            }

            // Apply the last operation if we parsed a value
            if (parsing_number)
            {
                if (current_op == '+')
                    result += current_value;
                else if (current_op == '-')
                    result -= current_value;
            }
            else
            {
                // Expression ended with an operator
                return 0;
            }

            return result;
        }
        catch (...)
        {
            // Any exception returns 0
            return 0;
        }
    }
};
