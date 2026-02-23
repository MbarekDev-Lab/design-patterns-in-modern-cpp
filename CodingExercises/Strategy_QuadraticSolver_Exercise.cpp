/*
 * Strategy Pattern - Quadratic Equation Solver Coding Exercise
 *
 * Problem: Implement a quadratic equation solver that can use different
 * strategies for calculating the discriminant.
 *
 * Quadratic equation: ax^2 + bx + c = 0
 * Discriminant: Δ = b^2 - 4ac
 * Solutions: x = (-b ± √Δ) / 2a
 *
 * Two strategies:
 * 1. OrdinaryDiscriminantStrategy: Returns any discriminant (can be negative)
 * 2. RealDiscriminantStrategy: Returns NaN if discriminant is negative
 *
 * This allows handling of complex solutions vs real-only solutions
 */

#include <iostream>
#include <complex>
#include <tuple>
#include <cmath>
#include <limits>
#include <cassert>
#include <iomanip>
using namespace std;

// ============================================================================
// STRATEGY INTERFACE
// ============================================================================

struct DiscriminantStrategy
{
    virtual ~DiscriminantStrategy() = default;
    virtual double calculate_discriminant(double a, double b, double c) = 0;
};

// ============================================================================
// STRATEGY IMPLEMENTATIONS
// ============================================================================

struct OrdinaryDiscriminantStrategy : DiscriminantStrategy
{
    /*
     * Ordinary strategy: Always return the calculated discriminant
     * This allows for complex number solutions if discriminant is negative
     */
    double calculate_discriminant(double a, double b, double c) override
    {
        return b * b - 4 * a * c;
    }
};

struct RealDiscriminantStrategy : DiscriminantStrategy
{
    /*
     * Real strategy: Return NaN if discriminant is negative
     * This restricts solutions to real numbers only
     */
    double calculate_discriminant(double a, double b, double c) override
    {
        double result = b * b - 4 * a * c;
        return result >= 0 ? result : numeric_limits<double>::quiet_NaN();
    }
};

// ============================================================================
// QUADRATIC EQUATION SOLVER
// ============================================================================

class QuadraticEquationSolver
{
    DiscriminantStrategy &strategy;

public:
    QuadraticEquationSolver(DiscriminantStrategy &strategy)
        : strategy(strategy)
    {
    }

    tuple<complex<double>, complex<double>> solve(double a, double b, double c)
    {
        if (a == 0)
            throw runtime_error("Coefficient a cannot be zero");

        double disc = strategy.calculate_discriminant(a, b, c);
        complex<double> disc_complex{disc, 0};
        auto root_disc = sqrt(disc_complex);

        complex<double> x1 = (-b + root_disc) / (2.0 * a);
        complex<double> x2 = (-b - root_disc) / (2.0 * a);

        return {x1, x2};
    }
};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

bool are_close(complex<double> a, complex<double> b, double epsilon = 1e-9)
{
    return abs(a.real() - b.real()) < epsilon &&
           abs(a.imag() - b.imag()) < epsilon;
}

bool is_nan_complex(complex<double> c)
{
    return isnan(c.real()) && isnan(c.imag());
}

void print_solution(complex<double> x, int num)
{
    cout << "x" << num << " = ";
    if (isnan(x.real()) && isnan(x.imag()))
    {
        cout << "NaN (no real solution)";
    }
    else if (abs(x.imag()) < 1e-9)
    {
        cout << x.real();
    }
    else if (x.imag() > 0)
    {
        cout << x.real() << " + " << x.imag() << "i";
    }
    else
    {
        cout << x.real() << " - " << (-x.imag()) << "i";
    }
    cout << endl;
}

// ============================================================================
// TEST SUITE
// ============================================================================

void test_ordinary_positive_discriminant()
{
    OrdinaryDiscriminantStrategy strategy;
    QuadraticEquationSolver solver{strategy};

    // x^2 + 10x + 16 = 0
    // Discriminant = 100 - 64 = 36
    // Solutions: x = (-10 ± 6) / 2 = -2, -8
    auto [x1, x2] = solver.solve(1, 10, 16);

    assert(are_close(x1, complex<double>(-2, 0)));
    assert(are_close(x2, complex<double>(-8, 0)));
    cout << "✓ Ordinary strategy with positive discriminant" << endl;
}

