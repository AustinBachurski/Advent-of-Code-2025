#include "day07/part1/part1.hpp"
#include "common/common.hpp"

#include <cstddef>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>


namespace day07::part1
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day07.txt") };
    auto diagram{ common::splitStringOn(file, '\n') };

    std::set<std::size_t> beams;
    beams.insert((*diagram.begin()).find('S'));

    std::size_t splits{};
    std::vector<std::size_t> toInsert;
    std::vector<std::size_t> toRemove;

    for (auto line : diagram | std::views::drop(1))
    {
        for (auto const i : beams)
        {
            if (line.at(i) == '^')
            {
                toInsert.push_back(i - 1);
                toInsert.push_back(i + 1);
                toRemove.push_back(i);
                ++splits;
            }
        }

        for (auto const rem : toRemove)
        {
            beams.erase(rem);
        }

        toRemove.clear();

        for (auto const ins : toInsert)
        {
            beams.insert(ins);
        }

        toInsert.clear();
    }

    return std::to_string(splits);
}

} // namespace day07::part1

