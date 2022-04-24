fidgety_add_my_library(_FidgetyUtils STATIC string.cpp)
set_target_properties(_FidgetyUtils PROPERTIES OUTPUT_NAME __fidgety_utils)
target_link_libraries(
    _FidgetyUtils PRIVATE
    Fidgety::FidgetyHeaders Boost::boost
)
fidgety_install_library(_FidgetyUtils _fidgety_utils_config.cmake)

fidgety_add_my_library(_FidgetyUtilsJson STATIC json.cpp)
set_target_properties(_FidgetyUtilsJson PROPERTIES OUTPUT_NAME __fidgety_utils_json)
target_link_libraries(
    _FidgetyUtilsJson PRIVATE
    Fidgety::FidgetyHeaders nlohmann_json::nlohmann_json
)
fidgety_link_fmt(_FidgetyUtilsJson)
fidgety_install_library(_FidgetyUtilsJson _fidgety_utils_json_config.cmake)

fidgety_add_my_library(_FidgetyUtilsQt STATIC qt.cpp)
set_target_properties(_FidgetyUtilsQt PROPERTIES OUTPUT_NAME __fidgety_utils_qt)
target_link_libraries(_FidgetyUtilsQt PRIVATE FidgetyHeaders)
fidgety_link_qt_base(_FidgetyUtilsQt)
fidgety_install_library(_FidgetyUtilsQt _fidgety_utils_qt_config.cmake)
