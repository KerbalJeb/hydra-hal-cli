# todo: setup CTest
include(FetchContent)
project(Tests)

set(CATCH2_VERSION v3.0.0-preview3)
FetchContent_Declare(catch2
                     GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
                     GIT_TAG "${CATCH2_VERSION}"
                     )
FetchContent_MakeAvailable(catch2)
list (APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
enable_testing()

add_executable(test_gpio test_gpio.cpp)
target_link_libraries(test_gpio Catch2::Catch2WithMain hh::peripheral)
target_compile_features(test_gpio PRIVATE cxx_std_17)


include(CTest)
include(Catch)

catch_discover_tests(test_gpio)

add_subdirectory(device_testing)