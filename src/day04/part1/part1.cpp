#include "day04/part1/part1.hpp"
#include "common/common.hpp"

#include <array>
#include <algorithm>
#include <span>
#include <string>
#include <string_view>


namespace
{

constexpr char TP{ '@' };

struct Map
{
    std::string_view map;
    std::size_t columns;
    std::size_t rows;
};

int countForRow(Map const input, std::span<std::size_t const> const indices)
{
    std::size_t hMax{ (indices.front() / input.rows + 1) * input.columns };
    std::size_t hMin{ hMax - input.columns };

    int count{};

    for (auto const i : indices | std::views::drop(1))
    {
        if (i >= input.map.size())
        { continue; }

        if (i >= hMin && i < hMax && input.map.at(i) == TP)
        {
            ++count;
        }
    }

    return count;
}

bool isValid(Map const input, std::size_t const index)
{
    std::array<std::size_t, 3> sameRow{{
        index,
        index - 1,
        index + 1
    }};
    std::array<std::size_t, 4> above{{
        index + input.columns,
        index + input.columns,
        index + input.columns - 1,
        index + input.columns + 1,
    }};
    std::array<std::size_t, 4> below{{
        index - input.columns,
        index - input.columns,
        index - input.columns - 1,
        index - input.columns + 1,
    }};

    int sum{};

    sum += countForRow(input, sameRow);
    sum += countForRow(input, above);
    sum += countForRow(input, below);

    return sum < 4;
}

int accessibleRolls(Map const &input)
{
    int sum{};

    for (auto const [i, c] : std::views::enumerate(input.map))
    {
        if (c == TP && isValid(input, static_cast<std::size_t>(i)))
        {
            ++sum;
        }
    }

    return sum;
}

} // anonymous namespace

namespace day04::part1
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day04.txt") };
    std::size_t columns{ file.find('\n') };
    file.erase(std::remove(file.begin(), file.end(), '\n'), file.end());
    std::size_t rows{ file.size() / columns };

    Map const input{ .map = file, .columns = columns, .rows = rows };

    return std::to_string(accessibleRolls(input));
}

} // namespace day04::part1

