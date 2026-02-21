#include <iostream>
#include <cassert>
#include "../include/behavioral/interpreter.h"

int main()
{
  cout << "=== INTERPRETER PATTERN CODING EXERCISE ===" << endl;
  cout << endl;

  cout << "Exercise: Implement SimpleExpressionProcessor::calculate()" << endl;
  cout << "Requirements:" << endl;
  cout << "  1. Support single digit integers" << endl;
  cout << "  2. Support single-character variables" << endl;
  cout << "  3. Support + and - operators" << endl;
  cout << "  4. Return 0 for invalid variables" << endl;
  cout << endl;

  cout << "--- Test Cases ---" << endl;
  cout << endl;

  // Test 1: Single number
  {
    cout << "Test 1: Single number" << endl;
    SimpleExpressionProcessor ep;
    int result = ep.calculate("1");
    cout << "  Input: \"1\"" << endl;
    cout << "  Expected: 1" << endl;
    cout << "  Got: " << result << endl;
    assert(result == 1);
    cout << "  ✓ PASS" << endl;
    cout << endl;
  }

  // Test 2: Simple addition
  {
    cout << "Test 2: Simple addition" << endl;
    SimpleExpressionProcessor ep;
    int result = ep.calculate("1+2");
    cout << "  Input: \"1+2\"" << endl;
    cout << "  Expected: 3" << endl;
    cout << "  Got: " << result << endl;
    assert(result == 3);
    cout << "  ✓ PASS" << endl;
    cout << endl;
  }

  // Test 3: Variable addition
  {
    cout << "Test 3: Variable addition" << endl;
    SimpleExpressionProcessor ep;
    ep.variables['x'] = 5;
    int result = ep.calculate("1+x");
    cout << "  Variables: x=5" << endl;
    cout << "  Input: \"1+x\"" << endl;
    cout << "  Expected: 6" << endl;
    cout << "  Got: " << result << endl;
    assert(result == 6);
    cout << "  ✓ PASS" << endl;
    cout << endl;
  }

  // Test 4: Invalid variable
  {
    cout << "Test 4: Invalid variable" << endl;
    SimpleExpressionProcessor ep;
    ep.variables['x'] = 5;
    int result = ep.calculate("1+xy");
    cout << "  Variables: x=5" << endl;
    cout << "  Input: \"1+xy\"" << endl;
    cout << "  Expected: 0 (xy is not a valid single-char variable)" << endl;
    cout << "  Got: " << result << endl;
    assert(result == 0);
    cout << "  ✓ PASS" << endl;
    cout << endl;
  }

  // Test 5: Subtraction
  {
    cout << "Test 5: Subtraction" << endl;
    SimpleExpressionProcessor ep;
    ep.variables['a'] = 10;
    int result = ep.calculate("a-3");
    cout << "  Variables: a=10" << endl;
    cout << "  Input: \"a-3\"" << endl;
    cout << "  Expected: 7" << endl;
    cout << "  Got: " << result << endl;
    assert(result == 7);
    cout << "  ✓ PASS" << endl;
    cout << endl;
  }

  // Test 6: Complex expression
  {
    cout << "Test 6: Complex expression" << endl;
    SimpleExpressionProcessor ep;
    ep.variables['x'] = 5;
    ep.variables['y'] = 3;
    int result = ep.calculate("x+y-2");
    cout << "  Variables: x=5, y=3" << endl;
    cout << "  Input: \"x+y-2\"" << endl;
    cout << "  Expected: 6 (5+3-2)" << endl;
    cout << "  Got: " << result << endl;
    assert(result == 6);
    cout << "  ✓ PASS" << endl;
    cout << endl;
  }

  // Test 7: Negative result
  {
    cout << "Test 7: Negative result" << endl;
    SimpleExpressionProcessor ep;
    int result = ep.calculate("1-5");
    cout << "  Input: \"1-5\"" << endl;
    cout << "  Expected: -4" << endl;
    cout << "  Got: " << result << endl;
    assert(result == -4);
    cout << "  ✓ PASS" << endl;
    cout << endl;
  }

  // Test 8: All variables
  {
    cout << "Test 8: All variables" << endl;
    SimpleExpressionProcessor ep;
    ep.variables['a'] = 2;
    ep.variables['b'] = 3;
    ep.variables['c'] = 5;
    int result = ep.calculate("a+b+c");
    cout << "  Variables: a=2, b=3, c=5" << endl;
    cout << "  Input: \"a+b+c\"" << endl;
    cout << "  Expected: 10" << endl;
    cout << "  Got: " << result << endl;
    assert(result == 10);
    cout << "  ✓ PASS" << endl;
    cout << endl;
  }

  // Test 9: Zero
  {
    cout << "Test 9: Zero in expression" << endl;
    SimpleExpressionProcessor ep;
    ep.variables['x'] = 5;
    int result = ep.calculate("0+x");
    cout << "  Variables: x=5" << endl;
    cout << "  Input: \"0+x\"" << endl;
    cout << "  Expected: 5" << endl;
    cout << "  Got: " << result << endl;
    assert(result == 5);
    cout << "  ✓ PASS" << endl;
    cout << endl;
  }

  // Test 10: Multiple same variables
  {
    cout << "Test 10: Multiple same variables" << endl;
    SimpleExpressionProcessor ep;
    ep.variables['x'] = 5;
    int result = ep.calculate("x+x");
    cout << "  Variables: x=5" << endl;
    cout << "  Input: \"x+x\"" << endl;
    cout << "  Expected: 10" << endl;
    cout << "  Got: " << result << endl;
    assert(result == 10);
    cout << "  ✓ PASS" << endl;
    cout << endl;
  }

  cout << "=== ALL EXERCISE TESTS PASSED ===" << endl;
  return 0;
}
