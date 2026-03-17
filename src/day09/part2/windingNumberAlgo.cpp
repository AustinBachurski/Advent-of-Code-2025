#include "day09/part2/windingNumberAlgo.hpp"
#include "day09/part2/polygon.hpp"

#include <ranges>


namespace
{

int windingNumber(polygon::Point const &point,
                  std::span<polygon::Point const> outer)
{
    int winding{};
    polygon::LineSegment line;

    auto startToEndInclusive{
        std::views::concat(outer, outer | std::views::take(1)) };

    for (auto pair : startToEndInclusive | std::views::slide(2))
    {
        line = { pair.front(), pair.back() };

        if (polygon::pointLiesOnLine(point, line))
        { return 1; }

        if (line.start.y <= point.y)
        {
            if (line.end.y > point.y && polygon::crossProduct(line, point) > 0)
            {
                ++winding;
            }
        }
        else
        {
            if (line.end.y <= point.y && polygon::crossProduct(line, point) < 0)
            {
                --winding;
            }
        }
    }

    return winding;
}

} // anonymous namespace

namespace winding
{

bool pointsAreContained(std::span<polygon::Point const, 4> points,
                        std::span<polygon::Point const> outer)
{
    for (auto const &point : points)
    {
        if (!windingNumber(point, outer))
        {
            return false;
        }
    }

    return true;
}

} // namespace winding

