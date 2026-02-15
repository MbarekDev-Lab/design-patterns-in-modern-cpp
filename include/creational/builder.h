#ifndef BUILDER_H
#define BUILDER_H

#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <iostream>

/**
 * Builder Pattern (Creational Pattern)
 *
 * Intent: Separate the construction of a complex object from its representation
 * so that the same construction process can create different representations.
 *
 * Use when:
 * - An object has many optional parameters
 * - You want to construct complex objects step by step
 * - You want to avoid "telescoping constructors" (many overloads)
 * - You want to create immutable objects
 *
 * This example demonstrates building HTML documents step by step
 */

namespace builder_pattern
{

    // ============ Complex Object: HtmlElement ============

    class HtmlElement
    {
    private:
        std::string name;
        std::string text;
        std::vector<HtmlElement> elements;
        static constexpr size_t indent_size = 2;

    public:
        HtmlElement() = default;

        HtmlElement(const std::string &name, const std::string &text = "")
            : name(name), text(text) {}

        /**
         * Render HTML with proper indentation
         */
        std::string str(int indent = 0) const
        {
            std::ostringstream oss;
            std::string i(indent_size * indent, ' ');

            oss << i << "<" << name << ">\n";

            if (!text.empty())
            {
                oss << std::string(indent_size * (indent + 1), ' ') << text << "\n";
            }

            for (const auto &element : elements)
            {
                oss << element.str(indent + 1);
            }

            oss << i << "</" << name << ">\n";
            return oss.str();
        }

        friend class HtmlBuilder;
    };

    // ============ Builder: HtmlBuilder ============

    /**
     * ✅ SOLUTION: Builder pattern for constructing HtmlElement
     * Benefits:
     * 1. Clear, readable fluent interface
     * 2. No "telescoping constructors"
     * 3. Step-by-step construction
     * 4. Method chaining for elegant syntax
     */
    class HtmlBuilder
    {
    private:
        HtmlElement root;

    public:
        explicit HtmlBuilder(const std::string &root_name)
        {
            root.name = root_name;
        }

        /**
         * Add a child element (fluent interface - returns reference to this)
         */
        HtmlBuilder &add_child(const std::string &child_name,
                               const std::string &child_text = "")
        {
            HtmlElement child(child_name, child_text);
            root.elements.emplace_back(child);
            return *this;
        }

        /**
         * Alternative: pointer-based method chaining
         */
        HtmlBuilder *add_child_ptr(const std::string &child_name,
                                   const std::string &child_text = "")
        {
            HtmlElement child(child_name, child_text);
            root.elements.emplace_back(child);
            return this;
        }

        /**
         * Get the built element
         */
        HtmlElement build() const
        {
            return root;
        }

        /**
         * Implicit conversion to HtmlElement
         */
        operator HtmlElement() const
        {
            return root;
        }

        /**
         * Direct string output
         */
        std::string str() const
        {
            return root.str();
        }
    };

    /**
     * Nested Builder Pattern (alternative approach)
     * Build complex nested structures more naturally
     */
    class Document
    {
    private:
        std::string title;
        std::vector<std::string> paragraphs;
        std::vector<std::string> list_items;

    public:
        Document() = default;

        std::string get_html() const
        {
            std::ostringstream oss;
            oss << "<!DOCTYPE html>\n<html>\n<head>\n";
            oss << "  <title>" << title << "</title>\n";
            oss << "</head>\n<body>\n";

            for (const auto &para : paragraphs)
            {
                oss << "  <p>" << para << "</p>\n";
            }

            if (!list_items.empty())
            {
                oss << "  <ul>\n";
                for (const auto &item : list_items)
                {
                    oss << "    <li>" << item << "</li>\n";
                }
                oss << "  </ul>\n";
            }

            oss << "</body>\n</html>\n";
            return oss.str();
        }

        friend class DocumentBuilder;
    };

    class DocumentBuilder
    {
    private:
        Document doc;

    public:
        DocumentBuilder &add_title(const std::string &title)
        {
            doc.title = title;
            return *this;
        }

        DocumentBuilder &add_paragraph(const std::string &paragraph)
        {
            doc.paragraphs.emplace_back(paragraph);
            return *this;
        }

        DocumentBuilder &add_list_item(const std::string &item)
        {
            doc.list_items.emplace_back(item);
            return *this;
        }

        Document build() const
        {
            return doc;
        }

        operator Document() const
        {
            return doc;
        }
    };

    // ============ VIOLATION: Without Builder Pattern ============

    /**
     * ❌ VIOLATION: Manual HTML construction
     * Problems:
     * 1. Verbose and error-prone
     * 2. Hard to read
     * 3. Easy to forget closing tags
     * 4. No structure or validation
     */
    inline std::string build_html_manual()
    {
        std::ostringstream oss;
        oss << "<ul>\n";
        oss << "  <li>Hello</li>\n";
        oss << "  <li>World</li>\n";
        oss << "</ul>\n";
        return oss.str();
    }

    inline std::string build_html_with_string_concat()
    {
        std::string result;
        result += "<ul>\n";
        result += "  <li>Hello</li>\n";
        result += "  <li>World</li>\n";
        result += "</ul>\n";
        return result;
    }

} // namespace builder_pattern

#endif // BUILDER_H
