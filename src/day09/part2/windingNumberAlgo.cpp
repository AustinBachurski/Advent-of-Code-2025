#include "day09/part2/windingNumberAlgo.hpp"
#include "day09/part2/polygon.hpp"

#include <algorithm>
#include <ranges>

namespace
{

int64_t crossProduct(polygon::LineSegment const &line,
                     polygon::Point const &point)
{
    return (line.end.x - line.start.x) * (point.y - line.start.y)
           - (line.end.y - line.start.y) * (point.x - line.start.x);
}

bool pointLiesOnLine(polygon::Point const & point,
                     polygon::LineSegment const &line)
{
    return crossProduct(line, point) == 0
    && point.x >= std::min(line.start.x, line.end.x)
    && point.x <= std::max(line.start.x, line.end.x)
    && point.y >= std::min(line.start.y, line.end.y)
    && point.y <= std::max(line.start.y, line.end.y);
}

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

        if (pointLiesOnLine(point, line))
        { return 0; }

        if (line.start.y <= point.y)
        {
            if (line.end.y > point.y && crossProduct(line, point) > 0)
            {
                ++winding;
            }
        }
        else
        {
            if (line.end.y <= point.y && crossProduct(line, point) < 0)
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

bool pointsAreContained( std::span<polygon::Point const, 4> points,
                        std::span<polygon::Point const> outer)
{
    for (auto const &point : points)
    {
        if (windingNumber(point, outer))
        {
            return false;
        }
    }

    return true;
}


} // namespace winding

