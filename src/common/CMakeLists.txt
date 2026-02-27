add_library(aocCommonUtils STATIC)

target_link_libraries(aocCommonUtils PRIVATE projectCompileFlags)

target_sources(aocCommonUtils PRIVATE
    common.cpp
    common.hpp
)

