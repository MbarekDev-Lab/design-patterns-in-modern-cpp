#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <map>
#include <memory>

using namespace std;

/*
 * FLYWEIGHT SENTENCE CODING EXERCISE
 *
 * PROBLEM:
 * Implement a Sentence class that allows per-word formatting (capitalize, bold, italic)
 * without storing formatting data for each word instance. Use the Flyweight pattern
 * to share formatting information efficiently.
 *
 * REQUIREMENTS:
 * 1. Parse sentence into words
 * 2. Use operator[] to access WordToken for each word
 * 3. Support capitalize attribute
 * 4. Return formatted sentence via str() method
 * 5. Memory efficient (tokens are lightweight)
 *
 * VARIATIONS:
 * 1. Basic: Single attribute (capitalize)
 * 2. Enhanced: Multiple attributes (bold, italic, underline)
 * 3. Shared: Using flyweight factory for token sharing
 */

namespace flyweight_sentence
{

    // ============================================================================
    // SOLUTION 1: Basic Sentence with Single Attribute
    // ============================================================================

    namespace basic
    {

        struct Sentence
        {
            struct WordToken
            {
                bool capitalize = false;
            };

            vector<string> words;
            vector<WordToken> tokens;

            Sentence(const string &text)
            {
                stringstream ss(text);
                string word;

                while (ss >> word)
                {
                    words.push_back(word);
                    tokens.emplace_back();
                }
            }

            WordToken &operator[](size_t index)
            {
                return tokens[index];
            }

            string str() const
            {
                string result;

                for (size_t i = 0; i < words.size(); ++i)
                {
                    string word = words[i];

                    if (tokens[i].capitalize)
                    {
                        for (char &c : word)
                            c = toupper(c);
                    }

                    result += word;

                    if (i + 1 < words.size())
                        result += " ";
                }

                return result;
            }
        };

    } // namespace basic

    // ============================================================================
    // SOLUTION 2: Enhanced Sentence with Multiple Attributes
    // ============================================================================

    namespace enhanced
    {

        struct Sentence
        {
            struct WordToken
            {
                bool capitalize = false;
                bool bold = false;
                bool italic = false;
                bool underline = false;
            };

            vector<string> words;
            vector<WordToken> tokens;

            Sentence(const string &text)
            {
                stringstream ss(text);
                string word;

                while (ss >> word)
                {
                    words.push_back(word);
                    tokens.emplace_back();
                }
            }

            WordToken &operator[](size_t index)
            {
                return tokens[index];
            }

            string str() const
            {
                string result;

                for (size_t i = 0; i < words.size(); ++i)
                {
                    string word = words[i];

                    // Apply capitalization
                    if (tokens[i].capitalize)
                    {
                        for (char &c : word)
                            c = toupper(c);
                    }

                    result += word;

                    if (i + 1 < words.size())
                        result += " ";
                }

                return result;
            }

            // Get HTML formatted text with tags
            string html() const
            {
                string result;

                for (size_t i = 0; i < words.size(); ++i)
                {
                    string word = words[i];

                    // Apply formatting
                    if (tokens[i].underline)
                        result += "<u>";
                    if (tokens[i].bold)
                        result += "<b>";
                    if (tokens[i].italic)
                        result += "<i>";

                    // Apply capitalization
                    if (tokens[i].capitalize)
                    {
                        for (char &c : word)
                            c = toupper(c);
                    }

                    result += word;

                    // Close formatting tags
                    if (tokens[i].italic)
                        result += "</i>";
                    if (tokens[i].bold)
                        result += "</b>";
                    if (tokens[i].underline)
                        result += "</u>";

                    if (i + 1 < words.size())
                        result += " ";
                }

                return result;
            }
        };

    } // namespace enhanced

    // ============================================================================
    // SOLUTION 3: Shared Flyweight Token Factory
    // ============================================================================

    namespace shared_flyweight
    {

        // Shared token type (true flyweight)
        struct TokenType
        {
            bool capitalize, bold, italic, underline;

            TokenType(bool c = false, bool b = false, bool i = false, bool u = false)
                : capitalize(c), bold(b), italic(i), underline(u) {}

            string key() const
            {
                return to_string(capitalize) + to_string(bold) +
                       to_string(italic) + to_string(underline);
            }

            bool operator==(const TokenType &other) const
            {
                return capitalize == other.capitalize &&
                       bold == other.bold &&
                       italic == other.italic &&
                       underline == other.underline;
            }
        };

        // Factory for flyweight tokens
        class TokenFactory
        {
        private:
            static map<string, shared_ptr<TokenType>> tokens;

        public:
            static shared_ptr<TokenType> get_token(bool capitalize = false,
                                                   bool bold = false,
                                                   bool italic = false,
                                                   bool underline = false)
            {
                TokenType temp(capitalize, bold, italic, underline);
                string k = temp.key();

                if (tokens.find(k) != tokens.end())
                {
                    return tokens[k]; // Return cached token
                }

                auto token = make_shared<TokenType>(capitalize, bold, italic, underline);
                tokens[k] = token;
                return token;
            }

