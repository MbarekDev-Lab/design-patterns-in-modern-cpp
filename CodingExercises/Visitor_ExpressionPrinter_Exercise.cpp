/*
 * Visitor Pattern - Expression Printer Coding Exercise
 *
 * The Visitor pattern allows adding new operations to existing object structures
 * without changing those structures. This is achieved through double-dispatch:
 * 1. Call visit() on the expression (virtual call)
 * 2. The expression calls back to the visitor with its concrete type
 *
 * In this exercise:
 * - Expression hierarchy: Value, AdditionExpression, MultiplicationExpression
 * - ExpressionPrinter visitor prints expressions with proper formatting
 * - Addition expressions get parentheses: (a+b)
 * - Multiplication expressions don't: a*b
 * - Nested expressions are handled through recursion
 */

#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
using namespace std;

// Forward declarations
struct Expression;
struct Value;
struct AdditionExpression;
struct MultiplicationExpression;

// Visitor interface
struct ExpressionVisitor
{
    virtual ~ExpressionVisitor() = default;

    // Accept methods for each expression type
    virtual void visit(Value &v) = 0;
    virtual void visit(AdditionExpression &ae) = 0;
    virtual void visit(MultiplicationExpression &me) = 0;
};

// Expression base class
struct Expression
{
    virtual ~Expression() = default;

    // Double-dispatch: calls the appropriate visit method on the visitor
    virtual void visit(ExpressionVisitor &ev) = 0;
};

// Leaf node: represents a numeric value
struct Value : Expression
{
    int value;

    Value(int value) : value(value) {}

    void visit(ExpressionVisitor &ev) override
    {
        ev.visit(*this);
    }
};

// Binary operator: addition
struct AdditionExpression : Expression
{
    Expression &lhs, &rhs;

    AdditionExpression(Expression &lhs, Expression &rhs)
        : lhs(lhs), rhs(rhs) {}

    void visit(ExpressionVisitor &ev) override
    {
        ev.visit(*this); 
    }
};

// Binary operator: multiplication
struct MultiplicationExpression : Expression
{
    Expression &lhs, &rhs;

    MultiplicationExpression(Expression &lhs, Expression &rhs)
        : lhs(lhs), rhs(rhs) {}

    void visit(ExpressionVisitor &ev) override
    {
        ev.visit(*this);
    }
};

// Visitor implementation: prints expressions
struct ExpressionPrinter : ExpressionVisitor
{
    ostringstream oss;

    void visit(Value &v) override
    {
        // Print the numeric value
        oss << v.value;
    }

    void visit(AdditionExpression &ae) override
    {
        // Addition needs parentheses: (a+b)
        oss << "(";
        ae.lhs.visit(*this); // Visit left operand recursively
        oss << "+";
        ae.rhs.visit(*this); // Visit right operand recursively
        oss << ")";
    }

    void visit(MultiplicationExpression &me) override
    {
        // Multiplication doesn't need parentheses: a*b
        me.lhs.visit(*this); // Visit left operand recursively
        oss << "*";
        me.rhs.visit(*this); // Visit right operand recursively
    }

    string str() const
    {
        return oss.str();
    }
};

// ============================================================================
// TEST SUITE
// ============================================================================

void test_simple_literal()
{
    cout << "\n=== Test: Simple Literal ===" << endl;
    Value v2{2};
    ExpressionPrinter ep;
    v2.visit(ep);
    assert(ep.str() == "2");
    cout << "✓ Single value: '2'" << endl;
}

void test_simple_addition()
{
    cout << "\n=== Test: Simple Addition ===" << endl;
    // Example from exercise: (2+3)
    Value v2{2};
    Value v3{3};
    AdditionExpression simple{v2, v3};
    ExpressionPrinter ep;
    simple.visit(ep);
    assert(ep.str() == "(2+3)");
    cout << "✓ (2+3)" << endl;
}

void test_simple_multiplication()
{
    cout << "\n=== Test: Simple Multiplication ===" << endl;
    Value v2{2};
    Value v3{3};
    MultiplicationExpression simple{v2, v3};
    ExpressionPrinter ep;
    simple.visit(ep);
    assert(ep.str() == "2*3");
    cout << "✓ 2*3 (no parentheses for top-level multiplication)" << endl;
}

void test_addition_plus_multiplication()
{
    cout << "\n=== Test: Addition Plus Multiplication ===" << endl;
    // (2+3)*4
    Value v2{2};
    Value v3{3};
    Value v4{4};
    AdditionExpression add{v2, v3};         // 2+3
    MultiplicationExpression mult{add, v4}; // (2+3)*4
    ExpressionPrinter ep;
    mult.visit(ep);
    assert(ep.str() == "(2+3)*4");
    cout << "✓ (2+3)*4 (Addition inside multiplication keeps parentheses)" << endl;
}

