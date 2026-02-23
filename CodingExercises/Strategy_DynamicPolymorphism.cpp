/*
 * Strategy Pattern - Dynamic Polymorphism (Pointer/Virtual-Based)
 *
 * The Strategy pattern defines a family of algorithms, encapsulates each one,
 * and makes them interchangeable. The pattern lets the algorithm vary
 * independently from clients that use it.
 *
 * This implementation uses DYNAMIC polymorphism (virtual functions/runtime):
 * - Strategy selection at runtime
 * - Can be changed dynamically
 * - Best for: flexible, runtime-configurable algorithms
 */

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <cassert>
using namespace std;

enum class OutputFormat
{
    Markdown,
    Html
};

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
// TEXT PROCESSOR WITH DYNAMIC STRATEGY (Virtual pointer-based)
// ============================================================================

struct TextProcessor
{
    void clear()
    {
        oss.str("");
        oss.clear();
    }

    void append_list(const vector<string> &items)
    {
        if (!list_strategy)
            throw runtime_error("Strategy not set");

        list_strategy->start(oss);
        for (const auto &item : items)
            list_strategy->add_list_item(oss, item);
        list_strategy->end(oss);
    }

    void set_output_format(const OutputFormat format)
    {
        switch (format)
        {
        case OutputFormat::Markdown:
            list_strategy = make_unique<MarkdownListStrategy>();
            break;
        case OutputFormat::Html:
            list_strategy = make_unique<HtmlListStrategy>();
            break;
        default:
            throw runtime_error("Unsupported format");
        }
    }

    void set_strategy(unique_ptr<ListStrategy> strategy)
    {
        list_strategy = move(strategy);
    }

    string str() const
    {
        return oss.str();
    }

private:
    ostringstream oss;
    unique_ptr<ListStrategy> list_strategy;
};

// ============================================================================
// TEST SUITE
// ============================================================================

void test_markdown_strategy()
{
    TextProcessor tp;
    tp.set_output_format(OutputFormat::Markdown);
    tp.append_list({"foo", "bar", "baz"});

    string result = tp.str();
    assert(result.find(" * foo") != string::npos);
    assert(result.find(" * bar") != string::npos);
    assert(result.find(" * baz") != string::npos);
    cout << "✓ Markdown strategy" << endl;
}

void test_html_strategy()
{
    TextProcessor tp;
    tp.set_output_format(OutputFormat::Html);
    tp.append_list({"foo", "bar", "baz"});

    string result = tp.str();
    assert(result.find("<ul>") != string::npos);
    assert(result.find("<li>foo</li>") != string::npos);
    assert(result.find("<li>bar</li>") != string::npos);
    assert(result.find("<li>baz</li>") != string::npos);
    assert(result.find("</ul>") != string::npos);
    cout << "✓ HTML strategy" << endl;
}

void test_switch_strategy_at_runtime()
{
    TextProcessor tp;

    // Start with Markdown
    tp.set_output_format(OutputFormat::Markdown);
    tp.append_list({"item1", "item2"});
    string markdown_result = tp.str();

    assert(markdown_result.find(" * item1") != string::npos);

    // Switch to HTML
    tp.clear();
    tp.set_output_format(OutputFormat::Html);
    tp.append_list({"item1", "item2"});
    string html_result = tp.str();

    assert(html_result.find("<li>item1</li>") != string::npos);
    assert(html_result.find("<ul>") != string::npos);
    cout << "✓ Switch strategy at runtime" << endl;
}

void test_clear_between_strategies()
{
    TextProcessor tp;

    tp.set_output_format(OutputFormat::Markdown);
    tp.append_list({"a", "b"});
    assert(!tp.str().empty());

    tp.clear();
    assert(tp.str().empty());

    tp.set_output_format(OutputFormat::Html);
    tp.append_list({"c", "d"});
    assert(!tp.str().empty());
    cout << "✓ Clear between strategies" << endl;
}

