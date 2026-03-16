#include "day09/part2/part2.hpp"
#include "common/common.hpp"
#include "day09/part2/polygon.hpp"
#include "day09/part2/windingNumberAlgo.hpp"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>


namespace
{

std::vector<polygon::Point> extractPointsFromInput(auto coordinates)
{
    std::vector<polygon::Point> points;

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

std::map<int64_t, std::vector<polygon::Rectangle>, std::greater<int64_t>>
findLargestAreaRectangles(std::span<polygon::Point const> points)
{
    std::map<int64_t, std::vector<polygon::Rectangle>, std::greater<int64_t>> areas;

    for (auto const p : points)
    {
        for (auto const otherP : points)
        {
            if (p == otherP)
            { continue; }

            // Add 1 for inclusive area.
            int64_t xResult{ std::abs(p.x - otherP.x) + 1 };
            int64_t yResult{ std::abs(p.y - otherP.y) + 1 };

            auto area{ xResult * yResult };

            areas[area].emplace_back(
                polygon::Point{ .x = p.x, .y = p.y },
                polygon::Point{ .x = p.x, .y = otherP.y },
                polygon::Point{ .x = otherP.x, .y = otherP.y },
                polygon::Point{ .x = otherP.x, .y = p.y }
            );
        }
    }

    return areas;
}

//bool edgesAreContained(
//    std::span<polygon::Point const, 4> points, std::span<polygon::Point const> outer)
//{
//    return true;
//}

bool isContained(polygon::Rectangle rectangle, std::span<polygon::Point const> outer)
{
    return winding::pointsAreContained(rectangle.points, outer);
    //&& edgesAreContained(rectangle.points, outer);
}

int64_t findLargestContainedRectangle(
    std::map<int64_t, std::vector<polygon::Rectangle>, std::greater<int64_t>> largestAreas,
    std::span<polygon::Point const> outer)
{
    for (auto&& [area, rectangles] : largestAreas)
    {
        for (auto const &rectangle : rectangles)
        {
            if (isContained(rectangle, outer))
            {
                return area;
            }
        }
    }

    throw std::runtime_error("Didn't find any enclosed rectangles.");
}

} // anonymous namespace

namespace day09::part2
{

std::string solve()
{
    auto file{ common::readEntireInputFile("day09.txt") };
    auto coordinates{ common::splitStringOn(file, '\n') };
    auto points{ extractPointsFromInput(coordinates) };
    auto largestAreasForVectors{ findLargestAreaRectangles(points) };

    int64_t largestArea{
        findLargestContainedRectangle(largestAreasForVectors, points) };

    return std::to_string(largestArea);
}

} // namespace day09::part2

