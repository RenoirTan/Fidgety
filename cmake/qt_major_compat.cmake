macro(fidgety_find_qt)
    #ARGN: Required components
    find_package(Qt${FIDGETY_QT_MAJOR} COMPONENTS ${ARGN} REQUIRED)
endmacro()

macro(fidgety_add_qt_component)
    # ARGV0: Upper case version of the library name
    # ARGV1: Pascal case version of the library name
    if(ARGC LESS 2)
        message(FATAL_ERROR "you must specify the name of the qt component")
    endif()
    set(FIDGETY_QT_${ARGV0} Qt${FIDGETY_QT_MAJOR}::${ARGV1})
endmacro()

macro(fidgety_setup_qt_dependency)
    # FIDGETY_COMPONENTS_UPPER
    # FIDGETY_COMPONENTS_PASCAL
    fidgety_find_qt(${FIDGETY_COMPONENTS_PASCAL})
    foreach(upper pascal IN ZIP_LISTS FIDGETY_COMPONENTS_UPPER FIDGETY_COMPONENTS_PASCAL)
        fidgety_add_qt_component(${upper} ${pascal})
    endforeach()
endmacro()

# Make a qt cmake function compatible between Qt5 and Qt6 build options.
macro(fidgety_compat_qt_callable)
    # ARGV0: Suffix of the function/macro. (e.g. `add_resources` in `qt5_add_resources`)
    if(ARGC LESS 1)
        message(FATAL_ERROR "you must specify the suffix of a qt callable")
    endif()
    macro(qt_${ARGV0})
        cmake_language(CALL qt${FIDGETY_QT_MAJOR}_${ARGV0} ${ARGN})
    endmacro()
    #set(qt_${ARGV0} qt${FIDGETY_QT_MAJOR}_${ARGV0})
endmacro()
