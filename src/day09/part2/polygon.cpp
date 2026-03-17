#include "day09/part2/polygon.hpp"

#include <algorithm>


namespace polygon
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

} // namespace polygon

