#include "part1/part1.hpp"
#include "part2/part2.hpp"

#include <print>


int main()
{
    std::println("Day 1:\n\tPart 1: {}\n\tPart 2: {}",
                 day01::part1::solve(),
                 day01::part2::solve());
}

