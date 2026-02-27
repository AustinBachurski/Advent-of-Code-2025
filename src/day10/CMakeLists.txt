add_executable(solve_day10)

target_link_libraries(solve_day10 PRIVATE projectCompileFlags)
target_link_libraries(solve_day10 PRIVATE aocCommonUtils)

target_sources(solve_day10 PRIVATE
    solve.cpp
    part1/part1.cpp
    part2/part2.cpp

    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS
        part1
        part2
)

