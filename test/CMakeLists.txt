# todo: setup CTest
include(FetchContent)

set(CATCH2_VERSION v3.0.0-preview3)
FetchContent_Declare(catch2
                     GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
                     GIT_TAG "${CATCH2_VERSION}"
                     )
FetchContent_MakeAvailable(catch2)

list (APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")


add_library(test_sources OBJECT test_gpio.cpp)
target_link_libraries(test_sources Catch2::Catch2 hh::peripheral)

add_executable(tests test_main.cpp)
target_link_libraries(tests PRIVATE test_sources)

include(CTest)
include(Catch)
catch_discover_tests(tests)