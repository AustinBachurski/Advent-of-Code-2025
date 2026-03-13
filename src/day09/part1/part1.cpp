#include "day09/part1/part1.hpp"
#include "common/common.hpp"

#include <algorithm>
#include <cassert>
#include <compare>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>


namespace
{

struct Point
{
    int64_t x{};
    int64_t y{};

    auto operator<=>(Point const &other) const = default;
};

std::vector<Point> extractPointsFromInput(auto coordinates)
{
    std::vector<Point> points;

    for (auto coordinate : coordinates)
    {
        auto pair{ common::splitStringOn(coordinate, ',') };
        assert(std::distance(pair.begin(), pair.end()) == 2);
        auto x{ common::stringviewToNumber<int64_t>(*pair.begin()) };
        auto y{ common::stringviewToNumber<int64_t>(*++pair.begin()) };
        points.emplace_back(x, y);
    }

    return points;
}

int64_t calculateMaxArea(std::span<Point const> points)
{
    int64_t max{};

    for (auto const p : points)
    {
        for (auto const otherP : points)
        {
            if (p == otherP)
            { continue; }

            // Add 1 for inclusive area.
            int64_t xResult{ std::abs(p.x - otherP.x) + 1 };
            int64_t yResult{ std::abs(p.y - otherP.y) + 1 };

            max = std::max(max, xResult * yResult);
        }
    }

    return max;
}

} // anonymous namespace

namespace day09::part1
{

std::string solve()
{
    auto file{ common::readEntireInputFile("day09.txt") };
    auto coordinates{ common::splitStringOn(file, '\n') };
    auto points{ extractPointsFromInput(coordinates) };

    return std::to_string(calculateMaxArea(points));
}

} // namespace day09::part1

