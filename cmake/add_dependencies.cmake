macro(fidgety_add_dependency_sourced_url)
    # ARGV0: name
    # ARGV1: url
    if(ARGC LESS 2)
        message(
            FATAL_ERROR
            "to add a dependency that is to be built from source, \
you must provide its name and URL to a certain release"
        )
    endif()
    message(STATUS "downloading ${ARGV0}'s source code and building it from source")
    FetchContent_Declare(${ARGV0} URL ${ARGV1})
    FetchContent_MakeAvailable(${ARGV0})
endmacro()

macro(fidgety_add_dependency_sourced_git)
    # ARGV0: name
    # ARGV1: url
    # ARGV2: tag
    if(ARGC LESS 3)
        message(
            FATAL_ERROR
            "to add a dependency that is to be built from source, \
you must provide its name, git repository location and tag of the relevant release"
        )
    endif()
    message(STATUS "downloading ${ARGV0}'s source code and building it from source")
    FetchContent_Declare(${ARGV0} GIT_REPOSITORY ${ARGV1} GIT_TAG ${ARGV2})
    FetchContent_MakeAvailable(${ARGV0})
endmacro()

macro(fidgety_add_dependency_installed)
    # ARGV0: name
    if(ARGC LESS 1)
        message(FATAL_ERROR "to add an already installed depedency, you must provide its name")
    endif()
    message(STATUS "searching for ${ARGV0} using CMake's find_package")
    find_package(${ARGV0} CONFIG REQUIRED)
endmacro()
