add_executable(solve_day09)

target_link_libraries(solve_day09 PRIVATE projectCompileFlags)
target_link_libraries(solve_day09 PRIVATE aocCommonUtils)

target_sources(solve_day09 PRIVATE
    solve.cpp
    part1/part1.cpp
    part2/part2.cpp
    part2/intersection.cpp
    part2/polygon.cpp
    part2/windingNumberAlgo.cpp

    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS
        part1
        part2
)

