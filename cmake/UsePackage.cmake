# https://gitlab.com/mariokonrad/timesheet/-/blob/master/cmake/UsePackage.cmake

macro(use_package)
    include(CMakeParseArguments)
    cmake_parse_arguments(
            ARG
            "" # no values
            "TARGET;GIT_TAG;LOCAL;REMOTE" # single values
            "" # no multi values
            ${ARGN}
    )

    file(TO_CMAKE_PATH "${ARG_LOCAL}" path)
    if (IS_DIRECTORY "${path}")
        set(target_repo "file://${path}")
    else ()
        if (CLONE_TO_LOCAL)
            find_package(Git REQUIRED QUIET)
            execute_process(COMMAND ${GIT_EXECUTABLE} clone "${ARG_REMOTE}" "${ARG_LOCAL}")
            set(target_repo "file://${path}")
        else ()
            set(target_repo "${ARG_REMOTE}")
        endif ()
    endif ()

    include(FetchContent)
    message(STATUS "${target_repo}")
    FetchContent_Declare(${ARG_TARGET}
            GIT_REPOSITORY "${target_repo}"
            GIT_TAG ${ARG_GIT_TAG}
            GIT_SUBMODULES ""
            )
    FetchContent_MakeAvailable(${ARG_TARGET})
endmacro()