#include "day09/part2/intersection.hpp"
#include "day09/part2/polygon.hpp"

#include <ranges>


namespace
{

bool edgesIntersect(polygon::LineSegment const &inner, polygon::LineSegment const &outer)
{
    auto innerStartToOuterLine { polygon::crossProduct(outer, inner.start) };
    auto innerEndToOuterLine   { polygon::crossProduct(outer, inner.end)   };
    auto outerStartToInnerLine { polygon::crossProduct(inner, outer.start) };
    auto outerEndToInnerLine   { polygon::crossProduct(inner, outer.end)   };

    return
    (
        (innerStartToOuterLine > 0 && innerEndToOuterLine < 0)
        || (innerStartToOuterLine < 0 && innerEndToOuterLine > 0)
    )
    && 
    (
        (outerStartToInnerLine > 0 && outerEndToInnerLine < 0)
        || (outerStartToInnerLine < 0 && outerEndToInnerLine > 0)
    );
}

} // anonymous namespace

namespace intersection
{

bool edgesAreContained(std::span<polygon::Point const, 4> inner,
                    std::span<polygon::Point const> outer)
{
    auto innerStartToEndInclusive{
        std::views::concat(inner, inner | std::views::take(1)) };
    auto outerStartToEndInclusive{
        std::views::concat(outer, outer | std::views::take(1)) };
    polygon::LineSegment innerEdge;
    polygon::LineSegment outerEdge;

    for (auto innerPair : innerStartToEndInclusive | std::views::slide(2))
    {
        innerEdge = { .start = innerPair.front(), .end = innerPair.back() };

        for (auto outerPair : outerStartToEndInclusive | std::views::slide(2))
        {
            outerEdge = { .start = outerPair.front(), .end = outerPair.back() };

            if (edgesIntersect(innerEdge, outerEdge))
            {
                return false;
            }
        }
    }

    return true;
}

}
