#include "common.hpp"

#include <format>
#include <iterator>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>


namespace common
{

std::string readEntireInputFile(std::string_view fileName)
{
    std::filesystem::path inputDir{ "input" };
    std::ifstream file{ inputDir / fileName };

    if (!file.is_open())
    {
        throw std::invalid_argument( std::format("Failed to open file: {}/{}",
                                                 inputDir.string(), fileName));
    }

    return { std::istreambuf_iterator<char>(file),
             std::istreambuf_iterator<char>() };
}

} //namespace common

