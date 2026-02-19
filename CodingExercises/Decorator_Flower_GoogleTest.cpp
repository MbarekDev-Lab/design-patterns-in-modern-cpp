#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>

using namespace std;

// ============================================================================
// FLOWER DECORATOR EXERCISE - Google Test Version
// ============================================================================
//
// A classic decorator pattern exercise demonstrating:
// - Smart color deduplication
// - Proper "and" handling with multiple colors
// - Complex nested decorator behavior
//
// The key insight: Decorators check if their color already exists
// before adding it, preventing duplicates and using proper connectors.

struct Flower
{
    virtual string str() = 0;
    virtual ~Flower() = default;
};

struct Rose : Flower
{
    string str() override
    {
        return "A rose";
    }
};

struct RedFlower : Flower
{
    Flower &flower;

    RedFlower(Flower &flower) : flower(flower) {}

    string str() override
    {
        string s = flower.str();

        // Already red - don't duplicate
        if (s.find("red") != string::npos)
            return s;

        // Blue exists, add "and red"
        else if (s.find("blue") != string::npos)
        {
            return s + " and red";
        }

        // First color - use "that is"
        else
            return s + " that is red";
    }
};

struct BlueFlower : Flower
{
    Flower &flower;

    BlueFlower(Flower &flower) : flower(flower) {}

    string str() override
    {
        string s = flower.str();

        // Already blue - don't duplicate
        if (s.find("blue") != string::npos)
            return s;

        // Red exists, add "and blue"
        else if (s.find("red") != string::npos)
        {
            return s + " and blue";
        }

        // First color - use "that is"
        else
            return s + " that is blue";
    }
};

// ============================================================================
// GOOGLE TEST TESTS
// ============================================================================

#include "gtest/gtest.h"

namespace
{
    class Evaluate : public testing::Test
    {
    public:
        Rose rose;
        BlueFlower blue_rose{rose};
        RedFlower red_rose{rose};
    };

    // Test 1: Baseline - plain rose
    TEST_F(Evaluate, BaselineTest)
    {
        ASSERT_EQ("A rose", Rose{}.str());
    }

    // Test 2: Single colors applied independently
    TEST_F(Evaluate, SingleColorTests)
    {
        ASSERT_EQ("A rose that is blue", blue_rose.str());
        ASSERT_EQ("A rose that is red", red_rose.str());
    }

    // Test 3: Repetition - applying same color twice doesn't duplicate
    TEST_F(Evaluate, RepetitionTest)
    {
        ASSERT_EQ("A rose that is red", RedFlower{red_rose}.str())
            << "This rose was made red twice, but you should print once.";
    }

    // Test 4: Multiple colors - proper "and" handling
    TEST_F(Evaluate, MultiColorTests)
    {
        // Red first, then blue
        ASSERT_EQ("A rose that is red and blue",
                  BlueFlower{red_rose}.str());

        // Blue first, then red
        ASSERT_EQ("A rose that is blue and red",
                  RedFlower{blue_rose}.str());
    }

    // Test 5: Complex nested case - Red{Blue{Red{Rose{}}}}
    TEST_F(Evaluate, NestedRepetitionTest)
    {
        BlueFlower b_r_rose{red_rose};
        RedFlower r_b_r_rose{b_r_rose};

        ASSERT_EQ("A rose that is red and blue",
                  r_b_r_rose.str())
            << "This is a complicated case. I'm expecting that "
            << "a rose defined as Red{Blue{Red{Rose{}}}} is printed "
            << "as 'red and blue'.";
    }

} // namespace

// ============================================================================
// MAIN
// ============================================================================

int main(int ac, char *av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}
