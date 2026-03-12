#include "day07/part2/part2.hpp"
#include "common/common.hpp"

#include <cstddef>
#include <ranges>
#include <string>
#include <string_view>


namespace
{

std::size_t findPath(std::size_t const beam, auto diagram, std::ptrdiff_t atRow)
{
    std::size_t paths{};

    for (auto [row, line] : std::views::enumerate(diagram)
                            | std::views::drop(atRow))
    {
        if (line.at(beam) == '^')
        {
            paths += findPath(beam - 1, diagram, row);
            paths += findPath(beam + 1, diagram, row);
            return paths;
        }
    }

    return 1uz;
}

} // anonymous namespace

namespace day07::part2
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day07.txt") };
    auto diagram{ common::splitStringOn(file, '\n') };
    std::size_t beam{ (*diagram.begin()).find('S') };

    std::size_t totalPaths{ findPath(beam, diagram, 1) };

    return std::to_string(totalPaths);
}

} // namespace day07::part2

