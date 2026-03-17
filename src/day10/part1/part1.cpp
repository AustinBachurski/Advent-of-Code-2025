#include "day10/part1/part1.hpp"
#include "common/common.hpp"

#include <bitset>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <iostream>
#include <ranges>
#include <unordered_set>
#include <span>
#include <string>
#include <string_view>
#include <vector>


namespace
{

constexpr int N{ 16 };
constexpr int lightDiagram{ 1 };
constexpr int joltageDiagram{ 1 };

std::bitset<N> extractTargetBits(std::string_view target)
{
    auto chars{ target | std::views::filter([](char const c)
                                            { return c != '[' && c != ']'; }) };

    assert(std::distance(chars.begin(), chars.end()) < N);

    std::bitset<N> bits;

    for (auto&& [i, c] : std::views::enumerate(chars))
    {
         if (c == '#')
         {
            bits.set(static_cast<std::size_t>(i));
         }
    }

    return bits;
}

std::bitset<N> makeButton(std::string_view diagram)
{
    auto noOpeningBrace{ diagram.substr(diagram.find('(') + 1) };
    auto numbers{ common::splitStringOn(noOpeningBrace, ',') };

    std::bitset<N> button;

    for (auto number : numbers)
    {
        button.set(common::stringviewToNumber<unsigned>(number));
    }

    return button;
}

std::vector<std::bitset<N>> extractButtons(auto schematic)
{
    auto buttonCount{
        std::distance(schematic.begin(), schematic.end())
        - lightDiagram
        - joltageDiagram
    };

    std::vector<std::bitset<N>> buttons;

    for (auto buttonDiagram : schematic | std::views::drop(lightDiagram)
                                        | std::views::take(buttonCount))
    {
        buttons.push_back(makeButton(buttonDiagram));
    }

    return buttons;
}

int findBestSequenceOfButtonsToTarget(std::bitset<N> const target,
                                      std::span<std::bitset<N> const> buttons)
{
    std::unordered_set<std::bitset<N>> values;
    values.insert_range(buttons);

    std::vector<std::bitset<N>> insertBuffer;
    insertBuffer.reserve(buttons.size());
    std::vector<std::bitset<N>> removeBuffer;
    removeBuffer.reserve(buttons.size());

    int buttonPresses{ 1 };

    while (true)
    {
        ++buttonPresses;

        for (auto const value : values)
        {
            for (auto const button : buttons)
            {
                auto current{ value ^ button };

                if (current == target)
                {
                    return buttonPresses;
                }

                if (!values.contains(current))
                {
                    insertBuffer.push_back(current);
                    removeBuffer.push_back(value);
                }
            }
        }

        for (auto const each : removeBuffer)
        {
            values.erase(each);
        }

        values.insert_range(insertBuffer);
        insertBuffer.clear();

        removeBuffer.clear();
    }
}

int buttonPressesRequired(auto manualLine)
{
    auto segments{ common::splitStringOn(manualLine, ' ') };
    auto const target{ extractTargetBits(segments.front()) };
    auto const buttons{ extractButtons(segments) };

    return findBestSequenceOfButtonsToTarget(target, buttons);
}

} // anonymous namespace

namespace day10::part1
{

std::string solve()
{
    auto file{ common::readEntireInputFile("day10.txt") };
    auto lines{ common::splitStringOn(file, '\n') };

    int sum{};

    for (auto line : lines)
    {
        sum += buttonPressesRequired(line);
    }

    return std::to_string(sum);
}

} // namespace day10::part1
