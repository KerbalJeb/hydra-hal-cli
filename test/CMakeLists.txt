# todo: setup CTest
include(FetchContent)

set(CATCH2_VERSION v2.13.7)
FetchContent_Declare(catch2
                     GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
                     GIT_TAG "${CATCH2_VERSION}"
                     )
FetchContent_MakeAvailable(catch2)