fidgety_add_my_library(
    FidgetyEditor SHARED
    editor.cpp app.cpp homepage.cpp
)
set_target_properties(FidgetyEditor PROPERTIES OUTPUT_NAME fidgety_editor)
fidgety_set_output_directory(FidgetyEditor)
fidgety_link_common_libraries(FidgetyEditor)
fidgety_link_exception(FidgetyEditor)
fidgety_link_qt(FidgetyEditor)
target_link_libraries(FidgetyEditor PUBLIC Boost::boost Boost::filesystem)
fidgety_install_library(FidgetyEditor fidgety_editor_config.cmake)