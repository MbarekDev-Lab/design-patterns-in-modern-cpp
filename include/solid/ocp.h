#ifndef OCP_H
#define OCP_H

#include <string>
#include <vector>
#include <memory>

/**
 * Open/Closed Principle (OCP)
 *
 * "Software entities (classes, modules, functions, etc.) should be OPEN for extension,
 * but CLOSED for modification."
 *
 * This means:
 * - You should be able to add new functionality without changing existing code
 * - Minimize changes to existing classes when requirements change
 *
 * This example shows:
 * 1. VIOLATION: ProductFilter requires modification for each new filter type
 * 2. SOLUTION: Specification pattern - extend with new specifications, don't modify existing code
 */

namespace ocp
{

    enum class Color
    {
        red,
        green,
        blue
    };
    enum class Size
    {
        small,
        medium,
        large
    };

    struct Product
    {
        std::string name;
        Color color;
        Size size;

        Product(const std::string &name, Color color, Size size)
            : name(name), color(color), size(size) {}
    };

    using Products = std::vector<Product *>;

    // ============ VIOLATION: Open for modification, Closed for extension ============
    /**
     * ❌ PROBLEM: Every time we need a new filter:
     * 1. We must MODIFY the ProductFilter class
     * 2. Add a new method
     * 3. Violates OCP - class is CLOSED for modification
     */
    class ProductFilterBad
    {
    public:
        Products by_color(const Products &items, Color color);
        Products by_size(const Products &items, Size size);
        Products by_size_and_color(const Products &items, Size size, Color color);
        // Need more filters? Modify this class! ❌
    };

    // ============ SOLUTION: Open for extension, Closed for modification ============

    /**
     * ✅ SOLUTION: Abstract specification allows extension without modification
     */
    template <typename T>
    struct Specification
    {
        virtual ~Specification() = default;
        virtual bool is_satisfied(T *item) const = 0;
    };

    template <typename T>
    struct Filter
    {
        virtual std::vector<T *> filter(const std::vector<T *> &items,
                                        const Specification<T> &spec) const = 0;
        virtual ~Filter() = default;
    };

    /**
     * ✅ BetterFilter is CLOSED for modification
     * It doesn't know about specific product properties
     * It works with ANY specification
     */
    class BetterFilter : public Filter<Product>
    {
    public:
        std::vector<Product *> filter(const std::vector<Product *> &items,
                                      const Specification<Product> &spec) const override;
    };

    // ============ Specific Specifications (Easy to extend!) ============

    /**
     * ✅ Add new filter types WITHOUT modifying existing code
     * Just create a new Specification subclass
     */
    class ColorSpecification : public Specification<Product>
    {
    private:
        Color color;

    public:
        explicit ColorSpecification(Color color) : color(color) {}
        bool is_satisfied(Product *item) const override;
    };

    class SizeSpecification : public Specification<Product>
    {
    private:
        Size size;

    public:
        explicit SizeSpecification(Size size) : size(size) {}
        bool is_satisfied(Product *item) const override;
    };

    /**
     * ✅ Composite specification for combining filters
     * This can be extended indefinitely without modifying FilterProductFilter
     */
    template <typename T>
    class AndSpecification : public Specification<T>
    {
    private:
        const Specification<T> &first;
        const Specification<T> &second;

    public:
        AndSpecification(const Specification<T> &first, const Specification<T> &second)
            : first(first), second(second) {}

        bool is_satisfied(T *item) const override
        {
            return first.is_satisfied(item) && second.is_satisfied(item);
        }
    };

    // ✅ Operator overload for elegant composition
    template <typename T>
    AndSpecification<T> operator&&(const Specification<T> &first,
                                   const Specification<T> &second)
    {
        return AndSpecification<T>(first, second);
    }

    // Similarly, we could add ||, ! operators for OR, NOT operations

} // namespace ocp

#endif // OCP_H
