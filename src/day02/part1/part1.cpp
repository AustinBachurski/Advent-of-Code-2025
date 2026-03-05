#include "day02/part1/part1.hpp"
#include "common/common.hpp"

#include <string>
#include <string_view>


namespace
{

int countDigits(std::size_t value)
{
    int digits{};

    do
    {
        ++digits;
    } while (value /= 10);

    return digits;
}

std::size_t nHighDigits(std::size_t value, int digits)
{
    while (digits--)
    {
        value /= 10;
    }

    return value;
}

std::size_t nLowDigits(std::size_t value, int digits)
{
    std::size_t result{};
    std::size_t shift{ 1 };

    while (digits--)
    {
        result += (value % 10) * shift;
        shift *= 10;
        value /= 10;
    }

    return result;
}

bool halvesMatch(std::size_t value, int digits)
{
    digits /= 2;
    return nHighDigits(value, digits) == nLowDigits(value, digits);
}

bool allDigitsAreEqual(std::size_t value)
{
    std::size_t digit{ value % 10 };

    while (value /= 10)
    {
        if (digit != value % 10)
        {
            return false;
        }
    }

    return true;
}

std::size_t checkRange(std::string_view pair)
{
    auto values{ common::splitStringOn(pair, '-') };

    auto first{ *values.begin() };
    auto second{ *(++values.begin()) };

    if (first.size() & 1 && second.size() & 1)
    {
        return 0;
    }

    auto begin{ common::stringviewToNumber<std::size_t>(first) };
    auto end{ common::stringviewToNumber<std::size_t>(second) };

    std::size_t invalids{};

    for (auto value : std::views::iota(begin, end + 1))
    {
        if (value < 11) { continue; }

        int digits{ countDigits(value) };

        if (digits & 1) { continue; }

        if (halvesMatch(value, digits))
        {
            invalids += value;
        }
        else if (allDigitsAreEqual(value))
        {
            invalids += value;
        }
    }

    return invalids;
}

} // anonymous namespace


namespace day02::part1
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

} // namespace day02::part1

