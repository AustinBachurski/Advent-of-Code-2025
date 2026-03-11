#include "day06/part2/part2.hpp"
#include "common/common.hpp"

#include <algorithm>
#include <charconv>
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

struct Problem
{
    std::vector<std::size_t> numbers;
    Operation op{};
};

//std::size_t addIndexOf(std::size_t i, std::span<std::vector<std::size_t>> rows)
//{
//    std::size_t sum{};
//
//    for (auto const &row : rows)
//    {
//        sum += row.at(i);
//    }
//
//    return sum;
//}
//
//std::size_t mulIndexOf(std::size_t i, std::span<std::vector<std::size_t>> rows)
//{
//    std::size_t product{ rows.front().at(i) };
//
//    for (auto const &row : rows | std::views::drop(1))
//    {
//        product *= row.at(i);
//    }
//
//    return product;
//}

std::vector<Problem> extractProblems(std::span<std::string_view> rows)
{
    std::size_t numRows{ rows.size() };
    std::size_t numCols{ rows.front().size() };

    std::vector<Problem> problems;
    Problem p;
    unsigned pIndex{ 0 };

    for (auto const x : std::views::iota(0uz, numCols) | std::views::reverse)
    {
        bool allEmpty{ true };
        unsigned shift{ 10 };
        p.numbers.emplace_back();

        for (auto const y : std::views::iota(0uz, numRows))
        {
            char current{ rows.at(y).at(x) };

            if (current == ' ')
            {
                continue;
            }
            else
            {
                allEmpty = false;

                if (current >= '0')
                {
                    std::size_t value{ static_cast<std::size_t>(current - 48) };
                    p.numbers.at(pIndex) *= shift;
                    p.numbers.at(pIndex) += value;
                }
                else
                {
                    p.op = current == '*' ? Operation::Mul : Operation::Add;
                }
            }
        }

        if (!allEmpty)
        {
            ++pIndex;
        }
        else
        {
            p.numbers.pop_back(); // Remove extra element added by outer loop.
            problems.push_back(p);
            p = {};
            pIndex = 0;
        }
    }

    problems.push_back(p);
    return problems;
}

} // anonymous namespace

namespace day06::part2
{

std::string solve()
{
    std::string file{ common::readEntireInputFile("day06.txt") };
    std::vector<std::string_view> rows;
    rows.append_range( common::splitStringOn(file, '\n'));

    auto problems{ extractProblems(rows) };

    return "";
}

} // namespace day06::part2