            static size_t token_count()
            {
                return tokens.size();
            }

            static void clear()
            {
                tokens.clear();
            }
        };

        map<string, shared_ptr<TokenType>> TokenFactory::tokens;

        struct Sentence
        {
            vector<string> words;
            vector<shared_ptr<TokenType>> tokens;

            Sentence(const string &text)
            {
                stringstream ss(text);
                string word;

                while (ss >> word)
                {
                    words.push_back(word);
                    tokens.push_back(TokenFactory::get_token()); // Default token
                }
            }

            void capitalize(size_t index)
            {
                if (index < words.size())
                {
                    auto old_token = tokens[index];
                    tokens[index] = TokenFactory::get_token(true, old_token->bold,
                                                            old_token->italic,
                                                            old_token->underline);
                }
            }

            void set_bold(size_t index)
            {
                if (index < words.size())
                {
                    auto old_token = tokens[index];
                    tokens[index] = TokenFactory::get_token(old_token->capitalize, true,
                                                            old_token->italic,
                                                            old_token->underline);
                }
            }

            void set_italic(size_t index)
            {
                if (index < words.size())
                {
                    auto old_token = tokens[index];
                    tokens[index] = TokenFactory::get_token(old_token->capitalize,
                                                            old_token->bold, true,
                                                            old_token->underline);
                }
            }

            size_t word_count() const
            {
                return words.size();
            }

            string str() const
            {
                string result;

                for (size_t i = 0; i < words.size(); ++i)
                {
                    string word = words[i];

                    if (tokens[i]->capitalize)
                    {
                        for (char &c : word)
                            c = toupper(c);
                    }

                    result += word;

                    if (i + 1 < words.size())
                        result += " ";
                }

                return result;
            }

            shared_ptr<const TokenType> get_token(size_t index) const
            {
                if (index < tokens.size())
                    return tokens[index];
                return nullptr;
            }
        };

    } // namespace shared_flyweight

    // ============================================================================
    // SOLUTION 4: Range-Based Formatting
    // ============================================================================

    namespace range_based
    {

        struct Sentence
        {
            struct WordToken
            {
                bool capitalize = false;
                bool bold = false;
                bool italic = false;
            };

            struct WordRange
            {
                size_t start, end;
                bool capitalize, bold, italic;

                WordRange(size_t s, size_t e)
                    : start(s), end(e), capitalize(false), bold(false), italic(false) {}

                bool covers(size_t position) const
                {
                    return position >= start && position <= end;
                }
            };

            vector<string> words;
            vector<WordRange> ranges;

            Sentence(const string &text)
            {
                stringstream ss(text);
                string word;

                while (ss >> word)
                {
                    words.push_back(word);
                }
            }

            WordRange &format_range(size_t start, size_t end)
            {
                ranges.emplace_back(start, end);
                return ranges.back();
            }

            string str() const
            {
                string result;

                for (size_t i = 0; i < words.size(); ++i)
                {
                    string word = words[i];

                    // Apply all applicable ranges
                    for (const auto &range : ranges)
                    {
                        if (range.covers(i))
                        {
                            if (range.capitalize)
                            {
                                for (char &c : word)
                                    c = toupper(c);
                            }
                        }
                    }

                    result += word;

                    if (i + 1 < words.size())
                        result += " ";
                }

                return result;
            }

            size_t get_word_count() const
            {
                return words.size();
            }
        };

    } // namespace range_based

} // namespace flyweight_sentence

// ============================================================================
// TESTS
// ============================================================================

#include <cassert>

int test_count = 0;
int test_passed = 0;

#define TEST(name)                      \
    cout << "\nTest: " << name << "\n"; \
    test_count++;

#define ASSERT_EQ(a, b)                                       \
    if ((a) == (b))                                           \
    {                                                         \
        cout << "  ✓ PASS\n";                                 \
        test_passed++;                                        \
    }                                                         \
    else                                                      \
    {                                                         \
        cout << "  ✗ FAIL: " << (a) << " != " << (b) << "\n"; \
    }

#define ASSERT_TRUE(condition) \
    if (condition)             \
    {                          \
        cout << "  ✓ PASS\n";  \
        test_passed++;         \
    }                          \
    else                       \
    {                          \
        cout << "  ✗ FAIL\n";  \
    }

