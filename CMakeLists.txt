cmake_minimum_required (VERSION 3.20)
project ("Hydra Hal Peripheral Drivers" LANGUAGES C CXX ASM)
list (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Doxygen Documentation Target
add_subdirectory (docs)

if (NOT ${CMAKE_CROSSCOMPILING})
  set(MCU_LINE stm32f0)
  add_definitions(-DSTM32F042x6)
endif ()

# Get CMSIS
set (CMSIS_VERSION 5.8.0)
include (cmsis)

add_library (hal_peripheral src/gpio.cpp)
target_include_directories (hal_peripheral PUBLIC include)
target_link_libraries (hal_peripheral cmsis::cmsis)
target_compile_features(hal_peripheral PRIVATE cxx_std_17)
add_library(hh::peripheral ALIAS hal_peripheral)

add_subdirectory (test)
