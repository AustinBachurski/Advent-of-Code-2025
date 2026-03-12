#include "day07/part2/part2.hpp"
#include "common/common.hpp"

#include <compare>
#include <cstddef>
#include <functional>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>


namespace
{

struct Beam
{
    std::size_t index{};
    std::ptrdiff_t row{};

    auto operator<=>(Beam const &other) const = default;
};

struct BeamHasher
{
    std::size_t operator()(Beam const beam) const noexcept
    {
        auto h1{ std::hash<std::size_t>{}(beam.index) };
        auto h2{ std::hash<std::ptrdiff_t>{}(beam.row) };
        return h1 ^ (h2 << 1);
    }
};

using BeamCache = std::unordered_map<Beam, std::size_t, BeamHasher>;
std::size_t findPath(Beam const beam, auto diagram, BeamCache &cache)
{
    if (cache.contains(beam))
    {
        return cache.at(beam);
    }

    std::size_t paths{};

    for (auto [row, line] : std::views::enumerate(diagram)
                            | std::views::drop(beam.row + 1))
    {
        if (line.at(beam.index) == '^')
        {
            Beam const left { .index = beam.index - 1, .row = row };
            Beam const right{ .index = beam.index + 1, .row = row };

            paths += findPath(left, diagram, cache);
            paths += findPath(right, diagram, cache);

            cache.insert_or_assign(beam, paths);
            return paths;
        }
    }

    cache.insert_or_assign(beam, 1uz);
    return 1uz;
}

} // anonymous namespace

namespace day07::part2
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day07.txt") };
    auto diagram{ common::splitStringOn(file, '\n') };
    Beam primaryBeam{ (*diagram.begin()).find('S'), 0 };

    std::unordered_map<Beam, std::size_t, BeamHasher> cache;
    cache.reserve(1024);

    std::size_t totalPaths{ findPath(primaryBeam, diagram, cache) };

    return std::to_string(totalPaths);
}

} // namespace day07::part2

