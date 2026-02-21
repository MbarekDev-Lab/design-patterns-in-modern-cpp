#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cctype>
#include <iostream>

using namespace std;

// ============================================================================
// SIMPLE VERSION: Expression Processor (Exercise Approach)
// ============================================================================
// Simple interpreter that evaluates expressions with variables
// Supports: numbers, variables (single char), +, - operators
// Example: "1+2", "x+5", "a-b+3"
// ============================================================================

inline vector<string> split(const string &expression)
{
    vector<string> result;
    size_t pos = 0, lastPos = 0;
    while ((pos = expression.find_first_of("+-", lastPos)) != string::npos)
    {
        result.push_back(expression.substr(lastPos, pos - lastPos + 1));
        lastPos = pos + 1;
    }
    result.push_back(expression.substr(lastPos));
    return result;
}

struct SimpleExpressionProcessor
{
    map<char, int> variables;

    enum NextOp
    {
        nothing,
        plus,
        minus
    };

    int calculate(const string &expression)
    {
        int current = 0;
        auto next_op = nothing;

        auto parts = split(expression);

        for (auto &part : parts)
        {
            auto first = part.substr(0, part.find_first_of("+-"));
            int value = 0;

            // Try to parse as integer
            try
            {
                value = stoi(first);
            }
            catch (const invalid_argument &)
            {
                // Try as variable
                if (first.length() == 1 && variables.find(first[0]) != variables.end())
                {
                    value = variables[first[0]];
                }
                else
                {
                    return 0; // Invalid variable
                }
            }

            switch (next_op)
            {
            case nothing:
                current = value;
                break;
            case plus:
                current += value;
                break;
            case minus:
                current -= value;
                break;
            }

            // Determine next operation
            if (!part.empty())
            {
                char last_char = part.back();
                if (last_char == '+')
                    next_op = plus;
                else if (last_char == '-')
                    next_op = minus;
            }
        }

        return current;
    }
};

// ============================================================================
// ADVANCED VERSION: Lexing/Parsing/Evaluation (Three-Stage Interpreter)
// ============================================================================
// Full interpreter with lexical analysis, parsing, and evaluation
// Supports: integers, +, -, (, ) operators
// Example: "2+3", "(1+2)*3", "10-5+3"
// ============================================================================

struct Token
{
    enum Type
    {
        integer,
        plus,
        minus,
        lparen,
        rparen
    } type;
    string text;

    explicit Token(Type type, const string &text)
        : type{type}, text{text}
    {
    }

    friend ostream &operator<<(ostream &os, const Token &obj)
    {
        return os << "`" << obj.text << "`";
    }
};

// ============================================================================
// LEXING PHASE: Convert string to tokens
// ============================================================================
vector<Token> lex(const string &input)
{
    vector<Token> result;

    for (size_t i = 0; i < input.size(); ++i)
    {
        switch (input[i])
        {
        case '+':
            result.push_back(Token{Token::plus, "+"});
            break;
        case '-':
            result.push_back(Token{Token::minus, "-"});
            break;
        case '(':
            result.push_back(Token{Token::lparen, "("});
            break;
        case ')':
            result.push_back(Token{Token::rparen, ")"});
            break;
        case ' ':
            // Skip whitespace
            break;
        default:
            if (isdigit(input[i]))
            {
                ostringstream buffer;
                buffer << input[i];
                for (size_t j = i + 1; j < input.size(); ++j)
                {
                    if (isdigit(input[j]))
                    {
                        buffer << input[j];
                        ++i;
                    }
                    else
                    {
                        break;
                    }
                }
                result.push_back(Token{Token::integer, buffer.str()});
            }
            break;
        }
    }

    return result;
}

// ============================================================================
// PARSING PHASE: Convert tokens to Abstract Syntax Tree (AST)
// ============================================================================

struct Element
{
    virtual ~Element() = default;
    virtual int eval() const = 0;
};

struct Integer : Element
{
    int value;

    explicit Integer(int value)
        : value(value)
    {
    }

    int eval() const override { return value; }
};

struct BinaryOperation : Element
{
    enum Type
    {
        addition,
        subtraction
    } type;
    shared_ptr<Element> lhs, rhs;

    int eval() const override
    {
        if (type == addition)
            return lhs->eval() + rhs->eval();
        return lhs->eval() - rhs->eval();
    }
};

class Parser
{
public:
    Parser(const vector<Token> &tokens)
        : tokens(tokens), pos(0)
    {
    }

    shared_ptr<Element> parse()
    {
        auto result = parseExpression();
        return result;
    }

private:
    const vector<Token> &tokens;
    size_t pos;

    shared_ptr<Element> parseExpression()
    {
        auto lhs = parsePrimary();

        while (pos < tokens.size() &&
               (tokens[pos].type == Token::plus || tokens[pos].type == Token::minus))
        {
            auto op_token = tokens[pos++];
            auto rhs = parsePrimary();

            auto result = make_shared<BinaryOperation>();
            result->type = (op_token.type == Token::plus)
                               ? BinaryOperation::addition
                               : BinaryOperation::subtraction;
            result->lhs = lhs;
            result->rhs = rhs;
            lhs = result;
        }

        return lhs;
    }

    shared_ptr<Element> parsePrimary()
    {
        if (pos >= tokens.size())
            throw runtime_error("Unexpected end of expression");

        auto &token = tokens[pos];

        if (token.type == Token::integer)
        {
            pos++;
            int value = stoi(token.text);
            return make_shared<Integer>(value);
        }
        else if (token.type == Token::lparen)
        {
            pos++; // consume '('
            auto expr = parseExpression();
            if (pos >= tokens.size() || tokens[pos].type != Token::rparen)
                throw runtime_error("Expected ')'");
            pos++; // consume ')'
            return expr;
        }
        else
        {
            throw runtime_error("Unexpected token: " + token.text);
        }
    }
};

// ============================================================================
// EVALUATION PHASE: Execute the AST
// ============================================================================
// (Already implemented in Element and subclass eval() methods)
