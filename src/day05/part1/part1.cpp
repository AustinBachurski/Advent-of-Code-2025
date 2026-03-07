#include "day05/part1/part1.hpp"
#include "common/common.hpp"

#include <algorithm>
#include <cstddef>
#include <span>
#include <string>
#include <string_view>
#include <vector>


namespace day05::part1
{

struct Values
{
    std::size_t min{};
    std::size_t max{};
};

std::vector<Values> getFreshValues(auto freshRanges)
{
    std::vector<Values> freshValues;

    std::size_t begin{};
    std::size_t end{};

    for (auto const range : freshRanges)
    {
        if (range.empty())
        { continue; }

        auto values{ common::splitStringOn(range, '-') };
        
        begin = common::stringviewToNumber<std::size_t>(*values.begin());
        end = common::stringviewToNumber<std::size_t>(*++values.begin());

        freshValues.emplace_back(begin, end);
    }

    return freshValues;
}

std::size_t countFreshIngredients(
    std::span<Values const> freshValues, auto ingredients)
{
    std::size_t count{};

    std::size_t value{};

    for (auto const id : ingredients)
    {
        if (id.empty())
        { continue; }

        value = common::stringviewToNumber<std::size_t>(id);

        if (std::ranges::any_of(freshValues, [value](Values pair)
                        { return value >= pair.min && value <= pair.max; } ))
        {
            ++count;
        }
    }

    return count;
}

std::string solve()
{
    std::string file{ common::readEntireInputFile("day05.txt") };

    using namespace std::string_view_literals;
    auto separated{ common::splitStringOn(file, "\n\n"sv) };

    auto freshRanges{ common::splitStringOn(*separated.begin(), '\n') };
    auto ingredients{ common::splitStringOn(*++separated.begin(), '\n') };

    auto freshValues = getFreshValues(freshRanges);
    

    return std::to_string(countFreshIngredients(freshValues, ingredients));
}

} // namespace day05::part1

