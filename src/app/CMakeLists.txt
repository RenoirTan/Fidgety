fidgety_add_my_executable(FidgetyApp app.cpp)
set_target_properties(FidgetyApp PROPERTIES OUTPUT_NAME fidgety)
fidgety_set_output_directory(FidgetyApp)
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
    Boost::boost
    Boost::filesystem
)
fidgety_link_exception(FidgetyApp)
fidgety_link_qt(FidgetyApp)
fidgety_install_executable(FidgetyApp fidgety_app_config.cmake)
