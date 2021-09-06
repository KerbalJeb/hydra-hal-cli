cmake_minimum_required (VERSION 3.16)
project ("Hydra Hal" C CXX ASM)
option(CLONE_TO_LOCAL OFF)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

include(cmake/Sanitizers.cmake)

add_library(hal_cli
            src/mini_stream.cpp
            src/shell.cpp
            src/ansi_parser.cpp)
target_include_directories(hal_cli PUBLIC include)
target_compile_features(hal_cli PUBLIC cxx_std_20)
target_compile_options(hal_cli PUBLIC -Wall -Wextra -pedantic)

add_library(hh::cli ALIAS hal_cli)

# Doxygen Documentation Target
add_subdirectory(docs)
add_subdirectory(test)

