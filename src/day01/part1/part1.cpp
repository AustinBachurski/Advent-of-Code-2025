#include "day01/part1/part1.hpp"
#include "common/common.hpp"

#include <string>
#include <string_view>


namespace
{

int rotateLeft(int dial, int &counter, int const ticks)
{
    dial -= ticks;

    while (dial < 0)
    {
        dial += 100;
    }

    if (dial == 0) { ++counter; }

    return dial;
}

int rotateRight(int dial, int &counter, int const ticks)
{
    dial += ticks;

    while (dial > 99)
    {
        dial -= 100;
    }

    if (dial == 0) { ++counter; }

    return dial;
}

}

namespace day01::part1
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day01.txt") };

    auto rotations{ common::splitStringOn(file, '\n') };

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

} // namespace day01::part1

