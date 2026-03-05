#include "day02/part2/part2.hpp"
#include "common/common.hpp"

#include <algorithm>
#include <string>
#include <string_view>


namespace
{

bool isInvalid(std::size_t number)
{
    auto valueStr{ std::to_string(number) };

    std::string_view value{ valueStr };

    std::string_view portion;

    for (auto width : std::views::iota(1uz, value.size()))
    {
        portion = value.substr(0, width);

        auto chunks{ value | std::views::chunk(width) 
                           | std::views::transform([](auto &&chunk) {
                                return std::string_view{ chunk };
            }) };

        if (std::ranges::all_of(chunks, [portion] (auto chunk) {
            return portion == chunk; }))
        {
            return true;
        }
    }

    return false;
}

std::size_t checkRange(std::string_view pair)
{
    auto values{ common::splitStringOn(pair, '-') };

    auto first{ *values.begin() };
    auto second{ *(++values.begin()) };

    auto begin{ common::stringviewToNumber<std::size_t>(first) };
    auto end{ common::stringviewToNumber<std::size_t>(second) };

    std::size_t invalids{};

    for (auto value : std::views::iota(begin, end + 1))
    {
        if (value < 11) { continue; }

        if (isInvalid(value))
        {
            invalids += value;
        }
    }

    return invalids;
}

} // anonymous namespace

namespace day02::part2
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day02.txt") };

    auto pairs{ common::splitStringOn(file, ',') };

    std::size_t sum{};

    for (auto pair : pairs)
    {
        if (pair.empty()) { break; }

        sum += checkRange(pair);
    }

    return std::to_string(sum);
}

} // namespace day02::part2


