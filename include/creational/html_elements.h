#ifndef HTML_ELEMENTS_H
#define HTML_ELEMENTS_H

#include <string>
#include <vector>
#include <iostream>
#include <memory>

/**
 * Type-Safe HTML Elements Pattern
 *
 * Demonstrates creating specialized HTML tag types via inheritance
 * Benefits:
 * 1. Type safety - compile-time checking
 * 2. Intuitive API - IMG("url"), P("text"), etc.
 * 3. Flexible - supports attributes, children, and text
 * 4. Clean output - proper HTML formatting
 * 5. Readable - natural code that looks like HTML
 */

namespace html
{

    // ============ Base Tag Class ============

    /**
     * Base class for all HTML tags
     * Provides common functionality for all tag types
     */
    class Tag
    {
    protected:
        std::string name;
        std::string text;
        std::vector<Tag> children;
        std::vector<std::pair<std::string, std::string>> attributes;

    public:
        /**
         * Protected constructors - only subclasses can create Tag
         */
        Tag(const std::string &name, const std::string &text = "")
            : name(name), text(text) {}

        Tag(const std::string &name, const std::vector<Tag> &children)
            : name(name), children(children) {}

        virtual ~Tag() = default;

        /**
         * Add an attribute (class, id, href, etc.)
         */
        void add_attribute(const std::string &key, const std::string &value)
        {
            attributes.emplace_back(key, value);
        }

        /**
         * Add a child element
         */
        void add_child(const Tag &child)
        {
            children.push_back(child);
        }

        /**
         * Stream output operator for rendering HTML
         */
        friend std::ostream &operator<<(std::ostream &os, const Tag &tag)
        {
            os << "<" << tag.name;

            // Add attributes
            for (const auto &attr : tag.attributes)
            {
                os << " " << attr.first << "=\"" << attr.second << "\"";
            }

            // Check if element is self-closing
            if (tag.children.empty() && tag.text.empty())
            {
                os << " />\n";
            }
            else
            {
                os << ">\n";

                // Add text content
                if (!tag.text.empty())
                {
                    os << tag.text << "\n";
                }

                // Add child elements
                for (const auto &child : tag.children)
                {
                    os << child;
                }

                // Closing tag
                os << "</" << tag.name << ">\n";
            }

            return os;
        }

        // Make subclasses work with children
        std::string get_name() const { return name; }
        std::string get_text() const { return text; }
        std::vector<Tag> get_children() const { return children; }
    };

    // ============ Specialized HTML Tags ============

    /**
     * ✅ Paragraph tag
     * Can contain text or other elements
     */
    class P : public Tag
    {
    public:
        explicit P(const std::string &text = "")
            : Tag("p", text) {}

        P(const std::initializer_list<Tag> &children)
            : Tag("p", std::vector<Tag>(children)) {}
    };

    /**
     * ✅ Heading tags (H1-H6)
     */
    class H1 : public Tag
    {
    public:
        explicit H1(const std::string &text = "")
            : Tag("h1", text) {}
    };

    class H2 : public Tag
    {
    public:
        explicit H2(const std::string &text = "")
            : Tag("h2", text) {}
    };

    class H3 : public Tag
    {
    public:
        explicit H3(const std::string &text = "")
            : Tag("h3", text) {}
    };

    /**
     * ✅ Image tag (self-closing)
     * Requires src attribute
     */
    class IMG : public Tag
    {
    public:
        explicit IMG(const std::string &src)
            : Tag("img", "")
        {
            add_attribute("src", src);
        }

        IMG &alt(const std::string &alt_text)
        {
            add_attribute("alt", alt_text);
            return *this;
        }

        IMG &width(int w)
        {
            add_attribute("width", std::to_string(w));
            return *this;
        }

        IMG &height(int h)
        {
            add_attribute("height", std::to_string(h));
            return *this;
        }
    };

    /**
     * ✅ Anchor (link) tag
     */
    class A : public Tag
    {
    public:
        explicit A(const std::string &href, const std::string &text = "")
            : Tag("a", text)
        {
            add_attribute("href", href);
        }

        A(const std::string &href, const std::initializer_list<Tag> &children)
            : Tag("a", std::vector<Tag>(children))
        {
            add_attribute("href", href);
        }
    };

    /**
     * ✅ List tags
     */
    class UL : public Tag
    {
    public:
        UL() : Tag("ul") {}

        UL(const std::initializer_list<Tag> &children)
            : Tag("ul", std::vector<Tag>(children)) {}
    };

    class OL : public Tag
    {
    public:
        OL() : Tag("ol") {}

        OL(const std::initializer_list<Tag> &children)
            : Tag("ol", std::vector<Tag>(children)) {}
    };

    class LI : public Tag
    {
    public:
        explicit LI(const std::string &text = "")
            : Tag("li", text) {}

        LI(const std::initializer_list<Tag> &children)
            : Tag("li", std::vector<Tag>(children)) {}
    };

    /**
     * ✅ Div (container)
     */
    class DIV : public Tag
    {
    public:
        DIV() : Tag("div") {}

        explicit DIV(const std::string &text)
            : Tag("div", text) {}

        DIV(const std::initializer_list<Tag> &children)
            : Tag("div", std::vector<Tag>(children)) {}

        DIV &css_class(const std::string &class_name)
        {
            add_attribute("class", class_name);
            return *this;
        }

        DIV &id(const std::string &id_name)
        {
            add_attribute("id", id_name);
            return *this;
        }
    };

    /**
     * ✅ Span
     */
    class SPAN : public Tag
    {
    public:
        explicit SPAN(const std::string &text = "")
            : Tag("span", text) {}

        SPAN &css_class(const std::string &class_name)
        {
            add_attribute("class", class_name);
            return *this;
        }
    };

    /**
     * ✅ Button
     */
    class BUTTON : public Tag
    {
    public:
        explicit BUTTON(const std::string &text = "")
            : Tag("button", text)
        {
            add_attribute("type", "button");
        }

        BUTTON &button_type(const std::string &type)
        {
            // Modify existing type attribute
            for (auto &attr : attributes)
            {
                if (attr.first == "type")
                {
                    attr.second = type;
                    return *this;
                }
            }
            add_attribute("type", type);
            return *this;
        }
    };

    /**
     * ✅ Form
     */
    class FORM : public Tag
    {
    public:
        FORM() : Tag("form") {}

        FORM(const std::initializer_list<Tag> &children)
            : Tag("form", std::vector<Tag>(children)) {}

        FORM &action(const std::string &action_url)
        {
            add_attribute("action", action_url);
            return *this;
        }

        FORM &method(const std::string &m)
        {
            add_attribute("method", m);
            return *this;
        }
    };

    /**
     * ✅ Input
     */
    class INPUT : public Tag
    {
    public:
        INPUT() : Tag("input", "")
        {
            add_attribute("type", "text");
        }

        INPUT &input_type(const std::string &type)
        {
            for (auto &attr : attributes)
            {
                if (attr.first == "type")
                {
                    attr.second = type;
                    return *this;
                }
            }
            add_attribute("type", type);
            return *this;
        }

        INPUT &name(const std::string &name_attr)
        {
            add_attribute("name", name_attr);
            return *this;
        }

        INPUT &placeholder(const std::string &text)
        {
            add_attribute("placeholder", text);
            return *this;
        }
    };

} // namespace html

#endif // HTML_ELEMENTS_H
