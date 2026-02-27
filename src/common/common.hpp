#ifndef AOC_2025_COMMON_UTILS_HPP
#define AOC_2025_COMMON_UTILS_HPP

#include <charconv>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>


namespace common
{

std::string readEntireInputFile(std::string_view fileName);

template <typename T> T stringviewToNumber(std::string_view sv)
{
    T number{};

    auto [_, ec]{ std::from_chars(std::to_address(sv.begin()),
                                  std::to_address(sv.end()), number) };

    if (ec != std::errc{})
    {
        throw std::invalid_argument(
            std::format("Could not convert value `{}` to number.", sv));
    }

    return number;
}

} //namespace common

#endif

