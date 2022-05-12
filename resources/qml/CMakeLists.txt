set(CURDIR ${PROJECT_SOURCE_DIR}/resources/qml)
set(TARGETDIR ${CMAKE_BINARY_DIR}/share/fidgety/qml)
set(QMLRES qml.qrc Homepage.qml ConfigFilesModel.qml)

# xiexie laotianye
# https://stackoverflow.com/a/2362222

execute_process(COMMAND mkdir -p ${TARGETDIR})

set(resourcefilelist)
foreach(resourcefile ${QMLRES})
    set(resourcefilesrc ${CURDIR}/${resourcefile})
    set(resourcefiledest ${TARGETDIR}/${resourcefile})
    message(STATUS "copying ${resourcefilesrc} to ${resourcefiledest}")
    add_custom_command(
        OUTPUT ${resourcefiledest}
        COMMAND ${CMAKE_COMMAND} -E copy ${resourcefilesrc} ${resourcefiledest}
        DEPENDS ${resourcefilesrc}
    )
    list(APPEND resourcefilelist ${resourcefiledest})
endforeach()

message(STATUS "compiling qml.qrc to qml.rcc")
add_custom_command(
    OUTPUT ${TARGETDIR}/qml.rcc
    COMMAND ${FIDGETY_RCC} -binary ${CURDIR}/qml.qrc -o ${TARGETDIR}/qml.rcc
    DEPENDS ${CURDIR}/qml.qrc ${resourcefilelist}
)
list(APPEND resourcefilelist ${TARGETDIR}/qml.rcc)

message(${resourcefilelist})

add_custom_target(_FidgetyQmlResourcesCustom DEPENDS ${resourcefilelist})
fidgety_add_my_library(_FidgetyQmlResources STATIC)
add_dependencies(_FidgetyQmlResources _FidgetyQmlResourcesCustom)
set_target_properties(_FidgetyQmlResources PROPERTIES OUTPUT_NAME __fidgety_qml_resources)
fidgety_install_library(_FidgetyQmlResources _fidgety_qml_resources_config.cmake)

install(
    DIRECTORY ${TARGETDIR} DESTINATION ${FIDGETY_EXPORT_DATA_DIR} FILES_MATCHING
    PATTERN *.qrc
    PATTERN *.qml
    PATTERN *.rcc
)
#install(DIRECTORY ${TARGETDIR} DESTINATION ${FIDGETY_EXPORT_DATA_DIR} FILES_MATCHING PATTERN *.qml)
