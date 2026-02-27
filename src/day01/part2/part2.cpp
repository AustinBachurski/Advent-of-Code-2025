#include "day01/part2/part2.hpp"
#include "common/common.hpp"

#include <ranges>
#include <string>
#include <string_view>


namespace
{

constexpr int dial100{ 100 };
constexpr int dial99{ 99 };
constexpr int dialZero{ 0 };

int rotateLeft(int dial, int &counter, int ticks)
{
    int modResult{ ticks % dial100 };
    counter += ticks / dial100;

    if (!modResult)
    {
        return dial;
    }

    if (!dial)
    {
        return dial100 - modResult;
    }

    dial -= modResult;

    if (dial > dialZero)
    {
        return dial;
    }

    ++counter;
    return dial == dialZero ? dial : dial100 + dial;
}

int rotateRight(int dial, int &counter, int ticks)
{
    int modResult{ ticks % dial100 };
    counter += ticks / dial100;

    if (!modResult)
    {
        return dial;
    }

    dial += modResult;

    if (dial < dial100)
    {
        return dial;
    }

    ++counter;
    return dial == dial100 ? dialZero : dial - dial100;
}

}

namespace day01::part2
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day01.txt") };

    auto rotations{ file | std::views::split('\n')
                         | std::views::transform([](auto &&each)
                             { return std::string_view{ each }; }) };

    int counter{};
    int dial{ 50 };

    for (auto each : rotations)
    {
        if (each.empty()) { break; }

        if (each.at(0) == 'L')
        {
            dial = rotateLeft(dial,
                              counter,
                              common::stringviewToNumber<int>(each.substr(1)));
        }
        else
        {
            dial = rotateRight(dial,
                               counter,
                               common::stringviewToNumber<int>(each.substr(1)));
        }
    }


    return std::to_string(counter);
}

} // namespace day01::part2