void test_multiplication_plus_addition()
{
    cout << "\n=== Test: Multiplication Plus Addition ===" << endl;
    // 2*3 + 4 = (2*3+4)
    Value v2{2};
    Value v3{3};
    Value v4{4};
    MultiplicationExpression mult{v2, v3}; // 2*3
    AdditionExpression add{mult, v4};      // 2*3+4
    ExpressionPrinter ep;
    add.visit(ep);
    assert(ep.str() == "(2*3+4)");
    cout << "✓ (2*3+4) (Multiplication inside addition doesn't need extra parentheses)" << endl;
}

void test_nested_additions()
{
    cout << "\n=== Test: Nested Additions ===" << endl;
    // ((2+3)+4)
    Value v2{2};
    Value v3{3};
    Value v4{4};
    AdditionExpression add1{v2, v3};   // 2+3
    AdditionExpression add2{add1, v4}; // (2+3)+4
    ExpressionPrinter ep;
    add2.visit(ep);
    assert(ep.str() == "((2+3)+4)");
    cout << "✓ ((2+3)+4) (Nested additions have nested parentheses)" << endl;
}

void test_complex_expression1()
{
    cout << "\n=== Test: Complex Expression 1 ===" << endl;
    // (2*3)+(4*5)
    Value v2{2};
    Value v3{3};
    Value v4{4};
    Value v5{5};
    MultiplicationExpression mult1{v2, v3}; // 2*3
    MultiplicationExpression mult2{v4, v5}; // 4*5
    AdditionExpression add{mult1, mult2};   // 2*3+4*5
    ExpressionPrinter ep;
    add.visit(ep);
    assert(ep.str() == "(2*3+4*5)");
    cout << "✓ (2*3+4*5)" << endl;
}

void test_complex_expression2()
{
    cout << "\n=== Test: Complex Expression 2 ===" << endl;
    // 2*(3+4)
    Value v2{2};
    Value v3{3};
    Value v4{4};
    AdditionExpression add{v3, v4};         // 3+4
    MultiplicationExpression mult{v2, add}; // 2*(3+4)
    ExpressionPrinter ep;
    mult.visit(ep);
    assert(ep.str() == "2*(3+4)");
    cout << "✓ 2*(3+4) (Parentheses inside multiplication preserve structure)" << endl;
}

void test_deep_nesting()
{
    cout << "\n=== Test: Deep Nesting ===" << endl;
    // ((2+3)*(4+5))+6
    Value v2{2};
    Value v3{3};
    Value v4{4};
    Value v5{5};
    Value v6{6};
    AdditionExpression add1{v2, v3};           // 2+3
    AdditionExpression add2{v4, v5};           // 4+5
    MultiplicationExpression mult{add1, add2}; // (2+3)*(4+5)
    AdditionExpression add3{mult, v6};         // (2+3)*(4+5)+6
    ExpressionPrinter ep;
    add3.visit(ep);
    assert(ep.str() == "((2+3)*(4+5)+6)");
    cout << "✓ ((2+3)*(4+5)+6)" << endl;
}

void test_large_numbers()
{
    cout << "\n=== Test: Large Numbers ===" << endl;
    // (100+200)
    Value v100{100};
    Value v200{200};
    AdditionExpression add{v100, v200};
    ExpressionPrinter ep;
    add.visit(ep);
    assert(ep.str() == "(100+200)");
    cout << "✓ (100+200)" << endl;
}

void test_multiplication_chain()
{
    cout << "\n=== Test: Multiplication Chain ===" << endl;
    // 2*3*4 = (2*3)*4
    Value v2{2};
    Value v3{3};
    Value v4{4};
    MultiplicationExpression mult1{v2, v3};    // 2*3
    MultiplicationExpression mult2{mult1, v4}; // 2*3*4
    ExpressionPrinter ep;
    mult2.visit(ep);
    assert(ep.str() == "2*3*4");
    cout << "✓ 2*3*4 (Chained multiplications don't add extra parentheses)" << endl;
}

int main()
{
    cout << "Visitor Pattern - Expression Printer Exercise\n"
         << endl;

    try
    {
        cout << "Running 11 comprehensive tests...\n"
             << endl;

        test_simple_literal();
        test_simple_addition();
        test_simple_multiplication();
        test_addition_plus_multiplication();
        test_multiplication_plus_addition();
        test_nested_additions();
        test_complex_expression1();
        test_complex_expression2();
        test_deep_nesting();
        test_large_numbers();
        test_multiplication_chain();

        cout << "\n✅ All 11 tests passed!" << endl;
        cout << "\nVisitor Pattern Benefits (Expression Printer Example):" << endl;
        cout << "  • Separates concerns: Expression structure from printing logic" << endl;
        cout << "  • Double-dispatch enables different code paths for different expression types" << endl;
        cout << "  • Easy to add new operations without modifying Expression classes" << endl;
        cout << "  • Visitor pattern follows Open/Closed Principle" << endl;
        cout << "  • Each visitor can maintain its own state (oss in this case)" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "❌ Test failed: " << e.what() << endl;
        return 1;
    }
}
