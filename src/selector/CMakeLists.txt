fidgety_add_my_library(FidgetySelector STATIC selector.cpp loader.cpp)
set_target_properties(FidgetySelector PROPERTIES OUTPUT_NAME fidgety_selector)
fidgety_set_output_directory(FidgetySelector)
target_link_libraries(
    FidgetySelector PUBLIC
    FidgetyDecoder FidgetyDecoder FidgetyOptions FidgetyVerifier FidgetyDylib
    Boost::boost Boost::regex Boost::filesystem
)
fidgety_link_common_libraries(FidgetySelector)
fidgety_link_exception(FidgetySelector)
fidgety_install_library(FidgetySelector fidgety_selector_config.cmake)