void test_ordinary_negative_discriminant()
{
    OrdinaryDiscriminantStrategy strategy;
    QuadraticEquationSolver solver{strategy};

    // x^2 + 4x + 5 = 0
    // Discriminant = 16 - 20 = -4
    // Solutions: x = (-4 ± 2i) / 2 = -2 ± i
    auto [x1, x2] = solver.solve(1, 4, 5);

    assert(are_close(x1, complex<double>(-2, 1)));
    assert(are_close(x2, complex<double>(-2, -1)));
    cout << "✓ Ordinary strategy with negative discriminant (complex)" << endl;
}

void test_ordinary_zero_discriminant()
{
    OrdinaryDiscriminantStrategy strategy;
    QuadraticEquationSolver solver{strategy};

    // x^2 - 2x + 1 = 0
    // Discriminant = 4 - 4 = 0
    // Solutions: x = 2/2 = 1, 1 (repeated root)
    auto [x1, x2] = solver.solve(1, -2, 1);

    assert(are_close(x1, complex<double>(1, 0)));
    assert(are_close(x2, complex<double>(1, 0)));
    cout << "✓ Ordinary strategy with zero discriminant" << endl;
}

void test_real_positive_discriminant()
{
    RealDiscriminantStrategy strategy;
    QuadraticEquationSolver solver{strategy};

    // x^2 + 10x + 16 = 0
    // Discriminant = 36 (positive)
    // Solutions: x = -2, -8
    auto [x1, x2] = solver.solve(1, 10, 16);

    assert(are_close(x1, complex<double>(-2, 0)));
    assert(are_close(x2, complex<double>(-8, 0)));
    cout << "✓ Real strategy with positive discriminant" << endl;
}

void test_real_negative_discriminant()
{
    RealDiscriminantStrategy strategy;
    QuadraticEquationSolver solver{strategy};

    // x^2 + 4x + 5 = 0
    // Discriminant = -4 (negative)
    // Should return NaN for real-only solutions
    auto [x1, x2] = solver.solve(1, 4, 5);

    assert(is_nan_complex(x1));
    assert(is_nan_complex(x2));
    cout << "✓ Real strategy with negative discriminant returns NaN" << endl;
}

void test_real_zero_discriminant()
{
    RealDiscriminantStrategy strategy;
    QuadraticEquationSolver solver{strategy};

    // x^2 - 2x + 1 = 0
    // Discriminant = 0
    // Solutions: x = 1
    auto [x1, x2] = solver.solve(1, -2, 1);

    assert(are_close(x1, complex<double>(1, 0)));
    assert(are_close(x2, complex<double>(1, 0)));
    cout << "✓ Real strategy with zero discriminant" << endl;
}

void test_different_coefficients()
{
    OrdinaryDiscriminantStrategy strategy;
    QuadraticEquationSolver solver{strategy};

    // 2x^2 - 5x + 2 = 0
    // Discriminant = 25 - 16 = 9
    // Solutions: x = (5 ± 3) / 4 = 2, 0.5
    auto [x1, x2] = solver.solve(2, -5, 2);

    assert(are_close(x1, complex<double>(2, 0)));
    assert(are_close(x2, complex<double>(0.5, 0)));
    cout << "✓ Different coefficients" << endl;
}

void test_negative_leading_coefficient()
{
    OrdinaryDiscriminantStrategy strategy;
    QuadraticEquationSolver solver{strategy};

    // -x^2 + 5x - 6 = 0
    // Discriminant = 25 - 24 = 1
    // Solutions: x = (-5 ± 1) / -2 = 2, 3
    auto [x1, x2] = solver.solve(-1, 5, -6);

    assert(are_close(x1, complex<double>(2, 0)));
    assert(are_close(x2, complex<double>(3, 0)));
    cout << "✓ Negative leading coefficient" << endl;
}

void test_zero_coefficient_a_throws()
{
    OrdinaryDiscriminantStrategy strategy;
    QuadraticEquationSolver solver{strategy};

    try
    {
        solver.solve(0, 5, 3);
        assert(false); // Should have thrown
    }
    catch (const runtime_error &e)
    {
        assert(string(e.what()).find("a cannot be zero") != string::npos);
        cout << "✓ Exception when a = 0" << endl;
    }
}

