add_executable(solve_day01)

target_link_libraries(solve_day01 PRIVATE projectCompileFlags)
target_link_libraries(solve_day01 PRIVATE aocCommonUtils)

target_sources(solve_day01 PRIVATE
    solve.cpp
    part1/part1.cpp
    part2/part2.cpp

    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS
        part1
        part2
)

