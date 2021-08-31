# todo: setup CTest
include(FetchContent)

set(CATCH2_VERSION v3.0.0-preview3)
FetchContent_Declare(catch2
                     GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
                     GIT_TAG "${CATCH2_VERSION}"
                     )
FetchContent_MakeAvailable(catch2)
list (APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")

add_executable(test_mini_stream test_mini_stream.cpp)
target_link_libraries(test_mini_stream Catch2::Catch2WithMain hh::cli)