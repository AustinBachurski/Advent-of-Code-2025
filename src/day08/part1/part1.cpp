#include "day08/part1/part1.hpp"
#include "common/common.hpp"

#include <cmath>
#include <compare>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>


namespace
{

class Box
{
public:
    Box(int const x, int const y, int const z)
    : m_x{ x }
    , m_y{ y }
    , m_z{ z }
    {}

    double distanceTo(Box const &other) const
    {
        return std::sqrt(
            (m_x - other.m_x) * (m_x - other.m_x)
            + (m_y - other.m_y) * (m_y - other.m_y)
            + (m_z - other.m_z) * (m_z - other.m_z)
        );
    }

    auto operator<=>(Box const &other) const = default;

private:
    int m_x{};
    int m_y{};
    int m_z{};
};

class CloseBoxPair
{
public:
    CloseBoxPair(Box const &a, Box const &b)
    : m_a{ a }
    , m_b{ b }
    {}

    bool operator==(CloseBoxPair const &other) const noexcept
    {
        return (m_a == other.m_a && m_b == other.m_b)
        || (m_a == other.m_b && m_b == other.m_a);
    }

    bool operator<(CloseBoxPair const &other) const noexcept
    {
        return m_a < other.m_a;
    }

    bool isConnectionOf(CloseBoxPair const &other) const noexcept
    {
        return m_a == other.m_a || m_b == other.m_b;
    }

private:
    Box m_a;
    Box m_b;
};

Box makeBox(auto tripletIter)
{
    return {
        common::stringviewToNumber<int>(*tripletIter++),
        common::stringviewToNumber<int>(*tripletIter++),
        common::stringviewToNumber<int>(*tripletIter)
    };
}

std::vector<Box> extractBoxesFromInput(auto coordinates)
{
    std::vector<Box> boxes;

    for (auto coordinate : coordinates)
    {
        auto triplet{ common::splitStringOn(coordinate, ',') };
        boxes.push_back(makeBox(triplet.begin()));
    }

    return boxes;
}

std::map<double, std::set<CloseBoxPair>> generateDistancesBetween(std::span<Box const> boxes)
{
    std::map<double, std::set<CloseBoxPair>> distances;

    for (auto const &box : boxes)
    {
        for (auto const &otherBox : boxes)
        {
            if (box == otherBox)
            { continue; }

            auto d{ box.distanceTo(otherBox) };

            distances[d].insert({ box, otherBox });
        }
    }

    return distances;
}

bool containsConnectionPoint(std::set<CloseBoxPair> set, CloseBoxPair pair)
{
    for (auto const existing : set)
    {
        if (existing.isConnectionOf(pair))
        {
            return true;
        }
    }
    return false;
}

std::vector<std::set<CloseBoxPair>> connectClosestNBoxes(
    std::map<double, std::set<CloseBoxPair>> distances, int n)
{
    std::vector<std::set<CloseBoxPair>> connections;

    int linkCount{};

    // Loop over sorted distances.
    for (auto &&[distance, pairs] : distances)
    {
        // Grab all paired boxes for current distance.
        for (auto const &pair : pairs)
        {
            // Add to existing connection if exists.
            for (auto &set : connections)
            {
                if (containsConnectionPoint(set, pair))
                {
                    set.insert(pair);

                    if (++linkCount == n)
                    {
                        return connections;
                    }

                    break;
                }
            }

            // Else add a new set of connections with current pair.
            connections.emplace_back();
            connections.back().insert(pair);

            if (++linkCount == n)
            {
                return connections;
            }
        }
    }

    return connections;
}

} // anonymous namespace

namespace day08::part1
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day08.txt") };
    auto coordinates{ common::splitStringOn(file, '\n') };
    auto boxes{ extractBoxesFromInput(coordinates) };
    auto distances{ generateDistancesBetween(boxes) };
    auto connections{ connectClosestNBoxes(distances, 10) };

    std::size_t product{1};

    for (auto const &set : connections | std::views::take(3))
    {
        product *= set.size();
    }

    return std::to_string(product);
}

} // namespace day08::part1