int main()
{
    cout << string(70, '=') << "\n";
    cout << "FLYWEIGHT SENTENCE - CODING EXERCISE\n";
    cout << string(70, '=') << "\n";

    // ========================================================================
    // BASIC SOLUTION TESTS
    // ========================================================================

    {
        using namespace flyweight_sentence::basic;

        TEST("Basic: Parse sentence");
        Sentence s("hello world");
        ASSERT_EQ(s.words.size(), 2);

        TEST("Basic: Capitalize single word");
        Sentence s2("hello world");
        s2[0].capitalize = true;
        ASSERT_EQ(s2.str(), "HELLO world");

        TEST("Basic: Capitalize multiple words");
        Sentence s3("the quick brown fox");
        s3[1].capitalize = true;
        s3[3].capitalize = true;
        ASSERT_EQ(s3.str(), "the QUICK brown FOX");

        TEST("Basic: No capitalization");
        Sentence s4("lowercase words");
        ASSERT_EQ(s4.str(), "lowercase words");

        TEST("Basic: Word token exists");
        Sentence s5("one two three");
        bool has_tokens = (s5.tokens.size() == 3);
        ASSERT_TRUE(has_tokens);
    }

    // ========================================================================
    // ENHANCED SOLUTION TESTS
    // ========================================================================

    {
        using namespace flyweight_sentence::enhanced;

        TEST("Enhanced: Multiple attributes");
        Sentence s("hello world");
        s[0].capitalize = true;
        s[1].bold = true;
        ASSERT_EQ(s.str(), "HELLO world");

        TEST("Enhanced: Bold and italic");
        Sentence s2("text formatting");
        s2[0].bold = true;
        s2[1].italic = true;
        string html_output = s2.html();
        bool has_bold = (html_output.find("<b>") != string::npos);
        ASSERT_TRUE(has_bold);

        TEST("Enhanced: Underline support");
        Sentence s3("underlined text");
        s3[0].underline = true;
        bool has_underline_attr = s3.tokens[0].underline;
        ASSERT_TRUE(has_underline_attr);
    }

    // ========================================================================
    // SHARED FLYWEIGHT TESTS
    // ========================================================================

    {
        using namespace flyweight_sentence::shared_flyweight;

        TEST("Shared Flyweight: Token factory");
        TokenFactory::clear();
        auto token1 = TokenFactory::get_token(true, false, false, false);
        auto token2 = TokenFactory::get_token(true, false, false, false);
        ASSERT_TRUE(token1.get() == token2.get()); // Same pointer

        TEST("Shared Flyweight: Different tokens");
        TokenFactory::clear();
        auto t1 = TokenFactory::get_token(true, false, false, false);
        auto t2 = TokenFactory::get_token(false, true, false, false);
        ASSERT_TRUE(t1.get() != t2.get()); // Different pointers

        TEST("Shared Flyweight: Sentence creation");
        TokenFactory::clear();
        Sentence s("hello world");
        ASSERT_EQ(s.word_count(), 2);

        TEST("Shared Flyweight: Capitalize word");
        TokenFactory::clear();
        Sentence s2("hello world");
        s2.capitalize(0);
        ASSERT_EQ(s2.str(), "HELLO world");

        TEST("Shared Flyweight: Factory reuses tokens");
        TokenFactory::clear();
        Sentence s3("one two three");
        s3.capitalize(0);
        s3.capitalize(2);
        int token_count = TokenFactory::token_count();
        ASSERT_TRUE(token_count <= 3); // Tokens reused
    }

    // ========================================================================
    // RANGE-BASED FORMATTING TESTS
    // ========================================================================

    {
        using namespace flyweight_sentence::range_based;

        TEST("Range: Format range of words");
        Sentence s("the quick brown fox");
        s.format_range(1, 2).capitalize = true;
        ASSERT_EQ(s.str(), "the QUICK BROWN fox");

        TEST("Range: Single word range");
        Sentence s2("hello world");
        s2.format_range(0, 0).capitalize = true;
        ASSERT_EQ(s2.str(), "HELLO world");

        TEST("Range: Multiple overlapping ranges");
        Sentence s3("a b c d e");
        s3.format_range(0, 2).capitalize = true;
        s3.format_range(2, 4).capitalize = true;
        ASSERT_EQ(s3.str(), "A B C D E");

        TEST("Range: Word count");
        Sentence s4("one two three four");
        ASSERT_EQ(s4.get_word_count(), 4);
    }

    // ========================================================================
    // SUMMARY
    // ========================================================================

    cout << "\n"
         << string(70, '=') << "\n";
    cout << "TEST SUMMARY\n";
    cout << string(70, '=') << "\n\n";

    cout << "Passed: " << test_passed << " / " << test_count << "\n";
    cout << "Success Rate: " << (test_passed * 100 / test_count) << "%\n";

    if (test_passed == test_count)
    {
        cout << "\n✓ ALL TESTS PASSED\n";
    }
    else
    {
        cout << "\n✗ SOME TESTS FAILED\n";
    }

    cout << "\nKEY CONCEPTS DEMONSTRATED:\n";
    cout << "1. Basic Flyweight: Words + lightweight token metadata\n";
    cout << "2. Enhanced: Multiple formatting attributes per word\n";
    cout << "3. Shared Flyweight: Token factory reuses identical tokens\n";
    cout << "4. Range-Based: Format ranges of words efficiently\n";
    cout << "5. Memory Efficiency: Tokens store only what's needed\n";

    return test_passed == test_count ? 0 : 1;
}
