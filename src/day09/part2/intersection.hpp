#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "day09/part2/polygon.hpp"

#include <span>


namespace intersection
{

bool edgesAreContained(std::span<polygon::Point const, 4> points,
                        std::span<polygon::Point const> outer);

}

#endif

