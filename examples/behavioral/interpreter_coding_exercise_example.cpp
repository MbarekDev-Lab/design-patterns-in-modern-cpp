#include <iostream>
#include "../include/behavioral/interpreter_exercise.h"

int main()
{
    cout << "=== INTERPRETER CODING EXERCISE: Expression Processor ===" << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 1: Simple Numbers
    // ========================================================================
    cout << "--- EXAMPLE 1: Simple Numbers ---" << endl;
    ExpressionProcessor ep1;
    cout << "Expression: \"1\"" << endl;
    cout << "Result: " << ep1.calculate("1") << endl;
    cout << endl;

    cout << "Expression: \"42\"" << endl;
    cout << "Result: " << ep1.calculate("42") << endl;
    cout << endl;

    cout << "Expression: \"0\"" << endl;
    cout << "Result: " << ep1.calculate("0") << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 2: Addition
    // ========================================================================
    cout << "--- EXAMPLE 2: Addition ---" << endl;
    ExpressionProcessor ep2;
    cout << "Expression: \"1+2\"" << endl;
    cout << "Result: " << ep2.calculate("1+2") << " (expected 3)" << endl;
    cout << endl;

    cout << "Expression: \"1+2+3\"" << endl;
    cout << "Result: " << ep2.calculate("1+2+3") << " (expected 6)" << endl;
    cout << endl;

    cout << "Expression: \"10+20+30\"" << endl;
    cout << "Result: " << ep2.calculate("10+20+30") << " (expected 60)" << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 3: Subtraction
    // ========================================================================
    cout << "--- EXAMPLE 3: Subtraction ---" << endl;
    ExpressionProcessor ep3;
    cout << "Expression: \"10-2\"" << endl;
    cout << "Result: " << ep3.calculate("10-2") << " (expected 8)" << endl;
    cout << endl;

    cout << "Expression: \"10-5-2\"" << endl;
    cout << "Result: " << ep3.calculate("10-5-2") << " (expected 3)" << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 4: Variables
    // ========================================================================
    cout << "--- EXAMPLE 4: Variables ---" << endl;
    ExpressionProcessor ep4;
    ep4.variables['x'] = 5;
    ep4.variables['y'] = 3;
    cout << "Variables: x=5, y=3" << endl;
    cout << endl;

    cout << "Expression: \"x\"" << endl;
    cout << "Result: " << ep4.calculate("x") << " (expected 5)" << endl;
    cout << endl;

    cout << "Expression: \"1+x\"" << endl;
    cout << "Result: " << ep4.calculate("1+x") << " (expected 6)" << endl;
    cout << endl;

    cout << "Expression: \"10-x\"" << endl;
    cout << "Result: " << ep4.calculate("10-x") << " (expected 5)" << endl;
    cout << endl;

    cout << "Expression: \"x+y\"" << endl;
    cout << "Result: " << ep4.calculate("x+y") << " (expected 8)" << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 5: Mixed Operations
    // ========================================================================
    cout << "--- EXAMPLE 5: Mixed Operations ---" << endl;
    ExpressionProcessor ep5;
    ep5.variables['a'] = 10;
    ep5.variables['b'] = 2;
    ep5.variables['c'] = 3;
    cout << "Variables: a=10, b=2, c=3" << endl;
    cout << endl;

    cout << "Expression: \"a-b-c\"" << endl;
    cout << "Result: " << ep5.calculate("a-b-c") << " (expected 5)" << endl;
    cout << endl;

    cout << "Expression: \"1+a+b-c\"" << endl;
    cout << "Result: " << ep5.calculate("1+a+b-c") << " (expected 10)" << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 6: Invalid Expressions
    // ========================================================================
    cout << "--- EXAMPLE 6: Invalid Expressions ---" << endl;
    ExpressionProcessor ep6;
    ep6.variables['x'] = 5;
    cout << "Variables: x=5" << endl;
    cout << endl;

    cout << "Expression: \"1+2+xy\" (xy is not a valid variable)" << endl;
    cout << "Result: " << ep6.calculate("1+2+xy") << " (expected 0)" << endl;
    cout << endl;

    cout << "Expression: \"1+unknown\" (unknown variable)" << endl;
    cout << "Result: " << ep6.calculate("1+unknown") << " (expected 0)" << endl;
    cout << endl;

    cout << "Expression: \"1+\" (ends with operator)" << endl;
    cout << "Result: " << ep6.calculate("1+") << " (expected 0)" << endl;
    cout << endl;

    cout << "Expression: \"+1\" (starts with operator)" << endl;
    cout << "Result: " << ep6.calculate("+1") << " (expected 0)" << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 7: Whitespace
    // ========================================================================
    cout << "--- EXAMPLE 7: Whitespace Handling ---" << endl;
    ExpressionProcessor ep7;
    ep7.variables['x'] = 5;
    cout << "Expression: \"1 + 2\" (spaces)" << endl;
    cout << "Result: " << ep7.calculate("1 + 2") << " (expected 3)" << endl;
    cout << endl;

    cout << "Expression: \" 1 + x \" (leading/trailing spaces)" << endl;
    cout << "Result: " << ep7.calculate(" 1 + x ") << " (expected 6)" << endl;
    cout << endl;

    // ========================================================================
    // EXAMPLE 8: Edge Cases
    // ========================================================================
    cout << "--- EXAMPLE 8: Edge Cases ---" << endl;
    ExpressionProcessor ep8;
    ep8.variables['x'] = 0;
    ep8.variables['z'] = 100;
    cout << "Variables: x=0, z=100" << endl;
    cout << endl;

    cout << "Expression: \"0\" (zero value)" << endl;
    cout << "Result: " << ep8.calculate("0") << " (expected 0)" << endl;
    cout << endl;

    cout << "Expression: \"x\" (zero variable)" << endl;
    cout << "Result: " << ep8.calculate("x") << " (expected 0)" << endl;
    cout << endl;

    cout << "Expression: \"1-1\" (result is zero)" << endl;
    cout << "Result: " << ep8.calculate("1-1") << " (expected 0)" << endl;
    cout << endl;

    cout << "Expression: \"z\" (large value)" << endl;
    cout << "Result: " << ep8.calculate("z") << " (expected 100)" << endl;
    cout << endl;

    return 0;
}
