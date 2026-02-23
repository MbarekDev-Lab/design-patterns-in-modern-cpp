/*
 * Strategy Pattern - Static Polymorphism (Template-Based)
 *
 * The Strategy pattern defines a family of algorithms, encapsulates each one,
 * and makes them interchangeable. The pattern lets the algorithm vary
 * independently from clients that use it.
 *
 * This implementation uses STATIC polymorphism (templates/compile-time):
 * - Strategy selection at compile time
 * - Zero runtime overhead (inlining)
 * - Best for: performance-critical code
 */

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <cassert>
using namespace std;

// ============================================================================
// STRATEGY INTERFACE & IMPLEMENTATIONS
// ============================================================================

struct ListStrategy
{
    virtual ~ListStrategy() = default;
    virtual void add_list_item(ostringstream &oss, const string &item) = 0;
    virtual void start(ostringstream &oss) = 0;
    virtual void end(ostringstream &oss) = 0;
};

struct MarkdownListStrategy : ListStrategy
{
    void start(ostringstream &oss) override
    {
        // Markdown doesn't need list wrapper
    }

    void end(ostringstream &oss) override
    {
        // Markdown doesn't need list wrapper
    }

    void add_list_item(ostringstream &oss, const string &item) override
    {
        oss << " * " << item << "\n";
    }
};

struct HtmlListStrategy : ListStrategy
{
    void start(ostringstream &oss) override
    {
        oss << "<ul>\n";
    }

    void end(ostringstream &oss) override
    {
        oss << "</ul>\n";
    }

    void add_list_item(ostringstream &oss, const string &item) override
    {
        oss << "  <li>" << item << "</li>\n";
    }
};

// ============================================================================
// TEXT PROCESSOR WITH STATIC STRATEGY (Template-based)
// ============================================================================

template <typename LS>
struct TextProcessor
{
    void clear()
    {
        oss.str("");
        oss.clear();
    }

    void append_list(const vector<string> &items)
    {
        list_strategy.start(oss);
        for (const auto &item : items)
            list_strategy.add_list_item(oss, item);
        list_strategy.end(oss);
    }

    string str() const
    {
        return oss.str();
    }

private:
    ostringstream oss;
    LS list_strategy;
};

// ============================================================================
// TEST SUITE
// ============================================================================

void test_markdown_empty_list()
{
    TextProcessor<MarkdownListStrategy> tp;
    tp.append_list({});
    string result = tp.str();
    assert(result.empty());
    cout << "✓ Markdown empty list" << endl;
}

void test_markdown_single_item()
{
    TextProcessor<MarkdownListStrategy> tp;
    tp.append_list({"Apple"});
    string result = tp.str();
    assert(result == " * Apple\n");
    cout << "✓ Markdown single item" << endl;
}

void test_markdown_multiple_items()
{
    TextProcessor<MarkdownListStrategy> tp;
    tp.append_list({"foo", "bar", "baz"});
    string result = tp.str();

    assert(result.find(" * foo") != string::npos);
    assert(result.find(" * bar") != string::npos);
    assert(result.find(" * baz") != string::npos);
    cout << "✓ Markdown multiple items" << endl;
}

void test_html_empty_list()
{
    TextProcessor<HtmlListStrategy> tp;
    tp.append_list({});
    string result = tp.str();
    assert(result == "<ul>\n</ul>\n");
    cout << "✓ HTML empty list" << endl;
}

void test_html_single_item()
{
    TextProcessor<HtmlListStrategy> tp;
    tp.append_list({"Apple"});
    string result = tp.str();

    assert(result.find("<ul>") != string::npos);
    assert(result.find("<li>Apple</li>") != string::npos);
    assert(result.find("</ul>") != string::npos);
    cout << "✓ HTML single item" << endl;
}

void test_html_multiple_items()
{
    TextProcessor<HtmlListStrategy> tp;
    tp.append_list({"foo", "bar", "baz"});
    string result = tp.str();

    assert(result.find("<ul>") != string::npos);
    assert(result.find("<li>foo</li>") != string::npos);
    assert(result.find("<li>bar</li>") != string::npos);
    assert(result.find("<li>baz</li>") != string::npos);
    assert(result.find("</ul>") != string::npos);
    cout << "✓ HTML multiple items" << endl;
}

void test_markdown_clear()
{
    TextProcessor<MarkdownListStrategy> tp;
    tp.append_list({"item1", "item2"});
    assert(!tp.str().empty());

    tp.clear();
    assert(tp.str().empty());
    cout << "✓ Markdown clear" << endl;
}

void test_html_clear()
{
    TextProcessor<HtmlListStrategy> tp;
    tp.append_list({"item1"});
    assert(!tp.str().empty());

    tp.clear();
    assert(tp.str().empty());
    cout << "✓ HTML clear" << endl;
}

void test_markdown_special_characters()
{
    TextProcessor<MarkdownListStrategy> tp;
    tp.append_list({"Item with *asterisks*", "Item with [brackets]", "Item with _underscores_"});
    string result = tp.str();

    assert(result.find(" * Item with *asterisks*") != string::npos);
    assert(result.find(" * Item with [brackets]") != string::npos);
    assert(result.find(" * Item with _underscores_") != string::npos);
    cout << "✓ Markdown special characters" << endl;
}

void test_html_special_characters()
{
    TextProcessor<HtmlListStrategy> tp;
    tp.append_list({"Item <with> tags"});
    string result = tp.str();

    // Note: In real HTML we might want to escape, but this tests literal strings
    assert(result.find("<li>Item <with> tags</li>") != string::npos);
    cout << "✓ HTML special characters" << endl;
}

void test_markdown_output_format()
{
    cout << "\n--- Markdown Output ---" << endl;
    TextProcessor<MarkdownListStrategy> tp;
    tp.append_list({"foo", "bar", "baz"});
    cout << tp.str();
}

void test_html_output_format()
{
    cout << "\n--- HTML Output ---" << endl;
    TextProcessor<HtmlListStrategy> tp;
    tp.append_list({"foo", "bar", "baz"});
    cout << tp.str();
}

int main()
{
    cout << "Strategy Pattern - Static Polymorphism (Template-Based)\n"
         << endl;

    try
    {
        cout << "Running tests...\n"
             << endl;

        test_markdown_empty_list();
        test_markdown_single_item();
        test_markdown_multiple_items();

        test_html_empty_list();
        test_html_single_item();
        test_html_multiple_items();

        test_markdown_clear();
        test_html_clear();

        test_markdown_special_characters();
        test_html_special_characters();

        test_markdown_output_format();
        test_html_output_format();

        cout << "\n✅ All 11 tests passed!" << endl;
        cout << "\nKey Characteristics of Static Polymorphism:" << endl;
        cout << "  • Strategy selection at COMPILE TIME (template parameter)" << endl;
        cout << "  • Zero runtime overhead - inlining optimization" << endl;
        cout << "  • Different types for each strategy configuration" << endl;
        cout << "  • Best for: performance-critical, compile-time known strategies" << endl;
        cout << "  • Downside: cannot change strategy at runtime" << endl;
        cout << "  • Type safety: compiler catches wrong strategy usage" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
}
