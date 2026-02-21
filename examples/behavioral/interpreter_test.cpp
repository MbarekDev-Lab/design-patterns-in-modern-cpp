#include <iostream>
#include <cassert>
#include "../include/behavioral/interpreter.h"

int main()
{
  cout << "=== INTERPRETER PATTERN TESTS ===" << endl;
  cout << endl;

  // ========================================================================
  // SIMPLE EXPRESSION PROCESSOR TESTS
  // ========================================================================

  // === TEST 1: Single Number ===
  {
    cout << "TEST 1: Single Number" << endl;
    SimpleExpressionProcessor sep;
    assert(sep.calculate("5") == 5);
    assert(sep.calculate("42") == 42);
    assert(sep.calculate("0") == 0);
    cout << "  ✓ Single numbers calculate correctly" << endl;
    cout << endl;
  }

  // === TEST 2: Simple Addition ===
  {
    cout << "TEST 2: Simple Addition" << endl;
    SimpleExpressionProcessor sep;
    assert(sep.calculate("1+2") == 3);
    assert(sep.calculate("5+10") == 15);
    assert(sep.calculate("0+0") == 0);
    cout << "  ✓ Addition works correctly" << endl;
    cout << endl;
  }

  // === TEST 3: Simple Subtraction ===
  {
    cout << "TEST 3: Simple Subtraction" << endl;
    SimpleExpressionProcessor sep;
    assert(sep.calculate("5-2") == 3);
    assert(sep.calculate("10-10") == 0);
    assert(sep.calculate("3-5") == -2);
    cout << "  ✓ Subtraction works correctly" << endl;
    cout << endl;
  }

  // === TEST 4: Multiple Operations ===
  {
    cout << "TEST 4: Multiple Operations" << endl;
    SimpleExpressionProcessor sep;
    assert(sep.calculate("1+2+3") == 6);
    assert(sep.calculate("10-5-2") == 3);
    assert(sep.calculate("1+2-3") == 0);
    assert(sep.calculate("5+5-3+2") == 9);
    cout << "  ✓ Multiple operations calculate correctly" << endl;
    cout << endl;
  }

  // === TEST 5: Variables ===
  {
    cout << "TEST 5: Variables" << endl;
    SimpleExpressionProcessor sep;
    sep.variables['x'] = 5;
    sep.variables['y'] = 3;
    
    assert(sep.calculate("x") == 5);
    assert(sep.calculate("1+x") == 6);
    assert(sep.calculate("x+y") == 8);
    assert(sep.calculate("x-y") == 2);
    assert(sep.calculate("x+y+1") == 9);
    cout << "  ✓ Variables work correctly" << endl;
    cout << endl;
  }

  // === TEST 6: Invalid Variable ===
  {
    cout << "TEST 6: Invalid Variable" << endl;
    SimpleExpressionProcessor sep;
    sep.variables['x'] = 5;
    
    assert(sep.calculate("1+xy") == 0);  // Unknown variable 'y'
    assert(sep.calculate("z") == 0);      // Unknown variable 'z'
    assert(sep.calculate("x+z") == 0);    // Unknown variable 'z'
    cout << "  ✓ Invalid variables return 0" << endl;
    cout << endl;
  }

  // ========================================================================
  // ADVANCED INTERPRETER TESTS (Lexing/Parsing)
  // ========================================================================

  // === TEST 7: Lexing Simple Expressions ===
  {
    cout << "TEST 7: Lexing Simple Numbers" << endl;
    auto tokens = lex("5");
    assert(tokens.size() == 1);
    assert(tokens[0].type == Token::integer);
    assert(tokens[0].text == "5");

    tokens = lex("42");
    assert(tokens.size() == 1);
    assert(tokens[0].text == "42");
    cout << "  ✓ Lexing single integers works" << endl;
    cout << endl;
  }

  // === TEST 8: Lexing Operators ===
  {
    cout << "TEST 8: Lexing Operators" << endl;
    auto tokens = lex("2+3");
    assert(tokens.size() == 3);
    assert(tokens[0].type == Token::integer);
    assert(tokens[1].type == Token::plus);
    assert(tokens[2].type == Token::integer);

    tokens = lex("10-5");
    assert(tokens.size() == 3);
    assert(tokens[0].text == "10");
    assert(tokens[1].type == Token::minus);
    assert(tokens[2].text == "5");
    cout << "  ✓ Lexing operators works" << endl;
    cout << endl;
  }

  // === TEST 9: Lexing Parentheses ===
  {
    cout << "TEST 9: Lexing Parentheses" << endl;
    auto tokens = lex("(2+3)");
    assert(tokens.size() == 5);
    assert(tokens[0].type == Token::lparen);
    assert(tokens[4].type == Token::rparen);

    tokens = lex("((5))");
    assert(tokens.size() == 5);
    assert(tokens[0].type == Token::lparen);
    assert(tokens[1].type == Token::lparen);
    assert(tokens[3].type == Token::rparen);
    assert(tokens[4].type == Token::rparen);
    cout << "  ✓ Lexing parentheses works" << endl;
    cout << endl;
  }

  // === TEST 10: Parsing Single Numbers ===
  {
    cout << "TEST 10: Parsing Single Numbers" << endl;
    auto tokens = lex("5");
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast->eval() == 5);

    tokens = lex("42");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 42);
    cout << "  ✓ Parsing single numbers works" << endl;
    cout << endl;
  }

  // === TEST 11: Parsing Addition ===
  {
    cout << "TEST 11: Parsing Addition" << endl;
    auto tokens = lex("2+3");
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast->eval() == 5);

    tokens = lex("10+20");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 30);
    cout << "  ✓ Parsing addition works" << endl;
    cout << endl;
  }

  // === TEST 12: Parsing Subtraction ===
  {
    cout << "TEST 12: Parsing Subtraction" << endl;
    auto tokens = lex("10-3");
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast->eval() == 7);

    tokens = lex("5-5");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 0);

    tokens = lex("3-10");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == -7);
    cout << "  ✓ Parsing subtraction works" << endl;
    cout << endl;
  }

  // === TEST 13: Parsing Multiple Operations ===
  {
    cout << "TEST 13: Parsing Multiple Operations" << endl;
    auto tokens = lex("1+2+3");
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast->eval() == 6);

    tokens = lex("10-2-3");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 5);

    tokens = lex("1+2-3+4");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 4);
    cout << "  ✓ Parsing multiple operations works" << endl;
    cout << endl;
  }

  // === TEST 14: Parsing Simple Parentheses ===
  {
    cout << "TEST 14: Parsing Simple Parentheses" << endl;
    auto tokens = lex("(5)");
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast->eval() == 5);

    tokens = lex("(2+3)");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 5);
    cout << "  ✓ Parsing parentheses works" << endl;
    cout << endl;
  }

  // === TEST 15: Parsing Nested Parentheses ===
  {
    cout << "TEST 15: Parsing Nested Parentheses" << endl;
    auto tokens = lex("((5))");
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast->eval() == 5);

    tokens = lex("(1+(2+3))+4");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 10);  // 1 + (2+3) + 4 = 1 + 5 + 4 = 10
    cout << "  ✓ Parsing nested parentheses works" << endl;
    cout << endl;
  }

  // === TEST 16: Complex Expressions ===
  {
    cout << "TEST 16: Complex Expressions" << endl;
    auto tokens = lex("(10-5)+3");
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast->eval() == 8);  // (10-5) + 3 = 5 + 3 = 8

    tokens = lex("10-(5-3)");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 8);  // 10 - (5-3) = 10 - 2 = 8

    tokens = lex("((10-5)+3)-2");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 6);  // ((10-5)+3)-2 = (5+3)-2 = 8-2 = 6
    cout << "  ✓ Complex expressions evaluate correctly" << endl;
    cout << endl;
  }

  // === TEST 17: Whitespace Handling ===
  {
    cout << "TEST 17: Whitespace Handling" << endl;
    auto tokens = lex("2 + 3");
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast->eval() == 5);

    tokens = lex(" ( 5 ) ");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 5);
    cout << "  ✓ Whitespace is handled correctly" << endl;
    cout << endl;
  }

  // === TEST 18: Multi-digit Numbers ===
  {
    cout << "TEST 18: Multi-digit Numbers" << endl;
    auto tokens = lex("100+200");
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast->eval() == 300);

    tokens = lex("1000-999");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 1);
    cout << "  ✓ Multi-digit numbers work" << endl;
    cout << endl;
  }

  // === TEST 19: Zero in Expressions ===
  {
    cout << "TEST 19: Zero in Expressions" << endl;
    auto tokens = lex("0+5");
    Parser parser(tokens);
    auto ast = parser.parse();
    assert(ast->eval() == 5);

    tokens = lex("5-0");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 5);

    tokens = lex("0");
    parser = Parser(tokens);
    ast = parser.parse();
    assert(ast->eval() == 0);
    cout << "  ✓ Zero in expressions works correctly" << endl;
    cout << endl;
  }

  cout << "=== ALL TESTS PASSED ===" << endl;
  return 0;
}
