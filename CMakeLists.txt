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


if (NOT ${CMAKE_CROSSCOMPILING})
    set(MCU_LINE stm32f0)
    add_definitions(-DSTM32F042x6)
endif ()
# Get CMSIS
set(CMSIS_VERSION 5.8.0)
include(cmake/Cmsis.cmake)

if (${MCU_LINE} STREQUAL stm32f0)
    set(PORTABLE_DIR "portable/vendor/st")
else ()
    message(FATAL_ERROR "No valid mcu line set in MCU_LINE var (${MCU_LINE})")
endif ()
message(STATUS src/${PORTABLE_DIR}/gpio.cpp)
add_library(hal_peripheral src/${PORTABLE_DIR}/gpio.cpp)

target_include_directories(hal_peripheral PUBLIC include)
add_library(hh::peripheral ALIAS hal_peripheral)
target_compile_features(hal_peripheral PRIVATE cxx_std_17)
target_link_libraries(hal_peripheral cmsis::cmsis)