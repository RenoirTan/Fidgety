fidgety_add_my_executable(FidgetyApp app.cpp)
set_target_properties(FidgetyApp PROPERTIES OUTPUT_NAME fidgety)
fidgety_link_common_libraries(FidgetyApp)
target_link_libraries(
    FidgetyApp PRIVATE
    FidgetyDatabase FidgetyItoDatabase
    FidgetyDecoder
    FidgetyEditor
    FidgetyOptions
    _FidgetyUtils
    FidgetySelector
    FidgetyVerifier
)
fidgety_link_exception(FidgetyApp)
fidgety_link_qt(FidgetyApp)
fidgety_install_library(FidgetyApp fidgety_app_config.cmake)
