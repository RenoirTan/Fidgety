set(CURDIR ${PROJECT_SOURCE_DIR}/resources/qml/)
set(TARGETDIR ${CMAKE_BINARY_DIR}/share/fidgety/qml)

add_custom_target(_FidgetyQmlResources)

add_custom_command(
    TARGET _FidgetyQmlResources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CURDIR} ${TARGETDIR}
    VERBATIM
)

install(DIRECTORY ${CURDIR} DESTINATION ${TARGETDIR})
