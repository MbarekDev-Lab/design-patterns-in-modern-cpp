#include "solid/ocp.h"

namespace ocp
{

    Products ProductFilterBad::by_color(const Products &items, Color color)
    {
        Products result;
        for (auto item : items)
        {
            if (item->color == color)
            {
                result.push_back(item);
            }
        }
        return result;
    }

    Products ProductFilterBad::by_size(const Products &items, Size size)
    {
        Products result;
        for (auto item : items)
        {
            if (item->size == size)
            {
                result.push_back(item);
            }
        }
        return result;
    }

    Products ProductFilterBad::by_size_and_color(const Products &items, Size size, Color color)
    {
        Products result;
        for (auto item : items)
        {
            if (item->size == size && item->color == color)
            {
                result.push_back(item);
            }
        }
        return result;
    }

    std::vector<Product *> BetterFilter::filter(const std::vector<Product *> &items,
                                                const Specification<Product> &spec) const
    {
        std::vector<Product *> result;
        for (auto item : items)
        {
            if (spec.is_satisfied(item))
            {
                result.push_back(item);
            }
        }
        return result;
    }

    bool ColorSpecification::is_satisfied(Product *item) const
    {
        return item->color == color;
    }

    bool SizeSpecification::is_satisfied(Product *item) const
    {
        return item->size == size;
    }

} // namespace ocp
