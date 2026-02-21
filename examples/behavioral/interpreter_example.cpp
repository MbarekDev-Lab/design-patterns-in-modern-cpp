#include <iostream>
#include "../include/behavioral/interpreter.h"

int main()
{
  cout << "=== INTERPRETER PATTERN EXAMPLES ===" << endl;
  cout << endl;

  // ========================================================================
  // EXAMPLE 1: Simple Expression Processor with Variables
  // ========================================================================
  cout << "--- EXAMPLE 1: Simple Expression Processor ---" << endl;
  SimpleExpressionProcessor sep;
  sep.variables['x'] = 5;
  sep.variables['y'] = 10;
  sep.variables['z'] = 3;

  cout << "Variables: x=5, y=10, z=3" << endl;
  cout << endl;

  cout << "Expression: \"1\"" << endl;
  cout << "Result: " << sep.calculate("1") << endl;
  cout << endl;

  cout << "Expression: \"1+2\"" << endl;
  cout << "Result: " << sep.calculate("1+2") << endl;
  cout << endl;

  cout << "Expression: \"1+x\"" << endl;
  cout << "Result: " << sep.calculate("1+x") << endl;
  cout << endl;

  cout << "Expression: \"x+y\"" << endl;
  cout << "Result: " << sep.calculate("x+y") << endl;
  cout << endl;

  cout << "Expression: \"x+y-z\"" << endl;
  cout << "Result: " << sep.calculate("x+y-z") << endl;
  cout << endl;

  cout << "Expression: \"10-x\"" << endl;
  cout << "Result: " << sep.calculate("10-x") << endl;
  cout << endl;

  // ========================================================================
  // EXAMPLE 2: Advanced Interpreter with Lexing and Parsing
  // ========================================================================
  cout << "--- EXAMPLE 2: Advanced Interpreter (Lexing/Parsing) ---" << endl;
  
  vector<string> expressions{"2+3", "10-5", "1+2+3", "10-3-2", "(2+3)+4", "(10-5)*2"};
  
  for (const auto& expr : expressions)
  {
    try
    {
      cout << "Expression: \"" << expr << "\"" << endl;
      
      // Lexing phase
      auto tokens = lex(expr);
      cout << "  Tokens: ";
      for (auto& token : tokens)
        cout << token << " ";
      cout << endl;

      // Parsing phase
      Parser parser(tokens);
      auto ast = parser.parse();

      // Evaluation phase
      int result = ast->eval();
      cout << "  Result: " << result << endl;
      cout << endl;
    }
    catch (const exception& e)
    {
      cout << "  Error: " << e.what() << endl;
      cout << endl;
    }
  }

  // ========================================================================
  // EXAMPLE 3: Nested Expressions
  // ========================================================================
  cout << "--- EXAMPLE 3: Nested Expressions ---" << endl;
  vector<string> nested_expressions{
    "((5))",
    "(1+2)+3",
    "1+(2+3)",
    "((10-5)+3)-2",
    "5+((10-2)*(3-1))+1"  // Note: may fail due to no * operator
  };

  for (const auto& expr : nested_expressions)
  {
    try
    {
      cout << "Expression: \"" << expr << "\"" << endl;
      auto tokens = lex(expr);
      Parser parser(tokens);
      auto ast = parser.parse();
      int result = ast->eval();
      cout << "  Result: " << result << endl;
      cout << endl;
    }
    catch (const exception& e)
    {
      cout << "  Error: " << e.what() << endl;
      cout << endl;
    }
  }

  return 0;
}