void test_custom_strategy()
{
    struct JsonListStrategy : ListStrategy
    {
        void start(ostringstream &oss) override
        {
            oss << "[\n";
        }
        void end(ostringstream &oss) override
        {
            oss << "]\n";
        }
        void add_list_item(ostringstream &oss, const string &item) override
        {
            oss << "  \"" << item << "\",\n";
        }
    };

    TextProcessor tp;
    tp.set_strategy(make_unique<JsonListStrategy>());
    tp.append_list({"foo", "bar"});

    string result = tp.str();
    assert(result.find("[") != string::npos);
    assert(result.find("\"foo\"") != string::npos);
    assert(result.find("\"bar\"") != string::npos);
    assert(result.find("]") != string::npos);
    cout << "✓ Custom JSON strategy" << endl;
}

void test_empty_list_markdown()
{
    TextProcessor tp;
    tp.set_output_format(OutputFormat::Markdown);
    tp.append_list({});
    assert(tp.str().empty());
    cout << "✓ Empty list with Markdown" << endl;
}

void test_empty_list_html()
{
    TextProcessor tp;
    tp.set_output_format(OutputFormat::Html);
    tp.append_list({});
    string result = tp.str();
    assert(result == "<ul>\n</ul>\n");
    cout << "✓ Empty list with HTML" << endl;
}

void test_multiple_appends_same_format()
{
    TextProcessor tp;
    tp.set_output_format(OutputFormat::Markdown);

    tp.append_list({"item1"});
    string result1 = tp.str();

    tp.clear();
    tp.append_list({"item2", "item3"});
    string result2 = tp.str();

    assert(result1.find("item1") != string::npos);
    assert(result2.find("item2") != string::npos);
    assert(result2.find("item3") != string::npos);
    cout << "✓ Multiple appends same format" << endl;
}

void test_strategy_not_set_throws()
{
    TextProcessor tp;
    // Don't set strategy
    try
    {
        tp.append_list({"foo"});
        assert(false); // Should have thrown
    }
    catch (const runtime_error &e)
    {
        assert(string(e.what()).find("Strategy") != string::npos);
        cout << "✓ Exception when strategy not set" << endl;
    }
}

void test_markdown_output()
{
    cout << "\n--- Markdown Output ---" << endl;
    TextProcessor tp;
    tp.set_output_format(OutputFormat::Markdown);
    tp.append_list({"foo", "bar", "baz"});
    cout << tp.str();
}

void test_html_output()
{
    cout << "\n--- HTML Output ---" << endl;
    TextProcessor tp;
    tp.set_output_format(OutputFormat::Html);
    tp.append_list({"foo", "bar", "baz"});
    cout << tp.str();
}

int main()
{
    cout << "Strategy Pattern - Dynamic Polymorphism (Virtual-Based)\n"
         << endl;

    try
    {
        cout << "Running tests...\n"
             << endl;

        test_markdown_strategy();
        test_html_strategy();
        test_switch_strategy_at_runtime();
        test_clear_between_strategies();
        test_custom_strategy();
        test_empty_list_markdown();
        test_empty_list_html();
        test_multiple_appends_same_format();
        test_strategy_not_set_throws();

        test_markdown_output();
        test_html_output();

        cout << "\n✅ All 10 tests passed!" << endl;
        cout << "\nKey Characteristics of Dynamic Polymorphism:" << endl;
        cout << "  • Strategy selection at RUNTIME (virtual function calls)" << endl;
        cout << "  • Can be changed dynamically during execution" << endl;
        cout << "  • Single TextProcessor type for all strategies" << endl;
        cout << "  • Best for: flexible, configurable algorithms" << endl;
        cout << "  • Small runtime overhead (virtual function calls)" << endl;
        cout << "  • More flexible than static polymorphism" << endl;
        cout << "  • Can be extended with custom strategies (JsonListStrategy)" << endl;

        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Test failed: " << e.what() << endl;
        return 1;
    }
}
