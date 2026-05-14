#include "day10/part2/part2.hpp"
#include "common/common.hpp"

#include <cassert>
#include <cstddef>
#include <iterator>
#include <limits>
#include <ranges>
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
    std::vector<unsigned> values;

    bool operator==(JoltageValue const &other) const
    {
        for (auto const i : std::views::iota(0UZ, values.size()))
        {
            if (values.at(i) != other.values.at(i))
            {
                return false;
            }
        }
        return true;
    }

    bool operator<=(JoltageValue const &other) const
    {
        for (auto const i : std::views::iota(0UZ, values.size()))
        {
            if (values.at(i) > other.values.at(i))
            {
                return false;
            }
        }
        return true;
    }

    bool operator>=(JoltageValue const &other) const
    {
        for (auto const i : std::views::iota(0UZ, values.size()))
        {
            if (values.at(i) < other.values.at(i))
            {
                return false;
            }
        }
        return true;
    }
};

struct Button
{
    std::vector<unsigned> const indices;
};

class Solver
{
public:
    Solver(JoltageValue const &target, std::span<Button const> buttons)
        : target_{ target }
        , buttons_{ buttons }
        {
            currentValue_.values.resize(target.values.size());
        }

    int solve()
    {
        recursiveSolve(0UZ);
        return bestCount_;
    }

private:
    void recursiveSolve(std::size_t index)
    {
        if (index >= buttons_.size() || currentValue_ >= target_)
        {
            if (currentValue_ == target_)
            {
                bestCount_ = currentCount_ < bestCount_ ? currentCount_ : bestCount_;
            }
            return;
        }

        recursiveSolve(index);
        recursiveSolve(index + 1UZ);

        press(buttons_.at(index));
        ++currentCount_;

        if (currentValue_ <= target_)
        {
            recursiveSolve(index);
            recursiveSolve(index + 1UZ);
        }

        unpress(buttons_.at(index));
        --currentCount_;
    }

    void press(Button const &button)
    {
        for (auto const i : button.indices)
        {
            ++currentValue_.values.at(i);
        }
    }

    void unpress(Button const &button)
    {
        for (auto const i : button.indices)
        {
            --currentValue_.values.at(i);
        }
    }

private:
    JoltageValue const &target_;
    std::span<Button const> buttons_;
    int bestCount_{ std::numeric_limits<int>::max() };
    int currentCount_{};
    JoltageValue currentValue_;
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

int findBestSequenceOfButtonsToTarget(JoltageValue const &target,
        std::span<Button const> buttons)
{
    Solver solver(target, buttons);
    return solver.solve();
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

