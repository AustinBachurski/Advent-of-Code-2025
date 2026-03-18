cmake_minimum_required(VERSION 4.0)

project(Advent-of-Code-2025 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_library(projectCompileFlags INTERFACE)

target_include_directories(projectCompileFlags INTERFACE
    "${PROJECT_SOURCE_DIR}/src"
)

target_compile_features(projectCompileFlags INTERFACE cxx_std_26)

target_compile_options(projectCompileFlags INTERFACE
    -O0
    -g3
    -Werror
    -Wall
    -Wcast-align
    -Wconversion
    -Wdouble-promotion
    -Wduplicated-branches
    -Wduplicated-cond
    -Wextra
    -Wformat=2
    -Wimplicit-fallthrough
    -Wlogical-op
    -Wmisleading-indentation
    -Wnon-virtual-dtor
    -Wnull-dereference
    -Wold-style-cast
    -Woverloaded-virtual
    -Wpedantic
    -Wshadow
    -Wsign-conversion
    -Wunused
    -Wuseless-cast
    -fsanitize=address,undefined
)

target_link_options(projectCompileFlags INTERFACE
    -fsanitize=address,undefined
)

add_subdirectory(src)

