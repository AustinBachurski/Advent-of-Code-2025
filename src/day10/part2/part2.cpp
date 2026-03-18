#include "day10/part2/part2.hpp"
#include "common/common.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <ranges>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <span>
#include <string>
#include <string_view>
#include <vector>


namespace
{

constexpr int lightDiagram{ 1 };
constexpr int joltageDiagram{ 1 };

struct JoltageValue
{
    std::vector<unsigned> const values;
    auto operator<=>(JoltageValue const &other) const = default;
};

struct JoltageHasher
{
    std::size_t operator()(JoltageValue const &value) const noexcept
    {
        std::size_t hash{};

        for (auto&& [i, number] : std::views::enumerate(value.values))
        {
            hash ^= std::hash<std::size_t>{}(number) << i;
        }

        return hash;
    }
};

struct Button
{
    std::vector<unsigned> const indices;
};

JoltageValue extractTarget(auto segments)
{
    auto segmentsIter{ segments.begin() };
    std::advance(segmentsIter, std::distance(segments.begin(), segments.end()) - 1);

    auto target{ *segmentsIter };
    auto targets{ common::splitStringOn(target, ',') };

    std::vector<unsigned> values;

    // Skip leading brace.
    values.push_back(
        common::stringviewToNumber<unsigned>(targets.front().substr(1)));

    for (auto number : targets | std::views::drop(1))
    {
        values.push_back(common::stringviewToNumber<unsigned>(number));
    }

    values.front() += 12;
    return { values };
}

Button makeButton(std::string_view diagram)
{
    auto noOpeningBrace{ diagram.substr(diagram.find('(') + 1) };
    auto numbers{ common::splitStringOn(noOpeningBrace, ',') };

    std::vector<unsigned> button;

    for (auto number : numbers)
    {
        button.push_back(common::stringviewToNumber<unsigned>(number));
    }

    return { button };
}

std::vector<Button> extractButtons(auto schematic)
{
    auto buttonCount{
        std::distance(schematic.begin(), schematic.end())
        - lightDiagram
        - joltageDiagram
    };

    std::vector<Button> buttons;
    buttons.reserve(static_cast<std::size_t>(buttonCount));

    for (auto buttonDiagram : schematic | std::views::drop(lightDiagram)
    | std::views::take(buttonCount))
    {
        buttons.push_back(makeButton(buttonDiagram));
    }

    return buttons;
}

JoltageValue pressButtonOnValue(Button const &button,
                                JoltageValue const &value)
{
    std::vector<unsigned> result = value.values;

    for (auto const i : button.indices)
    {
        ++result.at(i);
    }

    return { result };
}

std::vector<JoltageValue> setInitialValues(JoltageValue const &target,
                                           std::span<Button const> buttons)
{
    std::vector<JoltageValue> initial;
    initial.reserve(buttons.front().indices.size());

    for (auto const &button : buttons)
    {
        std::vector<unsigned> values;
        values.resize(target.values.size());

        for (auto const i : button.indices)
        {
            ++values.at(i);
        }

        initial.emplace_back(values);
    }

    return initial;
}

bool anyValueHasOverrun(
    std::span<unsigned const> current, std::span<unsigned const> target)
{
    for (auto&& [c, t] : std::views::zip(current, target))
    {
        if (c > t)
        { return true; }
    }

    return false;
}

int findBestSequenceOfButtonsToTarget(JoltageValue const &target,
                                      std::span<Button const> buttons)
{
    std::unordered_set<JoltageValue, JoltageHasher> seen;
    auto currentValues{ setInitialValues(target, buttons) };

    if (std::ranges::any_of(currentValues, [&target](JoltageValue const &value)
                            { return value == target; }))
    { return 1; }

    std::vector<JoltageValue> nextValues;
    nextValues.reserve(currentValues.size());

    int buttonPresses{ 1 };

    while (!currentValues.empty())
    {
        ++buttonPresses;

        for (auto const &value : currentValues)
        {
            for (auto const &button : buttons)
            {
                auto current{ pressButtonOnValue(button, value) };

                if (current == target)
                { return buttonPresses; }

                if (anyValueHasOverrun(current.values, target.values))
                {
                    continue;
                }

                if (!seen.contains(current))
                {
                    seen.insert(current);
                    nextValues.push_back(current);
                }
            }
        }

        std::swap(currentValues, nextValues);
        nextValues.clear(); 
     }

    throw std::runtime_error(
        "Possible values exhausted before target was found.");
}

int buttonPressesRequired(auto manualLine)
{
    auto segments{ common::splitStringOn(manualLine, ' ') };
    auto const target{ extractTarget(segments) };
    auto const buttons{ extractButtons(segments) };

    return findBestSequenceOfButtonsToTarget(target, buttons);
}

} // anonymous namespace

namespace day10::part2
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

} // namespace day10::part2

