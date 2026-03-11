#include "day06/part1/part1.hpp"
#include "common/common.hpp"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>


namespace
{

enum class Operation
{
    Err,
    Add,
    Mul,
};

std::size_t addIndexOf(std::size_t i, std::span<std::vector<std::size_t>> rows)
{
    std::size_t sum{};

    for (auto const &row : rows)
    {
        sum += row.at(i);
    }

    return sum;
}

std::size_t mulIndexOf(std::size_t i, std::span<std::vector<std::size_t>> rows)
{
    std::size_t product{ rows.front().at(i) };

    for (auto const &row : rows | std::views::drop(1))
    {
        product *= row.at(i);
    }

    return product;
}


} // anonymous namespace

namespace day06::part1
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day06.txt") };
    auto rows{ common::splitStringOn(file, '\n') };

    std::vector<std::vector<std::size_t>> values;
    std::vector<Operation> ops;

    for (auto row : rows)
    {
        auto splitRow{ common::splitStringOn(row, ' ') };

        if (splitRow.front().contains('+') || splitRow.front().contains('*'))
        {
            using namespace std::string_view_literals;
            std::ranges::transform(splitRow, std::back_inserter(ops),
               [](auto&& view)
               { return view == "*"sv ? Operation::Mul : Operation::Add; });
        }
        else
        {
            auto numbers{ splitRow | std::views::transform(
                [](auto&& view)
                { return common::stringviewToNumber<std::size_t>(view) ; }) };

            std::vector<std::size_t> vec;
            vec.append_range(numbers);

            values.push_back(vec);
        }
    }

    std::size_t sum{};

    for (auto [i, op] : std::views::enumerate(ops))
    {
        if (op == Operation::Add)
        {
            sum += addIndexOf(static_cast<std::size_t>(i), values);
        }
        else
        {
            sum += mulIndexOf(static_cast<std::size_t>(i), values);
        }
    }

    return std::to_string(sum);
}

} // namespace day06::part1

