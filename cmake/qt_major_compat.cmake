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
