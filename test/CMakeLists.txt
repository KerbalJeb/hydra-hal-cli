include(UsePackage)
SET(CMAKE_CXX_STANDARD 20)

set(CATCH2_VERSION v3.0.0-preview3)
use_package(TARGET catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
        GIT_TAG "${CATCH2_VERSION}"
        )
list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")

add_executable(test_mini_stream test_mini_stream.cpp)
target_link_libraries(test_mini_stream Catch2::Catch2WithMain hh::cli)

add_executable(test_ansi_parser test_ansi_parser.cpp)
target_link_libraries(test_ansi_parser Catch2::Catch2WithMain hh::cli)

add_executable(test_shell test_shell.cpp)
target_link_libraries(test_shell Catch2::Catch2WithMain hh::cli)

add_executable(test_cmd_history test_cmd_history.cpp)
target_link_libraries(test_cmd_history Catch2::Catch2WithMain hh::cli)

add_executable(test_fixed_string test_fixed_string.cpp)
target_link_libraries(test_fixed_string Catch2::Catch2WithMain hh::cli)

add_executable(all_tests
        test_fixed_string.cpp
        test_ansi_parser.cpp
        test_cmd_history.cpp
        test_mini_stream.cpp
        test_shell.cpp)

target_link_libraries(all_tests Catch2::Catch2WithMain hh::cli)

include(CTest)
include(Catch)

catch_discover_tests(all_tests)
