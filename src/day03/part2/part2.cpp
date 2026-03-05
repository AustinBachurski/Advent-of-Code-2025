#include "part2.hpp"
#include "common/common.hpp"

#include <cstddef>
#include <ranges>
#include <string>
#include <string_view>


namespace
{

std::size_t maxFromBank([[maybe_unused]] std::string_view bank)
{
    std::string value;
    value.reserve(12);

    char valueToFind{ '9' };
    std::size_t previousChars{};

    while (valueToFind != '0')
    {
        for (auto const [i, c] : std::views::enumerate(bank)
        | std::views::take(bank.size() - 11uz + value.size())
        | std::views::drop(previousChars))
        {
            if (c == valueToFind)
            {
                value += c;

                if (value.size() == 12uz)
                {
                    return common::stringviewToNumber<std::size_t>(value);
                }

                previousChars = static_cast<std::size_t>(i) + 1u;
                valueToFind = ':';
                break;
            }
        }

        --valueToFind;
    }

    return 0;
}

} // anonymous namespace

namespace day03::part2
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day03.txt") };

    auto banks{ common::splitStringOn(file, '\n') };

    std::size_t sum{};

    for (auto bank : banks)
    {
        sum += maxFromBank(bank);
    }

    return std::to_string(sum);
}

} // namespace day03::part2