void test_large_numbers()
{
    OrdinaryDiscriminantStrategy strategy;
    QuadraticEquationSolver solver{strategy};

    // Large coefficient equation
    // 1000x^2 + 2000x + 1 = 0
    (void)solver.solve(1000, 2000, 1);

    // Just verify no exception is thrown
    cout << "✓ Large numbers handling" << endl;
}

void test_fractional_solutions()
{
    OrdinaryDiscriminantStrategy strategy;
    QuadraticEquationSolver solver{strategy};

    // 3x^2 - 6x + 2 = 0
    // Discriminant = 36 - 24 = 12
    // Solutions: x = (6 ± √12) / 6 = 1 ± (√3/3)
    auto [x1, x2] = solver.solve(3, -6, 2);

    // Approximate values
    assert(x1.real() > 1.4 && x1.real() < 1.6);
    assert(x2.real() > 0.4 && x2.real() < 0.6);
    cout << "✓ Fractional solutions" << endl;
}

void test_strategy_switching()
{
    // Start with ordinary strategy
    OrdinaryDiscriminantStrategy ord_strategy;
    QuadraticEquationSolver solver1{ord_strategy};
    auto [x1_ord, x2_ord] = solver1.solve(1, 4, 5);

    // Should have complex solutions
    assert(abs(x1_ord.imag()) > 0);

    // Switch to real strategy
    RealDiscriminantStrategy real_strategy;
    QuadraticEquationSolver solver2{real_strategy};
    auto [x1_real, x2_real] = solver2.solve(1, 4, 5);

    // Should have NaN
    assert(isnan(x1_real.real()));

    cout << "✓ Strategy switching" << endl;
}

void test_output_demonstration()
{
    cout << "\n--- Quadratic Solver Examples ---\n"
         << endl;

    cout << "Example 1: x^2 + 10x + 16 = 0" << endl;
    cout << "Using Ordinary Strategy:" << endl;
    OrdinaryDiscriminantStrategy ord;
    QuadraticEquationSolver solver1{ord};
    auto [x1, x2] = solver1.solve(1, 10, 16);
    print_solution(x1, 1);
    print_solution(x2, 2);

    cout << "\nExample 2: x^2 + 4x + 5 = 0" << endl;
    cout << "Using Ordinary Strategy (gets complex solutions):" << endl;
    auto [x3, x4] = solver1.solve(1, 4, 5);
    print_solution(x3, 1);
    print_solution(x4, 2);

    cout << "\nUsing Real Strategy (gets NaN):" << endl;
    RealDiscriminantStrategy real;
    QuadraticEquationSolver solver2{real};
    auto [x5, x6] = solver2.solve(1, 4, 5);
    print_solution(x5, 1);
    print_solution(x6, 2);
}

int main()
{
    cout << "Strategy Pattern - Quadratic Equation Solver Exercise\n"
         << endl;

    try
    {
        cout << "Running tests...\n"
             << endl;

        test_ordinary_positive_discriminant();
        test_ordinary_negative_discriminant();
        test_ordinary_zero_discriminant();

        test_real_positive_discriminant();
        test_real_negative_discriminant();
        test_real_zero_discriminant();

        test_different_coefficients();
        test_negative_leading_coefficient();
        test_zero_coefficient_a_throws();
        test_large_numbers();
        test_fractional_solutions();
        test_strategy_switching();

        test_output_demonstration();

        cout << "\n✅ All 12 tests passed!" << endl;
        cout << "\nStrategy Pattern Benefits in This Example:" << endl;
        cout << "  • Separates discriminant calculation from solving logic" << endl;
        cout << "  • Two different strategies for different requirements:" << endl;
        cout << "    - Ordinary: Allows complex solutions" << endl;
        cout << "    - Real: Returns NaN for imaginary solutions" << endl;
        cout << "  • Easy to add new strategies without modifying solver" << endl;
        cout << "  • Runtime strategy selection via constructor" << endl;
        cout << "  • Clean separation of concerns" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Test failed with exception: " << e.what() << endl;
        return 1;
    }
}
