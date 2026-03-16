#ifndef WINDING_NUMBER_ALGORITHM_HPP
#define WINDING_NUMBER_ALGORITHM_HPP

#include "day09/part2/polygon.hpp"

#include <span>

namespace winding
{

bool pointsAreContained(std::span<polygon::Point const, 4> points,
                        std::span<polygon::Point const> outer);

} // namespace winding

#endif

