#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <array>
#include <compare>
#include <cstdint>


namespace polygon
{

struct Point
{
    int64_t x{};
    int64_t y{};

    auto operator<=>(Point const &other) const = default;
};

struct LineSegment
{
    Point start;
    Point end;
};

struct Rectangle
{
    Rectangle(Point const &a, Point const &b, Point const &c, Point const &d)
    : points{ a, b, c, d }
    {}

    std::array<Point, 4> points;
};

} //namespace polygon

#endif

