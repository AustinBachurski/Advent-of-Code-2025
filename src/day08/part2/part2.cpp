#include "day08/part2/part2.hpp"
#include "common/common.hpp"

#include <algorithm>
#include <cassert>
#include <compare>
#include <cstdint>
#include <iterator>
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
    Box() = default;
    Box(int64_t const x, int64_t const y, int64_t const z)
        : m_x{ x }
        , m_y{ y }
        , m_z{ z }
    {}

    int64_t distanceTo(Box const &other) const
    {
        // Taking the square root doesn't really matter here as we're not
        // concerned with the actual distance, just which is smallest.
        return (m_x - other.m_x) * (m_x - other.m_x)
        + (m_y - other.m_y) * (m_y - other.m_y)
        + (m_z - other.m_z) * (m_z - other.m_z) ;
    }

    auto operator<=>(Box const &other) const = default;

    int64_t xMul(Box const &other) const
    {
        return m_x * other.m_x;
    }

private:
    int64_t m_x{};
    int64_t m_y{};
    int64_t m_z{};
};

class BoxPair
{
public:
    BoxPair() = default;
    BoxPair(Box const &a, Box const &b)
        : m_a{ a }
        , m_b{ b }
    {}

    // Ordering doesn't actually matter, just wanting to make sure that a pair
    // of boxes is in a set only once regardless of the ordering of the boxes
    // in the pair.
    bool operator<(BoxPair const &other) const noexcept
    {
        return !((m_a == other.m_a && m_b == other.m_b)
        || (m_a == other.m_b && m_b == other.m_a));
    }

    bool contains(Box const &other) const noexcept
    {
        return m_a == other || m_b == other;
    }

    void insertBoxesInto(std::set<Box> &set)
    {
        set.insert(m_a);
        set.insert(m_b);
    }

    int64_t xMul()
    {
        return m_a.xMul(m_b);
    }

private:
    Box m_a;
    Box m_b;
};

Box makeBox(auto tripletIter)
{
    // Assert at call site confirms 3 elements.
    return {
        common::stringviewToNumber<int64_t>(*tripletIter++),
        common::stringviewToNumber<int64_t>(*tripletIter++),
        common::stringviewToNumber<int64_t>(*tripletIter)
    };
}

std::vector<Box> extractBoxesFromInput(auto coordinates)
{
    std::vector<Box> boxes;

    for (auto coordinate : coordinates)
    {
        auto triplet{ common::splitStringOn(coordinate, ',') };

        assert(std::distance(triplet.begin(), triplet.end()) == 3);
        boxes.push_back(makeBox(triplet.begin()));
    }

    return boxes;
}

std::map<int64_t, BoxPair> generateDistancesBetween(std::span<Box const> boxes)
{
    std::map<int64_t, BoxPair> distances;

    for (auto const &box : boxes)
    {
        for (auto const &otherBox : boxes)
        {
            if (box == otherBox)
            { continue; }

            auto d{ box.distanceTo(otherBox) };

            distances[d] = { box, otherBox };
        }
    }

    return distances;
}

long countConnectionPoints(std::set<Box> set, BoxPair pair)
{
    return std::ranges::count_if(set, [&pair](Box const &existing)
                                 { return pair.contains(existing); });
}

std::vector<std::size_t> connectionFoundIn(
    std::span<std::set<Box>> const circuits, BoxPair const &pair)
{
    std::vector<std::size_t> indices;

    for (auto&& [i, set] : std::views::enumerate(circuits))
    {
        if (std::ranges::any_of(set, [&pair](Box const &box)
                                { return pair.contains(box); }))
        {
            indices.push_back(static_cast<std::size_t>(i));
        }
    }

    return indices;
}

int64_t connectClosestBoxesUntilSize(
    std::map<int64_t, BoxPair> &distances, std::size_t targetSize)
{
    std::vector<std::set<Box>> circuits;

    for (auto &&[distance, pair] : distances)
    {
        auto indices{ connectionFoundIn(circuits, pair) };

        // New connection.
        if (indices.empty())
        {
            circuits.emplace_back();
            pair.insertBoxesInto(circuits.back());
        }
        // Add to existing connection.
        else if (indices.size() == 1uz)
        {
            auto &targetSet{ circuits.at(indices.front()) };

            // If the pair is already in the same circuit, nothing happens!
            if (countConnectionPoints(targetSet, pair) != 2)
            {
                pair.insertBoxesInto(targetSet);
            }
        }
        // Bridge existing connections.
        else
        {
            auto &destinationSet{ circuits.at(indices.front()) };
            auto &sourceSet{ circuits.at(indices.back()) };
            destinationSet.insert_range(sourceSet);
            circuits.erase(circuits.begin() + static_cast<int64_t>(indices.back()));
        }

        if (circuits.front().size() == targetSize)
        {
            return pair.xMul();
        }
    }

    return 0;
}

} // anonymous namespace

namespace day08::part2
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day08.txt") };
    auto coordinates{ common::splitStringOn(file, '\n') };
    auto boxes{ extractBoxesFromInput(coordinates) };
    auto distances{ generateDistancesBetween(boxes) };
    auto product{ connectClosestBoxesUntilSize(distances, boxes.size()) };

    return std::to_string(product);
}

} // namespace day08::part2

