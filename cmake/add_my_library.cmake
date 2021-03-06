macro(fidgety_add_my_library)
    # ARGV0: name of the library (no namespace)
    # ARGV1: library type
    # ARGV2..N: library sources
    if(ARGC LESS 2)
        message(FATAL_ERROR "bad call to fidgety_add_my_library: must have 2 or more arguments")
    endif()
    add_library(${ARGN})
    add_library("Fidgety::${ARGV0}" ALIAS ${ARGV0})
endmacro()

macro(fidgety_add_my_executable)
    # ARGV0: name of the executable (no namespace)
    # ARGV1..N: sources
    if (ARGC LESS 1)
        message(FATAL_ERROR "bad call to fidgety_add_my_executable: must have 1 or more arguments")
    endif()
    add_executable(${ARGN})
    add_executable("Fidgety::${ARGV0}" ALIAS ${ARGV0})
endmacro()

macro(fidgety_set_output_directory)
    # ARGV0: name of the target
    set_target_properties(
        ${ARGV0} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
endmacro()

macro(fidgety_link_fmt)
    # ARGV0: name of the library (no namespace)
    if(ARGC LESS 1)
        message(
            FATAL_ERROR
            "to link libraries to a target, you must provide the name of the target"
        )
    endif()
    if(${FIDGETY_FMTLIB_HEADER_ONLY})
        target_link_libraries(${ARGV0} PRIVATE fmt::fmt-header-only)
    else()
        target_link_libraries(${ARGV0} PRIVATE fmt::fmt)
    endif()
endmacro()

macro(fidgety_link_qt_base)
    # ARGV0: name of the library (no namespace)
    if(ARGC LESS 1)
        message(
            FATAL_ERROR
            "to link libraries to a target, you must provide the name of the target"
        )
    endif()
    target_link_libraries(
        ${ARGV0} PRIVATE
        ${FIDGETY_QT_CORE} ${FIDGETY_QT_GUI} ${FIDGETY_QT_WIDGETS} ${FIDGETY_QT_QUICK}
        ${FIDGETY_QT_QML}
    )
endmacro()

macro(fidgety_link_qt)
    # ARGV0: name of the library (no namespace)
    if(ARGC LESS 1)
        message(
            FATAL_ERROR
            "to link libraries to a target, you must provide the name of the target"
        )
    endif()
    fidgety_link_qt_base(${ARGN})
    target_link_libraries(${ARGV0} PRIVATE _FidgetyUtilsQt)
endmacro()

macro(fidgety_link_common_libraries)
    # ARGV0: name of the library (no namespace)
    if(ARGC LESS 1)
        message(
            FATAL_ERROR
            "to link libraries to a target, you must provide the name of the target"
        )
    endif()
    target_link_libraries(${ARGV0} PUBLIC Fidgety::FidgetyHeaders)
    target_link_libraries(${ARGV0} PRIVATE Fidgety::_FidgetyUtils)
    target_link_libraries(${ARGV0} PRIVATE spdlog::spdlog)
    fidgety_link_fmt(${ARGV0})
endmacro()

macro(fidgety_link_exception)
    # ARGV0: name of the library
    if(ARGC LESS 1)
        message(
            FATAL_ERROR
            "you must provide the name of the target to link Fidgety::FidgetyException to"
        )
    endif()
    target_link_libraries(${ARGV0} PUBLIC Fidgety::FidgetyException)
endmacro()

macro(fidgety_install_library)
    # ARGV0: name of the library
    # ARGV1: name of the CMake file
    if(ARGC LESS 2)
        message(
            FATAL_ERROR
            "you need to provide the name of the library and the name its CMake file"
        )
    endif()
    if(FIDGETY_MASTER_PROJECT)
        install(
            TARGETS ${ARGV0}
            EXPORT ${ARGV0}
            DESTINATION ${FIDGETY_EXPORT_LIB_DIR}
        )
        install(
            EXPORT ${ARGV0}
            DESTINATION ${FIDGETY_EXPORT_CMAKE_DIR}
            NAMESPACE Fidgety::
            FILE ${ARGV1}
        )
    endif()
endmacro()

macro(fidgety_install_executable)
    # ARGV0: name of the executable
    # ARGV1: name of the CMake file
    if(ARGC LESS 2)
        message(
            FATAL_ERROR
            "you need to provide the name of the executable and the name its CMake file"
        )
    endif()
    if(FIDGETY_MASTER_PROJECT)
        install(
            TARGETS ${ARGV0}
            EXPORT ${ARGV0}
            DESTINATION ${FIDGETY_EXPORT_BIN_DIR}
        )
        install(
            EXPORT ${ARGV0}
            DESTINATION ${FIDGETY_EXPORT_CMAKE_DIR}
            NAMESPACE Fidgety::
            FILE ${ARGV1}
        )
    endif()
endmacro()

macro(fidgety_install_extension)
    # ARGV0: name of the extension
    # ARGV1: name of the CMake file
    if(ARGC LESS 2)
        message(
            FATAL_ERROR
            "you need to provide the name of the extension and the name its CMake file"
        )
    endif()
    if(FIDGETY_MASTER_PROJECT)
        install(
            TARGETS ${ARGV0}
            EXPORT ${ARGV0}
            DESTINATION ${FIDGETY_EXPORT_EXTLIB_DIR}
        )
        install(
            EXPORT ${ARGV0}
            DESTINATION ${FIDGETY_EXPORT_CMAKE_DIR}
            NAMESPACE Fidgety::
            FILE ${ARGV1}
        )
    endif()
endmacro()

macro(fidgety_install_extension_library)
# ARGV0: name of the library
    # ARGV1: name of the CMake file
    if(ARGC LESS 2)
        message(
            FATAL_ERROR
            "you need to provide the name of the library and the name its CMake file"
        )
    endif()
    if(FIDGETY_MASTER_PROJECT)
        install(
            TARGETS ${ARGV0}
            EXPORT ${ARGV0}
            DESTINATION ${FIDGETY_EXPORT_LIB_DIR}
        )
        install(
            EXPORT ${ARGV0}
            DESTINATION ${FIDGETY_EXPORT_CMAKE_DIR}
            NAMESPACE Fidgety::
            FILE ${ARGV1}
        )
    endif()
endmacro()

macro(fidgety_link_test_libraries)
    # ARGV0: name of the test
    if(ARGC LESS 1)
        message(
            FATAL_ERROR
            "to link libraries to a target, you must provide the name of the target"
        )
    elseif(NOT BUILD_TESTING)
        message(FATAL_ERROR "tests must be enabled to use this macro")
    endif()
    target_link_libraries(${ARGV0} PRIVATE nlohmann_json::nlohmann_json spdlog::spdlog)
    target_link_libraries(${ARGV0} PRIVATE ${GTEST} ${GTEST_MAIN})
endmacro()

macro(fidgety_create_test)
    # ARGV0: name of the test
    # ARGV1..N: source files
    if(ARGC LESS 2)
        message(FATAL_ERROR "you must at least provide the name of the test and its source code")
    elseif(NOT BUILD_TESTING)
        message(FATAL_ERROR "tests must be enabled to use this macro")
    endif()
    add_executable(${ARGN})
    fidgety_link_test_libraries(${ARGV0})
    add_test(NAME ${ARGV0}_test COMMAND ${ARGV0})
endmacro()
