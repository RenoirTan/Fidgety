fidgety_add_my_library(
    FidgetyOptions STATIC
    options.cpp option_identifier.cpp option_value.cpp validator.cpp
)
set_target_properties(FidgetyOptions PROPERTIES OUTPUT_NAME fidgety_options)
fidgety_link_common_libraries(FidgetyOptions)
fidgety_link_exception(FidgetyOptions)
target_link_libraries(FidgetyOptions PRIVATE Boost::boost)
fidgety_install_library(FidgetyOptions fidgety_options_config.cmake)