#include "day03/part1/part1.hpp"
#include "common/common.hpp"

#include <algorithm>
#include <string>
#include <string_view>


namespace
{

int maxTwoFromBank(std::string_view bank)
{
    if (bank.empty()) { return 0; }

    auto maxIter{ std::ranges::max_element(bank) };
    int max{ *maxIter - 48 };

    auto before{ std::max_element(bank.begin(), maxIter) };
    int valueBefore{ before != maxIter ? (*before - 48) * 10 + max : max };

    auto after{ std::max_element(++maxIter, bank.end()) };
    int valueAfter{ after != bank.end() ? max * 10 + (*after - 48) : max };

    return valueBefore > valueAfter ? valueBefore : valueAfter;
}

} // anonymous namespace


namespace day03::part1
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day03.txt") };

    auto banks{ common::splitStringOn(file, '\n') };

    int sum{};

    for (auto bank : banks)
    {
        sum += maxTwoFromBank(bank);
    }

    return std::to_string(sum);
}

} // namespace day03::part1

