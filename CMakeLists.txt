cmake_minimum_required (VERSION 3.16)
project ("Arm Cortex-M Base" C CXX ASM)
option(CLONE_TO_LOCAL OFF)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(Sanitizers.cmake)

add_library(hal_cli
            src/mini_stream.cpp
            src/shell.cpp
            src/ansi_parser.cpp)
target_include_directories(hal_cli PUBLIC include)
target_compile_features(hal_cli PUBLIC cxx_std_20)

add_library(hh::cli ALIAS hal_cli)

# Doxygen Documentation Target
add_subdirectory(docs)
add_subdirectory(test)

