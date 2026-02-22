#include <iostream>
#include <cassert>
#include "../include/behavioral/interpreter_exercise.h"

int main()
{
    cout << "=== INTERPRETER CODING EXERCISE TESTS ===" << endl;
    cout << endl;

    // === TEST 1: Single Integer ===
    {
        cout << "TEST 1: Single Integer" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("1") == 1);
        assert(ep.calculate("0") == 0);
        assert(ep.calculate("42") == 42);
        assert(ep.calculate("999") == 999);
        cout << "  ✓ Single integers calculate correctly" << endl;
        cout << endl;
    }

    // === TEST 2: Simple Addition ===
    {
        cout << "TEST 2: Simple Addition" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("1+2") == 3);
        assert(ep.calculate("5+10") == 15);
        assert(ep.calculate("0+5") == 5);
        cout << "  ✓ Addition works correctly" << endl;
        cout << endl;
    }

    // === TEST 3: Multiple Addition ===
    {
        cout << "TEST 3: Multiple Addition" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("1+2+3") == 6);
        assert(ep.calculate("10+20+30") == 60);
        assert(ep.calculate("1+1+1+1+1") == 5);
        cout << "  ✓ Multiple additions calculate correctly" << endl;
        cout << endl;
    }

    // === TEST 4: Simple Subtraction ===
    {
        cout << "TEST 4: Simple Subtraction" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("10-2") == 8);
        assert(ep.calculate("5-5") == 0);
        assert(ep.calculate("3-10") == -7);
        cout << "  ✓ Subtraction works correctly" << endl;
        cout << endl;
    }

    // === TEST 5: Multiple Subtraction ===
    {
        cout << "TEST 5: Multiple Subtraction" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("10-2-3") == 5);
        assert(ep.calculate("100-10-20-30") == 40);
        assert(ep.calculate("1-1-1") == -1);
        cout << "  ✓ Multiple subtractions calculate correctly" << endl;
        cout << endl;
    }

    // === TEST 6: Mixed Addition and Subtraction ===
    {
        cout << "TEST 6: Mixed Addition and Subtraction" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("1+2-3") == 0);
        assert(ep.calculate("10+5-3") == 12);
        assert(ep.calculate("1-2+3") == 2);
        assert(ep.calculate("5+5-2-1") == 7);
        cout << "  ✓ Mixed operations calculate correctly" << endl;
        cout << endl;
    }

    // === TEST 7: Single Variable ===
    {
        cout << "TEST 7: Single Variable" << endl;
        ExpressionProcessor ep;
        ep.variables['x'] = 5;
        assert(ep.calculate("x") == 5);

        ep.variables['y'] = 10;
        assert(ep.calculate("y") == 10);
        cout << "  ✓ Single variables work correctly" << endl;
        cout << endl;
    }

    // === TEST 8: Variable in Addition ===
    {
        cout << "TEST 8: Variable in Addition" << endl;
        ExpressionProcessor ep;
        ep.variables['x'] = 5;
        assert(ep.calculate("1+x") == 6);
        assert(ep.calculate("x+2") == 7);
        assert(ep.calculate("x+x") == 10);
        cout << "  ✓ Variables in addition work correctly" << endl;
        cout << endl;
    }

    // === TEST 9: Variable in Subtraction ===
    {
        cout << "TEST 9: Variable in Subtraction" << endl;
        ExpressionProcessor ep;
        ep.variables['x'] = 5;
        assert(ep.calculate("10-x") == 5);
        assert(ep.calculate("x-2") == 3);
        assert(ep.calculate("x-x") == 0);
        cout << "  ✓ Variables in subtraction work correctly" << endl;
        cout << endl;
    }

    // === TEST 10: Multiple Variables ===
    {
        cout << "TEST 10: Multiple Variables" << endl;
        ExpressionProcessor ep;
        ep.variables['x'] = 5;
        ep.variables['y'] = 3;
        ep.variables['z'] = 2;
        assert(ep.calculate("x+y") == 8);
        assert(ep.calculate("x+y+z") == 10);
        assert(ep.calculate("x-y") == 2);
        assert(ep.calculate("x-y-z") == 0);
        cout << "  ✓ Multiple variables work correctly" << endl;
        cout << endl;
    }

    // === TEST 11: Complex Mix ===
    {
        cout << "TEST 11: Complex Mix (Numbers and Variables)" << endl;
        ExpressionProcessor ep;
        ep.variables['a'] = 10;
        ep.variables['b'] = 3;
        assert(ep.calculate("1+a") == 11);
        assert(ep.calculate("a-b") == 7);
        assert(ep.calculate("1+a-b") == 8);
        assert(ep.calculate("a+b+5") == 18);
        cout << "  ✓ Complex mix calculates correctly" << endl;
        cout << endl;
    }

    // === TEST 12: Unknown Variable ===
    {
        cout << "TEST 12: Unknown Variable" << endl;
        ExpressionProcessor ep;
        ep.variables['x'] = 5;
        assert(ep.calculate("1+z") == 0); // z not defined
        assert(ep.calculate("y") == 0);   // y not defined
        assert(ep.calculate("a+b") == 0); // neither defined
        cout << "  ✓ Unknown variables return 0" << endl;
        cout << endl;
    }

    // === TEST 13: Multi-letter Variable ===
    {
        cout << "TEST 13: Multi-letter Variable (Invalid)" << endl;
        ExpressionProcessor ep;
        ep.variables['x'] = 5;
        assert(ep.calculate("1+xy") == 0); // xy is 2 letters
        assert(ep.calculate("abc") == 0);  // abc is 3 letters
        assert(ep.calculate("x+yz") == 0); // yz is 2 letters
        cout << "  ✓ Multi-letter variables return 0" << endl;
        cout << endl;
    }

    // === TEST 14: Operator at End ===
    {
        cout << "TEST 14: Operator at End (Invalid)" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("1+") == 0);
        assert(ep.calculate("5-") == 0);
        assert(ep.calculate("10+20-") == 0);
        cout << "  ✓ Trailing operators return 0" << endl;
        cout << endl;
    }

    // === TEST 15: Operator at Start ===
    {
        cout << "TEST 15: Operator at Start (Invalid)" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("+1") == 0);
        assert(ep.calculate("-5") == 0);
        assert(ep.calculate("+1+2") == 0);
        cout << "  ✓ Leading operators return 0" << endl;
        cout << endl;
    }

    // === TEST 16: Consecutive Operators ===
    {
        cout << "TEST 16: Consecutive Operators (Invalid)" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("1++2") == 0);
        assert(ep.calculate("1--2") == 0);
        assert(ep.calculate("1+-2") == 0);
        cout << "  ✓ Consecutive operators return 0" << endl;
        cout << endl;
    }

    // === TEST 17: Zero Values ===
    {
        cout << "TEST 17: Zero Values" << endl;
        ExpressionProcessor ep;
        ep.variables['z'] = 0;
        assert(ep.calculate("0") == 0);
        assert(ep.calculate("0+5") == 5);
        assert(ep.calculate("5-5") == 0);
        assert(ep.calculate("z") == 0);
        assert(ep.calculate("0+z") == 0);
        cout << "  ✓ Zero values handled correctly" << endl;
        cout << endl;
    }

    // === TEST 18: Whitespace ===
    {
        cout << "TEST 18: Whitespace Handling" << endl;
        ExpressionProcessor ep;
        ep.variables['x'] = 5;
        assert(ep.calculate("1 + 2") == 3);
        assert(ep.calculate(" 5 ") == 5);
        assert(ep.calculate("1 + x ") == 6);
        assert(ep.calculate(" 10 - 3 ") == 7);
        cout << "  ✓ Whitespace is ignored correctly" << endl;
        cout << endl;
    }

    // === TEST 19: Large Numbers ===
    {
        cout << "TEST 19: Large Numbers" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("1000") == 1000);
        assert(ep.calculate("999+1") == 1000);
        assert(ep.calculate("100-99") == 1);
        cout << "  ✓ Large numbers handled correctly" << endl;
        cout << endl;
    }

    // === TEST 20: Negative Results ===
    {
        cout << "TEST 20: Negative Results" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("1-5") == -4);
        assert(ep.calculate("0-10") == -10);
        assert(ep.calculate("5-10-5") == -10);
        cout << "  ✓ Negative results calculated correctly" << endl;
        cout << endl;
    }

    // === TEST 21: Given Examples ===
    {
        cout << "TEST 21: Given Examples from Problem Statement" << endl;
        ExpressionProcessor ep;
        ep.variables['x'] = 3;

        assert(ep.calculate("1+2+3") == 6);
        cout << "  ✓ calculate(\"1+2+3\") == 6" << endl;

        assert(ep.calculate("1+2+xy") == 0);
        cout << "  ✓ calculate(\"1+2+xy\") == 0" << endl;

        assert(ep.calculate("10-2-x") == 5);
        cout << "  ✓ calculate(\"10-2-x\") == 5 (when x=3)" << endl;
        cout << endl;
    }

    // === TEST 22: Invalid Characters ===
    {
        cout << "TEST 22: Invalid Characters" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("1*2") == 0);  // * not allowed
        assert(ep.calculate("1/2") == 0);  // / not allowed
        assert(ep.calculate("1(2)") == 0); // () not allowed
        assert(ep.calculate("1#2") == 0);  // # not allowed
        cout << "  ✓ Invalid characters return 0" << endl;
        cout << endl;
    }

    // === TEST 23: Edge Case: Empty String ===
    {
        cout << "TEST 23: Edge Case - Empty String" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("") == 0);
        cout << "  ✓ Empty string returns 0" << endl;
        cout << endl;
    }

    // === TEST 24: Single Space ===
    {
        cout << "TEST 24: Edge Case - Only Space" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("   ") == 0);
        cout << "  ✓ Only whitespace returns 0" << endl;
        cout << endl;
    }

    // === TEST 25: Very Long Expression ===
    {
        cout << "TEST 25: Long Expression" << endl;
        ExpressionProcessor ep;
        assert(ep.calculate("1+1+1+1+1+1+1+1+1+1") == 10);
        assert(ep.calculate("100-1-1-1-1-1-1-1-1-1-1") == 90);
        cout << "  ✓ Long expressions handled correctly" << endl;
        cout << endl;
    }

    cout << "=== ALL TESTS PASSED ===" << endl;
    return 0;
}
