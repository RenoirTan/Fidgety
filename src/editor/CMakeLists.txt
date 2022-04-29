qt_add_resources(
    _FIDGETY_EDITOR_RES
    ${PROJECT_SOURCE_DIR}/resources/qml/homepage.qml
)

fidgety_add_my_library(FidgetyEditor STATIC editor.cpp ${_FIDGETY_EDITOR_RES})
set_target_properties(FidgetyEditor PROPERTIES OUTPUT_NAME fidgety_editor)
fidgety_link_common_libraries(FidgetyEditor)
fidgety_link_exception(FidgetyEditor)
fidgety_link_qt(FidgetyEditor)
fidgety_install_library(FidgetyEditor fidgety_editor_config.cmake)