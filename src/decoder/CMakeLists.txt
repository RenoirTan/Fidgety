add_library(FidgetyDecoder STATIC decoder.cpp)
add_library(Fidgety::FidgetyDecoder ALIAS FidgetyDecoder)
set_target_properties(FidgetyDecoder PROPERTIES OUTPUT_NAME fidgety_decoder)
target_link_libraries(FidgetyDecoder PUBLIC Fidgety::FidgetyHeaders)
target_link_libraries(FidgetyDecoder PRIVATE Fidgety::_FidgetyUtils)
target_link_libraries(FidgetyDecoder PRIVATE spdlog::spdlog)
install(
    TARGETS FidgetyDecoder
    EXPORT FidgetyDecoder
    DESTINATION ${export_lib_dir})
install(
    EXPORT FidgetyDecoder
    DESTINATION ${export_cmake_dir}
    NAMESPACE Fidgety::
    FILE fidgety_decoder_config.cmake
)

add_library(FidgetyNormalConfDecoder STATIC normal_conf_decoder.cpp)
add_library(Fidgety::FidgetyNormalConfDecoder ALIAS FidgetyNormalConfDecoder)
set_target_properties(FidgetyNormalConfDecoder PROPERTIES OUTPUT_NAME fidgety_normal_conf_decoder)
target_link_libraries(FidgetyNormalConfDecoder PUBLIC Fidgety::FidgetyDecoder)
target_link_libraries(FidgetyNormalConfDecoder PRIVATE Fidgety::_FidgetyUtils)
target_link_libraries(FidgetyNormalConfDecoder PRIVATE spdlog::spdlog)
target_link_libraries(FidgetyNormalConfDecoder PRIVATE nlohmann_json)
install(
    TARGETS FidgetyNormalConfDecoder
    EXPORT FidgetyNormalConfDecoder
    DESTINATION ${export_lib_dir}
)
install(
    EXPORT FidgetyNormalConfDecoder
    DESTINATION ${export_cmake_dir}
    NAMESPACE Fidgety::
    FILE fidgety_normal_conf_decoder_config.cmake
)
