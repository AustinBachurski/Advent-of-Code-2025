#include "day05/part2/part2.hpp"
#include "common/common.hpp"

#include <algorithm>
#include <compare>
#include <cstddef>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>


namespace
{

struct Range
{
    std::size_t min{};
    std::size_t max{};

    auto operator<=>(Range const &other) const = default;
};

Range makeRangeFromPair(auto pair)
{
    Range range;

    auto values{ common::splitStringOn(pair, '-') };

    std::from_chars(std::to_address((*values.begin()).begin()),
                    std::to_address((*values.begin()).end()),
                    range.min);

    std::from_chars(std::to_address((*++values.begin()).begin()),
                    std::to_address((*++values.begin()).end()),
                    range.max);

    return range;
}

void combineRangeWithExisting(auto rangesIterator, Range rangeToCombine)
{
    if (rangeToCombine.min < rangesIterator->min
        && rangeToCombine.max > rangesIterator->max)
    {
        rangesIterator->min = rangeToCombine.min;
        rangesIterator->max = rangeToCombine.max;
    }
    else if (rangeToCombine.min >= rangesIterator->min
        && rangeToCombine.min <= rangesIterator->max
        && rangeToCombine.max > rangesIterator->max)
    {
        rangesIterator->max = rangeToCombine.max;
    }
    else if (rangeToCombine.max >= rangesIterator->min
        && rangeToCombine.max <= rangesIterator->max
        && rangeToCombine.min < rangesIterator->min)
    {
        rangesIterator->min = rangeToCombine.min;
    }
}

std::vector<Range> getNumberRanges(auto numberRanges)
{
    Range localRange;
    std::vector<Range> ranges;

    auto canCombine{ [&localRange](Range const &range)
        {
            return (localRange.min < range.min && localRange.max > range.max)
            || (localRange.min >= range.min
            && localRange.min <= range.max
            && localRange.max > range.max)
            || (localRange.max >= range.min
            && localRange.max <= range.max
            && localRange.min < range.min);
        } };

    for (auto const pair : numberRanges)
    {
        localRange = makeRangeFromPair(pair);

        auto combinableRangeIter{ std::ranges::find_if(ranges, canCombine) };

        if (combinableRangeIter != ranges.end())
        {
            combineRangeWithExisting(combinableRangeIter, localRange);
        }
        else
        {
            ranges.emplace_back(localRange.min, localRange.max);
        }
    }

    return ranges;
}

auto canCombineRangeAtIndex(std::size_t index, std::vector<Range> &ranges)
{
    auto canCombine{ [index, ranges](Range const &range)
    {
        return (ranges[index].min < range.min && ranges[index].max > range.max)
        || (ranges[index].min >= range.min
        && ranges[index].min <= range.max
        && ranges[index].max > range.max)
        || (ranges[index].max >= range.min
        && ranges[index].max <= range.max
        && ranges[index].min < range.min);
    } };

    return std::ranges::find_if(ranges, canCombine);
}

void combineInclusiveRangesInPlace(std::vector<Range> &ranges)
{
    bool goAgain{ true };

    while (goAgain)
    {
        goAgain = false;

        for (auto [i, rangeToCheck] : std::views::enumerate(ranges))
        {
            if (auto rangesIter{
                canCombineRangeAtIndex(static_cast<std::size_t>(i), ranges) };
                rangesIter != ranges.end())
            {
                combineRangeWithExisting(rangesIter, rangeToCheck);
                ranges.erase(ranges.begin() + i);
                goAgain = true;
                break;
            }
        }
    }
}

void removeDuplicateRanges(std::vector<Range> &ranges)
{
    std::ranges::sort(ranges);
    ranges.erase(std::ranges::unique(ranges).begin(), ranges.end());
}

} // anonymous namespace

namespace day05::part2
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day05.txt") };

    using namespace std::string_view_literals;
    auto sections{ common::splitStringOn(file, "\n\n"sv) };
    auto numberRanges{ common::splitStringOn(*sections.begin(), '\n') };

    auto ranges{ getNumberRanges(numberRanges) };
    combineInclusiveRangesInPlace(ranges);
    removeDuplicateRanges(ranges);

    std::size_t sum{};

    for (auto const range : ranges)
    {
        sum += range.max - range.min + 1;
    }

    return std::to_string(sum);
}

} // namespace day05::part2

