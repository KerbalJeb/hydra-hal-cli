# https://gitlab.com/mariokonrad/timesheet/-/blob/master/cmake/UsePackage.cmake

macro(use_package)
    include(CMakeParseArguments)
    cmake_parse_arguments(
            ARG
            "" # no values
            "TARGET;GIT_REPOSITORY;GIT_TAG;GIT_SUBMODULES" # single values
            "" # no multi values
            ${ARGN}
    )

    # Only clone to local dir if LOCAL_REPO_DIR exists
    if (DEFINED LOCAL_REPO_DIR)
        set(path "${LOCAL_REPO_DIR}/${ARG_TARGET}")
        if (NOT IS_DIRECTORY "${path}")
            find_package(Git REQUIRED QUIET)
            execute_process(COMMAND ${GIT_EXECUTABLE} clone "${ARG_GIT_REPOSITORY}" "${path}")
        endif ()
        set(target_repo "file://${path}")
    else ()
        set(target_repo "${ARG_GIT_REPOSITORY}")
    endif ()

    include(FetchContent)
    FetchContent_Declare(${ARG_TARGET}
            GIT_REPOSITORY "${target_repo}"
            GIT_TAG ${ARG_GIT_TAG}
            GIT_SUBMODULES ${ARG_GIT_SUBMODULES}
            )
    FetchContent_MakeAvailable(${ARG_TARGET})
endmacro()